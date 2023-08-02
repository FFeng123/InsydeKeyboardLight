#include "InsydeDCHU.hpp"
#include "tools.hpp"

#include <iostream>
#include <Windows.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>

using std::cin, std::cout, std::endl,std::vector;

const std::string colorsPath = "colors.bin";


vector<Color> colors;
clock_t color_wait;
clock_t hz_wait;

void ReadColor() {
	std::ifstream f(colorsPath, std::ios::in);

	if (!f.is_open()) {
		return;
	}

	f.read((char*)&color_wait, sizeof(color_wait));
	f.read((char*)&hz_wait, sizeof(hz_wait));

	while (!f.eof()) {
		Color c;
		f.read((char*)&c, sizeof(Color));
		colors.push_back(c);
	}
}

void SetColor() {
	cout << "已进入颜色编辑模式，除非显式提示已保存，否则编辑不会被保存" << endl;


	cout << "请输入每个颜色持续的时间（毫秒）" << endl;
	cin >> color_wait;
	cout << "请输入刷新时间（毫秒），例如希望背光能够每秒钟10次的渐变，此处应该填写100（因为 1000 / 10 = 100）" << endl;
	cin >> hz_wait;
	

	char mode;
	while (true) {
		cout << "请输入接下来你想要怎么做？a: 保留当前颜色，追加新的颜色 c: 清空当前颜色，从头添加颜色 s: 不进行颜色操作，结束程序保存" << endl;
		cin >> mode;
		if (mode == 'a' || mode == 'c' || mode == 's') {
			break;
		}
	}
	if (mode == 'a' || mode == 'c') {
		if (mode == 'c') {
			colors.clear();
		}

		while (true) {
			cout << "请输入新颜色的RGB值（三个0-255数字，用空格回车等字符隔开）" << endl;
			int r, g, b;
			cin >>r >> g >> b;
			Color color{(unsigned char)r,(unsigned char)g,(unsigned char)b};
			
			InsydeDCHU::setColor(color.R, color.G, color.B);
			cout << "颜色已呈现至键盘背光，是否接受？y: 接收并输入下一个颜色 n: 拒绝并重新输入 e: 接受并结束程序保存" << endl;

			char i;
			while (true) {
				cin >> i;
				if (i == 'y' || i == 'n' || i == 'e') {
					break;
				}
			}

			if (i == 'y' || i == 'e') {
				colors.push_back(color);
			}
			if (i == 'e') {
				break;
			}
		}
	}
	// save

	std::ofstream f(colorsPath, std::ios::trunc | std::ios::out);

	f.write((char*)&color_wait, sizeof(color_wait));
	f.write((char*)&hz_wait, sizeof(hz_wait));

	for (auto c : colors) {
		f.write((char*)&c, sizeof(Color));
	}
	
	
	
	f.close();
	cout << "已保存" << endl;
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
		return -1;
	}
	ReadColor();

	if (argc >= 2) {
		if (!strcmp(argv[1], "setcolor")) {
			SetColor();
			return 0;
		}
	}
	FreeConsole();

	//cout << InsydeDCHU::getStatus() << endl;

	InsydeDCHU::setMode(InsydeDCHU::AllRGB);

	ColorLoop();
}