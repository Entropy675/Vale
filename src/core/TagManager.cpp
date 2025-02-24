#include "TagManager.h"
#include "Entity.h"
#include "PhysicsEntity.h"
#include "EnvironmentObject.h"

// Define static members
std::unordered_map<std::string, std::string> TagManager::tags;
std::unordered_map<std::string, PhysicsMetadata> TagManager::physicsTags;
bool TagManager::initOnce = false;
float TagManager::totalTime = 0.0f;

void TagManager::initialize(const std::unordered_map<std::string, std::string>& customTags, const std::unordered_map<std::string, PhysicsMetadata>& customPhysicsTags)
{
    // Add custom basic string tags
    for (const auto& [tag, contextInfo] : customTags)
        addTag(tag, contextInfo);

    // Add custom PhysicsMetadata tags
    for (const auto& [tag, contextInfo] : customPhysicsTags)
        addTag(tag, contextInfo);
}

bool TagManager::getTag(const std::string& tag, std::string& contextOut)
{
    auto it = tags.find(tag);
    if (it != tags.end())
    {
        contextOut = it->second;
        return true;
    }
    return false; // Tag not found
}

bool TagManager::getTag(const std::string& tag, PhysicsMetadata& contextOut)
{
    auto it = physicsTags.find(tag);
    if (it != physicsTags.end())
    {
        contextOut = it->second;
        return true;
    }
    return false; // Tag not found
}

bool TagManager::applyTag(Entity* target, const std::string& tag)
{
    if (target->hasTag(tag)) return false;
    if (hasPhysicsTag(tag)) target->addPhysicsMetadata(tag, physicsTags[tag]);
    return true;
}

bool TagManager::applyTag(PhysicsEntity* target, const std::string& tag)
{
    if (target->hasTag(tag)) return false; // don't add duplicates
    if (hasPhysicsTag(tag)) target->addPhysicsMetadata(tag, physicsTags[tag]);
    return true;
}

void TagManager::resolveCollisionTags(EnvironmentObject* env, PhysicsEntity* target)
{

    // Placeholder for environment collision logic
    // Called by environment against all physics entities
    // !! Assumes target is not self !!
    // Uses the id of the nearest point to the target, then calls that physics entities list
    // of physics metadata objects isColliding(vec3) functions on the target point
    // once we have the list of points/ID's to calculate collision for, we run their collision functions against the target
    // average the normal gotten across all collisions against the points, and apply that force

    int nearestID = env->nearestNeighbour(target->getPosition());
    Entity* temp = Entity::getEntityById(nearestID);

    glm::vec3 islandPos = temp->getPosition();
    islandPos += glm::vec3(0, -39080, 0);
    float distanceToEntity = glm::distance(target->getPosition(), islandPos);

    if (temp->hasTag("ocean"))
    {
        totalTime += 0.01f;
        if (totalTime > 1.2f)
        {
            target->moveTo(glm::vec3(0, 200, 0));
            // std::cout << "Drowned" << std::endl;
            totalTime = 0.0f;
        }
    }
    else if (temp->hasTag("island"))
    {
        if (distanceToEntity < 40000)
        {
            // std::cout << "COLLISION" << std::endl;
            glm::vec3 normal = glm::normalize(target->getPosition() - islandPos);
            target->moveTo(islandPos + normal * 40000);
            target->setVelocity(glm::vec3(0, 0, 0));
            target->setAcceleration(glm::vec3(0, 0, 0));

            glm::vec3 currentPos = target->getPosition();
            // Apply slight vertical offset to avoid repetitive collision

            target->moveTo(glm::vec3(currentPos.x, currentPos.y, currentPos.z));

            target->addTag("onGround");
        }
        else
            target->removeTag("onGround");
    }
}


template<typename T>
static std::vector<T*> TagManager::getEntitiesWithTag(const std::string& tag) 
{
    std::vector<T*> result;
    for(const auto& [id, entity] : Entity::getIdToEntityMap())
        if(entity->hasTag(tag))
            if(T* cast = dynamic_cast<T*>(entity))
                result.push_back(cast);
    return result;
}


