using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

public class Singleton<TYPE> where TYPE : new()
{
    public static TYPE Instance = new TYPE();
}
