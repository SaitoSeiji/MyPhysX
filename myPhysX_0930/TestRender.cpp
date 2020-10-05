#include<iostream>
#include <glut.h>
#include <vector>
#include "PxPhysicsAPI.h"
#include "Camera.h"
#include "RenderActor.h"
#include "FrameActor.h"
//#include "DtCulcurator.h"

using namespace std;
using namespace physx;

extern void initPhysics(bool init);
extern void stepPhysics(float dt);
extern void cleanupPhysics();
extern void keyPress(unsigned char key, const PxTransform& camera);
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
			keyPress(key, sCamera->getTransform());
	}
	void mouseCallback(int button, int state, int x, int y)
	{
		sCamera->handleMouse(button, state, x, y);
	}
	float dt=0;
	FrameActor _physcsFrameActor = FrameActor(1.0f / 60.0f);
	FrameActor _renderFrameActor= FrameActor(1.0f/4.0f);
	bool isFirst = true;
	void idleCallback()
	{
		//初期化処理
		if (isFirst) {
			isFirst = false;
			_physcsFrameActor.Reset();
			_renderFrameActor.Reset();
			return;
		}
		//経過時間の更新
		_physcsFrameActor.CalcDt();
		_renderFrameActor.CalcDt();

		//フレーム処理
		if (_physcsFrameActor.IsOverFrame()) {
			dt = _physcsFrameActor.GetDt();
			_physcsFrameActor.Reset();
			stepPhysics(dt);
			cout << "physcs_step " << dt << " sec" << "\n";
		}
		if (_renderFrameActor.IsOverFrame()) {
			//cout << "render_step " << _renderFrameActor.GetDt() << " sec" << "\n";
			_renderFrameActor.Reset();
			glutPostRedisplay();
		}
	}

	void timerCallback(int militime) {
		cout << "timer collback\n";
		//stepPhysics(CalcDt());
		glutTimerFunc(500, timerCallback, 1);
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


	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	//glutTimerFunc(500, timerCallback, 1);
	motionCallback(0, 0);
	atexit(exitCallback);

	initPhysics(true);
	glutMainLoop();

}