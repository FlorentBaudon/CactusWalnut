#pragma once

#include "glm/glm.hpp"

#include <vector>

struct Sphere
{
	glm::vec3 position{ 0.f, 0.f, 0.f };
	float radius;

	glm::vec3 albedo;
};
struct Scene
{
	std::vector<Sphere> spheres;
};

