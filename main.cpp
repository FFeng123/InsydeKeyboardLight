#include "InsydeDCHU.hpp"
#include "var.hpp"
#include "Timer.h"
#include "KeyListener.h"
#include "ChannelsCPP/ChannelsCPP/Channel.h"

#include <iostream>
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <thread>
using std::cin, std::cout, std::endl;

namespace ColorCall {
	clock_t addT = 0;// 相对于整数颜色，现在多出来多少时间
	size_t nowC = 0; // 当前颜色是第几个
	go::Chan<clock_t> update;
	

	

	

	void _updateColor(clock_t delta) {
		addT += delta;

		clock_t addC = addT / color_wait;
		addT -= addC * color_wait;
		if (addC != 0) {
			nowC = (nowC + addC) % colors.size();
		}

		size_t nextC = (nowC + 1) % colors.size();

		Color c = BlendColor(colors[nowC], colors[nextC], float(addT) / color_wait);

		InsydeDCHU::setColor(c.R, c.G, c.B);
		InsydeDCHU::SetBrightness(InsydeDCHU::byte(255 * KeyListener::Instance()->getLuminance() * brightness));
		cout << "set" << delta << endl;
	}

	void callthreadMain() {
		while (true) {
			clock_t d;
			d << update;
			_updateColor(d);
		}
	}

	std::thread callthread(callthreadMain);

	void init() {
		callthread.detach();
	}

	void updateColor(clock_t delta) {
		update << delta;
	}
}



void ColorLoop() {
	if (colors.size() == 0 || color_wait == 0) {
		return;
	}

	size_t nowC = 0;
	
	clock_t lastT = clock();
	clock_t addT = 0;

	while (true) {
		clock_t now = clock();
		addT += now - lastT;
		lastT = now;

		clock_t addC= addT / color_wait;
		addT -= addC * color_wait;
		if (addC != 0) {
			nowC = (nowC + addC) % colors.size();
		}

		size_t nextC = (nowC + 1) % colors.size();

		Color c = BlendColor(colors[nowC], colors[nextC], float(addT) / color_wait);

		InsydeDCHU::setColor(c.R, c.G, c.B);

		Sleep(hz_wait);
	}
}




int main(int argc, char** argv) {

	if (!InsydeDCHU::init()) {
		cout << "dll初始化失败" << endl;
		return -1;
	}
	ReadColor();
	InsydeDCHU::setMode(InsydeDCHU::AllRGB);

	if (argc >= 2) {
		if (!strcmp(argv[1], "setcolor")) {
			SetColor();
			return 0;
		}
	}
	FreeConsole();
	
	//cout << InsydeDCHU::getStatus() << endl;
	/**
	InsydeDCHU::setMode(InsydeDCHU::AllRGB);

	for (int i = 0; i < 256; i++) {
		cout << i << "模式" << endl;
		InsydeDCHU::setMode(InsydeDCHU::Mode(i));
		Sleep(200);
	}*/

	//ColorLoop();
	/**
	for (size_t i = 0; i < 256; i++){
		cout << i << endl;
		InsydeDCHU::SetBrightness(i);
		Sleep(100);

	}
	*/

	if (colors.size() == 0 || color_wait == 0) {
		return 0;
	}

	ColorCall::init();

	Timer::Instance()->updateFunc = ColorCall::updateColor;
	Timer::Instance()->sleepTime = hz_wait;
	KeyListener::Instance()->closeingTime = 1500;
	KeyListener::Instance()->sleepTime = sleep_wait;
	KeyListener::Instance()->keykawaru = keykawaru;

	Timer::Instance()->loop();

}