using UnityEngine;
using System.Collections;

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public partial class SocketHandler : EventDispatcher
{
    public SocketHandler()
    {
        this.addEventListener((int)PACKET_ID_ENUM.ID_NetLoginRes, new EventCallBack1<NetLoginRes>(this.onLogin));
        this.addEventListener((int)PACKET_ID_ENUM.ID_NetCreateRoleReq, new EventCallBack1<NetCreateRoleRes>(this.onCreateRole));
        this.addEventListener((int)PACKET_ID_ENUM.ID_NetSelectRoleReq, new EventCallBack1<NetSelectRoleRes>(this.onSelectRole));

        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetPlayerInfoNotify, this.onPlayerInfoNotify);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetCreateRoomRes, this.onNetCreateRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetEnterRoomRes, this.onNetEnterRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetLeaveRoomRes, this.onNetLeaveRoomRes);
        //this.addEventListener((int)PACKET_ID_ENUM.ID_NetDismissRoomRes, this.onNetDismissRoomRes);
    }
}
