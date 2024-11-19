#include "PopperScene.h"


BallObject::BallObject(glm::vec3 pos, float rad, int res)
    : PhysicsEntity(pos), radius(rad), resolution(res) { addTag("ball"); }

BallObject::~BallObject() {}

Entity* BallObject::clone() const
{
    return new BallObject(position, radius, resolution);
}

void BallObject::_collision(PhysicsEntity& target)
{
    if(!target.hasTag("ball"))
    {
        const ofMesh& targetMesh = target.getMesh();

        // Step 4: Check for intersection
        if (glm::length(position) - radius > distance)
        {
            // Collision detected; calculate the collision normal and correction distance
            glm::vec3 collisionNormal = glm::normalize(position);  // Normal points towards the origin (0,0,0) so I don't need to worry about position lol
            float penetrationDepth = distance - glm::length(position) + radius;

            glm::vec3 correction = position + collisionNormal * penetrationDepth ; //(log(glm::length(velocity) + 1)*0.002f);
            moveTo(correction);

            // Reflect velocity to simulate bounce if needed
            glm::vec3 reflectedVelocity = glm::reflect(velocity, collisionNormal);
            setVelocity(reflectedVelocity * 0.5f);  // Apply some damping factor
        }

    }
    else
    {
        // based on radius being the same value for all balls
        // Get target position and radius
        glm::vec3 targetPosition = target.getPosition();
        float targetRadius = radius;
        // hmm... maybe dimensions should be a part of entity? or perhaps properties pointer/map?
        // maybe we maintain a map between sets of tags and properties?

        // Calculate the vector between the centers of the two balls
        glm::vec3 directionToTarget = targetPosition - position;
        float distanceBetweenCenters = glm::length(directionToTarget);

        // Check if the balls are intersecting
        if (distanceBetweenCenters < radius/2 + targetRadius/2)
        {
            // Collision detected; normalize direction and calculate penetration depth
            glm::vec3 collisionNormal = glm::normalize(directionToTarget);
            float penetrationDepth = (radius/2 + targetRadius/2) - distanceBetweenCenters;
            glm::vec3 velocity = getVelocity();
            float velLength = glm::length(velocity);

            // Correct position to resolve overlap
            glm::vec3 correction = collisionNormal * penetrationDepth;// * (log(velLength + 1)*0.03f);
            moveTo(position - correction);
            //target.moveTo(targetPosition + correction);


            // Reflect velocities
            glm::vec3 targetVelocity = target.getVelocity();
            if (glm::length(targetVelocity) > 1 && glm::length(velocity) > 1)
            {
                // Calculate the relative velocity in the direction of the collision normal
                float relativeVelocity = glm::dot(velocity - targetVelocity, collisionNormal);
                if (relativeVelocity < 0.0f) // Apply the opposite impulse only if balls are moving towards each other
                {
                    // Calculate restitution (bounciness) and impulse scalar
                    float restitution = 0.01f;  // Adjust restitution as needed
                    float impulseMagnitude = -(1 + restitution) * relativeVelocity;
                    impulseMagnitude /= 1 / getMass() + 1 / target.getMass();

                    // Calculate impulse vectors
                    glm::vec3 impulse = impulseMagnitude * collisionNormal;
                    setVelocity(velocity*0.9 + impulse / getMass()); // add some damping so its not perfect (bc otherwise balls can float on others)
                    target.setVelocity(targetVelocity*0.9 - impulse / target.getMass());
                }
            }
        }
    }
    // Additional damping to stop small oscillations after landing
    if (glm::length(getVelocity()) < threshold)  // Threshold to detect rest
        setVelocity(glm::vec3(0, 0, 0));  // Stop the ball
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
    material2.setShininess(14);  // Higher shininess for sharper specular highlights
    material2.setDiffuseColor(ofColor(255, 10, 10));  // reddsh
    material2.setAmbientColor(ofColor(255, 255, 155));  // White ambient
    material2.setSpecularColor(ofColor(255, 255, 255));
}

void BallObject::_update()
{
    // Update position based on current velocity
    glm::vec3 currentPosition = getPosition();
    glm::vec3 newPosition = currentPosition + getVelocity() * ofGetLastFrameTime();
    moveTo(newPosition);

    // Apply gravity to the ball's acceleration
    glm::vec3 gravity(0, -980/2, 0);  // Downward gravity in y-axis (adjust for scale)
    addVelocity(gravity * ofGetLastFrameTime());  // Increment velocity based on gravity

    if (ofGetElapsedTimef() - lastActivationTime >= interval)
    {
        glm::vec3 vec = generateRandomVector(1630.5f);
        std::cout << "[Time: " << ofGetElapsedTimef() - lastActivationTime << "] adding random velocity: " << vec << std::endl;
        addVelocity(vec);
        lastActivationTime = ofGetElapsedTimef(); // Reset the timer
    }
}

void BallObject::_draw()
{
    material2.begin();
    ofDrawSphere(position.x, position.y, position.z, radius);
    material2.end();
}

void BallObject::_input()
{
    if (!inputManager)
    {
        std::cout << "Ball with id: " << getId() << " does not have an input manager!" << std::endl;
        return;
    }
    if(inputManager->getPressed('b'))
    {
        glm::vec3 vec = generateRandomVector(730.5f);
        std::cout << "adding random velocity: " << vec << std::endl;
        addVelocity(vec);
    }
}


