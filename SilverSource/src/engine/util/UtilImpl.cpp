#include "ColorUtil.h"
#include "VectorUtil.h"
#include "StringUtil.h"


std::string loadStringFromFile(std::string path)
{
	std::ifstream t(path);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return str;
}

std::vector<std::string> splitByDelimiter(std::string& data, char delim)
{
	std::vector<std::string> out;

	std::stringstream ss(data);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		if (!item.empty())
		{
			out.push_back(item);
		}
	}

	return out;
}

float getAngleVec(sf::Vector2f v)
{
	return radToDeg(std::atan2(v.y, v.x));
}

float magnitudevec(sf::Vector2f vec)
{
	return std::sqrtf(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f normalizevec(sf::Vector2f vec)
{
	float mag = magnitudevec(vec);
	return sf::Vector2f(vec.x / mag, vec.y / mag);
}

sf::Color lerpColor(sf::Color a, sf::Color b, float t)
{
	if (t < 0.0f) { t = 0.0f; }
	if (t > 1.0f) { t = 1.0f; }
	float ar = a.r; float ag = a.g; float ab = a.b; float aa = a.a;
	float br = b.r; float bg = b.g; float bb = b.b; float ba = b.a;
	float lr = (1.0 - t) * a.r + t * b.r;
	float lg = (1.0 - t) * a.g + t * b.g;
	float lb = (1.0 - t) * a.b + t * b.b;
	float la = (1.0 - t) * a.a + t * b.a;

	return sf::Color(lr, lg, lb, la);
}

sf::Vector2f rotateVec(sf::Vector2f v, float angle)
{
	float theta = degToRad(angle);

	float cosine = std::cos(theta);
	float sine = std::sin(theta);

	return sf::Vector2f(v.x * cosine - v.y * sine, v.x * sine + v.y * cosine);
}

float degToRad(float deg)
{
	return (deg / 180.0f) * PI_F;
}

float radToDeg(float rad)
{
	return (rad / PI_F) * 180.0f;
}

sf::Vector2f moveToVec(sf::Vector2f v, sf::Vector2f t, float step)
{
	return sf::Vector2f(moveTo(v.x, t.x, step), moveTo(v.y, t.y, step));
}

sf::Vector2f reduceVec(sf::Vector2f v, float step)
{
	return moveToVec(v, sf::Vector2f(0, 0), step);
}

float getDist(sf::Vector2f a, sf::Vector2f b)
{
	float xs = (a.x - b.x);
	float ys = (a.y - b.y);
	return std::sqrtf(xs * xs + ys * ys);
}

float getAngleBetween(sf::Vector2f a, sf::Vector2f b)
{
	float ang = radToDeg(std::atan2(b.y - a.y, b.x - a.x));
	if (ang < 0.0f) { ang += 360.0f; }

	return ang;
}

float moveTo(float a, float target, float step)
{
	if (a > target)
	{
		if (a - step < target)
		{
			return target;
		}
		else
		{
			return a - step;
		}
	}
	else if(a < target)
	{
		if (a + step > target)
		{
			return target;
		}
		else
		{
			return a + step;
		}
	}
	else
	{
		return target;
	}
}

float moveAngleTo(float a, float target, float step)
{
	float cwDist = a - target;
	float ccwDist = target - a;
	if (cwDist < 0) { cwDist = 360.0f + cwDist; }
	if (ccwDist < 0) { ccwDist = 360.0f + ccwDist; }
	
	if (cwDist > ccwDist)
	{
		if (a + step > target)
		{
			return target;
		}
		else
		{
			return a + step;
		}
	}
	else
	{
		if (a - step < target)
		{
			return target;
		}
		else
		{
			return a - step;
		}
	}
}