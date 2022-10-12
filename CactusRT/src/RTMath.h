#pragma once

#include <math.h>
#include <glm/glm.hpp>
#include <iostream>

class RTMath
{
public:
	static glm::vec2 SphereIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float sphereRadius) 
	{
		//Syntax sugar
		const glm::vec3 o = rayOrigin, d = rayDirection;
		const float r = sphereRadius;

		//Calculating each members of quadratic equation ax²+bx+c=0
		const float a = pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2);
		const float b = (2 * o.x * d.x) + (2 * o.y * d.y) + (2 * o.z * d.z);
		const float c = pow(o.x, 2) + pow(o.y, 2) + pow(o.z, 2) - pow(r,2);

		//calculate discriminant
		const float delta = pow(b, 2) - 4 * a * c;
		if (delta > 0.f)
		{
			float t1 = (b + sqrt(delta)) / (2 * a);
			float t2 = (b - sqrt(delta)) / (2 * a);

			std::cout << t1 << std::endl;
			std::cout << t2 << std::endl;

			return glm::vec2(t1,t2);
		}

		return glm::vec3();

	}
};