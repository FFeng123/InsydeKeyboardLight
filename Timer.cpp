#include "Timer.h"
#include "KeyListener.h"

#include <Windows.h>

Timer* Timer::_instance = nullptr;

Timer* Timer::Instance()
{
    if (!_instance) {
        _instance = new Timer();
    }
    return _instance;
}

void Timer::callUpdateFunc(){
    if (updateFunc) {
        clock_t now = clock();
        updateFunc(now - _lastCallTime);
        _lastCallTime = now;
    }
}

void Timer::loop(){
    size_t loopn = 0;
    while (true) {
        loopn += 1;
        if (loopn >= fractional) {
            loopn = 0;
            callUpdateFunc();
        }
        else {
            KeyListener::Instance()->getLuminance();
        }
        Sleep(sleepTime);
    }
}

Timer::Timer() {
    updateFunc = nullptr;
    sleepTime = 100;
    _lastCallTime = clock();
}

Timer::~Timer() {

}
