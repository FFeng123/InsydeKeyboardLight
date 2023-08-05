#include "var.hpp"
#include "InsydeDCHU.hpp"

#include <cstring>
#include <string>
#include <fstream>
#include <iostream>

using std::cin, std::cout, std::endl;


const std::string colorsPath = "colors.bin";


void SetColor() {
	cout << "�ѽ�����ɫ�༭ģʽ��������ʽ��ʾ�ѱ��棬����༭���ᱻ����" << endl;


	cout << "������ÿ����ɫ������ʱ�䣨���룩" << endl;
	cin >> color_wait;
	cout << "������ˢ��ʱ�䣨���룩������ϣ�������ܹ�ÿ����10�εĽ��䣬�˴�Ӧ����д100����Ϊ 1000 / 10 = 100��" << endl;
	cin >> hz_wait;
	cout << "��������̵ƹ����ȣ�0-1��" << endl;
	cin >> brightness;
	cout << "�������������ʱ�䣨���룩" << endl;
	cin >> sleep_wait;

	char mode;
	while (true) {
		cout << "�Ƿ���Ҫ��������任��ɫ(y/n)" << endl;
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
		InsydeDCHU::SetBrightness(255);
		while (true) {
			cout << "����������ɫ��RGBֵ������0-255���֣��ÿո�س����ַ�������" << endl;
			int r, g, b;
			cin >> r >> g >> b;
			Color color{ (unsigned char)r,(unsigned char)g,(unsigned char)b };

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
	f.write((char*)&brightness, sizeof(brightness));
	f.write((char*)&sleep_wait, sizeof(sleep_wait));
	f.write((char*)&keykawaru, sizeof(keykawaru));



	for (auto c : colors) {
		f.write((char*)&c, sizeof(Color));
	}



	f.close();
	cout << "�ѱ���" << endl;
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
