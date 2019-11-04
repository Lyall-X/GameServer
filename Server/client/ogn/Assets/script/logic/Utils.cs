using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
public class Utils
{
    public static string IntToString(int v, int count, char c)
    {
        string s = v.ToString();
        int len = s.Length;
        for (int i = 0; i < count - len; ++i)
            s = c + s;
        return s;
    }

    public static Point2 Vector2ToCellPoint2(Vector2 v, float w, float h)
    {
        Point2 pos = new Point2();
        pos.x = (int)Mathf.Floor(-v.y / h);
        pos.y = (int)Mathf.Floor(v.x / w);
        return pos;
    }

    public static Point2 Vector2ToCellPoint2(float x, float y, float w, float h)
    {
        Point2 pos = new Point2();
        pos.x = (int)Mathf.Floor(-y / h);
        pos.y = (int)Mathf.Floor(x / w);
        return pos;
    }

    public static Vector2 CellPoint2ToVector2(Point2 p, float w, float h)
    {
        Vector2 pos = new Vector2();
        pos.x = p.y * w + 0.5f * w;
        pos.y = -p.x * h - 0.5f * h;
        return pos;
    }

    public static Vector2 CellPoint2ToVector2(int x, int y, float w, float h)
    {
        Vector2 pos = new Vector2();
        pos.x = y * w + 0.5f * w;
        pos.y = -x * h - 0.5f * h;
        return pos;
    }

    public static bool Clamp(float tar, float v0, float v1)
    {
        return tar >= v0 && tar < v1;
    }
}
