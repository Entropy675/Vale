#include "DefaultPlayer.h"

DefaultPlayer::DefaultPlayer(glm::vec3 pos)
    : Player(pos, 50.0f, 500.0f, 20, 0.0f) { 

    // currently setting up and drawing with initialization
}

DefaultPlayer::~DefaultPlayer() {}


void DefaultPlayer::_setup() {
    pillBody.setRadius(radius);  
    pillBody.setResolution(resolution);  
    pillBody.setPosition(position);

    std::cout << "HELLO I HAPPENED" << std::endl;
    pillBody.setScale(1.0, 2.0, 1.0);
}

void DefaultPlayer::_draw() {
    ofSetColor(ofColor::black); 
    pillBody.draw();
}

void DefaultPlayer::_update() {

}

Entity* DefaultPlayer::clone() const
{
    return new DefaultPlayer(position);
}

void DefaultPlayer::_collision(PhysicsEntity& target) {

}

