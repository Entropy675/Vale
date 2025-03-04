#include "TagManager.h"
#include "Entity.h"
#include "PhysicsEntity.h"
#include "EnvironmentObject.h"

// Define static members
std::unordered_map<std::string, std::string> TagManager::tags;
std::unordered_map<std::string, PhysicsMetadata> TagManager::physicsTags;
std::vector<std::string> TagManager::supertypes;
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

<<<<<<< HEAD
void TagManager::registerSupertype(const std::string& supertypeTag)
{
    supertypes.push_back(supertypeTag);
}

bool TagManager::isSupertypeTag(const std::string& tag)
{
    return std::find(supertypes.begin(), supertypes.end(), tag) != supertypes.end();
}

bool TagManager::addTag(const std::string& tag, const PhysicsMetadata& contextInfo)
{ 
    if (physicsTags.find(tag) != physicsTags.end()) return false; 
    physicsTags[tag] = contextInfo; 
    return true; 
}

bool TagManager::addTag(const std::string& tag, const std::string& contextInfo)              
{ 
    if (tags.find(tag) != tags.end()) return false; 
    tags.insert({tag, contextInfo}); 
    return true; 
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
    if (!target) return false;
    if (target->hasTag(tag)) return false;
    if (!hasPhysicsTag(tag))
    {
        target->tags.push_back(tag);
        return true;
    }
    
    PhysicsEntity* physicsTarget = dynamic_cast<PhysicsEntity*>(target);
    physicsTarget->addPhysicsMetadata(tag, physicsTags[tag]);
    return true;
}

=======
    if (initOnce) return;
    initOnce = true;

    // Add default tags that identify primitives
    addTag("entity");
    addTag("physics");
    addTag("physics_controller");
    addTag("scene_manager");
    addTag("player");
}
>>>>>>> parent of 4cff123 (refactor silly tag manager)

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


<<<<<<< HEAD
template<typename T>
std::vector<T*> TagManager::getEntitiesWithTag(const std::string& tag) 
{
    static_assert(std::is_base_of<Entity, T>::value, "Tag must be a supertype derived from Entity!");
    std::vector<T*> result;
    for(const auto& [id, entity] : Entity::getIdToEntityMap())
        if(entity->hasTag(tag))
            if(T* cast = dynamic_cast<T*>(entity))
                result.push_back(cast);
    return result;
=======
bool TagManager::addTag(const std::string& tag, const std::string& contextInfo)
{
    if (tags.find(tag) != tags.end())
    {
        return false; // Tag already exists
    }
    tags[tag] = contextInfo;
    return true;
}

bool TagManager::addTag(const std::string& tag, const PhysicsMetadata& contextInfo)
{
    if (physicsTags.find(tag) != physicsTags.end())
    {
        return false; // Tag already exists
    }
    physicsTags[tag] = contextInfo;
    return true;
}

bool TagManager::applyTag(Entity* target, const std::string& tag)
{
    if (target->hasTag(tag)) return false;

    if (hasPhysicsTag(tag))
    {
        std::cout << "TagManager applyTag error: physics tag " << tag <<
        " cannot be applied to non physics entity: " << target->getId() << std::endl;
        return false;
    }

    if (!hasDefaultTag(tag)) addTag(tag, "ephemeral"); // add even if doesn't exist
    target->tags.push_back(tag);

    return true;
}

bool TagManager::applyTag(PhysicsEntity* target, const std::string& tag)
{
    if (target->hasTag(tag)) return false; // don't add duplicates

    if (!hasTag(tag)) addTag(tag, "ephemeral"); // add even if doesn't exist

    target->tags.push_back(tag);
    if (hasPhysicsTag(tag)) target->tagsIndexToContext[target->tags.size() - 1] = physicsTags[tag];
    return true;
}

bool TagManager::hasTag(const std::string& tag)
{
    return hasDefaultTag(tag) || hasPhysicsTag(tag);
}

bool TagManager::hasDefaultTag(const std::string& tag)
{
    return tags.find(tag) != tags.end();
}

bool TagManager::hasPhysicsTag(const std::string& tag)
{
    return physicsTags.find(tag) != physicsTags.end();
}

bool TagManager::getTagDefaultContext(const std::string& tag, std::string& contextOut)
{
    auto it = tags.find(tag);
    if (it != tags.end())
    {
        contextOut = it->second;
        return true;
    }
    return false; // Tag not found
}

bool TagManager::getTagDefaultContext(const std::string& tag, PhysicsMetadata& contextOut)
{
    auto it = physicsTags.find(tag);
    if (it != physicsTags.end())
    {
        contextOut = it->second;
        return true;
    }
    return false; // Tag not found
>>>>>>> parent of 4cff123 (refactor silly tag manager)
}


