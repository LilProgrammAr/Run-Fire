#ifndef ENTITY_H
#define ENTITY_H
#include "map.h"
#include "Golem.h"
#include "Bullet.h"
#include <vector>
#include "headers_which_you_need.h"
using namespace sf;

//������������ ����� �������
class Entity {
public:
	int health;
	//��������; ��������� ���������� (x,y); ������ �������; ��� ���������;
	Entity(Image &image, float X, float Y, int W, int H, String Name);
	void control();
	void update(float,Map &, std::vector<Golem> &);
	void check_collision(float, float,Map &);
	void check_collision(std::vector<Golem> &);
	Sprite get_sprite();
	void fire();
	void draw_bullet(float time, Map & map, RenderWindow & window, std::vector<Golem> & golems);
	
private:
	std::vector<Bullet> bul;
	enum { right, left, up, down, jump, stay } state;
	float x, y;//���������� ������ � � �
	float dx, dy, speed;//�������� ��������� � ������� ������ ������������ ����, � ����������� �������� ���������
	int w, h;//������� ������� � ���-�� �������� ���������
	bool life, onGround, is_right, space_pressed, sprite_right, with_mob;
	Texture texture;
	Sprite sprite;
	String name;
	Texture bullet_texture;
};
#endif
