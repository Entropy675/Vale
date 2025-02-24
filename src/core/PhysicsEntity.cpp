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


bool PhysicsEntity::addPhysicsMetadata(const std::string& physicsTag, const PhysicsMetadata& context)
{
    if(!TagManager::getTag(physicsTag, context)) return false;
    if(context.isLocal) LocalPhysicsMetadata.insert(physicsTag, context);
}
    
// Retrieve a PhysicsMetadata by tag index
bool PhysicsEntity::getPhysicsMetadata(const std::string& physicsTag, PhysicsMetadata& out)
{
    auto it = LocalPhysicsMetadata.find(physicsTag);
    if (it != tagsIndexToContext.end())
    {
        out = it->second;
        return true;
    }
    return false;
}

void PhysicsEntity::removeTag(const std::string& tag)
{
    Entity::removeTag(tag);
    if (TagManager::hasPhysicsTag(tag)) LocalPhysicsMetadata.erase(tag);
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
