#ifndef GOLEM_H
#define GOLEM_H
#include "map.h"
#include "Monster.h"
#include "headers_which_you_need.h"
using namespace sf;

//������������ ����� �������
class Golem : public Monster {
public:
	//��������; ��������� ���������� (x,y); ������ �������; ��� ���������;
	Golem(Image &image, float X, float Y, int W, int H, String Name);
	void update(float, Map &);
	void check_collision(float, float, Map &);
	Sprite get_sprite();
private:
	float x, y;
	float dx, dy, speed;
	int w, h, health;
	bool life, onGround;
	Texture texture;
	Sprite sprite;
	String name;
};
#endif
