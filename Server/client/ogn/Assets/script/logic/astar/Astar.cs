using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

enum aStarDir{
    D_UP, D_UPRIGHT, D_RIGHT, D_DOWNRIGHT, D_DOWN, D_DOWNLEFT, D_LEFT, D_UPLEFT, D_COUNT,
}
enum FindState{
    FP_NOWALK,FP_ISWALK,FP_CLOSE
}


public class Point2
{
    public int x;
    public int y;
}
public class  asNode
{
    public int row;
    public int col;
    public byte data;
    public byte findState;
    public int h;
    public int g;
    public int f;

    public asNode parent;
    public asNode child;
}

public class Astar
{
    public static int MAP_PASS = 0;
    public asNode[] mData; // [mRow][mCol]
    public int mRow, mCol;
    public Point2 mStart = new Point2();
    public Point2 mEnd = new Point2();
    public asNode mEndNode;
    public List<asNode> mList = new List<asNode>();

    public Astar()
    {

    }

    public bool create(int row, int col, byte[] bytes)
    {
        mList.Clear();
        mRow = row;
        mCol = col;
        mData = new asNode[mRow * mCol];

        for (int i = 0; i < mRow; ++i)
        {
            for (int j = 0; j < mCol; ++j)
            {
                asNode node = new asNode();
                mData[i * mCol + j] = node;
                node.row = i;
                node.col = j;
                node.data = bytes[i * mCol + j];
                node.f = node.g = node.h = 0;
                node.findState = 0;

                node.parent = null;
                node.child = null;
            }
        }

        return true;
    }

    Point2 tempPos = new Point2();
    Point2 curPos = new Point2();

    public bool find(int startRow, int startCol, int endRow, int endCol)
    {
        mList.Clear();
        mEndNode = null;
        clearData();
        if (!_check_point(startRow, startCol) || !_check_point(endRow, endCol))
            return false;
        mStart.x = startRow;
        mStart.y = startCol;
        mEnd.x = endRow;
        mEnd.y = endCol;

        asNode currentNode = mData[startRow * mCol + startCol];

        if (currentNode.data != MAP_PASS)
            return false;

        mList.Add(currentNode);
        asNode node = null;
        while (true)
        {
            //node = this->front(m_openList);
            //this->pop_front(m_openList);
            if (mList.Count <= 0)
                return false;
            node = this.getFminNode();

            curPos.x = node.row;
            curPos.y = node.col;

            for (int i = 0; i < 8; ++i)
            {
                switch ((aStarDir)i)
                {
                    case aStarDir.D_UP:
                        tempPos.x = curPos.x - 1;
                        tempPos.y = curPos.y;
                        this._find(10, curPos, tempPos);
                        //10
                        break;
                    case aStarDir.D_UPRIGHT:
                        tempPos.x = curPos.x - 1;
                        tempPos.y = curPos.y + 1;
                        this._find(14, curPos, tempPos);
                        //14
                        break;
                    case aStarDir.D_RIGHT:
                        tempPos.x = curPos.x;
                        tempPos.y = curPos.y + 1;
                        this._find(10, curPos, tempPos);
                        //10
                        break;
                    case aStarDir.D_DOWNRIGHT:
                        tempPos.x = curPos.x + 1;
                        tempPos.y = curPos.y + 1;
                        this._find(14, curPos, tempPos);
                        //14
                        break;
                    case aStarDir.D_DOWN:
                        tempPos.x = curPos.x + 1;
                        tempPos.y = curPos.y;
                        this._find(10, curPos, tempPos);
                        //10
                        break;
                    case aStarDir.D_DOWNLEFT:
                        tempPos.x = curPos.x + 1;
                        tempPos.y = curPos.y - 1;
                        this._find(14, curPos, tempPos);
                        //14
                        break;
                    case aStarDir.D_LEFT:
                        tempPos.x = curPos.x;
                        tempPos.y = curPos.y - 1;
                        this._find(10, curPos, tempPos);
                        //10
                        break;
                    case aStarDir.D_UPLEFT:
                        tempPos.x = curPos.x - 1;
                        tempPos.y = curPos.y - 1;
                        this._find(14, curPos, tempPos);
                        //14
                        break;
                }

                if (this._end_point(tempPos.x, tempPos.y))
                {
                    mEndNode = mData[tempPos.x * mCol + tempPos.y];//  终点;
                    return true;
                }
            }
            mData[curPos.x * mCol + curPos.y].findState = (byte)FindState.FP_CLOSE;
        }

        return true;
    }

    public asNode endNode()
    {
        return mEndNode;
    }

    public static bool points(List<Point2> lstPoints, asNode node)
    {
        asNode _node = node;
        while (_node != null)
        {
            Point2 pt = new Point2();
            pt.x = _node.row;
            pt.y = _node.col;
            lstPoints.Insert(0, pt);
            _node = _node.parent;
        }
        return true;
    }

    protected void _find(int g, Point2 curPos, Point2 checkPos)
    {
        if (this._can_walk(checkPos))
        {
            asNode checkNode = mData[checkPos.x * mCol + checkPos.y];
            asNode curNode = mData[curPos.x * mCol + curPos.y];

            if (checkNode.findState == (byte)FindState.FP_NOWALK)
            {
                checkNode.findState = (byte)FindState.FP_ISWALK;
                checkNode.parent = curNode;
                curNode.child = checkNode;

                checkNode.g += checkNode.parent.g + g;
                checkNode.h = (Math.Abs(mEnd.x - checkPos.x) + Math.Abs(mEnd.y - checkPos.y)) * 10;
                checkNode.f = checkNode.g + checkNode.h;

                mList.Add(checkNode);
            }
            else if (checkNode.findState == (byte)FindState.FP_ISWALK)
            {
                if (curNode.g + g < checkNode.g)
                {
                    checkNode.g = curNode.g + g;
                    checkNode.f = checkNode.g + checkNode.h;
                    checkNode.parent = curNode;
                    curNode.child = checkNode;
                }
            }
        }
    }

    protected bool _can_walk(Point2 checkPos)
    {
        if (!_check_point(checkPos.x, checkPos.y))
            return false;
        asNode node = mData[checkPos.x * mCol + checkPos.y];

        if (mRow > checkPos.x && mCol > checkPos.y &&
            checkPos.x >= 0 && checkPos.y >= 0)
        {
            if (node.data == MAP_PASS)
                return true;
        }

        return false;
    }

    protected bool _end_point(int row, int col)
    {
        if (mEnd.x == row && mEnd.y == col)
            return true;

        return false;
    }

    protected asNode getFminNode()
    {
        int f = 0x00FFFFFF;
	    asNode reslut = null;
        foreach(asNode node in mList)
        {
            if (node.f < f)
            {
                f = node.f;
                reslut = node;
            }
        }

        mList.Remove(reslut);
	    return reslut;
    }

    protected bool _check_point(int row, int col)
    {
        if (row >= 0 && row < mRow &&
            col >= 0 && col < mCol)
        {
            return true;
        }

        return false;
    }
    protected void clearData()
    {
        for (int i = 0; i < mRow; ++i)
        {
            for (int j = 0; j < mCol; ++j)
            {
                asNode  node = mData[i * mCol + j];

                node.f = node.g = node.h = 0;
                node.findState = 0;
                node.parent = null;
                node.child = null;
            }
        }
    }
}


