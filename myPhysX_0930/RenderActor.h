#pragma once
#include <glut.h>
#include "PxPhysicsAPI.h"
#include "RenderRogic.h"
using namespace physx;
#define MAX_NUM_ACTOR_SHAPES 128
namespace RenderActor
{
	void setupDefaultWindow(const char* name);
	void setupDefaultRenderState();
	void startRender(const PxVec3& cameraEye, const PxVec3& cameraDir, PxReal clipNear = 1.f, PxReal clipFar = 10000.f);
	void finishRender();
	void renderActors(PxRigidActor** actors, const PxU32 numActors, bool shadows = false, const PxVec3& color = PxVec3(0.0f, 0.75f, 0.0f));
};

