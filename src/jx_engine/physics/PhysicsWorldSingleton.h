#ifndef PHYSICSWORLDSINGLETON_CLASS_H
#define PHYSICSWORLDSINGLETON_CLASS_H

#ifdef _WIN32
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#else
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>
#endif

class PhysicsWorldSingleton {
private:
    PhysicsWorldSingleton();
    static PhysicsWorldSingleton* instance;

    btBroadphaseInterface* broadphase; //Broad Phase when checking for Collisions

    btDefaultCollisionConfiguration* collisionConfiguration; // Coupled with "dispatcher", the config is default for now...

    btCollisionDispatcher* dispatcher; //Setting a Collision Algorithm 

    btSequentialImpulseConstraintSolver* solver;

public:
    btDiscreteDynamicsWorld* dynamicsWorld;

    static PhysicsWorldSingleton* getInstance();
};

#endif