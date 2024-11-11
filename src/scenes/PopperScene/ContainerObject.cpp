#include "PopperScene.h"


ContainerObject::ContainerObject(glm::vec3 pos, float rad, int res)
    : Entity(pos), radius(rad), resolution(res) {}

ContainerObject::~ContainerObject() {}

void ContainerObject::_setup() 
{
    mesh = ofMesh::sphere(radius, resolution);
}

void ContainerObject::_update() 
{
    
}

void ContainerObject::_draw() 
{
    
}
