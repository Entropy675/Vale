#include "PopperScene.h"


BallObject::BallObject(glm::vec3 pos, float rad, int res)
    : PhysicsEntity(pos), radius(rad), resolution(res) { addTag("ball"); }

BallObject::~BallObject() {}

void BallObject::_collision(const ofMesh& targetMesh)
{
    //Iterate through each vertex of the target mesh
    for (int i = 0; i < targetMesh.getNumVertices(); ++i)
    {
        glm::vec3 vertexPosition = targetMesh.getVertex(i);

        // Step 3: Calculate the distance between the vertex and the center of the BallObject
        glm::vec3 directionToVertex = vertexPosition - position;
        float distance = glm::length(directionToVertex);

        // Step 4: Check for intersection
        if (distance - 3.6*radius < radius)
        {
            // Collision detected; calculate the collision normal and correction distance
            glm::vec3 collisionNormal = glm::normalize(position);  // Normal points towards the origin (0,0,0) so I don't need to worry about position lol
            float penetrationDepth = radius - distance;

            glm::vec3 correction = collisionNormal * penetrationDepth;
            moveTo(position + correction*0.1);

            // Reflect velocity to simulate bounce if needed
            glm::vec3 velocity = getVelocity();
            glm::vec3 reflectedVelocity = glm::reflect(velocity, collisionNormal);
            setVelocity(reflectedVelocity * 0.8f);  // Apply some damping factor

            // Exit early if single correction suffices (optional, depending on your collision needs)
            return;
        }
    }
    
    // Additional damping to stop small oscillations after landing
    if (glm::length(getVelocity()) < 0.1f)  // Threshold to detect rest
        setVelocity(glm::vec3(0, 0, 0));  // Stop the ball
}

PhysicsEntity* BallObject::clone() const
{
    return new BallObject(position, radius, resolution);
}

glm::vec3 generateRandomVector(float totalDistance) 
{
    float x = ofRandom(-totalDistance, totalDistance);
    float y = ofRandom(-totalDistance, totalDistance);
    
    while (x * x + y * y > totalDistance * totalDistance)
    {
        x = ofRandom(-totalDistance, totalDistance);
        y = ofRandom(-totalDistance, totalDistance);
    } 

    float z = std::sqrt(totalDistance * totalDistance - x * x - y * y);

    // Randomly choose the sign of z to ensure the vector can point in all directions
    z *= (ofRandom(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f;

    return glm::vec3(x, y, z);
}

void BallObject::_setup() 
{
    mesh = ofMesh::sphere(radius, resolution);
    material.setShininess(14);  // Higher shininess for sharper specular highlights
    material.setDiffuseColor(ofColor(255, 10, 10));  // reddsh
    material.setAmbientColor(ofColor(255, 255, 155));  // White ambient
    material.setSpecularColor(ofColor(255, 255, 255));
}

void BallObject::_update() 
{
    // Apply gravity to the ball's acceleration
    glm::vec3 gravity(0, -19.81f, 0);  // Downward gravity in y-axis (adjust for scale)
    addVelocity(gravity * ofGetLastFrameTime()*5);  // Increment velocity based on gravity

    // Update position based on current velocity
    glm::vec3 currentPosition = getPosition();
    glm::vec3 newPosition = currentPosition + getVelocity() * ofGetLastFrameTime();
    moveTo(newPosition);
    
    if (ofGetElapsedTimef() - lastActivationTime >= interval) 
    {
        glm::vec3 vec = generateRandomVector(1130.5f);
        std::cout << "[Time: " << ofGetElapsedTimef() - lastActivationTime << "] adding random velocity: " << vec << std::endl; 
        addVelocity(vec);
        lastActivationTime = ofGetElapsedTimef(); // Reset the timer
    }
}

void BallObject::_draw() 
{
    material.begin();
    ofDrawSphere(position.x, position.y, position.z, radius);
    material.end();
}

void BallObject::_input() 
{
    if (!inputManager)
    {
        std::cout << "Ball with id: " << getId() << " does not have an input manager!" << std::endl;
        return;
    }
    if(inputManager->get('b')) 
    {
        glm::vec3 vec = generateRandomVector(730.5f);
        std::cout << "adding random velocity: " << vec << std::endl; 
        addVelocity(vec);
    }
} 


