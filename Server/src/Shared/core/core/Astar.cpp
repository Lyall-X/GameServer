#include "Shared.hpp"

#define  MAP_PASS	0

Astar::Astar(void):
mData(NULL),
mRow(0),
mCol(0),
mEndNode(NULL)
{
}

Astar::~Astar(void)
{
	SAFE_DELETE_ARRAY(mData);
}

bool Astar::create(int32 row, int32 col, const char* data)
{
	mList.clear();
	SAFE_DELETE_ARRAY(mData);
	mRow = row;
	mCol = col;
	mData = new asNode[mRow * mCol];

	for (int32 i = 0; i < mRow; ++i)
	{
		for (int32 j = 0; j < mCol; ++j)
		{
			asNode& node = mData[i * mCol + j];

			node.row = i;
			node.col = j;
			node.data = data[i * mCol + j];
			node.f = node.g = node.h = 0;
			node.findState = 0;

			node.parent = NULL;
			node.child = NULL;
		}
	}
	return true;
}

bool Astar::find(int32 startRow, int32 startCol, int32 endRow, int32 endCol)
{
	mList.clear();
	mEndNode = NULL;
	clearData();
	if (!_check_point(startRow, startCol) || !_check_point(endRow, endCol))
		return false;
	mStart.x = startRow;
	mStart.y = startCol;
	mEnd.x = endRow;
	mEnd.y = endCol;

	asNode* currentNode = &(mData[startRow * mCol + startCol]);

	if (currentNode->data != MAP_PASS)
		return false;

	mList.push_back(currentNode);
	Point2 tempPos = {0};
	Point2 curPos = {0};
	asNode* node = NULL;
	while (true)
	{
		//node = this->front(m_openList);
		//this->pop_front(m_openList);
		if (mList.size() <= 0)
			return false;
		node = this->getFminNode();

		curPos.x = node->row;
		curPos.y = node->col;

		for (int32 i = 0; i < 8; ++i)
		{
			switch (i)
			{
			case D_UP:
				tempPos.x = curPos.x - 1;
				tempPos.y = curPos.y;
				this->_find(10, curPos, tempPos);
				//10
				break;
			case D_UPRIGHT:
				tempPos.x = curPos.x - 1;
				tempPos.y = curPos.y + 1;
				this->_find(14, curPos, tempPos);
				//14
				break;
			case D_RIGHT:
				tempPos.x = curPos.x;
				tempPos.y = curPos.y + 1;
				this->_find(10, curPos, tempPos);
				//10
				break;
			case D_DOWNRIGHT:
				tempPos.x = curPos.x + 1;
				tempPos.y = curPos.y + 1;
				this->_find(14, curPos, tempPos);
				//14
				break;
			case D_DOWN:
				tempPos.x = curPos.x + 1;
				tempPos.y = curPos.y;
				this->_find(10, curPos, tempPos);
				//10
				break;
			case D_DOWNLEFT:
				tempPos.x = curPos.x + 1;
				tempPos.y = curPos.y - 1;
				this->_find(14, curPos, tempPos);
				//14
				break;
			case D_LEFT:
				tempPos.x = curPos.x;
				tempPos.y = curPos.y - 1;
				this->_find(10, curPos, tempPos);
				//10
				break;
			case D_UPLEFT:
				tempPos.x = curPos.x - 1;
				tempPos.y = curPos.y - 1;
				this->_find(14, curPos, tempPos);
				//14
				break;
			}

			if (this->_end_point(tempPos.x, tempPos.y))
			{
				mEndNode = &(mData[tempPos.x * mCol + tempPos.y]);//  жу╣Ц;
				return true;
			}
		}
		mData[curPos.x * mCol + curPos.y].findState = FP_CLOSE;
	}

	return false;
}

void Astar::_find(const int32 g, const Point2& curPos, const Point2& checkPos)
{

	if (this->_can_walk(checkPos))
	{
		asNode* checkNode = &(mData[checkPos.x * mCol + checkPos.y]);
		asNode* curNode = &(mData[curPos.x * mCol + curPos.y]);

		if (checkNode->findState == FP_NOWALK)
		{
			checkNode->findState = FP_ISWALK;
			checkNode->parent = curNode;
			curNode->child = checkNode;

			checkNode->g += checkNode->parent->g + g;
			checkNode->h = (abs(mEnd.x - checkPos.x) + abs(mEnd.y - checkPos.y)) * 10;
			checkNode->f = checkNode->g + checkNode->h;
			
			//asNode* head = this->front(m_openList);
			//if (head)
			//{
			//	if (checkNode->f <= head->f)
			//	{
			//		this->push_front(m_openList, checkNode);
			//	}
			//	else
			//	{
			//		this->push_back(m_openList, checkNode);
			//	}
			//}
			//else
			//{
			//	this->push_back(m_openList, checkNode);
			//}			

			mList.push_back(checkNode);
		}
		else if (checkNode->findState == FP_ISWALK)
		{
			if (curNode->g + g < checkNode->g)
			{
				checkNode->g = curNode->g + g;
				checkNode->f = checkNode->g + checkNode->h;
				checkNode->parent = curNode;
				curNode->child = checkNode;
				//this->_check_node(m_openList, checkNode);
			}
		}
	}
}

bool Astar::_can_walk(const Point2& checkPos)
{
	if (!_check_point(checkPos.x, checkPos.y))
		return false;

	asNode& node = mData[checkPos.x * mCol + checkPos.y];
	if (mRow > checkPos.x && mCol > checkPos.y &&
		checkPos.x >= 0 && checkPos.y >= 0)
	{
		if (node.data == MAP_PASS)
			return true;
	}

	return false;
}

bool Astar::_end_point( const int32 row, const int32 col )
{
	if (mEnd.x == row && mEnd.y == col)
		return true;

	return false;
}


Astar::asNode* Astar::end_node()
{
	return mEndNode;
}

bool Astar::points(std::list<Point2>& vecPoints, asNode* node)
{
	asNode* _node = node;
	Point2 pt;
	while (_node)
	{
		pt.x = _node->row;
		pt.y = _node->col;
		vecPoints.push_front(pt);
		_node = _node->parent;
	}
	return true;
}

void Astar::clearData()
{
	for (int32 i = 0; i < mRow; ++i)
	{
		for (int32 j = 0; j < mCol; ++j)
		{
			asNode& node = mData[i * mCol + j];

			node.f = node.g = node.h = 0;
			node.findState = 0;
			node.parent = NULL;
			node.child = NULL;
		}
	}
}

Astar::asNode* Astar::getFminNode()
{
	int32 f = 0x00FFFFFF;
	asNode* reslut = NULL;
	for (auto itr : mList)
	{
		if (itr->f < f)
		{
			f = itr->f;
			reslut = itr;
		}
	}
	
	mList.remove(reslut);
	return reslut;
}

bool Astar::_check_point( const int32 row, const int32 col )
{
	if (row >= 0 && row < mRow &&
		col >= 0 && col < mCol)
	{
		return true;
	}

	return false;
}

