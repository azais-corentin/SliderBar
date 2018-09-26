#ifndef SYSTEMKEYBOARDHOOK_H
#define SYSTEMKEYBOARDHOOK_H

#include <QObject>
#include <windows.h>

class SystemKeyboardHook : public QObject
{

    Q_OBJECT
public:
    // Returns singleton instance
    static SystemKeyboardHook* instance();

    // Returns whether the keyboard hook is connected
    bool connected();
    // Connects / Disconnects the keyboard hook
    bool setConnected(bool state);

signals:
    // Broadcasts a key has been pressed
    void keyPressed(DWORD key);
    // Broadcasts a key has been released
    void keyReleased(DWORD key);

private:
    // Keyboard hook
    HHOOK keyboardHook;

    // Class constructor
    SystemKeyboardHook();

    // Identifies hook activity
    static LRESULT CALLBACK keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam);

};

#endif // SYSTEMKEYBOARDHOOK_H
