#include "SceneManager.h"

SceneManager::SceneManager() : Entity(glm::vec3(0, 0, 0)), phys(aggregateMesh)
{

}

SceneManager::~SceneManager()
{
    for (Scene* sc : scenes) delete sc;
}

void SceneManager::updateEnvironmentMesh()
{
    aggregateMesh.clear();
    for (Entity* entity : entities) 
    {
        ofMesh entityMesh = entity->getMesh();
        ofMatrix4x4 transformationMatrix = entity->getTransformationMatrix();

        std::cout << "pre: \n" << entityMesh.getVertex(entityMesh.getNumVertices()/2) << std::endl;
        std::cout << "translation #" << entity->hashId << ": \n" << (transformationMatrix) << std::endl;
    
        std::cout << "errrrrrrrrr #" << entity->hashId << ": \n" << (transformationMatrix) << std::endl;
        std::cout << transformationMatrix(3, 0) << ", " << transformationMatrix(3, 1) << ", " << transformationMatrix(3, 2) << std::endl;
        // Transform each vertex in the entity's mesh
        for (int i = 0; i < entityMesh.getNumVertices(); i++) 
        {
            ofVec3f vertex = entityMesh.getVertex(i);
            ofVec4f transformedVertex = transformationMatrix * ofVec4f(vertex.x, vertex.y, vertex.z, 1.0f); // Apply transformation
            float temp = 1.0f;
            entityMesh.setVertex(i, 
                ofVec3f(transformedVertex.x + temp*transformationMatrix(3, 0), // not sure why this is switched
                        transformedVertex.y + temp*transformationMatrix(3, 2), // conversion from x,y,z to of coords?
                        transformedVertex.z - temp*transformationMatrix(3, 1))
                ); // Set transformed vertex
        }
        std::cout << "post: \n" << entityMesh.getVertex(entityMesh.getNumVertices()/2) << std::endl;
        
        aggregateMesh.append(entityMesh);
    }
}

void SceneManager::loadScene(int index)
{
    if (index < 0 && index >= scenes.size()) 
    {
	    std::cerr << "Error: Scene index " << index << " is out of bounds." << std::endl;
    	return;
	}
	scenes[index]->loadScene(phys, &entities);
}

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
	if (scenes.size() == 1) loadScene(0);
}

void SceneManager::_setup()
{
    phys.setup();
    for (Entity* ptr : entities) ptr->setup();
	updateEnvironmentMesh();
}

void SceneManager::_update()
{
    phys.update();
    for (Entity* ptr : entities) ptr->update();
}

void SceneManager::_draw()
{
    phys.draw();
    for (Entity* ptr : entities) ptr->draw();
}
