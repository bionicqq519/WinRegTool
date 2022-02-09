#pragma once
// #include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
#include <locale>
#include <codecvt>

// registry default value
#define HKEY_PATH HKEY_USERS
#define HKEY_SUBFOLDER L"S-1-5-19\\SOFTWARE\\alPPDebug"
#define HKEY_KNAME_ALPP_DEBUG_FLAG L"alPP_debug_flag"
#define HKEY_KVALUE_ALPP_DEBUG_FLAG 0x00000000
#define HKEY_KNAME_DUMP_PATH L"alPP_dump_path"
#define HKEY_KVALUE_DUMP_PATH L""

#define TOTAL_BYTES_READ    1024
#define OFFSET_BYTES 1024

using namespace std;

typedef struct {
    unsigned int udDebug_func_flag;
    string str_DumpPath;
}RegistryData;

class alParseReg
{
public:
    alParseReg();
    ~alParseReg();

    int createAltekRegistry();
    int ParseReg();

    int GetDebug_func_flag(unsigned int &a_udDebug_func_flag);
    int GetDump_path(string &a_dump_path);

    int writeAltekRegistry(unsigned int a_udDebug_func_flag, string a_dump_path);
    int deleteAltekRegistry();
private:
    // convert string to wstring
    inline std::wstring to_wide_string(const std::string& input)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.from_bytes(input);
    }
    // convert wstring to string 
    inline std::string to_byte_string(const std::wstring& input)
    {
        //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(input);
    }

    //Create key in registry
    BOOL CreateRegistryKey(HKEY hKeyParent, PWCHAR subkey);
    //Write data in registry
    BOOL WriteDwordInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data);
    //Read data from registry
    BOOL readDwordValueRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD* readData);
    //Write range and type into the registry
    BOOL writeStringInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR strData);
    //read customer infromation from the registry
    BOOL readUserInfoFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR* readData);

    //Delete key in registry
    BOOL DeleteRegistryKey(HKEY hKeyParent, PWCHAR subkey);

    RegistryData m_RegData;
};