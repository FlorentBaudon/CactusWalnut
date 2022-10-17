#include "CactusLayer.h"
#include "imgui.h"
#include "Walnut/Timer.h"
#include "RTMath.h"

#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

CactusLayer::CactusLayer() : camera(45.f, 0.1f, 100.f), renderer()
{
    Sphere sphere1;
    sphere1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sphere1.radius = 0.5f;
    sphere1.albedo = glm::vec3(1.0f, 0.0, 1.0f);
    scene.spheres.push_back(sphere1);

    Sphere sphere2;
    sphere2.position = glm::vec3(1.0f, 0.0f, -5.0f);
    sphere2.radius = 1.5f;
    sphere2.albedo = glm::vec3(0.0f, 0.0, 1.0f);
    scene.spheres.push_back(sphere2);

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

    ImGui::Begin("Scene");

    for(size_t i = 0; i < scene.spheres.size(); i++)
    {
        ImGui::PushID(i);
        ImGui::Text("Sphere %i", i);
        ImGui::DragFloat3("Position", glm::value_ptr(scene.spheres[i].position), 0.1f);
        ImGui::DragFloat("Radius", &scene.spheres[i].radius, 0.1f);
        ImGui::ColorEdit3("Albedo", glm::value_ptr(scene.spheres[i].albedo), 0.1f);

        ImGui::Separator();

        ImGui::PopID();
    }
    ImGui::End();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f) );

    ImGui::Begin("Viewport");
    viewportWidth = ImGui::GetContentRegionAvail().x;
    viewportHeight = ImGui::GetContentRegionAvail().y;

    auto image = renderer.getFinalImage();
    if (image)
        ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
            ImVec2(0,1), ImVec2(1,0) ); //Invert Y coordinate

    ImGui::End();
    ImGui::PopStyleVar();

    Render();
}

void CactusLayer::OnUpdate(float ts)
{
    camera.OnUpdate(ts);
}

void CactusLayer::Render()
{
    Timer timer;

    renderer.OnResize(viewportWidth, viewportHeight);
    camera.OnResize(viewportWidth, viewportHeight);
    renderer.Render(scene, camera);

    lastRenderTime = timer.ElapsedMillis();
}
