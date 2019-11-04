using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class Npc : Entity
{
    protected int mNpcId;

    public int NpcId
    {
        get { return mNpcId; }
        set { mNpcId = value; }
    }
    public Npc()
    {

    }
}
