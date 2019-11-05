#include "Shared.hpp"

Bit::Bit(int32 bitCount)
{
	mBitBlock = sizeof(int32) * 8;
	mCount = bitCount / mBitBlock;
	bitCount % mBitBlock == 0 ? mCount : mCount++;
	mCount <= 0 ? mCount = 1 : mCount;
	mBit = new int32[mCount];
	memset(mBit, 0, sizeof(int32) * mCount);
}

Bit::~Bit()
{
	SAFE_DELETE_ARRAY(mBit);
}

bool Bit::GetBit(int32 pos)
{
	int32 idx = pos / mBitBlock;
	int32 p = pos % mBitBlock;
	if (idx >= mCount)
		return false;
	
	return mBit[idx] & BIT(p);
}

void Bit::SetBit(int32 pos, bool bit)
{
	int32 idx = pos / mBitBlock;
	int32 p = pos % mBitBlock;
	if (idx >= mCount)
		return;
	if (bit)
		mBit[idx] = mBit[idx] | BIT(p);
	else
		mBit[idx] = mBit[idx] & ~BIT(p);
}
