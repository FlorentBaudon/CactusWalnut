#pragma once

#include <memory>

#include "Walnut/Image.h"
#include "Camera.h"
#include "Ray.h"
#include "glm/glm.hpp"
class Renderer
{
public :
	Renderer() = default;

	void Render(const Camera& camera);
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> getFinalImage() { return finalImage;  }

private :
	glm::vec4 TraceRay(const Ray& ray);

//Members	
private :
	std::shared_ptr<Walnut::Image> finalImage;
	uint32_t* imageData = nullptr;

};

