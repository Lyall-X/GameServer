#include "Shared.hpp"

LogFileOutputer::LogFileOutputer()
{
	tm plt;
	time_t t = time(NULL);
	localtime_s(&plt, &t);
	strftime(mBuffer, 256, "%Y_%m_%d_%H_%M_%S", &plt);
	mFirstPath = mBuffer;
	mIdx = 1;
	sprintf_s(mBuffer, 256, "log/%s_%d.log", mFirstPath.c_str(), mIdx);
	Shared::CreateDirectory("log");
}

LogFileOutputer::~LogFileOutputer(void)
{
}

void LogFileOutputer::Outputer(const char* content, int color /* = 0 */, int level /* = logLevel_info */)
{
	sprintf_s(mBuffer, 256, "log/%s_%d.log", mFirstPath.c_str(), mIdx);
	FILE* file = fopen(mBuffer, "a");
	int32 err = GetLastError();
	if (file == NULL) return;
	fwrite(content, strlen(content), 1, file);
	fwrite("\r", 1, 1, file);
	if (Shared::GetFileLength(file) > 1024 * 1024)
		mIdx++;
	fclose(file);
}
