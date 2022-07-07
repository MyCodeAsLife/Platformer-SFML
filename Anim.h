#pragma once
#ifndef ANIM_H
#define ANIM_H

#include <SFML/Graphics.hpp>
using namespace sf;

//////////////////////////////////////////////////////Классы/////////////////////////////////////////////////////////

class Animation
{
public:
	std::vector<IntRect> m_frames, m_frames_flip;	// Массивы обычных и зеркальных фреймов/кадров
	float m_currentFrame, m_speed;					// Текущий фрейм/кадр и скорость
	bool m_flip, m_isPlaying;						// flip - зеркальный/нет. isPlaying - проигрывать анимацию/нет
	Sprite m_sprite;

public:
	Animation();
	Animation(Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void tick(float time);							// Воспроизведение анимации

};

class AnimationManager
{
public:
	String m_currentAnim;						// Имя текущий анимации
	std::map<String, Animation> m_animList;	// Список анимаций (соответствие имени, списку анимаций)

public:
	AnimationManager() {};
	void create(const String& name, Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void draw(RenderWindow& window, int x = 0, int y = 0);
	void set(const String& name) { m_currentAnim = name; }
	void flip(bool b = 1) { m_animList[m_currentAnim].m_flip = b; }
	void tick(float time) { m_animList[m_currentAnim].tick(time); }
	void pause() { m_animList[m_currentAnim].m_isPlaying = false; }
	void play() { m_animList[m_currentAnim].m_isPlaying = true; }

};

/////////////////////////////////////////////////////Методы классов//////////////////////////////////////////////////////

inline Animation::Animation()
{
	m_currentFrame = 0;
	m_isPlaying = true;
	m_flip = false;
	//m_loop = true;
}

inline Animation::Animation(Texture& texture, int x, int y, int width, int height, int count, float speed, int step) :
	m_speed(speed), m_currentFrame(0), m_isPlaying(true), m_flip(false)
{
	m_sprite.setTexture(texture);

	// Создаем 2 массива кадров, обычных и зеркальных для анимаций.
	for (int i(0); i < count; ++i)
	{
		m_frames.push_back(IntRect(x + i * step , y, width, height));
		m_frames_flip.push_back(IntRect(x + i * step + width, y, -width, height));
	}
}

inline void Animation::tick(float time)
{
	if (!m_isPlaying)
		return;

	m_currentFrame += m_speed * time;			// Анимация(текущий кадр)
	while (m_currentFrame > m_frames.size())	// Откат кадров анимации (цикл для того чтобы случайно не выскачить за массив кадров)
	{
		m_currentFrame -= m_frames.size();
		if (m_currentFrame < 0)
			m_currentFrame = 0;
	}
	if (!m_flip)	// Выбираем обычные или зеркальные фреймы/кадры
		m_sprite.setTextureRect(m_frames[static_cast<int>(m_currentFrame)]);
	else
		m_sprite.setTextureRect(m_frames_flip[static_cast<int>(m_currentFrame)]);
}

void AnimationManager::create(const String& name, Texture& texture, int x, int y, int width, int height, int count, float speed, int step = 0)
{
	m_animList[name] = Animation(texture, x, y, width, height, count, speed, step);
	m_currentAnim = name;
	//Animation a;
	//a.m_speed = speed;
	////a.m_loop = Loop;
	//a.m_sprite.setTexture(texture);
	//a.m_sprite.setOrigin(0, height);

	//for (int i = 0; i < count; i++)
	//{
	//	a.m_frames.push_back(IntRect(x + i * step, y, width, height));
	//	a.m_frames_flip.push_back(IntRect(x + i * step + width, y, -width, height));
	//}
	//m_animList[name] = a;
	//m_currentAnim = name;
}

inline void AnimationManager::draw(RenderWindow& window, int x, int y)
{
	m_animList[m_currentAnim].m_sprite.setPosition(x, y);
	window.draw(m_animList[m_currentAnim].m_sprite);
}

#endif // !ANIM_H
