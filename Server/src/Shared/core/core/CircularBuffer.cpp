#include "Shared.hpp"
#include <memory>

CircularBuffer::CircularBuffer(uint32 uiCount /* = 8192 */):
m_uiCount(uiCount),
m_isRelease(true),
m_uiReadPos(0),
m_uiWritePos(0)
{
	m_pBuffer = (char*)malloc(m_uiCount);
	memset(m_pBuffer, 0, m_uiCount);
}

CircularBuffer::CircularBuffer(uint32 uiCount, void* data):
m_isRelease(false),
m_uiCount(uiCount),
m_pBuffer((char*)data),
m_uiReadPos(0),
m_uiWritePos(0)
{
	memset(m_pBuffer, 0, m_uiCount);
}

CircularBuffer::~CircularBuffer(void)
{
	if (m_isRelease)
	{
		SAFE_FREE(m_pBuffer);
	}

	m_uiCount = 0;
	m_uiReadPos = 0;
	m_uiWritePos = 0;
}

char* CircularBuffer::datas()
{
	return m_pBuffer;
}

uint32 CircularBuffer::count()
{
	return m_uiCount;
}

uint32 CircularBuffer::get_data_length()
{
	if (m_uiReadPos == m_uiWritePos)
	{
		return 0;
	}

	if (m_uiReadPos > m_uiWritePos)
	{
		return m_uiCount - m_uiReadPos + m_uiWritePos;
	}

	return m_uiWritePos - m_uiReadPos;
}

bool CircularBuffer::Push(void* ptr, const uint32 uiCount)
{
	if (uiCount == 0) return false;

	if (m_uiCount - get_data_length() < uiCount)
	{
		// 空间不足;
		return false;
	}

	if (m_uiWritePos < m_uiReadPos)
	{
		memcpy(m_pBuffer + m_uiWritePos, ptr, uiCount);
		m_uiWritePos += uiCount;
		m_uiWritePos = (m_uiWritePos >= m_uiCount) ? 0 : m_uiWritePos;
		return true;
	}
	else
	{
		if (m_uiCount - m_uiWritePos >= uiCount)
		{
			memcpy(m_pBuffer + m_uiWritePos, ptr, uiCount);
			m_uiWritePos += uiCount;
			m_uiWritePos = (m_uiWritePos >= m_uiCount) ? 0 : m_uiWritePos;
			return true;
		}
		else
		{
			uint32 uiSize = m_uiCount - m_uiWritePos;
			memcpy(m_pBuffer + m_uiWritePos, ptr, uiSize);
			m_uiWritePos = 0;
			memcpy(m_pBuffer + m_uiWritePos, (char*)ptr + uiSize, uiCount - uiSize);
			m_uiWritePos += uiCount - uiSize;
			m_uiWritePos = (m_uiWritePos >= m_uiCount) ? 0 : m_uiWritePos;

			return true;
		}
	}

	return false;
}

bool CircularBuffer::Pop(void* ptr, int32 size_, uint32 uiCount)
{
	if (uiCount == 0) return false;

	if (size_ < uiCount)
	{
		return false;
	}

	if (get_data_length() < uiCount)
	{
		// 缓冲区中没有这么多有效数据;
		return false;
	}

	if (m_uiWritePos > m_uiReadPos)
	{
		memcpy(ptr, m_pBuffer + m_uiReadPos, uiCount);
		m_uiReadPos += uiCount;
		m_uiReadPos = (m_uiReadPos >= m_uiCount) ? 0 : m_uiReadPos;
		if (m_uiReadPos == m_uiWritePos)
		{
			m_uiReadPos = m_uiWritePos = 0;
		}
		return true;
	}
	else
	{
		if (m_uiCount - m_uiReadPos >= uiCount)
		{
			memcpy(ptr, m_pBuffer + m_uiReadPos, uiCount);
			m_uiReadPos += uiCount;
			m_uiReadPos = (m_uiReadPos >= m_uiCount) ? 0 : m_uiReadPos;
			if (m_uiReadPos == m_uiWritePos)
			{
				m_uiReadPos = m_uiWritePos = 0;
			}
			return true;
		}
		else
		{
			uint32 uiSize = m_uiCount - m_uiReadPos;
			if (uiSize > uiCount)
			{
				return false;
			}

			memcpy(ptr, m_pBuffer + m_uiReadPos, uiSize);
			m_uiReadPos = 0;
			if (uiCount - uiSize == 0)
			{
				return false;
			}
			memcpy((char*)ptr + uiSize, m_pBuffer + m_uiReadPos, uiCount - uiSize);
			m_uiReadPos += uiCount - uiSize;
			m_uiReadPos = (m_uiReadPos >= m_uiCount) ? 0 : m_uiReadPos;

			if (m_uiReadPos == m_uiWritePos)
			{
				m_uiReadPos = m_uiWritePos = 0;
			}
			return true;
		}
	}

	return false;
}

bool CircularBuffer::Read(void* ptr, int32 size_, uint32 uiCount)
{
	if (uiCount == 0) return false;

	if (size_ < uiCount)
	{
		return false;
	}

	if (get_data_length() < uiCount)
	{
		// 缓冲区中没有这么多有效数据;
		return false;
	}

	if (m_uiWritePos > m_uiReadPos)
	{
		memcpy(ptr, m_pBuffer + m_uiReadPos, uiCount);
		return true;
	}
	else
	{
		if (m_uiCount - m_uiReadPos >= uiCount)
		{
			memcpy(ptr, m_pBuffer + m_uiReadPos, uiCount);
			return true;
		}
		else
		{
			uint32 uiSize = m_uiCount - m_uiReadPos;
			memcpy(ptr, m_pBuffer + m_uiReadPos, uiSize);
			memcpy((char*)ptr + uiSize, m_pBuffer, uiCount - uiSize);
			return true;
		}
	}
	return false;
}

void CircularBuffer::Clear()
{
	m_uiReadPos = 0;
	m_uiWritePos = 0;
	memset(m_pBuffer, 0, m_uiCount);
}

uint32 CircularBuffer::get_can_write_length()
{
	if (m_uiReadPos > m_uiWritePos)
	{
		return m_uiReadPos - m_uiWritePos;
	}
	else if (m_uiReadPos < m_uiWritePos)
	{
		return m_uiCount - m_uiWritePos;
	}
	else
	{
		if (m_uiReadPos == 0)
		{
			return m_uiCount;
		}
	}

	return 0;
}

uint32 CircularBuffer::get_write_pos()
{
	return m_uiWritePos;
}

void CircularBuffer::set_write_pos( uint32 pos )
{
	m_uiWritePos = pos;
}

uint32 CircularBuffer::get_space_length()
{
	return m_uiCount - get_data_length();
}
