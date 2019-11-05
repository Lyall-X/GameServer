#include "Shared.hpp"

#ifdef __GNUC__

#define CSET_GBK    "GBK"
#define CSET_UTF8   "UTF-8"
#define LC_NAME_zh_CN   "zh_CN"

// ifdef __GNUC__
#elif defined(_MSC_VER)

#define CSET_GBK    "936"
#define CSET_UTF8   "65001"
#define LC_NAME_zh_CN   "Chinese_People's Republic of China"
#define LC_NAME_zh_CN_GBK       LC_NAME_zh_CN "." CSET_GBK
#define LC_NAME_zh_CN_UTF8      LC_NAME_zh_CN "." CSET_UTF8
#define LC_NAME_zh_CN_DEFAULT   LC_NAME_zh_CN_GBK

#endif

LogConsoleOutpter::LogConsoleOutpter(void)
{
#if (defined(WIN32) || defined(WIN64))
	m_hStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	//int hcin = _open_osfhandle ((intptr_t )m_hStdHandle, _O_TEXT);
	//int hcout = _open_osfhandle ((intptr_t )m_hStdHandle, _O_TEXT);
#endif // WIN32
}

LogConsoleOutpter::~LogConsoleOutpter(void)
{
}

void LogConsoleOutpter::Outputer(const char* content, int color, int level)
{
#if (defined(WIN32) || defined(WIN64))
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(m_hStdHandle, &info);

	switch (level)
	{
	case logLevel_info:
		{
			SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		}
		break;
	case logLevel_warn:
		{
			SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		break;
	case logLevel_error:
		{
			SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
		}
		break;
	case logLevel_debug:
		{
			switch (color)
			{
			case LogSystem::csl_color_white:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_b_w_f_g:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_b_w_f_r:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_RED | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_b_w_f_b:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_red:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_RED | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_green:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_blue:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_yellow:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_green_blue:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case LogSystem::csl_color_red_blue:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
				break;
			default:
				SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			}
		}
		break;
	case logLevel_hex:
		{
			SetConsoleTextAttribute(m_hStdHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		break;
	}
#endif

#if (defined(WIN32) || defined(WIN64))
	//setlocale(LC_ALL, LC_NAME_zh_CN_DEFAULT);
	printf(content);
	printf("\r\n");
	//DWORD nRet = 0;
	//WriteConsole(m_hStdHandle, content, lstrlen(content), &nRet, NULL);
	//WriteConsole(m_hStdHandle, "\r\n", 2, &nRet, NULL);
#else
	printf(content);
	printf("\r\n");
#endif

#if (defined(WIN32) || defined(WIN64))
	SetConsoleTextAttribute(m_hStdHandle, info.wAttributes);
#endif
}
