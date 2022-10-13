#pragma once
#include "Walnut/Layer.h"
#include "Walnut/Image.h"
#include "Renderer.h"
#include <memory>
class CactusLayer :
    public Walnut::Layer
{
public :
    CactusLayer();

    virtual void OnUIRender() override;
private:
    void Render();

    Renderer* renderer;
    uint32_t* imageData = nullptr;
    uint32_t viewportWidth = 0, viewportHeight = 0;
    float lastRenderTime = 0;
};

