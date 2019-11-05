#include "Shared.hpp"


char global_dump_file_name[MAX_PATH] = "dump_file.dmp";

LONG WINAPI CustomUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	tm _tm = {};
	time_t t = time(NULL);
	localtime_s(&_tm, &t);
	strftime(global_dump_file_name, MAX_PATH, "%Y%m%d_%H%M%S.dmp", &_tm);
	HANDLE lhDumpFile = CreateFileA(global_dump_file_name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL ,NULL);
	if ((lhDumpFile != NULL) && (lhDumpFile != INVALID_HANDLE_VALUE))
	{
		MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
		loExceptionInfo.ExceptionPointers = ExceptionInfo;
		loExceptionInfo.ThreadId = GetCurrentThreadId();
		loExceptionInfo.ClientPointers = TRUE;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);

		CloseHandle(lhDumpFile);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

Dump::Dump(const char *name)
{
	SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
}

Dump::~Dump()
{
	SetUnhandledExceptionFilter(NULL);
}