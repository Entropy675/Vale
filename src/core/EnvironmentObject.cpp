#include "EnvironmentObject.h"

EnvironmentObject::EnvironmentObject() : PhysicsEntity(glm::vec3(0, 0, 0))
{
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP); // Looks better for random points

}

EnvironmentObject::~EnvironmentObject() {}

void EnvironmentObject::clear()
{
    mesh.clear();
    kdTree.clear();
}

void EnvironmentObject::addVertex(const glm::vec3& v, int id)
{
    // Add vertex to mesh and KDTree
    kdTree.addPoint(v, id, 1); // radius is 1 for now, this is where spheres will be calc'd
}

void EnvironmentObject::setupEnvironment()
{
    std::cout << "Constructing KDTree with points " << kdTree.getAllPoints().size() << "... " << std::endl;
    if (kdTree.getAllPoints().size() == 0) kdTree.addPoint(glm::vec3(0,0,0), 0, 1);
    kdTree.constructKDTree(); // Build the KDTree after all points are added
    if (kdTree.getAllPoints().size() != mesh.getNumVertices()) recomputeMesh();
}

void EnvironmentObject::recomputeMesh()
{
    std::cout << "Environment Mesh Computing: kdtree - " << kdTree.getAllPoints().size() << " | -> | mesh - " << mesh.getNumVertices() << std::endl;
    mesh.clear();
    for (PointID i : kdTree.getAllPoints()) mesh.addVertex(i.position);
};

void EnvironmentObject::_collision(PhysicsEntity& target)
{
    // Placeholder for environment collision logic
    // Runs against every PhysicsEntity in the scene
    // !! Assumes target is not self !!
    // Uses the id of the nearest point to the target, then calls that physics entities list
    // of physics metadata objects isColliding(vec3) functions on the target point
    // once we have the list of points/ID's to calculate collision for, we run their collision functions against the target
    // average the normal gotten across all collisions against the points, and apply that force

    // Update lastTime for the next frame


    int nearestID = nearestNeighbour(target.getPosition());
    Entity* temp = Entity::getEntityById(nearestID);

    glm::vec3 islandPos = temp->getPosition();
    islandPos += glm::vec3(0, -39080, 0);
    float distanceToEntity = glm::distance(target.getPosition(), islandPos);

    if (temp->hasTag("ocean"))
    {
        totalTime += 0.01f;
        if (totalTime > 1.2f)
        {
            target.moveTo(glm::vec3(0, 200, 0));
            // std::cout << "Drowned" << std::endl;
            totalTime = 0.0f;
        }
    }
    else if (temp->hasTag("island"))
    {
        if (distanceToEntity < 40000)
        {
            // std::cout << "COLLISION" << std::endl;
            glm::vec3 normal = glm::normalize(target.getPosition() - islandPos);
            target.moveTo(islandPos + normal * 40000);
            target.setVelocity(glm::vec3(0, 0, 0));
            target.setAcceleration(glm::vec3(0, 0, 0));

            glm::vec3 currentPos = target.getPosition();
            // Apply slight vertical offset to avoid repetitive collision

            target.moveTo(glm::vec3(currentPos.x, currentPos.y, currentPos.z));

            target.addTag("onGround");
        }
        else
            target.removeTag("onGround");
    }


}

int EnvironmentObject::nearestNeighbour(const glm::vec3& queryPoint) // returns id of nearest neighbour
{
    // Get the nearest neighbor ID
    return kdTree.nearestNeighbour(queryPoint);
}

std::vector<int> EnvironmentObject::getNearestNeighbours(const glm::vec3& queryPoint, int k) // returnst list of nearest neighbours
{
    // Get the IDs of the nearest k neighbors
    return kdTree.getKNNIDs(queryPoint, k);
}
