using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class Player : Entity{
    protected uint mAccId;
    protected uint mUserId;
    public uint AccId
    {
        get { return mAccId; }
        set { mAccId = value; }
    }
    public uint UserId
    {
        get { return mUserId; }
        set { mUserId = value; }
    }

}
