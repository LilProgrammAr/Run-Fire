#include "Entity.h"
#include "functions.h"
using namespace sf;

Entity::Entity(Image &image, float X, float Y, int W, int H, String Name) {
	x = X; y = Y; w = W; h = H; name = Name;
	speed = 0; health = 100; dx = 0; dy = 0;
	life = true; onGround = false; space_pressed = false; sprite_right = true; with_mob = false;
	is_right = true;
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(3, 18, w, h));
	sprite.setOrigin(w / 2, h / 2);
	sprite.setPosition(x + w / 2, x + h / 2);

	Image bullet_Image; bullet_Image.loadFromFile("images/bullets.png");
	bullet_Image.createMaskFromColor(Color(0, 0, 0));
	bullet_texture.loadFromImage(bullet_Image);
}

Sprite Entity::get_sprite() {
	return sprite;
}

//������� ���������� ����������
void Entity::control() {
	
	if (Keyboard::isKeyPressed && !with_mob) {//���� ������ �������
		if (Keyboard::isKeyPressed(Keyboard::Left)) {//����
			state = left; speed = 0.1; is_right = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {//�����
			state = right; speed = 0.1; is_right = true;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
			state = jump; dy = -0.6; onGround = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			state = down;
		}
		if (Keyboard::isKeyPressed(Keyboard::Space) && !space_pressed) {
			fire();
			space_pressed = true;
		}
		//��� ���� �� ���
		// ������� (1) �� ������ ������� �����������
		if (!Keyboard::isKeyPressed(Keyboard::Space) && space_pressed) {
			space_pressed = false;
		}
	}
	else {
		state = stay;
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		x = 0;
		y = 0;
		dx = 0;
		dy = 0;
	}
}

void Entity::update(float time, Map & map, std::vector<Golem> & golems) {
	control();
	switch (state)//��������� �������� � ����������� �� ���������
	{
	case right: dx = speed; break;//��������� ���� ������
	case left: dx = -speed; break;//��������� ���� �����	
	case down: dx = 0; break;
	//case stay: dx = 0; dy = 0; break;
	}
	if (is_right && !sprite_right || !is_right && sprite_right) {
		sprite.scale(-1, 1); sprite_right = !sprite_right;
	}
	x += dx*time;
	check_collision(dx, 0, map);
	y += dy*time;
	check_collision(0, dy, map);

	check_collision(golems);

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
					if (with_mob) this->dx = 0;
					with_mob = false;
				}
				if (dy<0)
				{
					y = i * 32 + 32;
					this->dy = 0;
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

void Entity::check_collision(std::vector<Golem> & golems) {
	for (int i = 0; i < golems.size(); i++) {
		float gx = golems[i].get_x(), gy = golems[i].get_y(), gh = golems[i].get_h(), gw = golems[i].get_w();
		if (square_in_square(x, y, w, h, gx, gy, gw, gh) ||
			square_in_square(gx, gy, gw, gh, x, y, w, h) ){
			
			if (!with_mob) health -= golems[i].get_damage();
			golems[i].change_direction();
			with_mob = true;

			if (golems[i].get_right()) {
				dx = -0.2;
				dy = -0.3;
			}
			else {
				dx = 0.2;
				dy = -0.3;
			}

		}
	}
}

void Entity::fire() {
	
	Bullet temp(bullet_texture, sprite_right ? x+w : x, y+h/3, 13, 10, "piu",is_right);
	bul.push_back(temp);
}

void Entity::draw_bullet(float time, Map & map, RenderWindow & window, std::vector<Golem> & golems) {
	for (int i = 0; i < bul.size(); i++) {
		window.draw(bul[i].get_sprite());
		int temp = bul[i].update(time, map, golems);
		if (temp == -1) { 
   			bul.erase(bul.begin() + i); i--; }
		if (bul.size() == 0) return;
	}
}
