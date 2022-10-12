#pragma once

#include <math.h>
#include <glm/glm.hpp>
#include <iostream>

class RTMath
{
public:
	static glm::vec3 SphereIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float sphereRadius) 
	{
		//Syntax sugar
		const glm::vec3 o = rayOrigin, d = glm::normalize(rayDirection);
		const float r = sphereRadius;

		const glm::vec3 n = glm::normalize(rayDirection);
		//Calculating each members of quadratic equation ax²+bx+c=0
		const float a = pow(d.x, 2) + pow(d.y, 2) + pow(d.z, 2);
		const float b = (2 * o.x * d.x) + (2 * o.y * d.y) + (2 * o.z * d.z);
		const float c = pow(o.x, 2) + pow(o.y, 2) + pow(o.z, 2) - pow(r,2);

		//calculate discriminant
		const float delta = pow(b, 2) - 4 * a * c;
		if (delta > 0.f)
		{
			float t1 = (-b + sqrt(delta)) / (2 * a);
			float t2 = (-b - sqrt(delta)) / (2 * a);

			glm::vec3 p1 = glm::vec3(rayOrigin.x + d.x * (float)t1, rayOrigin.y + d.y * t1, rayOrigin.z + d.z * t1);
			glm::vec3 p2 = glm::vec3(rayOrigin.x + d.x * (float)t2, rayOrigin.y + d.y * t2, rayOrigin.z + d.z * t2);

			if(glm::distance(o, p1) < glm::distance(o, p2))
			{
				return p1;
			}
			else {
				return p2;
			}
		}

		return glm::vec3();

	}
};