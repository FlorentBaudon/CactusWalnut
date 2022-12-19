#pragma once

#include <memory>

#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "glm/glm.hpp"
class Renderer
{
public :
	struct Settings
	{
		bool accumulate = true;
	};
public :
	Renderer() = default;

	void Render(const Scene& scene, const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> getFinalImage() { return finalImage;  }

	void ResetFrameIndex() { frameIndex = 1; }
	Settings& GetSettings() { return settings; }

private :
	struct HitPayload
	{
		float HitDistance;
		glm::vec3 WorldPosition;
		glm::vec3 WorldNormal;

		int ObjectIndex;
	};

	HitPayload TraceRay(const Ray& ray);
	glm::vec4 PerPixel(uint32_t x, uint32_t y); //Raygen shader
	HitPayload ClosestHit(const Ray& ray, float hitDistance, int objectIndex);
	HitPayload Miss(const Ray& ray);

	
//Members	
private :
	const Scene* activeScene;
	const Camera* activeCamera;

	Settings settings;

	std::shared_ptr<Walnut::Image> finalImage;
	uint32_t* imageData = nullptr;
	glm::vec4* accumulationData = nullptr;

	uint32_t frameIndex = 1;

};

