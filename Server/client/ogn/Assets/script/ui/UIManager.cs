using UnityEngine;
using System.Collections;
using System.Xml;
using System.Collections.Generic;
public class UIManager : MonoBehaviour {

    public Dictionary<string, GameObject> dictUIPlane = new Dictionary<string, GameObject>();
    public GameObject currentWindow = null;
    void Awake()
    {
    }
	// Use this for initialization
	void Start () {

        XmlDocument doc = new XmlDocument();
        TextAsset text = AssetManager.Instance .getTextAssetAsset("config/ui_config");
        doc.LoadXml(System.Text.Encoding.UTF8.GetString(text.bytes));

        XmlNodeList nodeList = doc.SelectSingleNode("root").ChildNodes;
        foreach (XmlNode node in nodeList) {
            GameObject obj = AssetManager.Instance.getGameObjectAsset(node.Attributes["path"].Value);
            string script = node.Attributes["script"].Value;
            obj.AddComponent(System.Type.GetType(script));
            obj.SetActive(false);
            dictUIPlane.Add(node.Attributes["name"].Value, obj);
        }
        openWindow("ui_login");
    }

    // Update is called once per frame
    void Update () {
	
	}

    public GameObject openWindow(string name)
    {
        if (currentWindow != null)
            currentWindow.SetActive(false);

        currentWindow = getWindow(name);
        currentWindow.SetActive(true);
        return currentWindow;
    }

    public GameObject closeWindow(string name = "")
    {
        if (currentWindow != null)
            currentWindow.SetActive(false);

        currentWindow = null;
        if (name == "")
            return currentWindow;

        getWindow(name).SetActive(false);
        return null;
    }

    public GameObject getWindow(string name) {
        if (dictUIPlane.ContainsKey(name))
            return dictUIPlane[name];

        return null;
    }

    //public T GetComponent<T>(string name)
    //{
    //    GameObject obj = getWindow(name);
    //    if (obj != null)
    //        return obj as T;
    //    return null;
    //}

}
