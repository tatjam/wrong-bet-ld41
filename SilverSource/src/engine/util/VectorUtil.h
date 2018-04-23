#pragma once
#include "SFML/System.hpp"

#ifndef PI
#define PI 3.141592
#endif

#ifndef PI_F
#define PI_F 3.141592f
#endif

sf::Vector2f normalizevec(sf::Vector2f vec);
float magnitudevec(sf::Vector2f vec);

sf::Vector2f reduceVec(sf::Vector2f v, float step);
sf::Vector2f moveToVec(sf::Vector2f v, sf::Vector2f t, float step);

sf::Vector2f rotateVec(sf::Vector2f v, float angle);

float getAngleVec(sf::Vector2f v);
float getAngleBetween(sf::Vector2f a, sf::Vector2f b);

float getDist(sf::Vector2f a, sf::Vector2f b);

float degToRad(float deg);
float radToDeg(float rad);

float moveTo(float a, float target, float step);
// This version knows that a < 0 = 360 - a and will
// take the real shortest path
float moveAngleTo(float a, float target, float step);