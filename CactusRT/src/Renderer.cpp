#include "Renderer.h"
#include "Utils.h"
#include "Walnut/Random.h"

#include <execution>

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

    delete[] accumulationData;
    accumulationData = new glm::vec4[width * height];

    imageHorizontalIterator.resize(width);
    imageVerticalIterator.resize(height);

    for (uint32_t i = 0; i < width; i++)
    {
        imageHorizontalIterator[i] = i;
    }
    for (uint32_t i = 0; i < height; i++)
    {
        imageVerticalIterator[i] = i;
    }
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{
    this->activeScene = &scene;
    this->activeCamera = &camera;

    //Reset accumulation buffer
    if (frameIndex == 1)
        memset(accumulationData, 0, finalImage->GetWidth() * finalImage->GetHeight() * sizeof(glm::vec4));
#define MT 1
#if MT
    //Multithreaded 
    std::for_each(std::execution::par, imageVerticalIterator.begin(), imageVerticalIterator.end(), 
        [this](uint32_t y)
        {
            std::for_each(imageHorizontalIterator.begin(), imageHorizontalIterator.end(),
                [this, y](uint32_t x)
                {
                    glm::vec4 color = PerPixel(x, y);

                    accumulationData[y * finalImage->GetWidth() + x] += color;

                    glm::vec4 accumulatedColor = accumulationData[y * finalImage->GetWidth() + x];
                    accumulatedColor /= (float)frameIndex;

                    accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
                    imageData[y * finalImage->GetWidth() + x] = Utils::ConvertToRGBA(accumulatedColor);
                });
        });
#else
    //No multithreaded 
    for (uint32_t y = 0; y < finalImage->GetHeight(); y++) //we iterate with Y first to process pixels rows by rows and avoid memory jump
    {
        for (uint32_t x = 0; x < finalImage->GetWidth(); x++)
        {
            glm::vec4 color = PerPixel(x, y);

            accumulationData[y * finalImage->GetWidth() + x] += color;

            glm::vec4 accumulatedColor = accumulationData[y * finalImage->GetWidth() + x];
            accumulatedColor /= (float)frameIndex;

            accumulatedColor = glm::clamp(accumulatedColor, glm::vec4(0.0f), glm::vec4(1.0f));
            imageData[y * finalImage->GetWidth() + x] = Utils::ConvertToRGBA(accumulatedColor);
        }
    }
#endif

    finalImage->SetData(imageData); //upload to GPU

    if (settings.accumulate)
        frameIndex++;
    else
        frameIndex = 1;

}


glm::vec4 Renderer::PerPixel(uint32_t x, uint32_t y) //Raygen 
{
    Ray ray;
    ray.origin = activeCamera->GetPosition();
    ray.direction = activeCamera->GetRayDirections()[x + y * finalImage->GetWidth()];

    glm::vec3 color(0.0f);
    float multiplier = 1.0f;

    int bounces = 5;
    for (int i = 0; i < bounces; i++)
    {
        Renderer::HitPayload payload = TraceRay(ray);
         
        if (payload.HitDistance < 0.0f)
        {
            glm::vec3 skyColor = glm::vec3(0.6f, 0.7f, 0.9f);
            color += skyColor * multiplier;

            break;
        }

        glm::vec3 lightDirection = glm::normalize(glm::vec3(-1, -1, -1));
        float lightIntensity = glm::max(glm::dot(payload.WorldNormal, -lightDirection), 0.0f);

        const Sphere& sphere = activeScene->spheres[payload.ObjectIndex];
        const Material& material = activeScene->materials[sphere.materialIndex];

        glm::vec3 sphereColor = material.Albedo;

        sphereColor *= lightIntensity;

        color += sphereColor * multiplier;

        multiplier *= 0.5f;

        ray.origin = payload.WorldPosition + payload.WorldNormal * 0.0001f; //last number is the normal bias
        ray.direction = glm::reflect(ray.direction, payload.WorldNormal 
            + material.Roughness * Walnut::Random::Vec3(-0.5f, 0.5f) );//add random to simulate roughness
    }



    return glm::vec4(color, 1.0f);
}


Renderer::HitPayload Renderer::TraceRay(const Ray& ray)
{

    int closestSphere = -1;
    float hitDistance = FLT_MAX;
    for (size_t i = 0; i < activeScene->spheres.size(); i++)
    {
        const Sphere& sphere = activeScene->spheres[i];
        glm::vec3 origin = ray.origin - sphere.position;

        const float a = glm::dot(ray.direction, ray.direction); //square each parameters it's like a dot product of vector on itself // if direction is normalize, we don't need the dot product because resultat == 1, BUT dot product is less expansive than normalise and lead to the same result
        const float b = 2.f * glm::dot(origin, ray.direction);
        const float c = glm::dot(origin, origin) - sphere.radius * sphere.radius;

        const float delta = b * b - 4.f * a * c;

        if (delta < 0.0f)
            continue;

        //float t1 = (-b + sqrt(delta)) / (2 * a);
        float closestT = (-b - glm::sqrt(delta)) / (2 * a); //T2 always be the closest value

        if (closestT > 0.0f && closestT < hitDistance)
        {
            hitDistance = closestT;
            closestSphere = (int)i;
        }
    }

    if (closestSphere == -1)
        return Miss(ray);

    return ClosestHit(ray, hitDistance, closestSphere);
}

Renderer::HitPayload Renderer::ClosestHit(const Ray& ray, float hitDistance, int objectIndex)
{
    Renderer::HitPayload payload;
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere& closestSphere = activeScene->spheres[objectIndex];

    glm::vec3 origin = ray.origin - closestSphere.position;

    payload.WorldPosition = origin + ray.direction * hitDistance;
    payload.WorldNormal = glm::normalize(payload.WorldPosition);// - closestSphere->position);
    payload.WorldPosition += closestSphere.position;

    return payload;
}

Renderer::HitPayload Renderer::Miss(const Ray& ray)
{
    Renderer::HitPayload payload;
    payload.HitDistance = -1;
    payload.ObjectIndex = -1;

    return payload;
}