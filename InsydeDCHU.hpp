#pragma once

#include <string>

namespace InsydeDCHU {
    using std::string;
    typedef unsigned char byte;

    enum Mode {
        // ��һ��ɫ������ (���ȴ�0����󣬴�Լ2��)
        Breath = 16,
        // ��������ɫ���� (going from 0 to brightness in between, around 2sec). Blue, Green, Red, Blue?, Cyan, Yellow, Pink  
        Cycle = 51,
        // 0.5�� �����ɫ (���ֵ�ǰ����)
        Random = 112,
        // 0.5�������ɫ (��0���������)
        Dance = 128,
        // Flashes a colour, then goes off shortly, then flashes the same colour, then goes off longer ~1sec, then starts over with a different colour (goes from 0 to brightness)
        Tempo = 144,
        // Flashes every 1 sec with a different random colour (goes from 0 to brightness)
        Flash = 160,
        // Slowely (around every 10 sec) shows a random colour slowly increasing and then decreasing the brightness (goes from 0 to brightness)
        Wave = 176,
        // ȫ��
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