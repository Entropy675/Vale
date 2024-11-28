#ifndef ENVIRONMENTOBJECT_H__
#define ENVIRONMENTOBJECT_H__

class EnvironmentObject : public PhysicsEntity
{
private:
    vector<int> idMap;

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
    };

    void addVertex(const glm::vec3& v, size_t id)
    { // id add to internal map
        mesh.addVertex(v);
        idMap.push_back(id); // same size
    }

    PhysicsEntity* clone() const override           { return new EnvironmentObject(); };
    void _collision(PhysicsEntity& target) override {}; // environment collision logic, runs against every physicsentity in the scene, represents all entities

    // not used
    void _setup() override {};
    void _update() override {};
    void _draw() override {};
};

#endif
