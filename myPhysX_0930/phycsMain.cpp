#include <iostream>
#include "PxPhysicsAPI.h"

using namespace std;
using namespace physx;
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

PxDefaultAllocator      gAllocator;
PxDefaultErrorCallback  gErrorCallback;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxPvd* gPvd = NULL;


// Create Dynamic Rigidbody
PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, PxMaterial& material)
{
    PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, material, 10.0f);
    gScene->addActor(*dynamic);
    return dynamic;
}

//•¨‚Ì‰Šú”z’u‚ðs‚¤
void InitPhysicsEnviourment() {

    PxMaterial* material = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);    //static friction, dynamic friction, restitution

    PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *material);
    gScene->addActor(*groundPlane);
    createDynamic(PxTransform((PxVec3(0.0f, 10.0f, 0.0f))), PxSphereGeometry(1.0f), *material); //drop 1m sphere from 10m high

}
// Proceed the step of physics environment
void stepPhysics()
{
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
}

// Initialize PhysX
void initPhysics()
{
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

    // PVD(PhysX Visual Debugger) setting. To use PVD, we need to build the project as Debug mode.
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    PxInitExtensions(*gPhysics, gPvd);

    // Scene setting
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);          // Right-hand coordinate system, Y-UP.
    gDispatcher = PxDefaultCpuDispatcherCreate(1);         // The number of worker threads is one.
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = PxDefaultSimulationFilterShader;
    gScene = gPhysics->createScene(sceneDesc);

    // PVD setting
    PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
    InitPhysicsEnviourment();
}
void cleanupPhysics()
{
    PX_RELEASE(gScene);
    PX_RELEASE(gDispatcher);
    PX_RELEASE(gPhysics);
    if (gPvd)
    {
        PxPvdTransport* transport = gPvd->getTransport();
        gPvd->release();	gPvd = NULL;
        PX_RELEASE(transport);
    }
    PX_RELEASE(gFoundation);

    printf("SnippetHelloWorld done.\n");
}

void PMain() {

#if 0
    initPhysics();
    cout << "Free Fall" << endl;
    cout << "Start physics process" << endl;


    for (PxU32 i = 0; i != 500; i++)
    {
        if (i % 100 == 0)
            cout << "Simulation step: " << i << endl;
        stepPhysics();
    }

    cout << "End physics process." << endl;
    int tmp;
    cin >> tmp;
#else
    extern void renderLoop();
    renderLoop();
#endif
}