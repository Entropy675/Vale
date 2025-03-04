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
    static std::vector<std::string> supertypes; // Store registered supertypes
    static bool initOnce;

    static float totalTime; // (get rid of this later when implementing tag physics resolver) for drowning affect

public:
    static void initialize(const std::unordered_map<std::string, std::string>& customTags = {}, const std::unordered_map<std::string, PhysicsMetadata>& customPhysicsTags = {});

    // TODO restricting access to registerSupertype so that only the parent of Supertypes ctor can call it (prevent memory issues by wrong use)
    static void registerSupertype(const std::string& supertypeTag);
    static bool isSupertypeTag(const std::string& tag);

    // Check if a tag context exists
    static bool hasTag(const std::string& tag)              { return hasDefaultTag(tag) || hasPhysicsTag(tag); };
    static bool hasDefaultTag(const std::string& tag)       { return tags.find(tag) != tags.end(); };
    static bool hasPhysicsTag(const std::string& tag)       { return physicsTags.find(tag) != physicsTags.end(); };
    
    static bool addTag(const std::string& tag, const PhysicsMetadata& contextInfo);
    static bool addTag(const std::string& tag, const std::string& contextInfo);
    
    // Get context information for a tag (or false if it doesn't exist)
    static bool getTag(const std::string& tag, std::string& contextOut);
    static bool getTag(const std::string& tag, PhysicsMetadata& contextOut);
    
    static bool applyTag(Entity*, const std::string& tag); // works for physics tags as well
    
    static void resolveCollisionTags(EnvironmentObject*, PhysicsEntity* target);

    template<typename T>
    std::vector<T*> getEntitiesWithTag(const std::string& tag); // global across scenes
};



#endif
