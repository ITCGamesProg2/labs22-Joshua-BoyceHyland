#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
class Target
{
public:
	Target(sf::Texture const& t_texture, sf::Vector2f t_position, float t_float);
	void draw(sf::RenderWindow& t_window);
	void despawn();
	void respawn();
	void setPosition(sf::Vector2f t_position);
	void setOffSet(float t_offSet);
	sf::Sprite getBody();
	bool isAlive();
	bool beenShot();
	void updateTimer();

private:
	sf::Sprite m_target;
	sf::Texture m_texture;
	sf::Vector2f m_position;
	float m_offSet{ 0 };
	bool alive {false};
	bool m_beenShot{ false };
	sf::Clock m_timedLife;
	sf::Time  m_lifeSpan{ sf::seconds(6) };
	sf::Time m_blinkingTime{ sf::seconds(3) };
	bool m_blinkingState{ false };
	static int leftOverTime;
};