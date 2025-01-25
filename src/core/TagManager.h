#ifndef TAGMANAGER_H__
#define TAGMANAGER_H__

#include "defs.h"
#include "PhysicsMetadata.h"
#include <unordered_map>

class Entity;
class PhysicsEntity;
class EnvironmentObject;

class TagManager
{
private:
    // Static - master list of all tags and their associated context information
    static std::unordered_map<std::string, std::string> tags; // Tag to string context info
    static std::unordered_map<std::string, PhysicsMetadata> physicsTags; // Tag to PhysicsMetadata context
    static bool initOnce;
    
    static float totalTime; // (get rid of this later when implementing tag physics resolver) for drowning affect


public:
    static void initialize(const std::unordered_map<std::string, std::string>& customTags = {}, const std::unordered_map<std::string, PhysicsMetadata>& customPhysicsTags = {});

    static void resolveCollisionTags(EnvironmentObject*, PhysicsEntity* target);

    static bool addTag(const std::string& tag, const std::string& contextInfo = "default");
    static bool addTag(const std::string& tag, const PhysicsMetadata& contextInfo);

    static bool applyTag(Entity*, const std::string& tag);
    static bool applyTag(PhysicsEntity*, const std::string& tag);

    // Check if a tag context exists
    static bool hasTag(const std::string& tag);
    static bool hasDefaultTag(const std::string& tag);
    static bool hasPhysicsTag(const std::string& tag);

    // Get string context information for a tag
    static bool getTagDefaultContext(const std::string& tag, std::string& contextOut);
    static bool getTagDefaultContext(const std::string& tag, PhysicsMetadata& contextOut);
};



#endif
