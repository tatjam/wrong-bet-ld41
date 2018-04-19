#pragma once
#include <random>
#include "SFML/Graphics.hpp"

struct RangeFloat
{
	float min = -1.0f;
	float max = 1.0f;

	float operator()()
	{
		if (max < min)
		{
			std::swap(max, min);
		}

		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = max - min;
		float r = random * diff;
		return min + r;
	}

	RangeFloat(float min, float max) { this->min = min; this->max = max; }
};

struct RangeInt
{
	int min = -100;
	int max = 100;

	int operator()()
	{
		if (max < min)
		{
			std::swap(max, min);
		}

		return min + rand() % (max - min + 1);
	}

	RangeInt(int min, int max) { this->min = min; this->max = max; }
};

struct RangeColor
{
	sf::Color A;
	sf::Color B;

	sf::Color operator()()
	{
		return sf::Color(RangeInt(A.r, B.r)(), RangeInt(A.g, B.g)(), 
			RangeInt(A.b, B.b)(), RangeInt(A.a, B.a)());
	}

	RangeColor(sf::Color a, sf::Color b) { this->A = a; this->B = b; }
};

struct RangeVector2f
{
	sf::Vector2f A;
	sf::Vector2f B;

	sf::Vector2f operator()()
	{
		return sf::Vector2f(RangeFloat(A.x, B.x)(), RangeFloat(A.y, B.y)());
	}

	RangeVector2f(sf::Vector2f a, sf::Vector2f b) { this->A = a; this->B = b; }
};