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

void Renderer::Render(const Scene& scene, const Camera& camera)
{
    const glm::vec3 rayOrigin = camera.GetPosition();

    Ray ray;
    ray.origin = rayOrigin;

    for (uint32_t y = 0; y < finalImage->GetHeight(); y++) //we iterate with Y first to process pixels rows by rows and avoid memory jump
    {
        for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
        { 
            ray.direction = camera.GetRayDirections()[x + y * finalImage->GetWidth()];
            
            glm::vec4 color = TraceRay(scene, ray);
            color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
            imageData[y * finalImage->GetWidth() + x] = Utils::ConvertToRGBA(color);
        }
    }

    finalImage->SetData(imageData); //upload to GPU

}

glm::vec4 Renderer::TraceRay(const Scene& scene, const Ray& ray)
{
    if (scene.spheres.size() == 0)
        return glm::vec4(0, 0, 0, 1);

    const Sphere* closestSphere = nullptr;
    float hitDistance = FLT_MAX;
    for (const Sphere& sphere : scene.spheres)
    {

        glm::vec3 origin = ray.origin - sphere.position;

        const float a = glm::dot(ray.direction, ray.direction); //square each parameters it's like a dot product of vector on itself // if direction is normalize, we don't need the dot product because resultat == 1, BUT dot product is less expansive than normalise and lead to the same result
        const float b = 2.f * glm::dot(origin, ray.direction);
        const float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        const float delta = b * b - 4.f * a * c;

        if (delta < 0.0f)
            continue;

        //float t1 = (-b + sqrt(delta)) / (2 * a);
        float closestT = (-b - glm::sqrt(delta)) / (2 * a); //T2 always be the closest value

        if (closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphere = &sphere;
        }
    }

    if (closestSphere == nullptr)
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::vec3 origin = ray.origin - closestSphere->position;
    glm::vec3 lightDirection = glm::normalize(glm::vec3(-1, -1, -1));
    glm::vec3 sphereColor = closestSphere->albedo;


    glm::vec3 hitPoint = origin + ray.direction * hitDistance;
    glm::vec3 normal = glm::normalize(hitPoint);// - closestSphere->position);
    float angle = glm::max(glm::dot(normal, -lightDirection), 0.0f);

    sphereColor *= angle;

    return glm::vec4(sphereColor, 1.0f);
}