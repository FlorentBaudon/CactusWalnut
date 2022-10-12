#include "CactusLayer.h"
#include "imgui.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

using namespace Walnut;

void CactusLayer::OnUIRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("Last render : %.3f ms", lastRenderTime);
    if (ImGui::Button("Render"))
    {
        Render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f) );

    ImGui::Begin("Viewport");
    viewportWidth = ImGui::GetContentRegionAvail().x;
    viewportHeight = ImGui::GetContentRegionAvail().y;
    if (image)
        ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() });

    ImGui::End();
    ImGui::PopStyleVar();
}

void CactusLayer::Render()
{
    Timer timer;
    if (!image || viewportWidth != image->GetWidth() || viewportHeight != image->GetHeight())
    { 
        image = std::make_shared<Image>(viewportWidth, viewportHeight, ImageFormat::RGBA);
        delete[] imageData;
        imageData = new uint32_t[viewportWidth * viewportHeight];
    }

    for (uint32_t i = 0; i < (viewportHeight * viewportWidth); i++)
    {
        imageData[i] = Random::UInt(); //ABGR
        imageData[i] |= 0xff000000; // remove random from alpha channel
    }

    image->SetData(imageData); //upload to GPU

    lastRenderTime = timer.ElapsedMillis();
}
