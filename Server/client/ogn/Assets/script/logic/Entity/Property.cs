using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using UnityEngine;

public class Property : Header
{
    public virtual bool InfoToProperty(object info)
    {
        return true;
    }
    public virtual bool PropertyToInfo(object info)
    {
        return true;
    }
}

public class EntityProperty : Property
{
	int			    accountId;
	int			    roleId;
	int			    instanceId;
	string		    name;
	int			    mapId;
	Vector2			pos;
	byte			dir;
    public override bool serialize(BinaryStream bitStream)
    {
        bitStream.Write(accountId);
        bitStream.Write(roleId);
        bitStream.Write(instanceId);
        bitStream.Write(name);
        bitStream.Write(mapId);
        bitStream.Write(pos.x);
        bitStream.Write(pos.y);
        bitStream.Write(dir);
        return true;
    }

    public override bool deSerialize(BinaryStream bitStream)
    {
        bitStream.Read(ref accountId);
        bitStream.Read(ref roleId);
        bitStream.Read(ref instanceId);
        bitStream.Read(ref name);
        bitStream.Read(ref mapId);
        bitStream.Read(ref pos.x);
        bitStream.Read(ref pos.y);
        bitStream.Read(ref dir);
        return true;
    }
}
