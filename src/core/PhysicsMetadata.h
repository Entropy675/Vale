#ifndef PHYSICSMETADATA_H__
#define PHYSICSMETADATA_H__
#include <functional>
#include <string>
#include "defs.h"

struct PhysicsMetadata
{
    std::string tag;
    void* context = nullptr; // optional personal or privately shared & mutable version of below (what handles this ones mem?)

    std::function<bool(const glm::vec3&)> boundingEquation; // Checks for collision
    std::function<glm::vec3(const glm::vec3&)> collisionNormal; // Computes & returns collision normal

    PhysicsMetadata(std::string tg = "invalid",
                    std::function<bool(const glm::vec3&)> boundingEq = [](const glm::vec3&) { return false; },
                    std::function<glm::vec3(const glm::vec3&)> collisionNorm = [](const glm::vec3&) { return glm::vec3(0, 0, 0); },
                    void* ctx = nullptr)
            : tag(std::move(tg)), context(ctx), boundingEquation(std::move(boundingEq)), collisionNormal(std::move(collisionNorm)) {}

};




#endif
