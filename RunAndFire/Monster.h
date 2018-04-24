#ifndef MONSTER_H
#define MONSTER_H
#include "map.h"
#include "headers_which_you_need.h"
using namespace sf;

//������������ ����� �������
class Monster {
public:
	//��������; ��������� ���������� (x,y); ������ �������; ��� ���������;
	Monster(Image &image, float X, float Y, int W, int H, String Name);
	Sprite get_sprite();
protected:
	float x, y;
	float dx, dy, speed;
	int w, h, health;
	bool life, onGround;
	Texture texture;
	Sprite sprite;
	String name;
};
#endif
