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
