#pragma once

#include <memory>

#include "Walnut/Image.h"
#include "glm/glm.hpp"
class Renderer
{
public :
	Renderer() = default;

	void Render();
	void OnResize(uint32_t width, uint32_t height);
	std::shared_ptr<Walnut::Image> getFinalImage() { return finalImage;  }

private :
	glm::vec4 PerPixel(glm::vec2 coord);

//Members	
private :
	std::shared_ptr<Walnut::Image> finalImage;
	uint32_t* imageData = nullptr;

};

