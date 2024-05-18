#ifndef WIN32API_H
#define WIN32API_H

#define ID_PRINT 101
#define ID_EXIT  102

#define INITGUID
#include <windows.h>
#include <tchar.h>
#include <powrprof.h>
#include <ntstatus.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <initguid.h>
#include <shlobj.h>
#include <iostream>
#include <macro.h>
#include <winternl.h>
#include <iomanip>
#include <cstdint>
#include <shlwapi.h>
#include <commctrl.h>
#include <richedit.h>
#include <commdlg.h>
#include <string>
#include <sstream>
#include <future>
#include <vector>

typedef BOOLEAN (WINAPI *ExitWindowsExPtr)(UINT dwFlags,
                                           DWORD dwReason);

typedef BOOLEAN (WINAPI *SetSuspendStatePtr)(BOOLEAN bHibernate,
                                             BOOLEAN bForce,
                                             BOOLEAN bWakeupEventsDisabled);

typedef HANDLE (WINAPI *GetCurrentProcessPtr)();

typedef BOOLEAN (WINAPI *OpenProcessTokenPtr)(HANDLE ProcessHandle,
                                              DWORD DesiredAccess,
                                              PHANDLE TokenHandle);

typedef BOOLEAN (WINAPI *LookupPrivilegeValueWPtr)(LPCWSTR lpSystemName,
                                                   LPCWSTR lpName,
                                                   PLUID lpLuid);

typedef BOOLEAN (WINAPI *LookupPrivilegeValueAPtr)(LPCSTR lpSystemName,
                                                  LPCSTR lpName,
                                                  PLUID lpLuid);

typedef BOOLEAN (WINAPI *AdjustTokenPrivilegesPtr)(HANDLE TokenHandle,
                                                   BOOLEAN DisableAllPrivileges,
                                                   PTOKEN_PRIVILEGES NewState,
                                                   DWORD BufferLength,
                                                   PTOKEN_PRIVILEGES PreviuousState,
                                                   PDWORD ReturnLength);

typedef BOOLEAN (WINAPI *GetPwrCapabilitiesPtr)(PSYSTEM_POWER_CAPABILITIES lpspc);

typedef long (WINAPI *RtlSetProcessIsCritical) (
        BOOLEAN bNew,
        BOOLEAN *pbOld,
        BOOLEAN bNeedScb
);

typedef BOOLEAN (WINAPI *GetTokenInformationPtr)(HANDLE TokenHandle,
                                                 TOKEN_INFORMATION_CLASS TokenInformationClass,
                                                 LPVOID TokenInformation,
                                                 DWORD TokenInformationLength,
                                                 PDWORD ReturnLength);

extern ExitWindowsExPtr pExitWindowsEx;
extern SetSuspendStatePtr pSetSuspendState;
extern GetCurrentProcessPtr pGetCurrentProcess;
extern OpenProcessTokenPtr pOpenProcessToken;
extern LookupPrivilegeValueAPtr pLookupPrivilegeValueA;
extern LookupPrivilegeValueWPtr pLookupPrivilegeValueW;
extern AdjustTokenPrivilegesPtr pAdjustTokenPrivileges;
extern GetPwrCapabilitiesPtr pGetPwrCapabilities;

extern RtlSetProcessIsCritical SetCriticalProcess;

extern GetTokenInformationPtr pGetTokenInformation;

extern void InitializeWindowsAPI();
extern bool EnableShutdownPrivilege();
extern bool EnableRequiredPrivileges();
extern bool CheckElevated();
BOOL EnablePriv(LPCSTR lpszPriv);
HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR lpszArgs, LPCWSTR lpszIconPath, int iIconIndex);
BOOL FileExists(LPCSTR szPath);
BOOL FileExistsW(LPCWSTR szPath);
std::wstring desktop_directory();
std::wstring current_executable();

#endif // WIN32API_H
