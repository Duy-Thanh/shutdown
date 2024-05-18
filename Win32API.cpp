#include "Win32API.h"

ExitWindowsExPtr pExitWindowsEx;
SetSuspendStatePtr pSetSuspendState;
GetCurrentProcessPtr pGetCurrentProcess;
OpenProcessTokenPtr pOpenProcessToken;
LookupPrivilegeValueAPtr pLookupPrivilegeValueA;
LookupPrivilegeValueWPtr pLookupPrivilegeValueW;
AdjustTokenPrivilegesPtr pAdjustTokenPrivileges;
GetPwrCapabilitiesPtr pGetPwrCapabilities;
RtlSetProcessIsCritical SetCriticalProcess;
GetTokenInformationPtr pGetTokenInformation;

void InitializeWindowsAPI() {
    HMODULE user32_dll = LoadLibraryW(L"user32.dll");
    pExitWindowsEx = reinterpret_cast<ExitWindowsExPtr>(
        GetProcAddress(user32_dll, "ExitWindowsEx"));

    HMODULE powrprof_dll = LoadLibraryW(L"powrprof.dll");
    pSetSuspendState = reinterpret_cast<SetSuspendStatePtr>(
        GetProcAddress(powrprof_dll, "SetSuspendState"));

    pGetPwrCapabilities = reinterpret_cast<GetPwrCapabilitiesPtr>(
        GetProcAddress(powrprof_dll, "GetPwrCapabilities"));

    HMODULE kernel32_dll = LoadLibraryW(L"kernel32.dll");
    pGetCurrentProcess = reinterpret_cast<GetCurrentProcessPtr>(
        GetProcAddress(kernel32_dll, "GetCurrentProcess"));

    HMODULE advapi32_dll = LoadLibraryW(L"advapi32.dll");
    pOpenProcessToken = reinterpret_cast<OpenProcessTokenPtr>(
        GetProcAddress(advapi32_dll, "OpenProcessToken"));

    pLookupPrivilegeValueA = reinterpret_cast<LookupPrivilegeValueAPtr>(
        GetProcAddress(advapi32_dll, "LookupPrivilegeValueA"));

    pLookupPrivilegeValueW = reinterpret_cast<LookupPrivilegeValueWPtr>(
        GetProcAddress(advapi32_dll, "LookupPrivilegeValueW"));

    pAdjustTokenPrivileges = reinterpret_cast<AdjustTokenPrivilegesPtr>(
        GetProcAddress(advapi32_dll, "AdjustTokenPrivileges"));

    pGetTokenInformation = reinterpret_cast<GetTokenInformationPtr>(
        GetProcAddress(advapi32_dll, "GetTokenInformation"));
}

bool EnableShutdownPrivilege() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!pOpenProcessToken(GetCurrentProcess(),
                           TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                           &hToken)) {
        return false;
    }

    if (!pLookupPrivilegeValueW(nullptr,
                               L"SeShutdownPrivilege",
                               &tkp.Privileges[0].Luid)) {
        CloseHandle(hToken);
        return false;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!pAdjustTokenPrivileges(hToken,
                                FALSE,
                                &tkp,
                                0,
                                nullptr,
                                nullptr)) {
        CloseHandle(hToken);
        return false;
    }

    CloseHandle(hToken);
    return true;
}

BOOL EnablePriv(LPCSTR lpszPriv) {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tkprivs;
    ZeroMemory(&tkprivs, sizeof(tkprivs));

    if (!pOpenProcessToken(GetCurrentProcess(),
                           (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY),
                           &hToken)) {
        return FALSE;
    }

    if (!pLookupPrivilegeValueA(NULL, lpszPriv, &luid)) {
        CloseHandle(hToken);
        return FALSE;
    }

    tkprivs.PrivilegeCount = 1;
    tkprivs.Privileges[0].Luid = luid;
    tkprivs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = pAdjustTokenPrivileges(hToken, FALSE, &tkprivs, sizeof(tkprivs), NULL, NULL);
    CloseHandle(hToken);

    return bRet;
}

bool EnableRequiredPrivileges() {
    if (EnablePriv("SeDebugPrivilege") != TRUE) {
        return false;
    }

    HANDLE ntdll = LoadLibraryW(L"ntdll.dll");
    if (ntdll == NULL) return false;

    SetCriticalProcess = (RtlSetProcessIsCritical)(
        GetProcAddress((HINSTANCE)ntdll, "RtlSetProcessIsCritical"));

    if (!SetCriticalProcess) return false;
    else return true;
}

bool CheckElevated() {
    HANDLE hToken;
    if (!pOpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        std::cerr << "Failed to open process token." << std::endl;
        return false;
    }

    TOKEN_ELEVATION_TYPE elevationType;
    DWORD dwSize;
    if (!pGetTokenInformation(hToken, TokenElevationType, &elevationType, sizeof(elevationType), &dwSize)) {
        CloseHandle(hToken);
        std::cerr << "Failed to get token information." << std::endl;
        return false;
    }

    CloseHandle(hToken);

    return (elevationType == TokenElevationTypeFull);
}

HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIconPath, int iIconIndex) {
    HRESULT hRes;
    IShellLinkW* psl;

    hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (LPVOID*)&psl);
    if (SUCCEEDED(hRes)) {
        IPersistFile* ppf;

        psl->SetPath(lpszPathObj);
        psl->SetDescription(lpszDesc);
        psl->SetArguments(lpszArgs);
        psl->SetIconLocation(lpszIconPath, iIconIndex - 1);

        hRes = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);

        if (SUCCEEDED(hRes)) {
            hRes = ppf->Save(lpszPathLink, TRUE);

            ppf->Release();
        }

        psl->Release();
    }

    return hRes;
}

BOOL FileExists(LPCSTR szPath) {
    DWORD dwAttrib = GetFileAttributesA(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL FileExistsW(LPCWSTR szPath) {
    DWORD dwAttrib = GetFileAttributesW(szPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

std::wstring desktop_directory() {
    static wchar_t path[MAX_PATH + 1];

    if (SHGetSpecialFolderPathW(HWND_DESKTOP, path, CSIDL_DESKTOP, FALSE)) {
        return path;
    } else {
        return L"";
    }
}

std::wstring current_executable() {
    static wchar_t path[MAX_PATH];

    if (GetModuleFileNameW(NULL, path, MAX_PATH)) {
        return path;
    } else {
        return L"";
    }
}


