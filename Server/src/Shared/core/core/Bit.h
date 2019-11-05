#pragma once

class Bit
{
public:
	Bit(int32 bitCount = 1);
	~Bit();
	bool GetBit(int32 pos);
	void SetBit(int32 pos, bool bit);
protected:
	int32* mBit;
	int32 mCount;
	int32 mBitBlock;
};