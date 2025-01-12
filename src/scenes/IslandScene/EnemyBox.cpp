#include "EnemyBox.h"

void EnemyBox::_collision(PhysicsEntity& target)
{
}

void EnemyBox::_input()
{
    // Input logic
}

// plrs is never nullptr, called whenever plrs is validly set
void EnemyBox::_setPlayersList(std::vector<Player*>* plrs)
{
    if (plrs == nullptr )
    {
        std::cout << "_setPlayerList wrongly called on " << getId() << " with nullptr in callback value." << std::endl;
        return;
    }
    if (plrs->size()) player = (*plrs)[getId() % plrs->size()];
}


void EnemyBox::_setup()
{
    float size = 200; // length of the sides of the cube
    int resolution = 2; // # subdivides

    ofBoxPrimitive tempMesh;
    tempMesh.set(size, size, size, resolution, resolution, resolution);
    mesh.addVertices(tempMesh.getMesh().getVertices());
    mesh.addNormals(tempMesh.getMesh().getNormals());
    mesh.addTexCoords(tempMesh.getMesh().getTexCoords());
    mesh.addIndices(tempMesh.getMesh().getIndices());

    material.setShininess(14);  // Higher shininess for sharper specular highlights
    material.setDiffuseColor(ofColor(255, 10, 180));  // reddsh
    material.setAmbientColor(ofColor(255, 155, 155));  // White ambient
    material.setSpecularColor(ofColor(155, 255, 155));

}

void EnemyBox::_update()
{
    if (player == nullptr) return;
    glm::vec3 directionToPlayer = glm::normalize(player->getPosition() - getPosition());
    glm::vec3 newPosition = getPosition() + directionToPlayer * moveSpeed * ofGetElapsedTimef();
    moveTo(newPosition);
}


void EnemyBox::_draw()
{
    material.begin();
    mesh.draw();
    material.end();
}
