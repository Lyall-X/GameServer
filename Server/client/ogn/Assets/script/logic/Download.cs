using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;

public class Download : MonoBehaviour
{
    public string mUrl = "http://127.0.0.1:81/";
    public string config = "config.xml";
    public string path = "data/";
    public delegate void onCallback(string url, string dir, byte[] bytes);
    public delegate void onDownload();
    public onDownload func;
    public int total = 0;
    void Start()
    {
        FileStream file = new FileStream("config.xml", FileMode.OpenOrCreate);
        byte[] bytes = new byte[file.Length];
        file.Read(bytes, 0, bytes.Length);
        file.Close();
        XmlDocument doc = new XmlDocument();
        doc.LoadXml(System.Text.Encoding.UTF8.GetString(bytes));
        XmlNodeList nodeList = doc.SelectSingleNode("root").ChildNodes;
        foreach (XmlNode node in nodeList)
        {
            mUrl = node.Attributes["http"].Value;
            config = node.Attributes["config"].Value;
            path = node.Attributes["downloadPath"].Value;
        }

        StartCoroutine(StartDownload(mUrl + config, path, OnFinishConfigBase));
    }

    IEnumerator StartDownload(string url, string dir,onCallback call)
    {
        // url可以是网络网址，也可以是本地网址  
        WWW www = new WWW(url);
        total++;
        // 检测是否下载完毕，也可以通过IsDone函数检测  
        yield return www;
        if (www.isDone)
        {
            if (call != null)
                call(www.url, dir, www.bytes);
            total--;
            if (total <= 0)
            {
                if (func != null)
                    func();
            }
        }
        // 释放资源  
        www.Dispose();
    } 

    void OnFinishConfigBase(string url, string dir, byte[] bytes)
    {
        if (!Directory.Exists(dir))
            Directory.CreateDirectory(dir);

        FileStream file = new FileStream(dir + config, FileMode.OpenOrCreate);
        file.Write(bytes, 0, bytes.Length);
        file.Close();

        XmlDocument doc = new XmlDocument();
        doc.LoadXml(System.Text.Encoding.UTF8.GetString(bytes));
        XmlNodeList nodeList = doc.SelectSingleNode("root").ChildNodes;
        foreach (XmlNode node in nodeList)
        {
            string spath = node.Attributes["path"].Value;
            StartCoroutine(StartDownload(mUrl + spath, spath, OnFinishConfig));
        }
    }

    void OnFinishConfig(string url, string dir, byte[] bytes)
    {
        int s = dir.LastIndexOf('/');
        string p = "";
        if (s != -1)
            p = dir.Substring(0, s);
        p = path + p;
        if (!Directory.Exists(p))
            Directory.CreateDirectory(p);

        FileStream file = new FileStream(path + dir, FileMode.OpenOrCreate);
        file.Write(bytes, 0, bytes.Length);
        file.Close();
        Debug.Log(dir);
    }
}
