#include <iostream>
#include "alParseReg.h"
#include "alDebugFunc.h"
#include <fstream>
#include <sstream>

using namespace std;

int ParsingStr(string& OutText, int& OutPos,
	string InText, string left, string right, int start_Pos) {
	int i, j;
	int first_size = left.size();
	i = InText.find(left, start_Pos);
	OutPos = i;
	j = InText.find(right, i + first_size);
	OutText = InText.substr(i + first_size, j - (i + first_size));
	//cout<<jad;
	return 0;
}

int parseXML_alPPDebugFunc(unsigned int &a_debug_func_flag, string &a_dump_path)
{
	ifstream ifs("alPP_DebugFunc.xml", std::ios::in);
	if (!ifs.is_open()) {
		return -1;
	}

	std::stringstream ss;
	ss << ifs.rdbuf();
	std::string str(ss.str());
	// cout << str;
	ifs.close();

	int pos = 0;
	string by_pass, watermark, log, profile, dump;
	ParsingStr(by_pass, pos, str, "<alPP_DEBUG_FLAG_BYPASS_MODE>", "</alPP_DEBUG_FLAG_BYPASS_MODE>", pos);
	ParsingStr(watermark, pos, str, "<alPP_DEBUG_FLAG_ENABLE_WATERMARK>", "</alPP_DEBUG_FLAG_ENABLE_WATERMARK>", pos);
	ParsingStr(log, pos, str, "<alPP_DEBUG_FLAG_LOG_DEBUG>", "</alPP_DEBUG_FLAG_LOG_DEBUG>", pos);
	ParsingStr(profile, pos, str, "<alPP_DEBUG_FLAG_LOG_PROFILE>", "</alPP_DEBUG_FLAG_LOG_PROFILE>", pos);
	ParsingStr(dump, pos, str, "<alPP_DEBUG_FLAG_DUMP_DATA>", "</alPP_DEBUG_FLAG_DUMP_DATA>", pos);

	a_debug_func_flag |= (atoi(by_pass.c_str()) == 0) ? 0 : alPP_DEBUG_FLAG_BYPASS_MODE;
	a_debug_func_flag |= (atoi(watermark.c_str()) == 0) ? 0 : alPP_DEBUG_FLAG_ENABLE_WATERMARK;
	a_debug_func_flag |= (atoi(log.c_str()) == 0) ? 0 : alPP_DEBUG_FLAG_LOG_DEBUG;
	a_debug_func_flag |= (atoi(profile.c_str()) == 0) ? 0 : alPP_DEBUG_FLAG_LOG_PROFILE;
	a_debug_func_flag |= (atoi(dump.c_str()) == 0) ? 0 : alPP_DEBUG_FLAG_DUMP_DATA;

	string dump_path;
	ParsingStr(dump_path, pos, str, "<alPP_dump_path>", "</alPP_dump_path>", pos);
	a_dump_path = dump_path;
	return 0;
}

int printAltekReg(unsigned int a_debug_func_flag, string a_dump_path)
{
	if (a_debug_func_flag == alPP_DEBUG_FLAG_DISABLE)
	{
		printf("alPP_debug_flag = alPP_DEBUG_FLAG_DISABLE\n");
	}
	else if (a_debug_func_flag == alPP_DEBUG_FLAG_ENABLE_MASK)
	{
		printf("alPP_debug_flag = alPP_DEBUG_FLAG_ENABLE_MASK\n");
	}
	else
	{
		printf("alPP_DEBUG_FLAG_BYPASS_MODE : %d\n", (a_debug_func_flag & alPP_DEBUG_FLAG_BYPASS_MODE) != 0);
		printf("alPP_DEBUG_FLAG_ENABLE_WATERMARK : %d\n", (a_debug_func_flag & alPP_DEBUG_FLAG_ENABLE_WATERMARK)!=0);
		printf("alPP_DEBUG_FLAG_LOG_DEBUG : %d\n", (a_debug_func_flag & alPP_DEBUG_FLAG_LOG_DEBUG) != 0);
		printf("alPP_DEBUG_FLAG_LOG_PROFILE : %d\n", (a_debug_func_flag & alPP_DEBUG_FLAG_LOG_PROFILE) != 0);
		printf("alPP_DEBUG_FLAG_DUMP_DATA : %d\n", (a_debug_func_flag & alPP_DEBUG_FLAG_DUMP_DATA) != 0);
	}
	
	printf("alPP_dump_path : %s\n", a_dump_path.c_str());
	
	return 0;
}

int main(int argc, char *argv[])
// int main(void)
{
	int mode = 0;
	if (argc == 2)
	{
		mode = atoi(argv[1]);
	}
	else
	{
		printf("alPP_WinRegTool.exe [mode]\n");
		printf("mode 0 : read alPP debug registry if exist.\n");
		printf("mode 1 : write alPP debug registry by alPP_DebugFunc.xml. If you don't have this file, please check it with Altek.\n");
		printf("mode 2 : delete alPP debug registry\n");
		printf("alPP_WinRegTool.exe [mode]\n");
		return 0;
	}

	alParseReg* tRegParse = new alParseReg();
	unsigned int debug_func_flag = 0;
	string dump_path;
	switch (mode)
	{
		case 0:
			// read current registry
			if (tRegParse->ParseReg() == 0)
			{
				tRegParse->GetDebug_func_flag(debug_func_flag);
				tRegParse->GetDump_path(dump_path);
				printAltekReg(debug_func_flag, dump_path);
			}
			else
			{
				printf("read fail\n");
			}
			break;
		case 1:
			//write registry
			if (!parseXML_alPPDebugFunc(debug_func_flag, dump_path))
			{
				if (!tRegParse->createAltekRegistry())
				{
					tRegParse->writeAltekRegistry(debug_func_flag, dump_path);
				}
			}
			break;
		case 2:
			//delete registry
			tRegParse->deleteAltekRegistry();
			break;
		default:
			break;
	}

	if (tRegParse != nullptr)
		delete tRegParse;

	// system("pause");
	return 0;
}