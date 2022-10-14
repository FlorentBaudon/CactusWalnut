#include "Renderer.h"
#include "Utils.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (finalImage)
    {
        // If image existing and no resize needed, we end the function to avoid useless memory allocate
        if (finalImage->GetWidth() == width && finalImage->GetHeight() == height)
            return;

        finalImage->Resize(width, height);
    }
    else
    {
        finalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
    }

    delete[] imageData;
    imageData = new uint32_t[width * height];
}

void Renderer::Render(const Camera& camera)
{
    const glm::vec3 rayOrigin = camera.GetPosition();

    Ray ray;
    ray.origin = rayOrigin;

    for (uint32_t y = 0; y < finalImage->GetHeight(); y++) //we iterate with Y first to process pixels rows by rows and avoid memory jump
    {
        for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
        { 
            ray.direction = camera.GetRayDirections()[x + y * finalImage->GetWidth()];
            
            glm::vec4 color = TraceRay(ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            imageData[y * finalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
        }
    }

    finalImage->SetData(imageData); //upload to GPU

}

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
    glm::vec3 sphereColor(1, 0, 1);
    glm::vec3 sphereOrigin(0.0f);
    float sphereRadius = 1.f;

    glm::vec3 lightDirection = glm::normalize(glm::vec3(-1, -1, -1));

    const float a = glm::dot(ray.direction, ray.direction); //square each parameters it's like a dot product of vector on itself // if direction is normalize, we don't need the dot product because resultat == 1, BUT dot product is less expansive than normalise and lead to the same result
    const float b = 2.f * glm::dot(ray.origin, ray.direction);
    const float c = glm::dot(ray.origin, ray.origin) - sphereRadius * sphereRadius;

    const float delta = b*b - 4.f * a * c;

    if (delta < 0.0f)
        glm::vec4(0.f, 0.f, 0.f, 1.f);

    //float t1 = (-b + sqrt(delta)) / (2 * a);
    float t2 = (-b - glm::sqrt(delta)) / (2 * a); //T2 always be the closest value

    glm::vec3 hitPoint = ray.origin + ray.direction * t2;
    glm::vec3 normal = glm::normalize(hitPoint - sphereOrigin);
    float angle = glm::max(glm::dot(hitPoint, -lightDirection), 0.0f);

    sphereColor *= angle;

    return glm::vec4(sphereColor, 1.0f);
}