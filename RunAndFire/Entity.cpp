#include "Entity.h"
using namespace sf;

Entity::Entity(Image &image, float X, float Y, int W, int H, String Name) {
	x = X; y = Y; w = W; h = H; name = Name;
	speed = 0; health = 100; dx = 0; dy = 0;
	life = true; onGround = false;
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(3, 18, w, h));
	sprite.setOrigin(w / 2, h / 2);
	sprite.setPosition(w / 2, h / 2);
}

Sprite Entity::get_sprite() {
	return sprite;
}

//������� ���������� ����������
void Entity::control() {
	if (Keyboard::isKeyPressed) {//���� ������ �������
		if (Keyboard::isKeyPressed(Keyboard::Left)) {//����
			state = left; speed = 0.1;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {//�����
			state = right; speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
			state = jump; dy = -0.6; onGround = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = down;
		}
		if (Keyboard::isKeyPressed(Keyboard::R)) {
			x = 0;
			y = 0;
		}
	}
	else {
		state = stay;
	}
}

void Entity::update(float time, Map & map) {
	control();
	switch (state)//��������� �������� � ����������� �� ���������
	{
	case right: dx = speed; break;//��������� ���� ������
	case left: dx = -speed; break;//��������� ���� �����	
	case down: dx = 0; break;
	//case stay: dx = 0; dy = 0; break;
	}
	x += dx*time;
	check_collision(dx, 0, map);
	y += dy*time;
	check_collision(0, dy, map);

	sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
	if (health <= 0) { life = false; }
	speed = 0;
	dy = dy + 0.0015*time;//��������� ������������� � �����
}

void Entity::check_collision(float dx, float dy, Map & map) {
	for (int i = y / 32; i < (y + h) / 32; i++) {
		for (int j = x / 32; j < (x + w) / 32; j++) {
			if (map.get(i,j) == 'w')
			{
				if (dy>0)
				{
					y = i * 32 - h;
					this->dy = 0;
					onGround = true;
				}
				if (dy<0)
				{
					y = i * 32 + 32;
				}
				if (dx>0)
				{
					x = j * 32 - w;
				}
				if (dx < 0)
				{
					x = j * 32 + 32;
				}
			}
		}
	}
}