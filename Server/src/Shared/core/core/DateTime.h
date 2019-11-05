#pragma once

typedef struct win_time_val
{
    /** The seconds part of the time. */  
	int32    sec;
    /** The miliseconds fraction of the time. */  
	int32    msec;
} win_time_val_t;

typedef struct win_time
{  
    /** This represents day of week where value zero means Sunday */  
    int32 wday;
  
    /** This represents day of month: 1-31 */  
	int32 day;
  
    /** This represents month, with the value is 0 - 11 (zero is January) */  
	int32 mon;
  
    /** This represent the actual year (unlike in ANSI libc where 
     *  the value must be added by 1900). 
     */  
	int32 year;
  
    /** This represents the second part, with the value is 0-59 */  
	int32 sec;
  
    /** This represents the minute part, with the value is: 0-59 */  
	int32 min;

    /** This represents the hour part, with the value is 0-23 */  
	int32 hour;

    /** This represents the milisecond part, with the value is 0-999 */  
	int32 msec;
  
} win_time_t;

int gettimeofday(win_time_val_t *tv);

class DateTime
{
public:
	DateTime(void);
	DateTime(const DateTime& other);
	DateTime(const tm& t);
	~DateTime(void);
public:
	DateTime& operator = (const tm& t);
	DateTime& operator = (const DateTime& other);
public:
	static void updateFrame();
	static double GetDelay();
	static double GetTotalTime();
public:
	void toString(std::string& str);
	DateTime addYear(int32 year);
	DateTime addMonth(int32 month);
	DateTime addDay(float32 day);
	DateTime addHour(float32 hour);
	DateTime addMinute(float32 minute);
	DateTime addSecond(float32 second);
	DateTime addMillisecond(float32 millisecond);
	tm& getDataTime();
public:
	static float64 DateTime::GetNowAppUS();
	static uint32 Now();
	static uint32 Now(DateTime& time);
	static uint32 Now(std::string& time);

	static int64 getNowMillisecond();
	static void getNowMillisecond(std::string& time);
	static time_t ConvertStringToTime(const std::string& timestr);
	static int32 ConvertTimeToString(std::string& timestr, const time_t& timedat);
private:
	tm			m_time;
	static double sTime;
	static double sDelay;
};

