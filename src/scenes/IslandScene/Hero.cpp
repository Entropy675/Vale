#include "Hero.h"

Entity* Hero::clone() const
{
    return new Hero(*this);
}

void Hero::_collision(PhysicsEntity& target)
{
    if (target.hasTag("island"));
}

void Hero::_input()
{
    // Input logic
}

void Hero::_setup()
{
    float radius = 600;
    int resolution = 24;

    ofSpherePrimitive tempMesh;
    tempMesh.set(radius, resolution);

    mesh.addVertices(tempMesh.getMesh().getVertices());
    mesh.addNormals(tempMesh.getMesh().getNormals());
    mesh.addTexCoords(tempMesh.getMesh().getTexCoords());
    mesh.addIndices(tempMesh.getMesh().getIndices());

}

void Hero::_update()
{
    Player::_update(); 
}

void Hero::_draw()
{
    ofSetColor(0, 0, 255);
    mesh.draw();
}
