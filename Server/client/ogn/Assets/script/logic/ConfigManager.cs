using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using LitJson;
using System.IO;

public class ServerConfig
{
    public string Name;
    public string Host;
    public short Port;
    public string User;
    public string Password;
    public string DbName;
}

public class ServerConfigList
{
    public List<ServerConfig> config;
}


public class AnimationConfig
{
    public int ID;
    public string Desc;
    public int Status;
    public string Path;
    public double Delay;
    public int Loop;
}

public class ResourceConfig
{
    public int ID;
    public string Name;
}

public class ActorConfig
{
    public int ID;
    public string Name;
    public string Anim;
}
public class CharacterConfig
{
    public int ID;
    public int ActorId;
    public int PropertyId;
    public string Name;
    public short Type;
    public short Sex;
}

public class NpcConfig
{
	public int  ID;
	string Name;
	byte Type;
	public int CharId;
	public int MapId;
	public int PosX;
	public int PosY;
};

public class MapConfig
{
   	public int ID;
	public string Name;
    byte Type;
    public int Row;
    public int Col;
    public double Cell;
    public int LogicRow;
    public int LogicCol;
    public int ViewCell;
}

public class MapConfigList
{
    public List<MapConfig> config;
}

public class AnimationConfigList
{
    public List<AnimationConfig> config;
}
public class ResourceConfigList
{
    public List<ResourceConfig> config;
}

public class ActorConfigList
{
    public List<ActorConfig> config;
} 

public class CharacterConfigList
{
    public List<CharacterConfig> config;
}

public class NpcConfigList
{
    public List<NpcConfig> config;
}

public class ConfigManager : Singleton<ConfigManager>
{
    public AnimationConfigList mAnimConfigList;
    public Dictionary<int, AnimationConfig> mDictAnimConfig = new Dictionary<int, AnimationConfig>();


    public ResourceConfigList mResConfigList;
    public Dictionary<int, ResourceConfig> dictResourceConfig = new Dictionary<int, ResourceConfig>();


    public ActorConfigList mActorConfigList;
    public Dictionary<int, ActorConfig> mDictActorConfig = new Dictionary<int,ActorConfig>();
    
    public CharacterConfigList mCharConfigList;
    public Dictionary<int, CharacterConfig> mDictCharConfig = new Dictionary<int, CharacterConfig>();


    public NpcConfigList mNpcConfigList;
    public Dictionary<int, NpcConfig> mDictNpcConfig = new Dictionary<int, NpcConfig>();

    public ServerConfigList mServerConfigList;
    public Dictionary<string, ServerConfig> mDictServerConfig = new Dictionary<string, ServerConfig>();

    public MapConfigList mMapConfigList;
    public Dictionary<int, MapConfig> mDictMapConfig = new Dictionary<int, MapConfig>();

    public AnimationConfig getAnimConfig(int id)
    {
        if (mDictAnimConfig.ContainsKey(id))
            return mDictAnimConfig[id];

        Debug.LogError("getAnimConfig " + id);
        return null;
    }
    public ResourceConfig getResConfig(int id)
    {
        if (dictResourceConfig.ContainsKey(id))
            return dictResourceConfig[id];

        Debug.LogError("getResConfig " + id);
        return null;
    }

    public ActorConfig getActorConfig(int id)
    {
        if (mDictActorConfig.ContainsKey(id))
            return mDictActorConfig[id];

        Debug.LogError("getActorConfig " + id);
        return null;
    }
    public CharacterConfig getCharConfig(int id)
    {
        if (mDictCharConfig.ContainsKey(id))
            return mDictCharConfig[id];

        return null;
    }

    public NpcConfig getNpcConfig(int id)
    {
        if (mDictNpcConfig.ContainsKey(id))
            return mDictNpcConfig[id];
        return null;
    }

    public ServerConfig getServerConfig(string name)
    {
        if (mDictServerConfig.ContainsKey(name))
            return mDictServerConfig[name];
        return null;
    }

    public MapConfig getMapConfig(int id)
    {
        if (mDictMapConfig.ContainsKey(id))
            return mDictMapConfig[id];
        return null;
    }
    public ConfigManager()
    {
        //loadServerConfig("config.json");
        //loadAnimConfig("config/animation.json");
        //loadResourceConfig("config/resource.json");
        //loadCharacterConfig("config/character.json");
        //loadActorConfig("config/actor.json");
        //loadNpcConfig("config/npc.json");
        //loadMapConfig("config/map.json");

        Debug.Log("load config ok");
    }

    public string loadJson(string path)
    {
        //path = GameStart.download.path + path;

        path = "data/" + path;
        FileStream file = new FileStream(path, FileMode.OpenOrCreate);
        byte[] bytes = new byte[file.Length];
        file.Read(bytes, 0, (int)file.Length);
        file.Close();
        return System.Text.Encoding.Default.GetString(bytes);
    }

    public bool loadServerConfig(string path)
    {
        mServerConfigList = JsonMapper.ToObject<ServerConfigList>(loadJson(path));
        foreach (ServerConfig cfg in mServerConfigList.config)
            mDictServerConfig.Add(cfg.Name, cfg);
        return true;
    }

    public bool loadAnimConfig(string path)
    {
        mAnimConfigList = JsonMapper.ToObject<AnimationConfigList>(loadJson(path));
        foreach (AnimationConfig cfg in mAnimConfigList.config)
        {
            mDictAnimConfig.Add(cfg.ID, cfg);
        }

        return true;
    }

    public bool loadResourceConfig(string path)
    {
        mResConfigList = JsonMapper.ToObject<ResourceConfigList>(loadJson(path));
        foreach (ResourceConfig cfg in mResConfigList.config)
        {
            dictResourceConfig.Add(cfg.ID, cfg);
        }
        return true;
    }
    public bool loadActorConfig(string path)
    {
        mActorConfigList = JsonMapper.ToObject<ActorConfigList>(loadJson(path));
        foreach (ActorConfig cfg in mActorConfigList.config)
        {
            mDictActorConfig.Add(cfg.ID, cfg);
        }
        return true;
    }

    public bool loadCharacterConfig(string path)
    {
        mCharConfigList = JsonMapper.ToObject<CharacterConfigList>(loadJson(path));
        foreach (CharacterConfig cfg in mCharConfigList.config)
            mDictCharConfig.Add(cfg.ID, cfg);
        return true;
    }

    public bool loadNpcConfig(string path)
    {
        mNpcConfigList = JsonMapper.ToObject<NpcConfigList>(loadJson(path));
        foreach (NpcConfig cfg in mNpcConfigList.config)
            mDictNpcConfig.Add(cfg.ID, cfg);
        return true;
    }

    public bool loadMapConfig(string path)
    {
        mMapConfigList = JsonMapper.ToObject<MapConfigList>(loadJson(path));
        foreach (MapConfig cfg in mMapConfigList.config)
            mDictMapConfig.Add(cfg.ID, cfg);
        return true;
    }

}
