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
	void control();
	void update(float);
private:
	enum { right, left, up, down, jump, stay } state;
	float x, y;//���������� ������ � � �
	float dx, dy, speed;//�������� ��������� � ������� ������ ������������ ����, � ����������� �������� ���������
	int w, h, health;//������� ������� � ���-�� �������� ���������
	bool life, onGround;
	Texture texture;
	Sprite sprite;
	String name;
};
#endif
