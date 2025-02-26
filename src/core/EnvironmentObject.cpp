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

void EnvironmentObject::addMesh(const ofMesh& m)
{
    mesh.append(m);
    // this is where seed spheres will be calc'd
    // once solved, add via addSeedPoint function below
}

void EnvironmentObject::addSeedPoint(const glm::vec3& v, int id, int radius)
{
    // Add vertex to mesh and KDTree
    kdTree.addPoint(v, id, radius); 
}

void EnvironmentObject::setupEnvironment()
{
    std::cout << "Constructing KDTree with points " << kdTree.getAllPoints().size() << "... " << std::endl;
    if (kdTree.getAllPoints().size() == 0) kdTree.addPoint(glm::vec3(0,0,0), 0, 1);
    kdTree.constructKDTree(); // Build the KDTree after all points are added
    //if (kdTree.getAllPoints().size() != mesh.getNumVertices()) recomputeMesh();
}

void EnvironmentObject::recomputeMesh() // replace with compue sphere set
{
    std::cout << "Environment Mesh Computing: kdtree - " << kdTree.getAllPoints().size() << " | -> | mesh - " << mesh.getNumVertices() << std::endl;
    mesh.clear();
    for (PointID i : kdTree.getAllPoints()) mesh.addVertex(i.position);
};

void EnvironmentObject::_collision(PhysicsEntity& target)
{
    TagManager::resolveCollisionTags(this, &target);
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
