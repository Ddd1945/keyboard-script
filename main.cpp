#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>

HHOOK hook;
int delay = 0;

LRESULT CALLBACK keyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
    Sleep(delay);

    KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *) lParam;
    INPUT inputs[1];
    UINT ret;

    char wParamStr[16];
    char vkStr[16] = "";

    if (p->vkCode == VK_OEM_6) {
        PostQuitMessage(0);
        return 0;
    }

    if (wParam == WM_KEYDOWN)
        strcpy(wParamStr, "KEYDOWN");
    else if (wParam == WM_KEYUP)
        strcpy(wParamStr, "KEYUP");
    else if (wParam == WM_SYSKEYDOWN)
        strcpy(wParamStr, "SYSKEYDOWN");
    else if (wParam == WM_SYSKEYUP)
        strcpy(wParamStr, "SYSKEYUP");
    else
        strcpy(wParamStr, "UNKNOWN");

    if (p->vkCode == 10)
        strcpy(vkStr, "<LF>");
    else if (p->vkCode == 13)
        strcpy(vkStr, "<CR>");
    else
        vkStr[0] = p->vkCode;

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wScan = 0;
    inputs[0].ki.dwFlags = 0;
    inputs[0].ki.time = 0;
    inputs[0].ki.dwExtraInfo = 0;

    if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    }

    if (p->vkCode == 'A' && (p->flags & LLKHF_INJECTED) == 0) {
        inputs[0].ki.wVk = VK_LEFT;
        ret = SendInput(1, inputs, sizeof (INPUT));

        return 1;
    }

    if (p->vkCode == 'W' && (p->flags & LLKHF_INJECTED) == 0) {
        inputs[0].ki.wVk = VK_UP;
        ret = SendInput(1, inputs, sizeof (INPUT));

        return 1;
    }

    if (p->vkCode == 'S' && (p->flags & LLKHF_INJECTED) == 0) {
        inputs[0].ki.wVk = VK_DOWN;
        ret = SendInput(1, inputs, sizeof (INPUT));

        return 1;
    }

    if (p->vkCode == 'D' && (p->flags & LLKHF_INJECTED) == 0) {
        inputs[0].ki.wVk = VK_RIGHT;
        ret = SendInput(1, inputs, sizeof (INPUT));

        return 1;
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main(int argc, char **argv) {
    MSG messages;
    std::cout << "Script that's changing arrow keys - left, up, right, down to - w, a, s, d.";
    std::cout << "\nDeveloped by DeNdyy :)\nP.S. When script is on, press ']' button to restart the program. Enjoy ;p" << std::endl;

    std::cout << "Enter the delay time between presses in ms >> ";
    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);

        if (ss >> delay) {
            if (ss.eof()) {
                break;
            }
        }

        std::cout << "Error! Only integer numbers are allowed." << std::endl;
        std::cout << "Enter the delay time in ms >> ";
    }

    std::cout << "Success! Script is ready to be deployed!" << std::endl;
    std::cout << "Delay time = " << delay << "ms" << std::endl;

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, NULL, 0);

    if (hook == NULL) {
        std::cout << "Error " << GetLastError() << '\n';
        return 1;
    }

    while (GetMessage (&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    std::system("script.bat");
    std::exit(0);

    return 0;
}
