#ifndef ENVIRONMENTOBJECT_H__
#define ENVIRONMENTOBJECT_H__

// #include "SpherePackingExperiment.h"
#include "PhysicsEntity.h"
#include "ofxKDTree.h"
#include <iostream>

class EnvironmentObject : public PhysicsEntity
{
private:
    ofxKDTree kdTree; // KDTree to manage spatial points and IDs
    ofMesh mesh;

public:
    EnvironmentObject();
    ~EnvironmentObject();

    void clear();
    void addMesh(const ofMesh& m);
    void addSeedPoint(const glm::vec3& v, int id, int radius = 1);
    void setupEnvironment();
    void recomputeMesh(); 

    int nearestNeighbour(const glm::vec3& queryPoint); // returns id of nearest neighbour
    std::vector<int> getNearestNeighbours(const glm::vec3& queryPoint, int k); // returns list of nearest neighbours

    void _collision(PhysicsEntity& target) override;
    PhysicsEntity* clone() const override { return nullptr; } // Cannot be cloned

    // Placeholder functions (not currently used)
    void _setup() override {}
    void _update() override {}
    void _draw() override { mesh.draw(); }
};

#endif
