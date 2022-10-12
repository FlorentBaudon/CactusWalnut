#pragma once
#include "Walnut/Layer.h"
#include "Walnut/Image.h"
#include <memory>
class CactusLayer :
    public Walnut::Layer
{
    virtual void OnUIRender() override;
private:
    void Render();

    std::shared_ptr<Walnut::Image> image;
    uint32_t* imageData = nullptr;
    uint32_t viewportWidth = 0, viewportHeight = 0;
    float lastRenderTime = 0;
};

