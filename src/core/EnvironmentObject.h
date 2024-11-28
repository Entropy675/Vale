#ifndef ENVIRONMENTOBJECT_H__
#define ENVIRONMENTOBJECT_H__

#include "PhysicsEntity.h"
#include "ofxKDTree.h"

class EnvironmentObject : public PhysicsEntity
{
private:
    vector<int> idMap;
    ofxKDTree kdTree;

    std::vector<size_t> nearestIndexes;
    std::vector<float> nearestDistances;

public:
    EnvironmentObject() : PhysicsEntity(glm::vec3(0, 0, 0))
    {
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP); // looks better for random points
    };
    ~EnvironmentObject() {};

    void clear()
    {
        mesh.clear();
        idMap.clear();
        kdTree.clear();
    };

    void addVertex(const glm::vec3& v, size_t id)
    { // id add to internal map
        mesh.addVertex(v);
        idMap.push_back(id); // same size
        kdTree.addPoint(v);
        if (mesh.getNumVertices() != idMap.size())
            std::cout << "WHAT THE...! The environment mesh has something insane happening in it!!! These counts should always be equal. You've got a fatal error coming..." << std::endl;
    }

    void setupEnvironment()
    {
        kdTree.constructKDTree();
    }

    int nearestNeighbour(const glm::vec3& q)
    {
        kdTree.getKNN(q, 2, nearestIndexes, nearestDistances);
        int index = -1;
        float dist = std::numeric_limits<float>::max();

        for (size_t i = 0; i < nearestIndexes.size(); ++i)
        {
            std::cout << "Index: " << nearestIndexes[i] << " Distance: " << nearestDistances[i] << std::endl;
            if (nearestDistances[i] < dist)
            {
                dist = nearestDistances[i];
                index = nearestIndexes[i];
            }
        }

        return index != -1 ? idMap[index] : -1; // Return mapped ID or -1 if no neighbors
    }

    PhysicsEntity* clone() const override           { return nullptr; }; // cannot be cloned
    void _collision(PhysicsEntity& target) override {}; // environment collision logic, runs against every physicsentity in the scene, represents all entities

    // not used
    void _setup() override {};
    void _update() override {};
    void _draw() override {};
};

#endif
