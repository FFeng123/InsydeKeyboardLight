#include "var.hpp"
#include "InsydeDCHU.hpp"

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

using std::cin, std::cout, std::endl;


const std::string colorsPath = "colors.bin";


void SetColor() {
	cout << "已进入颜色编辑模式，除非显式提示已保存，否则编辑不会被保存" << endl;


	cout << "请输入每个颜色持续的时间（毫秒）" << endl;
	cin >> color_wait;
	cout << "请输入刷新时间（毫秒），例如希望背光能够每秒钟10次的渐变，此处应该填写100（因为 1000 / 10 = 100）" << endl;
	cin >> hz_wait;
	cout << "请输入键盘灯光亮度（0-1）" << endl;
	cin >> brightness;
	cout << "请输入键盘休眠时间（毫秒）" << endl;
	cin >> sleep_wait;

	char mode;
	while (true) {
		cout << "是否需要键盘随机变换颜色(y/n)" << endl;
		cin >> mode;
		if (mode == 'y') {
			keykawaru = true;
			break;
		}
		else if (mode == 'n') {
			keykawaru = false;
			break;
		}
	}

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
		InsydeDCHU::SetBrightness(255);
		while (true) {
			cout << "请输入新颜色的RGB值（三个0-255数字，用空格回车等字符隔开）" << endl;
			int r, g, b;
			cin >> r >> g >> b;
			Color color{ (unsigned char)r,(unsigned char)g,(unsigned char)b };

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
	f.write((char*)&brightness, sizeof(brightness));
	f.write((char*)&sleep_wait, sizeof(sleep_wait));
	f.write((char*)&keykawaru, sizeof(keykawaru));



	for (auto c : colors) {
		f.write((char*)&c, sizeof(Color));
	}



	f.close();
	cout << "已保存" << endl;
}

void ReadColor() {
	std::ifstream f(colorsPath, std::ios::in);

	if (!f.is_open()) {
		return;
	}

	f.read((char*)&color_wait, sizeof(color_wait));
	f.read((char*)&hz_wait, sizeof(hz_wait));
	f.read((char*)&brightness, sizeof(brightness));
	f.read((char*)&sleep_wait, sizeof(sleep_wait));
	f.read((char*)&keykawaru, sizeof(keykawaru));

	while (!f.eof()) {
		Color c;
		f.read((char*)&c, sizeof(Color));
		colors.push_back(c);
	}
}
