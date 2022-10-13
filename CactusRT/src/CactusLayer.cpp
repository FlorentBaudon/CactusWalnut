#include "CactusLayer.h"
#include "imgui.h"
#include "Walnut/Timer.h"
#include "RTMath.h"

using namespace Walnut;

CactusLayer::CactusLayer()
{
    renderer = new Renderer();
}

void CactusLayer::OnUIRender()
{
    ImGui::Begin("Settings");
    ImGui::Text("Last render : %.3f ms", lastRenderTime);
    if (ImGui::Button("Render"))
    {
        //Render();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f) );

    ImGui::Begin("Viewport");
    viewportWidth = ImGui::GetContentRegionAvail().x;
    viewportHeight = ImGui::GetContentRegionAvail().y;

    auto image = renderer->getFinalImage();
    if (image)
        ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
            ImVec2(0,1), ImVec2(1,0) ); //Invert Y coordinate

    ImGui::End();
    ImGui::PopStyleVar();

    Render();
}

void CactusLayer::Render()
{
    Timer timer;

    renderer->OnResize(viewportWidth, viewportHeight);
    renderer->Render();

    lastRenderTime = timer.ElapsedMillis();
}
