#pragma once
#include "Anim.h"

class Player
{
public:
	FloatRect m_rect;					// Данные персонажа, координаты(x/y) и размер(ширина/высота) спрайта
	//float m_x, m_y;						// Положение в пространстве
	float m_dx, m_dy;					// Ускорение
	//float m_width, m_height;			// Ширина и высота
	bool m_onLadder, m_shoot, m_hit;	// hit - получен урон(мигает/неуязвим)
	std::map<std::string, bool> m_key;	// Ассоциативный массив для хранения нажатых клавиш
	AnimationManager m_anim;			// Для работы с анимациями
	bool m_onGround;
	int m_dir;
	enum class State
	{
		stay,
		walk,
		duck,
		jump,
		climb,
		swim,
	} m_state;							// Текущее сотсояние

public:
	Player(AnimationManager& anim);
	void keyCheck();					// Обработка нажатых/отпущеных клавиш
	void update(const float time);
	void collision(const int dir);		// Обработка столкновений с препятствиями

};


inline Player::Player(AnimationManager& anim)
{
	m_anim = anim;
	m_state = State::stay;

	// Старые параметры
	//m_sprite.setTexture(texture);
	m_rect = FloatRect(100, 180, 16, 16);
	m_dx = m_dy = 0.f;
	//m_currentFrame = 0.f;
	m_onGround = false;
}

inline void Player::keyCheck()
{
	/////////////////////////////////////////При нажатии клавиши////////////////////////////////////////////////////
	if (m_key["L"])	// Нажата влево
	{
		m_dir = 1;
		if (m_state != State::duck)		// Если мы лежали, то ползем
			m_dx = -0.1f;
		if (m_state == State::stay)		// Если мы стояли то идем
			m_state = State::walk;
	}
	if (m_key["R"])	// Нажата вправо
	{
		m_dir = 0;
		if (m_state != State::duck)
			m_dx = 0.1f;
		if (m_state == State::stay)
			m_state = State::walk;
	}
	if (m_key["Up"])	// Нажата вверх
	{
		if (m_state == State::stay || m_state == State::walk)	// Если мы стояли или шли, то прыгаем
		{
			m_dy = -0.27f;
			m_state = State::jump;
		}
		if (m_state == State::swim || m_state == State::climb)	// Если мы плаваем или на лестнице, то медленно подымаемся
		{
			m_dy = -0.05f;
		}
	}
	if (m_key["Down"])	// Нажата вниз
	{
		if (m_state == State::stay || m_state == State::walk)	// Если мы стояли или шли, то ложимся
		{
			m_dx = 0.f;
			m_state = State::duck;
		}
		if (m_state == State::swim || m_state == State::climb)	// Если мы плаваем или на лестнице, то медленно опускаемся
		{
			m_dy = 0.05f;
		}
	}
	if (m_key["Space"])		// Нажато пробел
	{
		m_shoot = true;
	}
	/////////////////////////////////////////При отпускании клавиши////////////////////////////////////////////////////	
	if (!(m_key["R"] || m_key["L"]))
	{
		m_dx = 0;
		if (m_state == State::walk)
			m_state = State::stay;
	}
	if (!(m_key["Up"] || m_key["Down"]))
	{
		if (m_state == State::swim || m_state == State::climb)
			m_dy = 0;
	}
	if (!m_key["Down"])
	{
		if (m_state == State::duck)
			m_state = State::stay;
	}
	if (!m_key["Space"])
	{
		m_shoot = false;
	}
}

inline void Player::update(const float time)
{
	keyCheck();

	if (m_state == State::stay)
		m_anim.set("stay");
	if (m_state == State::walk)
		m_anim.set("walk");
	if (m_state == State::jump)
		m_anim.set("jump");
	if (m_state == State::duck)
		m_anim.set("duck");
	if (m_state == State::climb)
	{
		m_anim.set("climb");
		m_anim.pause();
		if (m_dy != 0)
			m_anim.play();
		if (!m_onLadder)
			m_state = State::stay;
	}
	if (m_shoot)
	{
		m_anim.set("shoot");
		if (m_state == State::walk)
			m_anim.set("shootAndWalk");
	}
	if (m_dir)			// Поворот анимации
		m_anim.flip();

	m_rect.left += m_dx * time;			// m_x
	collision(0);

	m_dy += 0.005 * time;		// Постоянное притяжение к земле
	m_rect.top += m_dy * time;					//
	collision(1);

	m_anim.tick(time);
	m_key["R"] = m_key["L"] = m_key["Up"] = m_key["Down"] = m_key["Space"] = false;
}

inline void Player::collision(const int dir)
{
	for (int i = m_rect.top / 16; i < (m_rect.top + m_rect.height) / 16; i++)
		for (int j = m_rect.left / 16; j < (m_rect.left + m_rect.width) / 16; j++)
		{
			if ((TileMap[i][j] == 'P') || (TileMap[i][j] == 'k') || (TileMap[i][j] == '0') || (TileMap[i][j] == 'r') || (TileMap[i][j] == 't'))
			{
				if (m_dy > 0 && dir == true)
				{
					m_rect.top = i * 16 - m_rect.height;
					m_dy = 0;
					m_onGround = true;
				}
				if (m_dy < 0 && dir == true)
				{
					m_rect.top = i * 16 + 16;
					m_dy = 0;
				}
				if (m_dx > 0 && dir == false)
				{
					m_rect.left = j * 16 - m_rect.width;
				}
				if (m_dx < 0 && dir == false)
				{
					m_rect.left = j * 16 + 16;
				}
			}

			if (TileMap[i][j] == 'c') {
				// TileMap[i][j]=' '; 
			}
		}
}
