/* Author: Noa Sendlhofer - noa.sendlhofer@wpn.ch
 * Desc: Windows Hotkey Shutdown command
 */

#include "windows.h"
#include <iostream>

int main(int argc, const char* argv[])
{
    if (RegisterHotKey(
            NULL,
            1,
            MOD_NOREPEAT,
            0x7B))
    {
        std::cout << "hotkey registered!" << std::endl;
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;

            if (!OpenProcessToken(GetCurrentProcess(),
                                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
                return 0;

            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
                                 &tkp.Privileges[0].Luid);

            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                                  (PTOKEN_PRIVILEGES)NULL, 0);

            if (GetLastError() != ERROR_SUCCESS)
                return 0;

            if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
                               SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
                               SHTDN_REASON_MINOR_UPGRADE |
                               SHTDN_REASON_FLAG_PLANNED))
                return 0;

            return 0;
        }
    }


    return 0;
}