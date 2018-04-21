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