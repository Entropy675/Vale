#include "Entity.h"

long long Entity::uniqueCounter = 0;
std::unordered_map<int, Entity*> Entity::idToEntityMap;

Entity::Entity(glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter)
{
    addTag("entity");
    idToEntityMap.insert({hashId, this});
}

Entity::Entity(const ofMesh& meshRef, glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter), mesh(meshRef)
{
    addTag("entity");
    idToEntityMap.insert({hashId, this});
}

Entity::~Entity()
{
    idToEntityMap.erase(hashId);
}; // each entity manages its own cleanup in its dtor

bool Entity::hasTag(const std::string& tag) const                           
{ 
    return std::find(tags.begin(), tags.end(), tag) != tags.end(); 
};

bool Entity::addTag(const std::string& tag)
{ 
    return TagManager::applyTag(this, tag);
};

void Entity::removeTag(const std::string& tag) 
{
    auto it = std::find(tags.begin() + 2, tags.end(), tag); // first two are always reserved
    if (it == tags.end()) return;
    if (it == tags.begin() + 2 && TagManager::isSupertypeTag(*it)) return;
    tags.erase(it); 
}

void Entity::update()
{
    if(inputManager != nullptr) _input();
    _update();

#ifdef PRINTALLENTITIES
    std::cout << "Id: " << getId() << " Tags: ";
    for (std::string s : tags) std::cout << "[" << s << "], ";
    std::cout << std::endl;
#endif
}

void Entity::draw()
{
    _draw();
}

void Entity::setup()
{
    // uncomment if tracing setup calls regardless of first time
    // std::cout << "Id: " << getId() << " attempted to call setup... ";
    if (!setupDone) setupDone = true;
    else return;

    std::cout << "First Time Setup Pass! Entity setup: " <<  hashId << std::endl;
    _setup();
}

