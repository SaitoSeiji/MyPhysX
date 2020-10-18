#include <iostream>
#include "PxPhysicsAPI.h"
 
using namespace std;
using namespace physx;
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define PVD_USEDEBUG false

PxDefaultAllocator      gAllocator;
PxDefaultErrorCallback  gErrorCallback;
PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxPvd* gPvd = NULL;
PxMaterial* gMaterial;

PxRigidActor* hook;

// Create Dynamic Rigidbody

PxRigidDynamic* createDynamic(const PxTransform & t, const PxGeometry & geometry, const PxVec3 & velocity = PxVec3(0))
{
    PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
    dynamic->setAngularDamping(0.5f);
    dynamic->setLinearVelocity(velocity);
    gScene->addActor(*dynamic);
    return dynamic;
}
PxRigidStatic* createStatic(const PxTransform& t, const PxGeometry& geometry)
{
    PxRigidStatic* static_actor = PxCreateStatic(*gPhysics, t, geometry, *gMaterial);
    gScene->addActor(*static_actor);
    return static_actor;
}



#pragma region  local
namespace {
    float _ballSpeed=20;
    void ChengeBallSpeed(float dv) {
        _ballSpeed += dv;
        if (_ballSpeed < 0)_ballSpeed = 0;
    }
    PxRigidActor* CreateChain(PxRigidActor* firstFrom, float x, float kHeight, float kHookHalfHeight, float createCount
        , int iterationCount_pos, int iterationCount_vel) {
        PxRigidActor* from, * next;//from と nextをjointしていく
        from = firstFrom;
        const float kElementRadius = 0.5f;
        const float kElementHeight = 1.0f;
        for (int i = 0; i < createCount; i++) {
            //基準点　- 固定差分　- 繰り返し当たりの差分
            float elementHeight = kHeight - (kElementHeight + kElementRadius + kHookHalfHeight) - ((kElementHeight + kElementRadius) * 2) * i;
            PxRigidDynamic* element = createDynamic(
                PxTransform(PxVec3(x, elementHeight, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f))),
                PxCapsuleGeometry(kElementRadius, kElementHeight)
            );
            element->setSolverIterationCounts(iterationCount_pos, iterationCount_vel);
            next = element;

            //結合処理
            float jointHeight = kHeight - kHookHalfHeight - ((kElementHeight + kElementRadius) * 2) * i;
            PxVec3 joint_pos(x, jointHeight, 0.0f);
            /*PxSphericalJoint* joint =*/ PxSphericalJointCreate(
                *gPhysics,
                from,
                PxTransform(from->getGlobalPose().q.rotateInv(PxVec3(joint_pos - from->getGlobalPose().p)), PxQuat(-PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f))),
                next,
                PxTransform(next->getGlobalPose().q.rotateInv(PxVec3(joint_pos - next->getGlobalPose().p)), PxQuat(-PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)))
            );
            from = element;
        }
        return from;
    }
}
#pragma endregion


//物の初期配置を行う
void InitPhysicsEnviourment() {


    PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
    gScene->addActor(*groundPlane);

    createDynamic(PxTransform((PxVec3(0.0f, 10.0f, 0.0f))), PxSphereGeometry(1.0f)); //drop 1m sphere from 10m high
    const float kHeight = 70.0f;
    const float kHookHalfHeight = 1.0f;
    //天井をstaticで作成する
    hook = createStatic(PxTransform(PxVec3(0, kHeight, 0)), PxBoxGeometry(20.0f, kHookHalfHeight, 1.0f));


    ////チェーンの作製
    CreateChain(hook, -12, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, -10, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, -8, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, -6, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, -4, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, -2, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 0, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 2, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 4, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 6, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 8, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 10, kHeight, kHookHalfHeight, 20, 10, 1);
    //CreateChain(hook, 12, kHeight, kHookHalfHeight, 20, 10, 1);

}
// Proceed the step of physics environment
void stepPhysics(float dt)
{
#if 0
    dtsum += dt;
    if (dtsum < unitTime)return;
    float stepCount = 0;
    while (dtsum>=unitTime)
    {
        dtsum -= unitTime;
        stepCount++;
    }
    cout << "step " <<stepCount << ":"<< stepCount * unitTime << " sec" << "\n";
    gScene->simulate(stepCount*unitTime);
    gScene->fetchResults(true);
#else
    gScene->simulate(dt);
    gScene->fetchResults(true);

#endif
}

// Initialize PhysX extern
void initPhysics(bool init)
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
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, PVD_USEDEBUG);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, PVD_USEDEBUG);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, PVD_USEDEBUG);
    }
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);    //static friction, dynamic friction, restitution
    if(init)InitPhysicsEnviourment();
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
int cheinPos = 0;
void keyPress(unsigned char key, const PxTransform& camera)
{
    switch (toupper(key))
    {
    case ' ':	createDynamic(camera, PxSphereGeometry(2.0f), camera.rotate(PxVec3(-0.3f, 0, -1)) * _ballSpeed);	break;
    case 'Q':ChengeBallSpeed(5.0f); break;
    case 'E':ChengeBallSpeed(-5.0f); break; case 'B': CreateChain(hook, cheinPos, 70, 1.0f, 20, 10, 1);
        cheinPos += 2;
        break;
    }
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