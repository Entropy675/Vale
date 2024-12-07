#include "PhysicsEntity.h"

PhysicsEntity::PhysicsEntity(glm::vec3 dim) : Entity(dim)
{
    rotation = ofQuaternion(0, 0, 0, 1);
    addTag("physics");
}

PhysicsEntity::PhysicsEntity(const ofMesh& meshRef, glm::vec3 dim) : Entity(meshRef, dim)
{
    rotation = ofQuaternion(0, 0, 0, 1);
}

PhysicsEntity::~PhysicsEntity() {}; // each PhysicsEntity manages its own cleanup in its dtor


void PhysicsEntity::addPhysicsMetadata(size_t tagIndex, const PhysicsMetadata& context)
{
    if (tagsIndexToContext.find(tagIndex) != tagsIndexToContext.end()) { return; }
    tagsIndexToContext[tagIndex] = context;
}

// Retrieve a PhysicsMetadata by tag index
bool PhysicsEntity::getPhysicsMetadata(size_t tagIndex, PhysicsMetadata& out)
{
    auto it = tagsIndexToContext.find(tagIndex);
    if (it != tagsIndexToContext.end())
    {
        out = it->second;
        return true;
    }
    return false;
}

bool PhysicsEntity::removePhysicsMetadata(size_t tagIndex)
{
    auto it = tagsIndexToContext.find(tagIndex);
    if (it != tagsIndexToContext.end())
    {
        tagsIndexToContext.erase(it);
        return true; // Successfully removed
    }
    return false; // No metadata found for the given tag index
}

void PhysicsEntity::removeTag(const std::string& tag)
{
    auto it = std::find(tags.begin(), tags.end(), tag);
    if (it != tags.end())
        tags.erase(it);
    if (TagManager::hasPhysicsTag(tag))
        removePhysicsMetadata(std::distance(tags.begin(), it));
}

void PhysicsEntity::collision(PhysicsEntity& target)
{
    _collision(target);
}

// all
void PhysicsEntity::setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration)
{
    position = newPosition;
    velocity = newVelocity;
    acceleration = newAcceleration;
}
