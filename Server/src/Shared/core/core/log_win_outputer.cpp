#include "Shared.hpp"

void LogWinOutputer::Outputer(const char* content, int color, int level)
{
#if (defined(WIN32) || defined(WIN64))

	switch (level)
	{
	case logLevel_info:
		{
			OutputDebugStringA("INFO:  ");
		}
		break;
	case logLevel_warn:
		{
			OutputDebugStringA("WARN:  ");
		}
		break;
	case logLevel_error:
		{
			OutputDebugStringA("ERROR: ");
		}
		break;
	case logLevel_debug:
		{
			OutputDebugStringA("DEBUG: ");
		}
		break;
	}

	OutputDebugStringA(content);
	OutputDebugStringA("\r\n");
#endif
}
