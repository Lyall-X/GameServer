using UnityEngine;
using System.Collections.Generic;

public class ObjectArray
{
    public Object[] objectArray;
}

public class AssetManager  : Singleton<AssetManager>
{

    public Dictionary<string, object> mDictAsset = new Dictionary<string, object>();

    public Dictionary<string, List<object>> mDictListGameObject = new Dictionary<string, List<object>>();
    public Dictionary<string, List<object>> mDictListDeleteObject = new Dictionary<string, List<object>>();

    public object getAssetObject(string path, System.Type systemTypeInstance, bool array = false)
    {
        
        if (mDictListDeleteObject.ContainsKey(path))
        {
            List<object> lst = mDictListDeleteObject[path];
            if (lst.Count > 0)
            {
                object obj = lst[0];
                lst.Remove(obj);
                if (lst.Count <= 0)
                    mDictListDeleteObject.Remove(path);

                if (mDictListGameObject.ContainsKey(path))
                {
                    lst = mDictListGameObject[path];
                    lst.Add(obj);
                }
                else
                {
                    lst = new List<object>();
                    lst.Add(obj);
                    mDictListGameObject.Add(path, lst);
                }
                return obj;
            }

            return getAsset(path, systemTypeInstance, array);
        }
        return getAsset(path, systemTypeInstance, array);
    }

    public void delAssetObject(string path, object obj)
    {
        List<object> lst = null;
        if (mDictListGameObject.ContainsKey(path))
        {
            lst = mDictListGameObject[path];
            lst.Remove(obj);
            if (lst.Count <= 0)
                mDictListGameObject.Remove(path);
        }

        if (mDictListDeleteObject.ContainsKey(path))
        {
            lst = mDictListDeleteObject[path];
            lst.Add(obj);
            return;
        }
        lst = new List<object>();
        lst.Add(obj);
        mDictListDeleteObject.Add(path, lst);
    }

    public void addGameObjectList(string path, object obj)
    {
        List<object> lst = null;
        if (mDictListGameObject.ContainsKey(path))
        {
            lst = mDictListGameObject[path];
            lst.Add(obj);
            return;
        }

        lst = new List<object>();
        lst.Add(obj);
        mDictListGameObject[path] = lst;
        return;
    }

    public object getAsset(string path, System.Type systemTypeInstance, bool array = false)
    {
        object obj = null;
        if (mDictAsset.ContainsKey(path))
        {
            if (array)
            {
                obj = mDictAsset[path];
            }
            else
            {
                obj = GameObject.Instantiate(mDictAsset[path] as Object);
            }
            addGameObjectList(path, obj as Object);
            return obj ;   
        }
        if (array)
        {
            Object[] objs = Resources.LoadAll(path, systemTypeInstance);
            ObjectArray objArray = new ObjectArray();
            objArray.objectArray = objs;
            obj = objArray;
            mDictAsset[path] = obj;

        }
        else
        {
            obj = Resources.Load(path, systemTypeInstance);
            mDictAsset[path] = obj;
            obj = GameObject.Instantiate(mDictAsset[path] as Object);
        }

        addGameObjectList(path, obj);
        return obj;
    }
    public Sprite getSpriteAsset(string path)
    {
        return getAssetObject(path, typeof(Sprite)) as Sprite;
    }

    public ObjectArray getSpritesAsset(string path)
    {
        return getAssetObject(path, typeof(Sprite), true) as object as ObjectArray;
    }
    public GameObject getGameObjectAsset(string path)
    {
        return getAssetObject(path, typeof(GameObject)) as GameObject;
    }

    public TextAsset getTextAssetAsset(string path)
    {
        return getAssetObject(path, typeof(TextAsset)) as TextAsset;
    }
}
