#include "PopperScene.h"


ContainerObject::ContainerObject(glm::vec3 pos, float rad, int res)
    : PhysicsEntity(pos), radius(rad), resolution(res) { addTag("container"); }

ContainerObject::~ContainerObject() {}

Entity* ContainerObject::clone() const
{
    return new ContainerObject(position, radius, resolution);
}

void ContainerObject::_setup() 
{
    mesh = ofMesh::sphere(radius, resolution);
}

void ContainerObject::_update() 
{
    
}

void ContainerObject::_draw() 
{
    mesh.draw();
}
