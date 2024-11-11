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
        
        // Get individual transformations
        glm::vec3 scale = entity->getScale();
        ofQuaternion rotation = entity->getRotation();
        glm::vec3 translation = entity->getTranslation();

        // Debug the transformation breakdown
        ofVec3f tempVert = entityMesh.getVertex(entityMesh.getNumVertices() / 2);
        std::cout << "pre: \n" << tempVert << std::endl;
        std::cout << "scale: \n" << scale << std::endl;
        std::cout << "rotation: \n" << rotation << std::endl;
        std::cout << "translation: \n" << translation << std::endl;


        // Transform each vertex in the entity's mesh
        for (size_t i = 0; i < entityMesh.getNumVertices(); i++) 
        {
            ofVec3f vertex = entityMesh.getVertex(i);
            
            // Scale
            vertex.x *= scale.x;
            vertex.y *= scale.y;
            vertex.z *= scale.z;
            
            // Rotate
            ofVec3f transformedVertex = rotation * vertex; 
            
            // Translate
            transformedVertex.x += translation.x;
            transformedVertex.y += translation.y;
            transformedVertex.z += translation.z;


            entityMesh.setVertex(i, ofVec3f(transformedVertex.x, transformedVertex.y, transformedVertex.z)); // Update the vertex
        }

        // Debug the transformed position of the analyzed point
        std::cout << "post: \n" << entityMesh.getVertex(entityMesh.getNumVertices() / 2) << std::endl;
        
        // Append the transformed mesh to the aggregate mesh
        aggregateMesh.append(entityMesh);
    }
}

void SceneManager::loadScene(size_t index)
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
