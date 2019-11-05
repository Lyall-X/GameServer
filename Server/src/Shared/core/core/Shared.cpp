#include "Shared.hpp"

union bit2
{
	int16 data1;
	int8 data2[2];
};

union bit4
{
	int32 data1;
	int8 data2[4];
	float32 data3;
};

union bit8
{
	int64 data1;
	int8 data2[8];
	float64 data3;
};

bool Shared::bitlocal()
{
	bit2 data;
	data.data1 = 0x01;
	return (data.data2[0] == 0);
}

bool Shared::bitswap(const int8* src, int32 s_len, int8* dest, int32 d_len )
{
	if (s_len != d_len)
		return false;
	for (int32 i = 0; i < s_len; ++i)
		dest[i] = src[s_len - 1 - i];
	return true;
}

short Shared::htons(int16 s)
{
	if (bitlocal())
		return s;
	bit2 data;
	bitswap((const char*)&s, sizeof(s), data.data2, 2);
	return data.data1;
}

short Shared::ntohs(int16 s)
{
	if (bitlocal())
		return s;

	bit2 data;
	bitswap((const int8*)&s, sizeof(s), data.data2, 2);
	return data.data1;
}

int Shared::htonl(int32 l)
{
	if (bitlocal())
		return l;
	bit4 data;
	bitswap((const int8*)&l, sizeof(l), data.data2, 4);
	return data.data1;
}

int32 Shared::ntohl( int32 l )
{
	if (bitlocal())
		return l;
	bit4 data;
	bitswap((const int8*)&l, sizeof(l), data.data2, 4);
	return data.data1;
}

int64 Shared::htonll(int64 ll )
{
	if (bitlocal())
		return ll;
	bit8 data;
	bitswap((const int8*)&ll, sizeof(ll), data.data2, 8);
	return data.data1;
}

int64 Shared::ntohll(int64 ll )
{
	if (bitlocal())
		return ll;
	bit8 data;
	bitswap((const int8*)&ll, sizeof(ll), data.data2, 8);
	return data.data1;
}

float32 Shared::htonf(float32 f )
{
	if (bitlocal())
		return f;

	bit4 data;
	bitswap((const int8*)&f, sizeof(f), data.data2, 4);
	return data.data3;
}

float32 Shared::ntohf(float32 f )
{
	if (bitlocal())
		return f;

	bit4 data;
	bitswap((const int8*)&f, sizeof(f), data.data2, 4);
	return data.data3;
}

float64 Shared::htond(float64 d)
{
	if (bitlocal())
		return d;

	bit8 data;
	bitswap((const int8*)&d, sizeof(d), data.data2, 8);
	return data.data3;
}

float64 Shared::ntohd(float64 d)
{
	if (bitlocal())
		return d;

	bit8 data;
	bitswap((const int8*)&d, sizeof(d), data.data2, 8);
	return data.data3;
}

void Shared::split(const std::string& str, std::vector<std::string>& dest, const std::string& separator)
{
	std::string::size_type last = 0;
	std::string::size_type index = str.find(separator, last);
	while (index != std::string::npos)
	{
		dest.push_back(str.substr(last, index - last));
		last = index + separator.length();
		index = str.find(separator, last);
	}
	dest.push_back(str.substr(last, str.length() - last));
}

std::string& Shared::replace(std::string& str, const std::string& oldstr, const std::string& newstr)
{
	std::string::size_type pos = str.find(oldstr);
	if (pos != std::string::npos)
		str.replace(pos, oldstr.length(), newstr);
	return str;
}

std::string& Shared::replace_all(std::string& str, const std::string& oldstr, const std::string& newstr)
{
	while (true) {
		std::string::size_type  pos = str.find(oldstr);
		if (pos != std::string::npos)
			str.replace(pos, oldstr.length(), newstr);
		else break;
	}
	return str;
}

std::string Shared::int32tostr(int32 val)
{
	int8 buf[32] = { 0 };
	sprintf_s(buf, 32, "%d", val);
	return std::string(buf);
}

std::string Shared::uint32tostr(uint32 val)
{
	int8 buf[32] = { 0 };
	sprintf_s(buf, 32, "%u", val);
	return std::string(buf);
}

std::string Shared::int64tostr(int64 val)
{
	int8 buf[64] = {0};
	sprintf_s(buf, 64, "%I64d", val);
	return std::string(buf);
}

std::string Shared::uint64tostr(uint64 val)
{
	int8 buf[64] = { 0 };
	sprintf_s(buf, 64, "%llu", val);
	return std::string(buf);
}

std::string Shared::float32tostr(float32 val)
{
	int8 buf[32] = { 0 };
	sprintf_s(buf, 32, "%f", val);
	return std::string(buf);
}

std::string Shared::float64tostr(float64 val)
{
	int8 buf[64] = { 0 };
	sprintf_s(buf, 64, "%f", val);
	return std::string(buf);
}

int32 Shared::strtoint32(const std::string& val)
{
	return ::atoi(val.c_str());
}

uint32 Shared::strtouint32(const std::string& val)
{
	return (uint32)::atoi(val.c_str());
}

int64 Shared::strtoint64(const std::string& val)
{
	return ::_atoi64(val.c_str());
}


uint64 Shared::strtouint64(const std::string& val)
{
	return (uint64)::_atoi64(val.c_str());
}

float32 Shared::strtofloat32(const std::string& val)
{
	return ::atof(val.c_str());
}

float64 Shared::strtofloat64(const std::string& val)
{
	return ::atof(val.c_str());
}

std::string Shared::hosttoaddr(const std::string& host)
{
	//uint32 addr = inet_addr(host);
	//if (addr == INADDR_NONE)
	//{
	//	//getaddrinfo(host, )
	//	hostent *pHost = gethostbyname(host.c_str());
	//	char host_ip[20] = {0};

	//	if(pHost != NULL)
	//	{
	//		if (pHost->h_addr_list[0])
	//			memcpy(host_ip, inet_ntoa(*((struct in_addr*)pHost->h_addr_list[0])), 20);
	//	}
	//	return host_ip;
	//}

	return host;
}

std::string Shared::localname()
{
	int8 szBuffer[MAX_PATH] = {0};
	gethostname(szBuffer, MAX_PATH);
	return std::string(szBuffer);
}

std::string Shared::localmac()
{
	int8 mac[MAX_PATH]={0};
#ifdef WIN32
	NCB ncb;
	typedef struct _ASTAT_
	{
		ADAPTER_STATUS   adapt;
		NAME_BUFFER   NameBuff   [30];
	}ASTAT,*PASTAT;
	ASTAT Adapter;
	typedef struct _LANA_ENUM
	{
		UCHAR   length;
		UCHAR   lana[MAX_LANA];
	}LANA_ENUM;
	LANA_ENUM lana_enum;
	UCHAR uRetCode;
	memset(&ncb, 0, sizeof(ncb));
	memset(&lana_enum, 0, sizeof(lana_enum));    
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (unsigned char *)&lana_enum;
	ncb.ncb_length = sizeof(LANA_ENUM);
	uRetCode = Netbios(&ncb);
	if(uRetCode != NRC_GOODRET)     
		return "";     
	for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET;
		ncb.ncb_lana_num = lana_enum.lana[lana];
		uRetCode = Netbios(&ncb); 
		if(uRetCode == NRC_GOODRET)
			break; 
	} 
	if(uRetCode != NRC_GOODRET)
		return "";
	memset(&ncb, 0, sizeof(ncb));
	ncb.ncb_command = NCBASTAT;
	ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy_s((char*)ncb.ncb_callname, 1, "*");
	ncb.ncb_buffer = (unsigned char *)&Adapter;
	ncb.ncb_length = sizeof(Adapter);
	uRetCode = Netbios(&ncb); 
	if(uRetCode != NRC_GOODRET)
		return "";
	sprintf_s(mac, 200,"%02X:%02X:%02X:%02X:%02X:%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);
#endif
	return std::string(mac);
}

std::string Shared::GBKToUTF8(const std::string& str)
{
	std::string result;
	WCHAR *strSrc;
	CHAR *szRes;

	//获得临时变量的大小;
	int i = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, strSrc, i);

	//获得临时变量的大小;
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	int j = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

std::string Shared::UTF8ToGBK(const std::string& str)
{
	std::string result;
	WCHAR *strSrc;
	CHAR *szRes;

	//获得临时变量的大小;
	int i = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	strSrc = new WCHAR[i + 1];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, strSrc, i);

	//获得临时变量的大小;
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i + 1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete [] strSrc;
	delete [] szRes;

	return result;
}


static const char safe[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  

std::string Shared::encode(const std::string &uri)
{  
	std::string ret;   
	const uint8 *ptr = (const uint8 *)uri.c_str();
	ret.reserve(uri.length());  

	for (; *ptr ; ++ptr)  
	{  
		if (!safe[*ptr])   
		{  
			int8 buf[5];  
			memset(buf, 0, 5);  
#ifdef WIN32  
			_snprintf_s(buf, 5, "%%%X", (*ptr));  
#else  
			snprintf(buf, 5, "%%%X", (*ptr));  
#endif  
			ret.append(buf);      
		}  
		else if (*ptr==' ')  
		{  
			ret+='+';  
		}  
		else{  
			ret += *ptr;  
		}  
	}  
	return ret;  
}  

std::string Shared::decode(const std::string &uri)
{  
	//Note from RFC1630:  "Sequences which start with a percent sign  
	//but are not followed by two hexadecimal characters (0-9,A-F) are reserved  
	//for future extension"  
	const uint8 *ptr = (const uint8 *)uri.c_str();
	std::string ret;  
	ret.reserve(uri.length());  
	for (; *ptr; ++ptr)  
	{  
		if (*ptr == '%')  
		{  
			if (*(ptr + 1))  
			{  
				int8 a = *(ptr + 1);  
				int8 b = *(ptr + 2);
				if (!((a >= 0x30 && a < 0x40) || (a >= 0x41 && a < 0x47))) continue;  
				if (!((b >= 0x30 && b < 0x40) || (b >= 0x41 && b < 0x47))) continue;  
				int8 buf[3];
				buf[0] = a;  
				buf[1] = b;  
				buf[2] = 0;  
				ret += (char)strtoul(buf, NULL, 16);  
				ptr += 2;  
				continue;  
			}  
		}  
		if (*ptr=='+')  
		{  
			ret+=' ';  
			continue;  
		}  
		ret += *ptr;  
	}  
	return ret;  
}  

int32 Shared::inet_addr(const std::string& host)
{
	struct in_addr s;
	return Shared::inet_addr(AF_INET, host.c_str(), (void*)&s);
}

int32 Shared::inet_addr(int32 family, const std::string& host, void* addr)
{
	return ::inet_pton(family, host.c_str(), addr);
}

std::string Shared::inet_ntoa(void* addr)
{
	int8 buf[64] = { 0 };
	return (char*)inet_ntop(AF_INET, addr, buf, 64);
}

void Shared::setConsoleTitle(const std::string& title)
{
	SetConsoleTitleA(title.c_str());
}

int32 Shared::setConsoleCtrlHandler(int32 (*cb)(int32 type), int32 add)
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)cb, add);
	return 0;
}

//void* operator new(size_t size)
//{
//	return malloc(size);
//}
//
//void* operator new[](size_t size)
//{
//	return malloc(size);
//}

void Shared::SetDirectory(const std::string& dir)
{
	_chdir(dir.c_str());
}

std::string Shared::GetDirectory()
{
	int8 path[MAX_PATH] = { 0 };
	_getcwd(path, MAX_PATH);
	return std::string(path);
}

void Shared::CreateDirectory(const std::string& dir)
{
	_mkdir(dir.c_str());
}

void Shared::GetDirectoryFiles(const std::string& path, std::vector<std::string>& files, const std::string slash /*= "/"*/)
{
	long hFile = 0;
	struct _finddata_t fileinfo;

	std::string checkpath = path + slash;
	if ((hFile = _findfirst((checkpath + "*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					GetDirectoryFiles(path + slash + fileinfo.name, files);
			}
			else
				files.push_back(checkpath + fileinfo.name);
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void Shared::Clipboard(const std::string& str)
{
#ifdef WIN32
	if (!OpenClipboard(NULL))
		return;
	//  注意 selStr 是你要复制到剪切板的字符串， 这里只做 声明用;
	uint32 nCount = (uint32)str.size();
	HGLOBAL hGlobalMem = GlobalAlloc(GMEM_MOVEABLE, (nCount + 1) * sizeof(TCHAR));
	if (!hGlobalMem)
	{
		CloseClipboard();
		return;
	}
	EmptyClipboard();
	LPTSTR lpszStr = (LPTSTR)GlobalLock(hGlobalMem);
	memcpy(lpszStr, str.c_str(), nCount * sizeof(TCHAR));
	lpszStr[nCount] = (TCHAR)0;
	GlobalUnlock(hGlobalMem);
#ifdef UNICODE
	SetClipboardData(CF_UNICODETEXT, hGlobalMem);
#else
	SetClipboardData(CF_TEXT, hGlobalMem);
#endif
	CloseClipboard();
#endif // WIN32

}

int32 Shared::GetFileLength(FILE* fp)
{
	int32 curpos = 0, length = 0;
	curpos = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	length = ftell(fp);
	fseek(fp, curpos, SEEK_SET);
	return length;
}

std::string Shared::GetCodeMessage(int32 error_code)
{
	std::string error_mes = "";
	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	error_mes = (int8*)lpMsgBuf;
	LocalFree(lpMsgBuf);
	return error_mes;
}

void Shared::srand(uint32 seed)
{
	::srand(seed);
}

void Shared::Sleep(uint32 milliseconds)
{
	::Sleep(milliseconds);
}

uint32 Shared::BKDRHash(const char* str)
{
	uint32 seed = 131; // 31 131 1313 13131 131313 etc..
	uint32 hash = 0;

	while (*str)
		hash = hash * seed + (*str++);
	return (hash & 0x7FFFFFFF);
}

void Shared::XOR(char* input, int len, cstring& key)
{
	if (key.length() == 0) return;
	const char* akey = key.c_str();
	uint32 keylen = key.length();
	uint32 keypos = 0;
	for (uint32 i = 0; i < len; ++i) {
		input[i] ^= akey[keypos];
		keypos = (keypos + 1) % keylen;
	}
}

std::string Shared::ConvertHex(char* datas, int32 length)
{
	char szBuffer[20480] = {};
	int tlen = 20480;
	if (length <= 0) return szBuffer;
	int len = 0;
	sprintf_s(szBuffer + len, tlen - len, "\n");
	len = strlen(szBuffer);
	for (int i = 0; i < length; i++)
	{
		sprintf_s(szBuffer + len, tlen - len, "%02X ", (unsigned char)datas[i]);
		len = strlen(szBuffer);
		if ((i + 1) % 4 == 0)
		{
			sprintf_s(szBuffer + len, tlen - len, " ");
			len = strlen(szBuffer);
		}
		if ((i + 1) % 8 == 0) {
			sprintf_s(szBuffer + len, tlen - len, "\n");
			len = strlen(szBuffer);
		}
	}
	sprintf_s(szBuffer + len, tlen - len, "\n");
	return szBuffer;
}

std::string Shared::tolower(const std::string& str)
{
	std::string tmpstr = str;
	for (size_t i = 0; i < tmpstr.length(); ++i)
		tmpstr[i] = (char)::tolower(tmpstr[i]);
	return tmpstr;
}

std::string Shared::toupper(const std::string& str)
{
	std::string tmpstr = str;
	for (size_t i = 0; i < tmpstr.length(); ++i)
		tmpstr[i] = (char)::toupper(tmpstr[i]);
	return tmpstr;
}
