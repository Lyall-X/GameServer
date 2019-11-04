using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class PlayerManager : Singleton<PlayerManager>
{
    protected LocalPlayer mLocalPlayer;
    public LocalPlayer localPlayer
    {
        get { return mLocalPlayer; }
        set { mLocalPlayer = value; }
    }

    public Dictionary<uint, Player> dictAccIdPlayer = new Dictionary<uint, Player>();
    public Dictionary<uint, Player> dictUserIdPlayer = new Dictionary<uint, Player>();
    public Dictionary<string, Player> dictNamePlayer = new Dictionary<string, Player>();
    public bool isLocalPlayer(long guid)
    {
        return localPlayer != null ? localPlayer.Guid == guid : false;
    }

    public LocalPlayer CreateLocalPlayer()
    {
        localPlayer = new LocalPlayer();
        return localPlayer;
    }

    public Player FindPlayerByAccId(uint accId)
    {
        if (dictAccIdPlayer.ContainsKey(accId))
            return dictAccIdPlayer[accId];
        return null;
    }

    public Player FindPlayerByUserId(uint userId)
    {
        if (dictUserIdPlayer.ContainsKey(userId))
            return dictUserIdPlayer[userId];
        return null;
    }

    public Player FindPlayerByName(string name)
    {
        if (dictNamePlayer.ContainsKey(name))
            return dictNamePlayer[name];
        return null;
    }
    public Player AddPlayer(Player aPlr)
    {
        if (dictAccIdPlayer.ContainsKey(aPlr.AccId)) return null;
        dictAccIdPlayer.Add(aPlr.AccId, aPlr);
        World.Instance.addEntity(aPlr);
        return aPlr;
    }

    public Player AddPlayerByUserId(Player aPlr)
    {
        if (dictUserIdPlayer.ContainsKey(aPlr.UserId)) return null;
        dictUserIdPlayer.Add(aPlr.UserId, aPlr);
        dictNamePlayer.Add(aPlr.Name, aPlr);
        return aPlr;
    }

    public void RemovePlayer(uint accId)
    {
        Player aPlr = FindPlayerByAccId(accId);
        if (aPlr == null) return;

        if (dictUserIdPlayer.ContainsKey(aPlr.UserId))
        {
            dictUserIdPlayer.Remove(aPlr.UserId);
        }

        if (dictNamePlayer.ContainsKey(aPlr.Name))
        {
            dictNamePlayer.Remove(aPlr.Name);
        }

        if (dictAccIdPlayer.ContainsKey(aPlr.AccId))
        {
            dictAccIdPlayer.Remove(aPlr.AccId);
        }
    }
}
