#pragma once

#include "glm/glm.hpp"

#include <vector>

struct Material
{
	glm::vec3 Albedo{ 1.0f };
	float Roughness = 1.0f;
	float Metallic = 0.0f;
};

struct Sphere
{
	glm::vec3 position{ 0.f, 0.f, 0.f };
	float radius;

	int materialIndex = 0;
};
struct Scene
{
	std::vector<Sphere> spheres;
	std::vector<Material> materials;
};

