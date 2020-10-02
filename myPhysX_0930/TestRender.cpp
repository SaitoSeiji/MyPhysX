#include<iostream>
#include <glut.h>
#include <vector>
#include "PxPhysicsAPI.h"
#include "Camera.h"
#include "RenderActor.h"
#include <time.h>

using namespace std;
using namespace physx;

extern void initPhysics();
extern void stepPhysics(float dt);
extern void cleanupPhysics();
extern void keyPress(unsigned char key, const PxTransform& camera);
namespace {
	unsigned int frameRange = 500;
	Camera* sCamera;

	long long beforest=-1;
	long long beforent=-1;
	float CalcDt() {
		timespec _time;
		if (timespec_get(&_time, TIME_UTC) == 0) {
			cerr << "時刻の取得に失敗した";
		}
		long long nowst = _time.tv_sec;
		long long nownt = _time.tv_nsec;
		if (beforent < 0) {
			beforest = nowst;
			beforent = nownt;
			return 0;
		}

		long long dnt = nownt - beforent;
		beforent = nownt;
		long dst = 0;
		if (nowst > beforest) {
		 dst = nowst - beforest;
			//cout << dst << "秒経過\n";
			dnt += dst * 1000000000.0f;
			beforest = nowst;
		}
		/*if (dnt < 0) {
			cout << "framemiss "<< dst<<" "<<dnt<<"\n";
			return 0;
		}*/
		return dnt/100000000.0f;
	}

	void motionCallback(int x, int y)
	{
		sCamera->handleMotion(x, y);
	}
	void keyboardCallback(unsigned char key, int x, int y)
	{
		if (key == 27)
			exit(0);

		if (!sCamera->handleKey(key, x, y)) {}
			keyPress(key, sCamera->getTransform());
	}
	void mouseCallback(int button, int state, int x, int y)
	{
		sCamera->handleMouse(button, state, x, y);
	}
	void idleCallback()
	{
		stepPhysics(CalcDt());
		glutPostRedisplay();
	}

	void timerCallback(int militime) {
		cout << "timer collback\n";
		//stepPhysics(CalcDt());
		glutTimerFunc(1000/60, timerCallback, 1);
	}
	void renderCallback() {
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
	cout << "test render\n";
	sCamera = new Camera(PxVec3(0.0f, 50.0f, 50.0f), PxVec3(0.0f, -0.1f, -0.7f));

	RenderActor::setupDefaultWindow("test");
	RenderActor::setupDefaultRenderState();


	glutTimerFunc(1000 / 60, timerCallback, 1);
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