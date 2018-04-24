#ifndef ENTITY_H
#define ENTITY_H
#include "map.h"
#include "Bullet.h"
#include <vector>
#include "headers_which_you_need.h"
using namespace sf;

//������������ ����� �������
class Entity {
public:
	//��������; ��������� ���������� (x,y); ������ �������; ��� ���������;
	Entity(Image &image, float X, float Y, int W, int H, String Name);
	void control();
	void update(float,Map &);
	void check_collision(float, float,Map &);
	Sprite get_sprite();
	void fire();
	void draw_bullet(float time, Map & map, RenderWindow & window);
	
private:
	std::vector<Bullet> bul;
	enum { right, left, up, down, jump, stay } state;
	float x, y;//���������� ������ � � �
	float dx, dy, speed;//�������� ��������� � ������� ������ ������������ ����, � ����������� �������� ���������
	int w, h, health;//������� ������� � ���-�� �������� ���������
	bool life, onGround, is_right;
	Texture texture;
	Sprite sprite;
	String name;
	Texture bullet_texture;
};
#endif
