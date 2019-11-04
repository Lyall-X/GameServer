using UnityEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public partial class SocketHandler : EventDispatcher
{
    public int onLogin(NetLoginRes res)
    {
        if (res.result == (int)NetResult.NResultSuccess)
        {
            LocalPlayer localPlayer = PlayerManager.Instance.CreateLocalPlayer();
            localPlayer.Guid = res.guid;

            localPlayer.AccId = res.accInfo.id;
            localPlayer.Name = res.accInfo.user;
            localPlayer.client = GameStart.sGame.socket;
            PlayerManager.Instance.AddPlayer(localPlayer);

            Debug.Log("[" + res.accInfo.user + "] login success");
            GameStart.uiMgr.closeWindow();

            if (res.roleInfos.Count == 0)
            {
                GameStart.uiMgr.openWindow("ui_createrole");
            }
            else
            {
                NetSelectRoleReq req = new NetSelectRoleReq();
                req.accId = (uint)res.accInfo.id;
                req.userId = (uint)res.roleInfos[0].id;
                localPlayer.SendPacket(req);
            }

            return 0;
        }

        Debug.Log("[" + res.accInfo.user + "] login fail");
        return 1;
    }

    public int onCreateRole(NetCreateRoleRes res)
    {
        LocalPlayer localPlayer = PlayerManager.Instance.localPlayer;
        return 1;
    }

    public int onSelectRole(NetSelectRoleRes res)
    {
        LocalPlayer localPlayer = PlayerManager.Instance.localPlayer;
        localPlayer.UserId = res.roleInfo.id;
        localPlayer.Name = res.roleInfo.name;
        GameStart.sPlrMgr.AddPlayerByUserId(localPlayer);
        return 1;
    }

    //public int onPlayerInfoNotify(object l, object w)
    //{
    //    SocketAngent socket = l as SocketAngent;
    //    NetPlayerInfoNotify res = w as NetPlayerInfoNotify;

    //    int propertyCount = 0;
    //    res.playerInfoStream.Read(ref propertyCount);
    //    for (int i = 0; i < propertyCount; ++i)
    //    {
    //        string propertyName = "";
    //        res.playerInfoStream.Read(ref propertyName);
    //        BinaryStream bit = new BinaryStream();
    //        res.playerInfoStream.Read(bit);

    //        Property property = PlayerManager.playerMgr.localPlayer.getProperty(propertyName);
    //        if (property == null)
    //        {
    //            Type type_ = Type.GetType(propertyName);
    //            if (type_ != null)
    //                property = type_.Assembly.CreateInstance(propertyName) as Property;

    //            if (property != null)
    //                PlayerManager.playerMgr.localPlayer.addProperty(propertyName, property);
    //        }

    //        if (property != null)
    //        {
    //            property.deSerialize(bit);
    //        }

    //    }

    //    ViewMainUI viewMainUI = GameStart.uiMgr.getWindow("main_ui").GetComponent<ViewMainUI>();
    //    viewMainUI.RefreshInfo(PlayerManager.playerMgr.localPlayer);
    //    return 1;
    //}
}
