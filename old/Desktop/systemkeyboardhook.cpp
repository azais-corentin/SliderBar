#include "systemkeyboardhook.h"

SystemKeyboardHook::SystemKeyboardHook() :
    QObject()
{
    // Assign to null
    keyboardHook = NULL;
}


LRESULT CALLBACK SystemKeyboardHook::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*) lParam;
            emit SystemKeyboardHook::instance()->keyPressed(pKeyboard->vkCode);
        }
        else if (wParam == WM_KEYUP)
        {
            KBDLLHOOKSTRUCT* pKeyboard = (KBDLLHOOKSTRUCT*) lParam;
            emit SystemKeyboardHook::instance()->keyReleased(pKeyboard->vkCode);
        }
        //reconnect
        //instance()->setConnected( false );
        //instance()->setConnected( true );
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

bool SystemKeyboardHook::connected()
{
    return keyboardHook;
}

bool SystemKeyboardHook::setConnected(bool state)
{
    if (state && keyboardHook == NULL)
    {
        keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);

        return keyboardHook;
    }
    else
    {
        UnhookWindowsHookEx(keyboardHook);
        keyboardHook = NULL;

        return keyboardHook;
    }
}

SystemKeyboardHook* SystemKeyboardHook::instance()
{
    static SystemKeyboardHook* pKeyboardReadWriteInstance = new SystemKeyboardHook();
    return pKeyboardReadWriteInstance;
}
