#include "InsydeDCHU.hpp"

#include <Windows.h>
#include <sstream>

using std::stringstream;

bool InsydeDCHU::init()
{
    auto MHID = LoadLibraryA("InsydeDCHU.dll");
    if (MHID == NULL) {
        return false;
    }

    SetDCHU_Data = (int (*)(int command, byte * buffer, int length))GetProcAddress(MHID, "SetDCHU_Data");
    WriteAppSettings = (int (*)(int page, int offset, int length, byte * buffer))GetProcAddress(MHID, "WriteAppSettings");
    ReadAppSettings = (int (*)(int page, int offset, int length, byte * buffer))GetProcAddress(MHID, "ReadAppSettings");

    return true;
}

std::string InsydeDCHU::getStatus()
{
    stringstream ss;
    
    byte brightness = 0;
    ReadAppSettings(2, 35, 1, &brightness);
    byte color[3] = { 0 };
    ReadAppSettings(2, 81, 3, color);
    ss << "RGB: " << "(" << (int)color[0] << ", " << (int)color[1] << ", " << (int)color[2] << ")\n";
    ss << "ÁÁ¶È: " << (int)brightness << "\n";
    byte mode = 0;
    ReadAppSettings(2, 32, 1, &mode);
    byte status = 0;
    ReadAppSettings(2, 84, 1, &status);
    ss << "µÆ¹â×´Ì¬: " << (status == 0 ? "¹Ø" : "¿ª") << '\n';
    ss << "µÆ¹âÄ£Ê½: " << (int) mode<< '\n';
    byte booteffect = 0;
    ReadAppSettings(2, 7, 1, &booteffect);
    ss << "Òýµ¼¸²¸Ç: " << (booteffect == 0 ? "Ä¬ÈÏ" : "¸²¸Ç") << "\n";
    byte sleep[3] = { 0 };
    ReadAppSettings(2, 37, 3, sleep);
    int sleepsec = sleep[0] * 3600 + sleep[1] * 60 + sleep[2];
    byte sleepstatus = 0;
    ReadAppSettings(2, 36, 1, &sleepstatus);
    ss << "Ë¯Ãß×´Ì¬: " << (sleepstatus == 0 ? "Î´Ë¯Ãß" : "ÕýÔÚË¯Ãß") << "\n";
    ss << "Ë¯ÃßÊ±¼ä: " << sleepsec << "Ãë\n";

    return ss.str();
    
}

void InsydeDCHU::setMode(Mode mode)
{
    byte data[] = { 255,255,255,(byte)mode };
    SetDCHU_Data(103, data, 4);
    //byte data2 = getModeNum(mode);
    //WriteAppSettings(2, 32, 1, &data2);
}

byte InsydeDCHU::getModeNum(Mode mode)
{
    switch (mode) {
    case Mode::Random:return 0;
    case Mode::Breath:return 2;
    case Mode::Cycle: return 3;
    case Mode::Wave: return 4;
    case Mode::Dance: return 5;
    case Mode::Tempo: return 6;
    case Mode::Flash: return 7;
    case Mode::AllRGB: return 8;
    default: throw 0;
    };
}

void InsydeDCHU::setColor(byte r, byte g, byte b)
{
    byte rgb[] = { r,g,b,240 };
    SetDCHU_Data(103, rgb, 4);
    //WriteAppSettings(2, 81, 3, rgb);
}

void InsydeDCHU::SetBrightness(byte a)
{
    byte data[] = { a, 0, 0, 244 };
    SetDCHU_Data(103,data , 4);
    //TODO: express the setting as a value in [0-4] as they do in their code
    // WriteAppSettings(2, 35, 1, ref new byte[1]{ value } [0] );
}
