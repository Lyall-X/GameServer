using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class UIEntity : MonoBehaviour
{
    protected Dictionary<string, float> dictButton = new Dictionary<string, float>();
  
    public void PostButton(string key, float t)
    {
        if (dictButton.ContainsKey(key))
        {
            dictButton[key] = t;
            return;
        }
        dictButton.Add(key, t);
    }

    public bool CheckClick(string key)
    {
        if (dictButton.ContainsKey(key))
            return false;
        return true;
    }

    void Awake()
    {
       
    }
    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        List<string> delList = new List<string>();
        foreach(var item in dictButton)
        {
            float value = item.Value - Time.time;
            if (value <= 0.0f)
            {
                delList.Add(item.Key);
            }
            else
            {
                dictButton[item.Key] = value;
            }
        }

        foreach(var item in delList)
        {
            dictButton.Remove(item);
        }
    }
   
}
