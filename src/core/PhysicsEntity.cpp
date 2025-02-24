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
    if(context.isLocal) LocalPhysicsMetadata.insert(std::pair<physicsTag, context>);
    return true;
}
    
// Retrieve a PhysicsMetadata by tag index
bool PhysicsEntity::getPhysicsMetadata(const std::string& physicsTag, PhysicsMetadata& out)
{
    if(!TagManager::getTag(physicsTag, out)) return false; // does not exist
    if(!out.isLocal) return true;
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
    // Due to the nature of the class hierarchy, the position of the supertype is fixed.
    // Either it is the 2nd or 3rd position, depending on Entity vs PhysicsEntityStart.
    // So we search from index 2, and check if that index is itself a supertype.
    // This is because supertypes are a property of the class itself, so they cannot be removed.
    auto it = std::find(tags.begin() + 2, tags.end(), tag); 

    if (it == tags.end()) return; 
    if (it == tags.begin() + 2 && TagManager::isSupertypeTag(*it)) return;
    if (TagManager::hasPhysicsTag(tag)) LocalPhysicsMetadata.erase(tag);
    
    Entity::removeTag(tag);
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
