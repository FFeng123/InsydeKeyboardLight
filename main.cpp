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
	cout << "�ѽ�����ɫ�༭ģʽ��������ʽ��ʾ�ѱ��棬����༭���ᱻ����" << endl;


	cout << "������ÿ����ɫ������ʱ�䣨���룩" << endl;
	cin >> color_wait;
	cout << "������ˢ��ʱ�䣨���룩������ϣ�������ܹ�ÿ����10�εĽ��䣬�˴�Ӧ����д100����Ϊ 1000 / 10 = 100��" << endl;
	cin >> hz_wait;
	

	char mode;
	while (true) {
		cout << "���������������Ҫ��ô����a: ������ǰ��ɫ��׷���µ���ɫ c: ��յ�ǰ��ɫ����ͷ�����ɫ s: ��������ɫ�������������򱣴�" << endl;
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
			cout << "����������ɫ��RGBֵ������0-255���֣��ÿո�س����ַ�������" << endl;
			int r, g, b;
			cin >>r >> g >> b;
			Color color{(unsigned char)r,(unsigned char)g,(unsigned char)b};
			
			InsydeDCHU::setColor(color.R, color.G, color.B);
			cout << "��ɫ�ѳ��������̱��⣬�Ƿ���ܣ�y: ���ղ�������һ����ɫ n: �ܾ����������� e: ���ܲ��������򱣴�" << endl;

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
	cout << "�ѱ���" << endl;
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