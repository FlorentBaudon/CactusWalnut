#include "CactusLayer.h"
#include "imgui.h"
#include "Walnut/Timer.h"
#include "RTMath.h"

#include "glm/gtc/type_ptr.hpp"

using namespace Walnut;

CactusLayer::CactusLayer() : camera(45.f, 0.1f, 100.f), renderer()
{
    Material& pinkSphere = scene.materials.emplace_back();
    pinkSphere.Albedo = { 1.0f, 0.0, 1.0f };
    pinkSphere.Roughness = 0.0f;

    Material& blueSphere = scene.materials.emplace_back();
    blueSphere.Albedo = { 0.0f, 0.0f, 1.0f };
    blueSphere.Roughness = 0.1f;


    Sphere sphere1;
    sphere1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    sphere1.radius = 1.0f;
    sphere1.materialIndex = 0;
    scene.spheres.push_back(sphere1);

    Sphere sphere2;
    sphere2.position = glm::vec3(0.0f, -101.0f, 0.0f);
    sphere2.radius = 100.0f;
    sphere2.materialIndex = 1;
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

        Sphere& sphere = scene.spheres[i];

        ImGui::Text("Sphere %i", i);
        ImGui::DragFloat3("Position", glm::value_ptr(sphere.position), 0.1f);
        ImGui::DragFloat("Radius", &sphere.radius, 0.1f);
        ImGui::DragInt("Material", &sphere.materialIndex, 1.0f, 0.0f, (int)scene.materials.size() - 1);
        ImGui::Separator();

        ImGui::PopID();
    }

    for (size_t i = 0; i < scene.materials.size(); i++)
    {
        ImGui::PushID(i);
        
        Material& material = scene.materials[i];
        ImGui::ColorEdit3("Albedo", glm::value_ptr(material.Albedo), 0.1f);
        ImGui::DragFloat("Roughness", &material.Roughness, 0.05f, 0.0f, 0.1f);
        ImGui::DragFloat("Metallic", &material.Metallic, 0.05f, 0.0f, 0.1f);

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
