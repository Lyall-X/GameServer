#include "Shared.hpp"

#define SECS_TO_FT_MULT 10000000  
static LARGE_INTEGER base_time;

double DateTime::sDelay = 0.0;
double DateTime::sTime = DateTime::GetNowAppUS();
// Find 1st Jan 1970 as a FILETIME
static void get_base_time(LARGE_INTEGER *base_time)  
{  
    SYSTEMTIME st;  
    FILETIME ft;  

    memset(&st,0,sizeof(st));
    st.wYear=1970;
    st.wMonth=1;
    st.wDay=1;
    SystemTimeToFileTime(&st, &ft);
    base_time->LowPart = ft.dwLowDateTime;
    base_time->HighPart = ft.dwHighDateTime;
    base_time->QuadPart /= SECS_TO_FT_MULT;
}

int32 gettimeofday(win_time_val_t *tv)
{  
    SYSTEMTIME st;
    FILETIME ft;
    LARGE_INTEGER li;
	static int8 get_base_time_flag=0;

    if (get_base_time_flag == 0)
        get_base_time(&base_time);

    /* Standard Win32 GetLocalTime */  
    GetLocalTime(&st);
    SystemTimeToFileTime(&st, &ft);

    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    li.QuadPart /= SECS_TO_FT_MULT;
    li.QuadPart -= base_time.QuadPart;

    tv->sec = li.LowPart;
    tv->msec = st.wMilliseconds;

    return 0;
}
  
int32 win_time(const win_time_val_t *tv, win_time_t *time)
{
    LARGE_INTEGER li;
    FILETIME ft;
    SYSTEMTIME st;
  
    li.QuadPart = tv->sec;
    li.QuadPart += base_time.QuadPart;
    li.QuadPart *= SECS_TO_FT_MULT;

    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;
    FileTimeToSystemTime(&ft, &st);

    time->year = st.wYear;
    time->mon = st.wMonth-1;
    time->day = st.wDay;
    time->wday = st.wDayOfWeek;

    time->hour = st.wHour;
    time->min = st.wMinute;
    time->sec = st.wSecond;
    time->msec = tv->msec;

    return 0;
}  

bool isLeapYear(int32 years)
{
	if (years % 4 == 0 && years % 100 != 0)
	{
		return true;
	}
	else if (years % 400 == 0)
	{
		return true;
	}

	return false;
}

static int g_MonthToDays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int32 getDaysToMonth(int32 years, int32 month)
{
	if (month > 12)
	{
		return 0;
	}

	if (!isLeapYear(years))
	{
		return g_MonthToDays[month - 1];
	}
	
	if (month == 2)
	{
		return 29;
	}

	return g_MonthToDays[month - 1];
}

DateTime::DateTime(void)
{
	memset(&m_time, 0, sizeof(m_time));
}

DateTime::DateTime( const DateTime& other )
{

}

DateTime::DateTime( const tm& t )
{

}

DateTime::~DateTime(void)
{
}

DateTime& DateTime::operator=(const tm& t)
{
	memcpy(&m_time, &t, sizeof(t));
	return *this;
}

DateTime& DateTime::operator=(const DateTime& other)
{
	memcpy(&m_time, &other.m_time, sizeof(tm));
	return *this;
}

double DateTime::GetNowAppUS()
{
#ifdef WIN32
	LARGE_INTEGER freq;
	LARGE_INTEGER counter;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&counter);
	return ((double)counter.QuadPart / freq.QuadPart);
#else
	timeval tv; 
	gettimeofday(&tv, NULL); 
	return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
#endif
}

uint32 DateTime::Now()
{
	return (int32)time(0);
}

uint32 DateTime::Now(DateTime& time)
{
	time_t t = (time_t)Now();
	tm localTime;
	localtime_s(&localTime, &t);
	time = localTime;
	return t;
}

uint32 DateTime::Now(std::string& time)
{
	DateTime dt;
	uint32 t = Now(dt);
	dt.toString(time);
	return t;
}

__int64 DateTime::getNowMillisecond()
{
	//tm tm_;
	//unsigned int msec = _getsystime(&tm_);
	//SYSTEMTIME st;
	//GetLocalTime(&st);
	//int sec = mktime(&tm_);
	//return sec * 1000.f + msec;
	return 0;
}

void DateTime::getNowMillisecond( std::string& time )
{
	__int64 time_ = DateTime::getNowMillisecond();
	char time_buf[64] = {0};
	sprintf_s(time_buf, 64, "%I64d", time_);
	time = time_buf;
}

void DateTime::updateFrame()
{
	sDelay = DateTime::GetNowAppUS() - sTime;
	sTime = DateTime::GetNowAppUS();
}

double DateTime::GetDelay()
{
	return sDelay;
}

double DateTime::GetTotalTime()
{
	return sTime;
}

void DateTime::toString(std::string& str)
{
	char szTime[32] = {0};
	if (str.length() <= 0)
		strftime(szTime, 256, "%Y-%m-%d %H:%M:%S", &m_time);
	else
		strftime(szTime, 256, str.c_str(), &m_time);
	str = szTime;
}

DateTime DateTime::addYear(int32 year)
{
	DateTime relust(*this);
	tm time = relust.getDataTime();
	time.tm_year += year;
	return relust;
}

DateTime DateTime::addMonth( int32 month )
{
	DateTime relust(*this);
	tm time = relust.getDataTime();

	if (month + time.tm_mon > 13)
	{
		time.tm_year += month / 13;
		time.tm_mon = 1;
	}
	else
	{
		time.tm_mon++;
	}

	return relust;
}

DateTime DateTime::addDay(float32 day)
{
	DateTime relust(*this);
	tm time = relust.getDataTime();

	if (day >= 1.0f)
	{

	}
	else
	{

	}

	return relust;
}

tm& DateTime::getDataTime()
{
	return m_time;
}

DateTime DateTime::addHour( float32 hour )
{
	return *this;
}

DateTime DateTime::addMinute(float32 minute )
{
	return *this;
}

DateTime DateTime::addSecond(float32 second )
{
	return *this;
}

DateTime DateTime::addMillisecond(float32 millisecond )
{
	return *this;
}

time_t DateTime::ConvertStringToTime(const std::string& timestr)
{  
	struct tm tm1;  
	time_t time1;  
	int i = sscanf_s(timestr.c_str(), "%d/%d/%d %d:%d:%d" ,
		&(tm1.tm_year),   
		&(tm1.tm_mon),   
		&(tm1.tm_mday),  
		&(tm1.tm_hour),  
		&(tm1.tm_min),  
		&(tm1.tm_sec));

	tm1.tm_year -= 1900;  
	tm1.tm_mon --;  
	tm1.tm_isdst=-1;  
	time1 = mktime(&tm1);

	return time1;  
}

int32 DateTime::ConvertTimeToString(std::string& timestr, const time_t& timedat)
{
	if (timedat <= 0)
		return 0;

	char buffer[64] = {0};
	struct tm p = { 0 };

	int32 res = localtime_s(&p, &timedat);
	if (res == 0)
	{
		p.tm_year = p.tm_year + 1900;
		p.tm_mon = p.tm_mon + 1;

		sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %d:%02d:%02d", p.tm_year, p.tm_mon, p.tm_mday, p.tm_hour, p.tm_min, p.tm_sec);
		timestr = buffer;
	}
	else
	{
		sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %d:%02d:%02d", 0, 0, 0, 0, 0, 0);
		timestr = buffer;
	}
	
	return 0;
}