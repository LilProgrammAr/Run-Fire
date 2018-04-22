#ifndef ENTITY_H
#define ENTITY_H
#include "headers_which_you_need.h"
using namespace sf;

//������������ ����� �������
class Entity {
public:
	//��������; ��������� ���������� (x,y); ������ �������; ��� ���������;
	Entity(Image &image, float X, float Y, int W, int H, String Name);
	Sprite get_sprite();
private:
	float x, y;//���������� ������ � � �
	float dx, dy, speed;//�������� ��������� � ������� ������ ������������ ����, � ����������� �������� ���������
	int w, h, health;//������� ������� � ���-�� �������� ���������
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
};
#endif
