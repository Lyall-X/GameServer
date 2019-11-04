using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public enum EntityType
{
    ET_None,
    ET_Player,
    ET_Npc,
    ET_Weapon,
}

public class Entity : MonoBehaviour
{
    public static int sId = 0;
    protected int mInsId;
    protected long mGuid;
    protected string mName;

    public int InsId
    {
        get { return mInsId; }
        set { mInsId = value; }
    }
    public long Guid
    {
        get { return mGuid; }
        set { mGuid = value; }
    }

    public string Name
    {
        get { return mName; }
        set { mName = value; }
    }

    public Dictionary<string, Property> dictProperty = new Dictionary<string, Property>();

    public void Init()
    {
    }
    void Start()
    {

    }
    public virtual void SendPacket(Packet packet)
    {

    }

    public virtual void setCharId(int charId)
    {
    }
    public virtual bool Destory()
    {
        return true;
    }

    void Update()
    {
    }
}
