#include "RenderActor.h"
using namespace physx;

namespace RenderActor {
	//これだけ定義がこっちにある
	//読み込まれるのはヘッダファイルだけなのでnamespace版privateになる
	static void reshapeCallback(int width, int height)
	{
		glViewport(0, 0, width, height);
	}
	void setupDefaultWindow(const char* name)
	{
		char* namestr = new char[strlen(name) + 1];
		strcpy(namestr, name);
		int argc = 1;
		char* argv[1] = { namestr };

		glutInit(&argc, argv);

		glutInitWindowSize(512, 512);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		int mainHandle = glutCreateWindow(name);
		glutSetWindow(mainHandle);
		glutReshapeFunc(reshapeCallback);

		delete[] namestr;
	}

	void setupDefaultRenderState()
	{
		// Setup default render states
		glClearColor(0.3f, 0.4f, 0.5f, 1.0);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);

		// Setup lighting
		glEnable(GL_LIGHTING);
		PxReal ambientColor[] = { 0.0f, 0.1f, 0.2f, 0.0f };
		PxReal diffuseColor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
		PxReal specularColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		PxReal position[] = { 100.0f, 100.0f, 400.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}
	void startRender(const PxVec3& cameraEye, const PxVec3& cameraDir, PxReal clipNear, PxReal clipFar)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Setup camera
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, GLdouble(glutGet(GLUT_WINDOW_WIDTH)) / GLdouble(glutGet(GLUT_WINDOW_HEIGHT)), GLdouble(clipNear), GLdouble(clipFar));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(GLdouble(cameraEye.x), GLdouble(cameraEye.y), GLdouble(cameraEye.z), GLdouble(cameraEye.x + cameraDir.x), GLdouble(cameraEye.y + cameraDir.y), GLdouble(cameraEye.z + cameraDir.z), 0.0, 1.0, 0.0);

		glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
	}
	void finishRender()
	{
		glutSwapBuffers();
	}

	//オブジェクトの描画処理
	void renderActors(PxRigidActor** actors, const PxU32 numActors, bool shadows, const PxVec3& color)
	{
		const PxVec3 shadowDir(0.0f, -0.7071067f, -0.7071067f);
		const PxReal shadowMat[] = { 1,0,0,0, -shadowDir.x / shadowDir.y,0,-shadowDir.z / shadowDir.y,0, 0,0,1,0, 0,0,0,1 };

		PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
		for (PxU32 i = 0; i < numActors; i++)
		{
			const PxU32 nbShapes = actors[i]->getNbShapes();
			PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
			actors[i]->getShapes(shapes, nbShapes);
			const bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

			for (PxU32 j = 0; j < nbShapes; j++)
			{
				const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
				const PxGeometryHolder h = shapes[j]->getGeometry();

				if (shapes[j]->getFlags() & PxShapeFlag::eTRIGGER_SHAPE)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				// render object
				glPushMatrix();
				glMultMatrixf(&shapePose.column0.x);
				if (sleeping)
				{
					const PxVec3 darkColor = color * 0.25f;
					glColor4f(darkColor.x, darkColor.y, darkColor.z, 1.0f);
				}
				else
					glColor4f(color.x, color.y, color.z, 1.0f);
				RenderRaw::renderGeometryHolder(h);
				glPopMatrix();

				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

				if (shadows)
				{
					glPushMatrix();
					glMultMatrixf(shadowMat);
					glMultMatrixf(&shapePose.column0.x);
					glDisable(GL_LIGHTING);
					glColor4f(0.1f, 0.2f, 0.3f, 1.0f);
					RenderRaw::renderGeometryHolder(h);
					glEnable(GL_LIGHTING);
					glPopMatrix();
				}
			}
		}
	}
}