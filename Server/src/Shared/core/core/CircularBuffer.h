#pragma once

class CircularBuffer
{
public:
	CircularBuffer(uint32 uiCount = PACKET_MAX_LENGTH);
	CircularBuffer(uint32 uiCount, void* data);
	~CircularBuffer(void);
	char* datas();
	uint32 count();
	uint32 get_data_length();		// 得到当前有效数据长度;
	uint32 get_can_write_length();	// 得到可写数据长度;
	uint32 get_space_length();		// 得到剩余空间长度;
	bool Push(void* ptr, const uint32 uiCount);
	bool Pop(void* ptr, int32 size_, uint32 uiCount);
	bool Read(void* ptr,  int32 size_, uint32 uiCount);
	void Clear();
	uint32 get_write_pos();
	void set_write_pos(uint32 pos);
private:
	char*				m_pBuffer;
	uint32				m_uiCount;
	uint32				m_uiReadPos;
	uint32				m_uiWritePos;
	bool				m_isRelease;
};

