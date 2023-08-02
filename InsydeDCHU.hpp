#pragma once

#include <string>

namespace InsydeDCHU {
    using std::string;
    typedef unsigned char byte;

    enum Mode {
        // 单一颜色呼吸灯 (亮度从0到最大，大约2秒)
        Breath = 16,
        // 呼吸灯颜色交替 (going from 0 to brightness in between, around 2sec). Blue, Green, Red, Blue?, Cyan, Yellow, Pink  
        Cycle = 51,
        // 0.5秒 随机颜色 (保持当前亮度)
        Random = 112,
        // 0.5秒随机颜色 (从0到最大亮度)
        Dance = 128,
        // Flashes a colour, then goes off shortly, then flashes the same colour, then goes off longer ~1sec, then starts over with a different colour (goes from 0 to brightness)
        Tempo = 144,
        // Flashes every 1 sec with a different random colour (goes from 0 to brightness)
        Flash = 160,
        // Slowely (around every 10 sec) shows a random colour slowly increasing and then decreasing the brightness (goes from 0 to brightness)
        Wave = 176,
        // 全彩
        AllRGB = 240,
    };

    static int (*SetDCHU_Data)(int command, byte* buffer, int length);
    static int (*WriteAppSettings)(int page, int offset, int length, byte* buffer);
    static int (*ReadAppSettings)(int page, int offset, int length, byte* buffer);

    bool init();
    string getStatus();
    void setMode(Mode mode);
    byte getModeNum(Mode mode);
    void setColor(byte r, byte g, byte b);


}