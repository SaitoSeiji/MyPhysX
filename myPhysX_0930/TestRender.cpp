#include<iostream>
#include <glut.h>
#include <vector>
#include "PxPhysicsAPI.h"
#include "Camera.h"
#include "RenderActor.h"

using namespace std;
using namespace physx;

extern void initPhysics();
extern void stepPhysics();
extern void cleanupPhysics();
namespace {
	Camera* sCamera;

	void motionCallback(int x, int y)
	{
		sCamera->handleMotion(x, y);
	}
	void keyboardCallback(unsigned char key, int x, int y)
	{
		if (key == 27)
			exit(0);

		if (!sCamera->handleKey(key, x, y)) {}
			//keyPress(key, sCamera->getTransform());
	}
	void mouseCallback(int button, int state, int x, int y)
	{
		sCamera->handleMouse(button, state, x, y);
	}
	void idleCallback()
	{
		glutPostRedisplay();
	}
	void renderCallback() {
		stepPhysics();

		RenderActor::startRender(sCamera->getEye(), sCamera->getDir());

		PxScene* scene;
		PxGetPhysics().getScenes(&scene, 1);
		PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		if (nbActors)
		{
			std::vector<PxRigidActor*> actors(nbActors);
			scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
			RenderActor::renderActors(&actors[0], static_cast<PxU32>(actors.size()), true);
		}
		RenderActor::finishRender();//ダブルバッファリングをしてる
	}
	void exitCallback(void)
	{
		delete sCamera;
		cleanupPhysics();
	}
}
void renderLoop() {
	cout << "test render";
	sCamera = new Camera(PxVec3(0.0f, 50.0f, 50.0f), PxVec3(0.0f, -0.1f, -0.7f));

	RenderActor::setupDefaultWindow("test");
	RenderActor::setupDefaultRenderState();

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	motionCallback(0, 0);

	atexit(exitCallback);

	initPhysics();
	glutMainLoop();

}