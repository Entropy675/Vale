#include "TagManager.h"
#include "PhysicsEntity.h"

// Define static members
std::unordered_map<std::string, std::string> TagManager::tags;
std::unordered_map<std::string, PhysicsMetadata> TagManager::physicsTags;
bool TagManager::initOnce = false;

void TagManager::initialize(const std::unordered_map<std::string, std::string>& customTags, const std::unordered_map<std::string, PhysicsMetadata>& customPhysicsTags)
{
    // Add custom basic string tags
    for (const auto& [tag, contextInfo] : customTags)
        addTag(tag, contextInfo);

    // Add custom PhysicsMetadata tags
    for (const auto& [tag, contextInfo] : customPhysicsTags)
        addTag(tag, contextInfo);

    if (initOnce) return;
    initOnce = true;

    // Add default tags that identify primitives
    addTag("entity");
    addTag("physics");
    addTag("physics_controller");
    addTag("scene_manager");
    addTag("player");
}

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
}


