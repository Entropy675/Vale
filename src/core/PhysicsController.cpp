#include "PhysicsController.h"

// the biggest difference between movement here and movement in actual object is that this does modify collision mesh
// maintains a small list of physics objects, cleared when the scene is changed
// scene can load entities into either SceneManager or PhysicsController
// only PhysicsEntities will do something different if in PhysicsController
// PhysicsEntities in SceneManager will be stored in a seperate list, and can be switched between
// being managed by the SceneManager (just drawn normally) or the PhysicsController (with physics).
PhysicsController::PhysicsController()
{

}

PhysicsController::~PhysicsController()
{

}

void PhysicsController::_setup()
{

}

void PhysicsController::_update()
{

}

void PhysicsController::_draw()
{

}
