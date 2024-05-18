#include "Win32API.h"

typedef struct {
    int arg_count;
    char **arg;
} APP_DATA, *PAPP_DATA;

PAPP_DATA app_data_shared = NULL;

void showHelp(void);
void viewLicense(void);
std::string checkCpuArch(void);
void viewLicense(void);
void replaceNewLinesWithCRLF(std::string& str);
void PrintEditDocument(HWND hEdit);

std::string checkCpuArch(void) {
    int arch = 0;

    int eax = 1, ebx, ecx, edx;                 // Registers
    asm("cpuid"
        : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
        : "a" (eax)
        : );

    // std::cout << "EAX: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uintptr_t)(eax) << std::endl;
    // std::cout << "EBX: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uintptr_t)(ebx) << std::endl;
    // std::cout << "ECX: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uintptr_t)(ecx) << std::endl;
    // std::cout << "EDX: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uintptr_t)(edx) << std::endl;

    // std::cout << "Value EDX: 0x" << std::hex << std::setw(8) << std::setfill('0') << (uintptr_t)(edx & (1 << 29)) << std::endl;

    if (edx & (1 << 29)) {
        return "x86_64 - 64-bit preferred";
    } else {
        return "x86";
    }
}

void viewLicense(void) {
    #include <gpl_license.h>
    std::cout << gpl_text << std::endl;
}

const char CLASS_NAME[] = "SimpleTextViewer";
HINSTANCE hInstance;

HWND hEdit;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            return 0;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE:
            InvalidateRect(hWnd, NULL, TRUE);
            return 0;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                EndPaint(hWnd, &ps);
            }
            return 0;
        case WM_COMMAND:
            switch (LOWORD(wParam)) 
            {
                case 101:
                    if (LOWORD(wParam) == ID_PRINT) {
                        PrintEditDocument(hEdit);
                    }
                    break;
                case 102:
                    PostQuitMessage(0);
                    break;
                default:
                    break;
            }
            return 0;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void replaceNewLinesWithCRLF(std::string& str) {
    size_t pos = 0;

    while ((pos = str.find('\n', pos)) != std::string::npos) {
        str.insert(pos, "\r");
        pos += 2;
    }
}

void replaceNewLinesWithCRLFW(std::wstring& str) {
    size_t pos = 0;

    while ((pos = str.find(L'\n', pos)) != std::wstring::npos) {
        str.insert(pos, L"\r");
        pos += 2;
    }
}

void PrintEditDocument(HWND hEdit) {
    GETTEXTLENGTHEX gtl = { sizeof(GETTEXTLENGTHEX) };
    LRESULT textLength = SendMessage(hEdit, EM_GETTEXTLENGTHEX, (WPARAM)&gtl, 0);

    // Initialize the PRINTDLG structure
    PRINTDLG pd = { sizeof(PRINTDLG) };
    pd.Flags = PD_ALLPAGES | PD_RETURNDC | PD_NOSELECTION;

    // Allocate memory for the text
    wchar_t* text = new wchar_t[textLength + 1];
    if (text) {
        // Retrieve the text
        TEXTRANGEW tr = { 0, -1, text };
        SendMessage(hEdit, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

        // Display the print dialog
        if (PrintDlg(&pd)) {
            // Get the device context for the printer
            HDC hdcPrinter = pd.hDC;

            DEVMODEW* pDevMode = (DEVMODEW *)GlobalLock(pd.hDevMode);
            pDevMode->dmFields |= DM_PAPERSIZE | DM_DEFAULTSOURCE;
            pDevMode->dmPaperSize = DMPAPER_A4;
            pDevMode->dmDefaultSource = DMBIN_AUTO;
            GlobalUnlock(pd.hDevMode);

            DocumentPropertiesW(NULL, hdcPrinter, NULL, pDevMode, pDevMode, DM_IN_BUFFER | DM_OUT_BUFFER);

            // Setup the document
            DOCINFO di = { sizeof(DOCINFO), TEXT("GNU GPL License"), NULL, NULL, NULL };
            if (StartDoc(hdcPrinter, &di) > 0) {
                if (StartPage(hdcPrinter) > 0) {
                    #include <gpl_license_wchar.h>

                    std::wstring gpl_text_str(text_to_print);

                    replaceNewLinesWithCRLFW(gpl_text_str);

                    std::wstringstream ss(gpl_text_str);
                    std::wstring line;
                    int yPos = 100; // Starting y position
                    SIZE textSize;
                    while (std::getline(ss, line)) {
                        // Calculate the size of the text
                        GetTextExtentPoint32W(hdcPrinter, line.c_str(), line.length(), &textSize);
                        // Print the text
                        TextOutW(hdcPrinter, 100, yPos, line.c_str(), line.length());
                        // Adjust the y position for the next line based on the text height
                        yPos += textSize.cy;
                    }

                    EndPage(hdcPrinter);
                }
                EndDoc(hdcPrinter);
            }

            DeleteDC(hdcPrinter);
        }
    }
}

void showDialogLicense(void) {
    #include <gpl_license.h>

    std::string gpl_text_str(gpl_text);

    replaceNewLinesWithCRLF(gpl_text_str);

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClass(&wc)) {
        std::cout << "Window registeration failed. Error: " << GetLastError() << std::endl;
        return;
    }

    HWND hWnd = CreateWindowExA(
        0,
        CLASS_NAME,
        "License - Text Viewer",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,

        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 768,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hWnd == NULL) {
        std::cout << "CreateWindowEx() failed. Error: " << GetLastError() << std::endl;
        return;
    }

    HICON hIcon = LoadIcon(NULL, IDI_WINLOGO);

    if (hIcon == NULL) {
        std::cout << "Load icon failed. Error: " << GetLastError() << std::endl;
    }

    LONG_PTR ptr = SetClassLongPtr(hWnd, GCLP_HICON, (LONG_PTR)hIcon);

    if (ptr == 0) {
        std::cout << "Failed to SetClassLongPtr(). Error: " << GetLastError() << std::endl;
    }

    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_STANDARD_CLASSES;

    InitCommonControlsEx(&icc);

    HWND toolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
                                  WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
                                  hWnd, NULL, hInstance, NULL);

    HIMAGELIST hImageList = ImageList_Create(24, 24, ILC_COLOR24 | ILC_MASK, 1, 0);
    SendMessage(toolbar, TB_SETIMAGELIST, 0, (LPARAM)hImageList);
    SendMessage(toolbar, TB_LOADIMAGES, (WPARAM)IDB_STD_LARGE_COLOR, (LPARAM)HINST_COMMCTRL);

    TBBUTTON tbButtons[1] = 
    {
        { MAKELONG(STD_PRINT, 0), ID_PRINT, TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0 }, 0, 0 }
    };

    SendMessage(toolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(toolbar, TB_ADDBUTTONS, (WPARAM)1, (LPARAM)&tbButtons);

    //SendMessage(toolbar, TB_SETBUTTONSIZE, 0, MAKELONG(50, 50));
    SendMessage(toolbar, TB_AUTOSIZE, 0, 0);

    RECT rect;
    GetClientRect(hWnd, &rect);

    hEdit = CreateWindowExA(
        WS_EX_CLIENTEDGE,
        "EDIT",
        "",
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_READONLY,
        0, 36, rect.right, rect.bottom - 38,
        hWnd,
        NULL,
        hInstance,
        NULL
    );

    if (hEdit == NULL) {
        std::cout << "CreateWindowExA() failed. Error: " << GetLastError() << std::endl;
        return;
    }

    SetWindowText(hEdit, gpl_text_str.c_str());

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    ShowWindow(toolbar, TRUE);
    ShowWindow(hEdit, TRUE);
    UpdateWindow(toolbar);
    UpdateWindow(hEdit);
    UpdateWindow(hWnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void showDialogHelp(void) {
    std::string cpu_arch = checkCpuArch();

    std::wstring str_title = L"Power Management Utility - Version 1.0.0.0 (" + std::wstring(cpu_arch.begin(), cpu_arch.end()) + L")\n";

    std::wstring messageBoxString = str_title + L"\n" + LR"(
This tool will make your power management easier. You can shutdown/restart/logoff/sleep in one click
Copyright © 2016 - 2024 CyberDay Studios. All right reserved.

This program is opened source. Please visit https://github.com/Duy-Thanh/shutdown to view
full source code

Usage: shutdown_ctrl [options]

Parameters:

--help:                      Show this help

--license:                  Show license of this program

--shutdown:            Shutdown the computer

--hybrid:                   Hybrid shutdown the computer

--logoff:                   Sign out current account

--sleep:                     Sleep the computer


    )";

    MessageBoxW(NULL, messageBoxString.c_str(), L"Power Management Utility - Help", MB_ICONINFORMATION | MB_OK);
}

void showHelp(void) {
    std::string cpu_arch = checkCpuArch();

    std::cout << "Power Management Utility - Version 1.0.0.0 (" << cpu_arch << ")" << std::endl;
    std::cout << "This tool will make your power management easier. You can shutdown/restart/logoff/sleep in one click" << std::endl;
    std::cout << "Copyright (C) 2016 - 2024 CyberDay Studios. All right reserved." << std::endl;
    std::cout << "This program licensed in GPLv2 License. To view license, type 'shutdown_ctrl.exe --license'" << std::endl << std::endl;
    std::cout << "This program is opened source. Please visit https://github.com/Duy-Thanh/shutdown to view full source code" << std::endl << std::endl;
    std::cout << std::endl << "Usage: shutdown_ctrl [options]" << std::endl << std::endl;
    std::cout << "Parameters: " << std::endl << std::endl;
    std::cout << std::endl;
    std::cout << "--help:\t\t\t\tShow this help" << std::endl << std::endl;
    std::cout << "--license:\t\t\tShow license of this program" << std::endl << std::endl;
    std::cout << "--shutdown:\t\t\tShutdown the computer" << std::endl << std::endl;
    std::cout << "--restart:\t\t\tRestart the computer" << std::endl << std::endl;
    std::cout << "--logoff:\t\t\tLogoff the computer" << std::endl << std::endl;
    std::cout << "--sleep:\t\t\tSleep the computer" << std::endl << std::endl;
}

void createShortcut(void) {
    HRESULT hRes = CoInitialize(NULL);
    if (SUCCEEDED(hRes)) {
        std::wstring Shutdown_Lnk = desktop_directory() + L"\\Shutdown.lnk";
        std::wstring Restart_Lnk = desktop_directory() + L"\\Restart.lnk";
        std::wstring Logoff_Lnk = desktop_directory() + L"\\Logoff.lnk";
        std::wstring Sleep_Lnk = desktop_directory() + L"\\Sleep.lnk";
        std::wstring License_Lnk = desktop_directory() + L"\\License.lnk";

        std::wstring app_path = current_executable();

        LPCWSTR argument_shutdown = L"--shutdown";
        LPCWSTR argument_restart = L"--restart";
        LPCWSTR argument_logoff = L"--logoff";
        LPCWSTR argument_sleep = L"--sleep";
        LPCWSTR argument_license = L"--license";

        LPCWSTR iconFile = L"C:\\Windows\\System32\\SHELL32.dll";
        int iShutdownID = 28;
        int iLogoffID = 45;
        int iSleepID = 26;
        int iRestartID = 218;
        int iLicenseID = 2;

        HRESULT hRes = CreateLink(app_path.c_str(), Shutdown_Lnk.c_str(), L"Shutdown the computer in one click", argument_shutdown, iconFile, iShutdownID);

        hRes = CreateLink(app_path.c_str(), Restart_Lnk.c_str(), L"Restart the computer in one click", argument_restart, iconFile, iRestartID);

        hRes = CreateLink(app_path.c_str(), Logoff_Lnk.c_str(), L"Logoff the computer in one click", argument_logoff, iconFile, iLogoffID);

        hRes = CreateLink(app_path.c_str(), Sleep_Lnk.c_str(), L"Sleep the computer in one click", argument_sleep, iconFile, iSleepID);

        hRes = CreateLink(app_path.c_str(), License_Lnk.c_str(), L"Display license of shutdown_ctrl.exe", argument_license, iconFile, iLicenseID);
    }

    CoUninitialize();
}

void ShutDown(void) {
    if (!pExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
                        SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
                        SHTDN_REASON_MINOR_UPGRADE |
                        SHTDN_REASON_FLAG_PLANNED)) {
        DWORD errorCode = GetLastError();

        std::cout << "Error: " << errorCode << std::endl;

        system("shutdown -s -t 00");
    }
}

void HybridShutdown(void) {
    MessageBoxA(NULL, "Hybrid shutdown is not available.", "Fatal Error", MB_ICONHAND | MB_OK);
}

void Restart(void) {
    if (!pExitWindowsEx(EWX_REBOOT | EWX_FORCE,
                        SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
                        SHTDN_REASON_MINOR_UPGRADE |
                        SHTDN_REASON_FLAG_PLANNED)) {
        DWORD errorCode = GetLastError();

        std::cout << "Error: " << errorCode << std::endl;

        system("shutdown -r -t 00");
    }
}

void Logoff(void) {
    if (!pExitWindowsEx(EWX_LOGOFF, 0)) {
        DWORD errorCode = GetLastError();

        std::cout << "Error: " << errorCode << std::endl;
    }
}

void Sleep(void) {
    SYSTEM_POWER_CAPABILITIES sysPowerCaps;

    if (pGetPwrCapabilities(&sysPowerCaps)) {
        if (sysPowerCaps.SystemS1 ||
            sysPowerCaps.SystemS2 ||
            sysPowerCaps.SystemS3) {
            if (!pSetSuspendState(FALSE, TRUE, FALSE)) {
                DWORD errorCode = GetLastError();

                std::cout << "Error" << errorCode << std::endl;
            }
        } else {
            MessageBoxA(NULL, "Your machine doesn't support sleep mode", "Fatal Error", MB_ICONHAND | MB_OK);
        }
    } else {
        MessageBoxA(NULL, "Cannot fetch power capabilities. Without power capabilities, this application cannot verify that your computer supported sleep mode.\n\nOperation cancelled.", "Fatal Error", MB_ICONHAND | MB_OK);
    }
}

int main(int argc, char **argv) {
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    
    if (!GetVersionEx(&osvi)) {
        asm (
            "int $0x3"
        );
    }

    if (osvi.dwMajorVersion < 5) {
        asm (
            "int $0x3"
        );
    }

    InitializeWindowsAPI();

    if (EnableShutdownPrivilege()) {
        createShortcut();

        std::wstring Shutdown_Lnk = desktop_directory() + L"\\Shutdown.lnk";
        std::wstring Restart_Lnk = desktop_directory() + L"\\Restart.lnk";
        std::wstring Logoff_Lnk = desktop_directory() + L"\\Logoff.lnk";
        std::wstring Sleep_Lnk = desktop_directory() + L"\\Sleep.lnk";
        std::wstring License_Lnk = desktop_directory() + L"\\License.lnk";

        std::wstring app_path = current_executable();

        std::wcout << "Application Path: " << current_executable() << std::endl;

        if (!FileExistsW(Shutdown_Lnk.c_str())  ||
            !FileExistsW(Restart_Lnk.c_str())   ||
            !FileExistsW(Logoff_Lnk.c_str())    ||
            !FileExistsW(Sleep_Lnk.c_str())     ||
            !FileExistsW(License_Lnk.c_str())) {
            std::wcout << L"Cannot found shortcut on " << desktop_directory() << std::endl;
            createShortcut();
        } else {
            std::wcout << "Shortcut already existed on " << desktop_directory() << std::endl;
        }

        app_data_shared = new APP_DATA;

        if (app_data_shared != NULL) {
            app_data_shared->arg_count = argc;
            app_data_shared->arg = argv;
        } else {
            std::cout << "Failed to allocate required memory zone. Aborting..." << std::endl;
            system("pause");

            return (uint32_t)(0xFFFFFFFF);
        }

        if (app_data_shared->arg_count <= 1) {
            showDialogHelp();
        } else {
            if (strcmp(app_data_shared->arg[1], "--help") == 0) {
                // Show help messages
                showHelp();
            }
            else if (strcmp(app_data_shared->arg[1], "--license") == 0) {
                showDialogLicense();
            }
            else if (strcmp(app_data_shared->arg[1], "--shortcut") == 0) {
                createShortcut();
            }
            else if (strcmp(app_data_shared->arg[1], "--shutdown") == 0) {
                ShutDown();
            }
            else if (strcmp(app_data_shared->arg[1], "--hybrid") == 0) {
                HybridShutdown();
            }
            else if (strcmp(app_data_shared->arg[1], "--restart") == 0) {
                Restart();
            }
            else if (strcmp(app_data_shared->arg[1], "--logoff") == 0) {
                Logoff();
            }
            else if (strcmp(app_data_shared->arg[1], "--sleep") == 0) {
                Sleep();
            }
            else {
                std::cout << "Invalid parameter: '" << app_data_shared->arg[1] << "'" << std::endl;
                std::cout << "Please type 'shutdown_ctrl.exe --help' for usage" << std::endl << std::endl;
            }
        }
    }

    return 0;
}
