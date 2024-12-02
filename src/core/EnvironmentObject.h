#ifndef ENVIRONMENTOBJECT_H__
#define ENVIRONMENTOBJECT_H__

#include "PhysicsEntity.h"
#include "ofxKDTree.h"

class EnvironmentObject : public PhysicsEntity
{
private:
    ofxKDTree kdTree; // KDTree to manage spatial points and IDs

public:
    EnvironmentObject() : PhysicsEntity(glm::vec3(0, 0, 0))
    {
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP); // Looks better for random points
    }

    ~EnvironmentObject() {}

    void clear()
    {
        mesh.clear();
        kdTree.clear();
    }

    void addVertex(const glm::vec3& v, int id)
    {
        // Add vertex to mesh and KDTree
        mesh.addVertex(v);
        kdTree.addPoint(v, id);
    }

    void setupEnvironment()
    {
        kdTree.constructKDTree(); // Build the KDTree after all points are added
    }

    int nearestNeighbour(const glm::vec3& queryPoint) // returns id of nearest neighbour
    {
        // Get the nearest neighbor ID
        return kdTree.nearestNeighbour(queryPoint);
    }

    std::vector<int> getNearestNeighbours(const glm::vec3& queryPoint, int k) // returnst list of nearest neighbours
    {
        // Get the IDs of the nearest k neighbors
        return kdTree.getKNNIDs(queryPoint, k);
    }

    PhysicsEntity* clone() const override { return nullptr; } // Cannot be cloned

    void _collision(PhysicsEntity& target) override 
    {
        // Placeholder for environment collision logic
        // Runs against every PhysicsEntity in the scene
        // !! Assumes target is not self !!
        // Uses the id of the nearest point to the target, then calls that physics entities list
        // of physics metadata objects isColliding(vec3) functions on the target point. If any are, 
        // check if the next nearest entity collides (so go through nearest neighbours list ignoring repeated ID's)
        // once we have the list of ID's to calculate collision for, we run their collision functions against the target.  
    }

    // Placeholder functions (not currently used)
    void _setup() override {}
    void _update() override {}
    void _draw() override {}
};

#endif
