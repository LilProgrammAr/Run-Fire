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
			state = jump; dy = -1; onGround = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = down;
		}
	}
	else {
		state = stay;
	}
}

void Entity::update(float time) {
	control();
	switch (state)//��������� �������� � ����������� �� ���������
	{
	case right:dx = speed; break;//��������� ���� ������
	case left:dx = -speed; break;//��������� ���� �����		
	}
	x += dx*time;
	y += dy*time;
	sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
	if (health <= 0) { life = false; }
	speed = 0;
	//dy = dy + 0.0015*time;//��������� ������������� � �����
}