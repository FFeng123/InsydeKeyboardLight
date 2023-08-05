#pragma once

#include<Windows.h>
#include <ctime>
#include <thread>

class KeyListener
{
public:
	static KeyListener* Instance();
	void updateKeyDown();
	float getLuminance();

	clock_t sleepTime;
	clock_t closeingTime;
	bool keykawaru;


private:
	typedef enum {
		ON,// Õý³£½¥±ä×´Ì¬
		Closeing,// ÕýÔÚÏ¨Ãð×´Ì¬
		OFF, // ÒÑÏ¨Ãð×´Ì¬
	} Mode;

	void setMode(Mode);


	static KeyListener* _instance;

	KeyListener();
	~KeyListener();

	clock_t _lastKeyDown;
	Mode _mode = ON;

	std::thread windowsEventT;
};

