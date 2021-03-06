#include "Entity.h"
#include "functions.h"
using namespace sf;

bool bossSpawned;
bool isLevelPassed;
size_t level_counter = 1;

Entity::Entity(Image &image, float X, float Y, int W, int H, String Name){
	isLevelPassed = false;
	doubleJump = false;
	up_pressed = false;
	isFly = false;
	up_pressed_second_time = false;
	clock.restart();
	x = X; y = Y; w = W; h = H; name = Name;
	bullets_quantity = PLAYET_BULLETS;
	speed = 0; health = PLAYER_HP ; dx = 0; dy = 0; static_speed = 0.2f; static_jump = 0.6f;
	life = true; onGround = false; space_pressed = false; sprite_right = true; with_mob = false;
	is_right = true;
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setTextureRect(IntRect(3, 18, w, h));
	sprite.setOrigin(w / 2.f, h / 2.f);
	sprite.setPosition(x + w / 2.f, x + h / 2.f);
	w /= 2; h /= 2;
	Image bullet_Image; bullet_Image.loadFromFile("images/bullets.png");
	bullet_Image.createMaskFromColor(Color(0, 0, 0));
	bullet_texture.loadFromImage(bullet_Image);

	Text text("HP: ", *font, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);//��� ������ ����� (�� ������)
	text.setFillColor(Color::Black);//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
	text.setStyle(sf::Text::Bold /*| sf::Text::Underlined*/);//������ � ������������ �����. �� ��������� �� "�����":)) � �� ������������
	String str = HP_TEXT;
	str.insert(str.getSize(), std::to_string(health));
	text.setString(str);
	text.setPosition(440, 30);
	hp_text = text;
	
}

Sprite& Entity::get_sprite() {
	return sprite;
}

void Entity::Restart(Map & map, std::vector<std::unique_ptr<Golem>> & golems, std::vector<std::unique_ptr<Ghost>> & ghosts, Loot & loot) {
		isLevelPassed = false;
		bossSpawned = false;
		isFly = false;
		spawn(map);
		map.reset();
		bul.clear();
		dx = 0;
		dy = 0;
		health = PLAYER_HP;
		state = State::stay;
		//hp_text.setString(HP_TEXT + std::to_string(health));
		bullets_quantity = PLAYET_BULLETS;
		if (!life) if (is_right) sprite.rotate(-90);
				   else sprite.rotate(90);
		life = true;
		loot.clear();
		golems.clear();
		ghosts.clear();
		Image monster_Image; monster_Image.loadFromFile("images/Monster.png");
		monster_Image.createMaskFromColor(Color(255, 255, 255));
		golems_spawn(monster_Image, 28, 34, golems, map);
		ghosts_spawn(monster_Image, 23, 28, ghosts, map);
		loot_spawn(loot, map);
}

//������� ���������� ����������
void Entity::control() {
	//467 53
	//493 100
	//34 47
	if (!with_mob) {//���� ������ �������
		if (Keyboard::isKeyPressed(Keyboard::A)) {//����
			state = State::left; speed = static_speed; is_right = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::D)) {//�����
			state = State::right; speed = static_speed; is_right = true;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::W)) && !up_pressed && !up_pressed_second_time && !onGround)
		{
			up_pressed_second_time = true;
			up_pressed = true;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::W)) && (onGround || doubleJump)) {//���� ������ ������� ����� � �� �� �����, �� ����� �������
			state = State::jump; dy = -static_jump; onGround = false; doubleJump = false; up_pressed = true;
			isFly = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::S) && (onGround || doubleJump)) {
			state = State::down;
		}
		if ((!Keyboard::isKeyPressed(Keyboard::Space) && !Keyboard::isKeyPressed(Keyboard::W)) && up_pressed)
		{
			up_pressed = false;
			//up_pressed_second_time = false;  
		}
		//fire
		if ((Keyboard::isKeyPressed(Keyboard::Up)
			|| Keyboard::isKeyPressed(Keyboard::Down)
			|| Keyboard::isKeyPressed(Keyboard::Left)
			|| Keyboard::isKeyPressed(Keyboard::Right)))
		{
			if (!space_pressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Left)) {
					is_right = false;
					dir = Directions::ul;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Up) && Keyboard::isKeyPressed(Keyboard::Right)) {
					is_right = true;
					dir = Directions::ur;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Left)) {
					is_right = false;
					dir = Directions::dl;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down) && Keyboard::isKeyPressed(Keyboard::Right)) {
					is_right = true;
					dir = Directions::dr;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Up)) {
					dir = Directions::u;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Down)) {
					dir = Directions::d;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Left)) {
					is_right = false;
					dir = Directions::l;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Right)) {
					is_right = true;
					dir = Directions::r;
				}
				if (bullets_quantity > 0) { fire(); }
				space_pressed = true;
			}
		}
		else if (space_pressed) space_pressed = false;

	}
	else {
		state = State::stay;
	}
	//Restart();
}

void Entity::update(float time, Map & map, std::vector<std::unique_ptr<Golem>> & golems, std::vector<std::unique_ptr<Ghost>> & ghosts, Loot & loot) {
	//std::cout << isFly << std::endl;
	if (Keyboard::isKeyPressed(Keyboard::R))
		Restart(map, golems, ghosts, loot);
		loot.update(map);
	if (life) {
		madeDoubleJumpOnThisTurn = up_pressed_second_time;
		control();
		if (madeDoubleJumpOnThisTurn != up_pressed_second_time) madeDoubleJumpOnThisTurn = true; else madeDoubleJumpOnThisTurn = false;
		//madeDoubleJumpOnThisTurn = true;
		switch (state)//��������� �������� � ����������� �� ���������
		{
		case State::right: dx = speed; break;//��������� ���� ������
		case State::left: dx = -speed; break;//��������� ���� �����	
		case State::down: dx = 0; break;
			//case stay: dx = 0; dy = 0; break;
		}
		if (is_right && !sprite_right || !is_right && sprite_right) {
			sprite.scale(-1, 1); sprite_right = !sprite_right;
		}
		x += dx*time;
		check_collision(dx, 0, map);
		y += dy*time;
		check_collision(0, dy, map);

		check_collision(golems, ghosts);
		check_collision(loot);

		sprite.setPosition(x + w / 2, y + h / 2); //������ ������� ������� � ����� ��� ������
		speed = 0;
		if (dy > 0 && !isFly) isFly = true;
		dy = dy + STATIC_G*time;//��������� ������������� � �����
		
		if (health <= 0 || y > 500) {
			life = false;
			health = 0;
			if (is_right) sprite.rotate(90);
			else sprite.rotate(-90);
			dy = -0.5;
			//dy = 0;//�������� �� ����� ������������ ���� ������� ��� ������
		}
		if (onGround) up_pressed_second_time = false;
	}
	else if (!onGround) {
		y += dy*time;
		//std::cout << dy << std::endl;
		check_collision(0, dy, map);
		sprite.setPosition(x + w / 2, y + h / 2);
		dy = dy + STATIC_G*time;
		isFly = true;
	}
}

void Entity::check_collision(float dx, float dy, Map & map) {
	for (int i = static_cast<int>(y / TITLE_SIZE); i < (y + h) / TITLE_SIZE; i++) {
		if (i < 0 || i >= map.get_h()) continue;
		for (int j = static_cast<int>(x / TITLE_SIZE); j < (x + w) / TITLE_SIZE; j++) {
			if (j < 0 || j >= map.get_w()) continue;
			onGround = false;
			if (map[i][j] == 'w' || map[i][j] == 'v' && !map.isInter())
			{

				if (dy > 0)
				{
					y = static_cast<float>(i * TITLE_SIZE - h);
					this->dy = 0;
					this->dx = 0;
					onGround = true;
					with_mob = false;
					state = State::stay;
					isFly = false;
					return;
				}
				if (dy < 0)
				{
					y = static_cast<float>(i * TITLE_SIZE + TITLE_SIZE);
					this->dy = 0;
				}
				if (dx > 0)
				{
					x = static_cast<float>(j * TITLE_SIZE - w);
				}
				if (dx < 0)
				{
					x = static_cast<float>(j * TITLE_SIZE + TITLE_SIZE);
				}
			}
			if (life)
			{
				/*
				if (map[i][j] == 'd' && up_pressed_second_time && madeDoubleJumpOnThisTurn) {
					map[i][j] = '0';
					doubleJump = true;
					up_pressed_second_time = false;
					isFly = false;
					crates.push_back(std::make_pair(Point(i, j), std::chrono::high_resolution_clock::now()));
					//!!!!
					if (j == 2 && level_counter == 1) map.move();
				}
				*/
				if (map[i][j] == 'h') {
					map[i][j] = '0';
					health += MED_KIT_HP_BOOST;
					if (health > PLAYER_HP) health = PLAYER_HP;
					//hp_text.setString(HP_TEXT + std::to_string(health));
				}

			}
		}
	}
	for (int i = static_cast<int>((y + 8) / TITLE_SIZE); i < (y + h - 8) / TITLE_SIZE; i++) {
		if (i < 0 || i >= map.get_h()) continue;
		for (int j = static_cast<int>((x + 8) / TITLE_SIZE); j < (x + w - 8) / TITLE_SIZE; j++) {
			if (j < 0 || j >= map.get_w()) continue;
			if (life)
			{
				if (map[i][j] == 's') {
					//life = false;
					health -= 100;
					if (health < 0) health = 0;
					dy = -static_jump / 4.f;
				}
			}
		}
	}
	for (int i = static_cast<int>((y - 17) / TITLE_SIZE); i < (y + h + 17) / TITLE_SIZE; i++) {
		if (i < 0 || i >= map.get_h()) continue;
		for (int j = static_cast<int>((x - 17) / TITLE_SIZE); j < (x + w + 17) / TITLE_SIZE; j++) {
			if (j < 0 || j >= map.get_w()) continue;
			if (life)
			{
				if (map[i][j] == 'd' && up_pressed_second_time && madeDoubleJumpOnThisTurn) {
					map[i][j] = '0';
					doubleJump = true;
					up_pressed_second_time = false;
					isFly = false;
					crates.push_back(std::make_pair(Point(i, j), std::chrono::high_resolution_clock::now()));
					//!!!!
					if (j == 2 && level_counter == 1) map.move();
				}
			}
		}
	}
	if (!crates.empty()) {	
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - crates.front().second).count() > CRATES_RESPAWN) {
			map[crates.front().first.x][crates.front().first.y] = 'd';
			crates.erase(crates.begin());
		}
	}
}

void Entity::check_collision(Loot & loot) {
	for (size_t i = 0; i < loot.ammos.size(); i++) {
		float gx = loot.ammos[i].x, gy = loot.ammos[i].y, gh = loot.get_rect().height, gw = loot.get_rect().width;
		if (square_in_square(x, y, static_cast<float>(w), static_cast<float>(h), gx, gy, gw, gh) ||
			square_in_square(gx, gy, gw, gh, x, y, static_cast<float>(w), static_cast<float>(h)) ){
				
			loot.ammo_aword(bullets_quantity);
			loot.ammos.erase(loot.ammos.begin() + i); i--;
		}
	}
	for (size_t i = 0; i < loot.portals.size(); i++) {
		float gx = loot.portals[i].x, gy = loot.portals[i].y, gh = loot.get_rect().height, gw = loot.get_rect().width;
		if (!isLevelPassed)
			if (square_in_square(x, y, static_cast<float>(w), static_cast<float>(h), gx, gy, gw, gh) ||
				square_in_square(gx, gy, gw, gh, x, y, static_cast<float>(w), static_cast<float>(h))) {
				level_counter++;
				std::cout << level_counter << std::endl;
				isLevelPassed = true;
			}
	}
}

void Entity::check_collision(std::vector<std::unique_ptr<Golem>> & golems, std::vector<std::unique_ptr<Ghost>> & ghosts) {
	for (size_t i = 0; i < golems.size(); i++) {
		float gx = golems[i]->get_x(), gy = golems[i]->get_y(), gh = static_cast<float>(golems[i]->get_h()), gw = static_cast<float>(golems[i]->get_w());
		if (square_in_square(x + 2, y + 2, static_cast<float>(w) - 4, static_cast<float>(h) - 4, gx, gy, gw, gh) ||
			square_in_square(gx, gy, gw, gh, x + 2, y + 2, static_cast<float>(w) - 4, static_cast<float>(h) - 4)) {

			if (!with_mob) health -= golems[i]->get_damage();
			if (health < 0) health = 0;
			if (golems[i]->get_right() && !this->is_right || !golems[i]->get_right() && this->is_right) golems[i]->change_direction();
			with_mob = true;

			if (golems[i]->get_right()) {
				dx = -static_speed * 1.5f;
				dy = -static_jump / 4.f;
			}
			else {
				dx = static_speed * 1.5f;
				dy = -static_jump / 4.f;
			}

		}
	}
	for (size_t i = 0; i < ghosts.size(); i++) {
		float gx = ghosts[i]->get_x(), gy = ghosts[i]->get_y(), gh = static_cast<float>(ghosts[i]->get_h()), gw = static_cast<float>(ghosts[i]->get_w());
		if (isFly)
			if (square_in_square(x + w/4, y + h, static_cast<float>(w) / 2, 1, gx, gy, gw, 8) ||
				square_in_square(gx, gy, gw, 8, x + w / 4, y + h, static_cast<float>(w) / 2, 1)) {

				ghosts[i]->damaged();
				if (health < 0) health = 0;
				//with_mob = true;
				dy = -static_jump;

			}
		if (ghosts[i]->checkBullets(x, y, w, h)) health -= ghosts[i]->get_damage();
		if (health < 0) health = 0;
	}
	//hp_text.setString(HP_TEXT + std::to_string(health));
}


void Entity::fire() {

	int t_x, t_y;
	switch (dir)
	{
	case Directions::u:
		t_x = x + w / 2;
		t_y = y;
		break;
	case Directions::d:
		t_x = x + w / 2;
		t_y = y + h;
		break;
	case Directions::l:
		t_x = x;
		t_y = y + h / 3;
		break;
	case Directions::r:
		t_x = x + w;
		t_y = y + h / 3;
		break;
	case Directions::ur:
		t_x = x + w;
		t_y = y;
		break;
	case Directions::ul:
		t_x = x;
		t_y = y;
		break;
	case Directions::dr:
		t_x = x + w;
		t_y = y + h;
		break;
	case Directions::dl:
		t_x = x;
		t_y = y + h;
		break;
	}
	Bullet temp(bullet_texture, t_x, t_y, 13, 10, "piu", dir);
	bullets_quantity--;
	bul.push_back(temp);
}

void Entity::draw_bullet(float time, Map & map, RenderWindow & window, std::vector<std::unique_ptr<Golem>> & golems, std::vector<std::unique_ptr<Ghost>> & ghosts) {
	for (size_t i = 0; i < bul.size(); i++) {
		window.draw(bul[i].get_sprite());
		int temp = bul[i].update(time, map, golems, ghosts);
		if (temp == -1) { 
   			bul.erase(bul.begin() + i); i--; }
		if (bul.size() == 0) return;
	}
}

bool Entity::alive() {
	return life;
}

int Entity::ammo() {
	return bullets_quantity;
}

void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

int Entity::hp() {
	return health;
}
void Entity::spawn(Map & map)
{
	for (int i = 0; i < map.get_h(); i++) {
		for (int j = 0; j < map.get_w(); j++) {
			if (map[i][j] == 'b')
			{
				x = j * TITLE_SIZE;
				y = i * TITLE_SIZE;
			}
		}
	}
}

float Entity::getX()
{
	return x;
}
float Entity::getY()
{
	return y;
}