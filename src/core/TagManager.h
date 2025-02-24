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
    static std::unordered_map<std::string, std::string> tags; // Tag to string context info (description of tag)
    static std::unordered_map<std::string, PhysicsMetadata> physicsTags; // Tag to PhysicsMetadata context
    static std::unordered_set<std::string> supertypes; // Store registered supertypes
    static bool initOnce;
    
    static float totalTime; // (get rid of this later when implementing tag physics resolver) for drowning affect


public:
    static void initialize(const std::unordered_map<std::string, std::string>& customTags = {}, const std::unordered_map<std::string, PhysicsMetadata>& customPhysicsTags = {});

    // TODO restricting access to registerSupertype so that only the parent of supertypes ctor can call it (prevent memory issues by wrong use) 
    static bool registerSupertype(const std::string& supertypeTag)                          { return supertypes.insert(supertypeTag); }
    static bool isSupertypeTag(const std::string& tag)                                      { return supertypes.find(tag) != supertypes.end(); }

    static bool addTag(const std::string& tag, const std::string& contextInfo)              { if (physicsTags.find(tag) != physicsTags.end()) return false; physicsTags[tag] = contextInfo; return true; };
    static bool addTag(const std::string& tag, const PhysicsMetadata& contextInfo)          { if (tags.find(tag) != tags.end()) return false; tags[tag] = contextInfo; return true; };

    // Check if a tag context exists
    static bool hasTag(const std::string& tag)                                              { return hasDefaultTag(tag) || hasPhysicsTag(tag); };
    static bool hasDefaultTag(const std::string& tag)                                       { return tags.find(tag) != tags.end(); };
    static bool hasPhysicsTag(const std::string& tag)                                       { return physicsTags.find(tag) != physicsTags.end(); };

    // Get string context information for a tag (or false if it doesn't exist)
    static bool getTag(const std::string& tag, std::string& contextOut);
    static bool getTag(const std::string& tag, PhysicsMetadata& contextOut);
    
    
    static bool applyTag(Entity*, const std::string& tag);
    static bool applyTag(PhysicsEntity*, const std::string& tag);
    
    
    static void resolveCollisionTags(EnvironmentObject*, PhysicsEntity* target);

    template<typename T>
    static std::vector<T*> getEntitiesWithTag(const std::string& tag) 
    {
        std::vector<T*> result;
        for(const auto& [id, entity] : Entity::idToEntityMap)
            if(entity->hasTag(tag))
                if(T* cast = dynamic_cast<T*>(entity))
                    result.push_back(cast);
        return result;
    }
    
};



#endif
