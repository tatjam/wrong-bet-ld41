#include "ColorUtil.h"

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