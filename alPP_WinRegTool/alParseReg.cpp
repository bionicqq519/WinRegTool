#include "alParseReg.h"

alParseReg::alParseReg()
{
    m_RegData.udDebug_func_flag = 0;
    m_RegData.str_DumpPath = "";
	return;
}

alParseReg::~alParseReg()
{
    m_RegData.udDebug_func_flag = 0;
    m_RegData.str_DumpPath = "";
	return;
}

// 1. parse registry if exist
int alParseReg::ParseReg()
{
    BOOL status;
    std::wstring w_setCompany = HKEY_SUBFOLDER; // L"Altek";
    std::wstring w_alPP_debug_flag = HKEY_KNAME_ALPP_DEBUG_FLAG;
    DWORD readData = 0;
    status = readDwordValueRegistry(HKEY_PATH, &w_setCompany[0], &w_alPP_debug_flag[0], &readData); //read dword
    if (status != TRUE) {
        printf("[alParseReg] readDwordValueRegistry fail = %d\n", status);
        return -1;
    }
    
    std::wstring setDumpPath = HKEY_KNAME_DUMP_PATH;
    PWCHAR readMessage = nullptr;
    status = readUserInfoFromRegistry(HKEY_PATH, &w_setCompany[0], &setDumpPath[0], &readMessage); //read string
    if (status != TRUE) {
        printf("[alParseReg] readUserInfoFromRegistry fail = %d\n", status);
        return -1;
    }

    //store data
    m_RegData.udDebug_func_flag = (unsigned int)readData;
    if (readMessage != nullptr && readMessage != L"")
    {
        m_RegData.str_DumpPath = to_byte_string(readMessage);
        // printf(" Message = %S\n", readMessage);
        free(readMessage);
        readMessage = nullptr;
    }

    return 0;
}

// 2. create registry by default if not exist
int alParseReg::createAltekRegistry()
{
    printf("[alParseReg] createAltekRegistry start\n");
    BOOL status;
    std::wstring w_setCompany = HKEY_SUBFOLDER; // L"Altek";
    status = CreateRegistryKey(HKEY_PATH, &w_setCompany[0]); //create key
    if (status != TRUE) {
        printf("[alParseReg] CreateRegistryKey fail = %d\n", status);
        return -1;
    }

    std::wstring w_alPP_debug_flag = HKEY_KNAME_ALPP_DEBUG_FLAG;
    status = WriteDwordInRegistry(HKEY_PATH, &w_setCompany[0], &w_alPP_debug_flag[0], HKEY_KVALUE_ALPP_DEBUG_FLAG); //write dword
    if (status != TRUE) {
        printf("[alParseReg] WriteDwordInRegistry fail = %d\n", status);
        return -1;
    }

    std::wstring setDumpPath = HKEY_KNAME_DUMP_PATH;
    std::wstring setString = HKEY_KVALUE_DUMP_PATH;
    status = writeStringInRegistry(HKEY_PATH, &w_setCompany[0], &setDumpPath[0], &setString[0]); //write string
    if (status != TRUE) {
        printf("[alParseReg] writeStringInRegistry fail = %d\n", status);
        return -1;
    }

    printf("[alParseReg] createAltekRegistry end\n");
    return 0;
}

// 3. assign registy to lib
int alParseReg::GetDebug_func_flag(unsigned int& a_udDebug_func_flag)
{
    a_udDebug_func_flag = m_RegData.udDebug_func_flag;
    return 0;
}
int alParseReg::GetDump_path(string& a_dump_path)
{
    a_dump_path = m_RegData.str_DumpPath;
    return 0;
}

int alParseReg::writeAltekRegistry(unsigned int a_udDebug_func_flag, string a_dump_path)
{
    printf("[alParseReg] writeAltekRegistry start\n");
    BOOL status;
    std::wstring w_setCompany = HKEY_SUBFOLDER; // L"Altek";

    std::wstring w_alPP_debug_flag = HKEY_KNAME_ALPP_DEBUG_FLAG;
    status = WriteDwordInRegistry(HKEY_PATH, &w_setCompany[0], &w_alPP_debug_flag[0], (DWORD)a_udDebug_func_flag); //write dword
    if (status != TRUE) {
        printf("[alParseReg] WriteDwordInRegistry fail\n");
        return -1;
    }

    std::wstring setDumpPath = HKEY_KNAME_DUMP_PATH;
    std::wstring setString = to_wide_string(a_dump_path);
    status = writeStringInRegistry(HKEY_PATH, &w_setCompany[0], &setDumpPath[0], &setString[0]); //write string
    if (status != TRUE) {
        printf("[alParseReg] writeStringInRegistry fail\n");
        return -1;
    }

    printf("[alParseReg] writeAltekRegistry end\n");
    return 0;
}

int alParseReg::deleteAltekRegistry()
{
    printf("[alParseReg] deleteAltekRegistry start\n");
    BOOL status;
    std::wstring w_setCompany = HKEY_SUBFOLDER; // L"Altek";
    status = DeleteRegistryKey(HKEY_PATH, &w_setCompany[0]); //create key
    if (status != TRUE) {
        printf("[alParseReg] deleteAltekRegistry fail\n");
        return -1;
    }

    printf("[alParseReg] deleteAltekRegistry end\n");
    return 0;
}

//Create key in registry
BOOL alParseReg::CreateRegistryKey(HKEY hKeyParent, PWCHAR subkey)
{
    DWORD dwDisposition; //It verify new key is created or open existing key
    HKEY  hKey;
    DWORD Ret;
    Ret =
        RegCreateKeyEx(
            hKeyParent,
            subkey,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_ALL_ACCESS,
            NULL,
            &hKey,
            &dwDisposition);
    if (Ret != ERROR_SUCCESS)
    {
        printf("Error opening or creating key. => %d\n", Ret);
        return FALSE;
    }
    RegCloseKey(hKey);
    return TRUE;
}
//Write data in registry
BOOL alParseReg::WriteDwordInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD data)
{
    DWORD Ret;
    HKEY hKey;
    //Open the key
    Ret = RegOpenKeyEx(
        hKeyParent,
        subkey,
        0,
        KEY_WRITE,
        &hKey
    );
    if (Ret == ERROR_SUCCESS)
    {
        //Set the value in key
        if (ERROR_SUCCESS !=
            RegSetValueEx(
                hKey,
                valueName,
                0,
                REG_DWORD,
                reinterpret_cast<BYTE*>(&data),
                sizeof(data)))
        {
            RegCloseKey(hKey);
            return FALSE;
        }
        //close the key
        RegCloseKey(hKey);
        return TRUE;
    }
    return FALSE;
}
//Read data from registry
BOOL alParseReg::readDwordValueRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, DWORD* readData)
{
    HKEY hKey;
    DWORD Ret;
    //Check if the registry exists
    Ret = RegOpenKeyEx(
        hKeyParent,
        subkey,
        0,
        KEY_READ,
        &hKey
    );
    if (Ret == ERROR_SUCCESS)
    {
        DWORD data;
        DWORD len = sizeof(DWORD);//size of data
        Ret = RegQueryValueEx(
            hKey,
            valueName,
            NULL,
            NULL,
            (LPBYTE)(&data),
            &len
        );
        if (Ret == ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            (*readData) = data;
            return TRUE;
        }
        RegCloseKey(hKey);
        printf("[readDwordValueRegistry] RegQueryValueEx = %d\n", Ret);
        return TRUE;
    }
    printf("[readDwordValueRegistry] RegOpenKeyEx = %d\n", Ret);
    return FALSE;
}
//Write range and type into the registry
BOOL alParseReg::writeStringInRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR strData)
{
    DWORD Ret;
    HKEY hKey;
    //Check if the registry exists
    Ret = RegOpenKeyEx(
        hKeyParent,
        subkey,
        0,
        KEY_WRITE,
        &hKey
    );
    if (Ret == ERROR_SUCCESS)
    {
        if (ERROR_SUCCESS !=
            RegSetValueEx(
                hKey,
                valueName,
                0,
                REG_SZ,
                (LPBYTE)(strData),
                ((((DWORD)lstrlen(strData) + 1)) * 2)))
        {
            RegCloseKey(hKey);
            printf("[writeStringInRegistry] RegSetValueEx = %d\n", Ret);
            return FALSE;
        }
        RegCloseKey(hKey);
        return TRUE;
    }
    printf("[writeStringInRegistry] RegOpenKeyEx = %d\n", Ret);
    return FALSE;
}
//read customer infromation from the registry
BOOL alParseReg::readUserInfoFromRegistry(HKEY hKeyParent, PWCHAR subkey, PWCHAR valueName, PWCHAR* readData)
{
    HKEY hKey;
    DWORD len = TOTAL_BYTES_READ;
    DWORD readDataLen = len;
    PWCHAR readBuffer = (PWCHAR)malloc(sizeof(PWCHAR) * len);
    if (readBuffer == NULL)
    {
        printf("[readUserInfoFromRegistry] readBuffer\n");
        return FALSE;
    }
    //Check if the registry exists
    DWORD Ret = RegOpenKeyEx(
        hKeyParent,
        subkey,
        0,
        KEY_READ,
        &hKey
    );
    if (Ret == ERROR_SUCCESS)
    {
        Ret = RegQueryValueEx(
            hKey,
            valueName,
            NULL,
            NULL,
            (BYTE*)readBuffer,
            &readDataLen
        );
        while (Ret == ERROR_MORE_DATA)
        {
            // Get a buffer that is big enough.
            len += OFFSET_BYTES;
            readBuffer = (PWCHAR)realloc(readBuffer, len);
            readDataLen = len;
            Ret = RegQueryValueEx(
                hKey,
                valueName,
                NULL,
                NULL,
                (BYTE*)readBuffer,
                &readDataLen
            );
        }
        if (Ret != ERROR_SUCCESS)
        {
            RegCloseKey(hKey);
            printf("[readUserInfoFromRegistry] RegQueryValueEx = %d\n", Ret);
            return false;;
        }
        *readData = readBuffer;
        RegCloseKey(hKey);
        return true;
    }
    else
    {
        printf("[readUserInfoFromRegistry] RegOpenKeyEx = %d\n", Ret);
        return false;
    }
    return true;
}

//Delete key in registry
BOOL alParseReg::DeleteRegistryKey(HKEY hKeyParent, PWCHAR subkey)
{
    DWORD dwDisposition; //It verify new key is created or open existing key
    HKEY  hKey;
    DWORD Ret;
    Ret =
        RegDeleteKeyEx(
            hKeyParent,
            subkey,
            KEY_ALL_ACCESS,
            0);
    if (Ret != ERROR_SUCCESS)
    {
        printf("[DeleteRegistryKey] RegDeleteKeyEx = %d\n", Ret);
        return FALSE;
    }
    return TRUE;
}