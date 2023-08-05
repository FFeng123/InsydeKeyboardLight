#include "KeyListener.h"
#include "Timer.h"
#include "var.hpp"

#include <thread>
#include <math.h>

KeyListener* KeyListener::_instance = nullptr;


KeyListener* KeyListener::Instance()
{
    if (!_instance) {
        _instance = new KeyListener();
    }
    return _instance;
}

void KeyListener::updateKeyDown(){
    _lastKeyDown = clock();
    if (_mode == Mode::OFF) {
        Timer::Instance()->callUpdateFunc();
    }
    else {
        Timer::Instance()->updateFunc( (1+rand() % (colors.size() - 1)) * color_wait);
        
    }
}


float KeyListener::getLuminance()
{
    Timer::Instance()->fractional;

    auto now = clock();

    switch (_mode)
    {
    case Mode::ON:
        if (now - _lastKeyDown >= sleepTime){
            _lastKeyDown = now - sleepTime;
            
            setMode(Mode::Closeing);
        }
        return 1.0f;
    case Mode::Closeing:
        if (now - _lastKeyDown > sleepTime + closeingTime) {
            setMode(Mode::OFF);
            return 0.0f;
        }
        else if (now - _lastKeyDown < sleepTime) {
            setMode(Mode::ON);
            return 1.0f;
        }
        return 1.0f - float(now - _lastKeyDown - sleepTime) / closeingTime;
    case Mode::OFF:
        if (now - _lastKeyDown < sleepTime) {
            setMode(Mode::ON);
            return 1.0f;
        }
        return 0.0f;
    }

    if (now - _lastKeyDown < sleepTime) {
        return 1.0f;
    }
    else if (now - _lastKeyDown > sleepTime + closeingTime) {
        

        return 0.0f;
    }
    else {
        return float(now - _lastKeyDown - sleepTime) / closeingTime;
    }
}

// ȫ�ֹ��ӻص�����
LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    // �жϹ����Ƿ���������̰�����Ϣ
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KeyListener::Instance()->updateKeyDown();
        }
    }

    // ����Ϣ���ݸ���һ������
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void KeyListener::setMode(Mode mode){
    _mode = mode;
    switch (mode)
    {
    case Mode::ON:
        Timer::Instance()->fractional = 1;
        Timer::Instance()->sleepTime = 500;
        break;

    case Mode::Closeing:
        Timer::Instance()->fractional = 1;
        Timer::Instance()->sleepTime = 10;
        break;

    case Mode::OFF:
        Timer::Instance()->fractional = 512;
        Timer::Instance()->sleepTime = 100;
        break;
    }
}

void windowsEnentTmain() {
    HHOOK _keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, NULL, 0); 
    if (_keyboardHook == NULL)
    {
        throw "�޷���װWindows����";
    }

    MSG message;
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    UnhookWindowsHookEx(_keyboardHook);
}

KeyListener::KeyListener() {
    windowsEventT = std::thread(windowsEnentTmain);
    windowsEventT.detach();
}


KeyListener::~KeyListener() {
}


