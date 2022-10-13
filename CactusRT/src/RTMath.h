#pragma once

#include <glm/glm.hpp>
#include <iostream>

class RTMath
{
public:
	static bool isSphereHit(glm::vec3 rayOrigin, glm::vec3 rayDirection, float sphereRadius)
	{
		//Syntax sugar
		const glm::vec3 o = rayOrigin, d = glm::normalize(rayDirection);
		const float r = sphereRadius;

		//Calculating each members of quadratic equation ax²+bx+c=0
		const float a = glm::dot(d, d); //square each parameters it's like a dot product of vector on itself
		const float b = 2.f * glm::dot(o, d);
		const float c = glm::dot(o, o) - r * r;

		//calculate discriminant
		const float delta = b * b - 4 * a * c;
		if (delta >= 0.f)
		{
			return true;
		}
		else 
		{
			return false;
		}

	}
	static glm::vec3 SphereIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, float sphereRadius) 
	{
		//Syntax sugar
		const glm::vec3 o = rayOrigin, d = glm::normalize(rayDirection);
		const float r = sphereRadius;

		//Calculating each members of quadratic equation ax²+bx+c=0
		const float a = glm::dot(d, d); //square each parameters it's like a dot product of vector on itself
		const float b = 2.f * glm::dot(o, d);
		const float c = glm::dot(o, o) - r * r;

		//calculate discriminant
		const float delta = b*b - 4 * a * c;
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