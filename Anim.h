#pragma once
#ifndef ANIM_H
#define ANIM_H

#include <SFML/Graphics.hpp>
using namespace sf;

//////////////////////////////////////////////////////������/////////////////////////////////////////////////////////

class Animation
{
public:
	std::vector<IntRect> m_frames, m_frames_flip;	// ������� ������� � ���������� �������/������
	float m_currentFrame, m_speed;					// ������� �����/���� � ��������
	bool m_flip, m_isPlaying;						// flip - ����������/���. isPlaying - ����������� ��������/���
	Sprite m_sprite;

public:
	Animation();
	Animation(Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void tick(const float time);

};

class AnimationManager
{
public:
	String m_currentAnim;						// ��� ������� ��������
	std::map<String, Animation> m_animList;	// ������ �������� (������������ �����, ������ ��������)

public:
	AnimationManager() {};
	void create(String& name, Texture& texture, int x, int y, int width, int height, int count, float speed, int step);
	void draw(RenderWindow& window, int x = 0, int y = 0);
	void set(String& name) { m_currentAnim = name; }
	void flip(bool b) { m_animList[m_currentAnim].m_flip = b; }
	void tick(const float time) { m_animList[m_currentAnim].tick(time); }
	void pause() { m_animList[m_currentAnim].m_isPlaying = false; }
	void play() { m_animList[m_currentAnim].m_isPlaying = true; }

};

/////////////////////////////////////////////////////������ �������//////////////////////////////////////////////////////

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

	// ������� 2 ������� ������, ������� � ���������� ��� ��������.
	for (int i(0); i < count; ++i)
	{
		m_frames.push_back(IntRect(x + i * step, y, width, height));
		m_frames_flip.push_back(IntRect(x + i * step + width, y, -width, height));
	}
}

inline void Animation::tick(const float time)
{
	if (!m_isPlaying)
		return;

	m_currentFrame += m_speed * time;		// �������� ��������
	if (m_currentFrame > m_frames.size())	// ��������
		m_currentFrame -= m_frames.size();

	if (!m_flip)	// �������� ������� ��� ���������� ������/�����
		m_sprite.setTextureRect(m_frames[m_currentFrame]);
	else
		m_sprite.setTextureRect(m_frames_flip[m_currentFrame]);
}

void AnimationManager::create(String& name, Texture& texture, int x, int y, int width, int height, int count, float speed, int step)
{
	m_animList[name] = Animation(texture, x, y, width, height, count, speed, step);
	m_currentAnim = name;
}

inline void AnimationManager::draw(RenderWindow& window, int x, int y)
{
	m_animList[m_currentAnim].m_sprite.setPosition(x, y);
	window.draw(m_animList[m_currentAnim].m_sprite);
}

#endif // !ANIM_H
