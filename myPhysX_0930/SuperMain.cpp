#include <iostream>
#include <glut.h>
#include "RenderActor.h"
#include "DtCulcurator.h"
#include "FrameAction.h"
using namespace std;

void idleCallback_dtculc();
void idleCallback_frame();
void renderCallback();
DtCulcurator testDt;
DtCulcurator testDt_2;
FrameAction _frameActor = FrameAction(1.0f / 1.0f);
FrameAction _frameActor_dummy= FrameAction(1.0f/10.0f);
int main(void)
{
#if 1//通常の物理演算
    extern void PMain();
    PMain();
    return 0;
#elif 0//dtCulcuratorのテスト
    cout << "hoge\n";

    RenderActor::setupDefaultWindow("test");
    RenderActor::setupDefaultRenderState();
    glutIdleFunc(idleCallback_dtculc);
    glutDisplayFunc(renderCallback);
    glutMainLoop();
#elif 0//frameActorのテスト

    RenderActor::setupDefaultWindow("test");
    RenderActor::setupDefaultRenderState();
    glutIdleFunc(idleCallback_frame);
    glutDisplayFunc(renderCallback);
    glutMainLoop();
#endif
}

void idleCallback_dtculc() {
    if (!testDt.IsStarted()) {
        testDt.Start();
        testDt_2.Start();
        cout << "start \n";
        return;
    }
    float dt = testDt.CalcDt_sec();
    testDt.Reset();
    cout << "dt :"<<dt << "\n";
    /*float dt2 = testDt_2.CalcDt_sec();
    testDt_2.Reset();
    cout <<"dt2:"<< dt2 << "\n";*/
}
void idleCallback_frame() {
    _frameActor.Update();
    if (_frameActor.IsOverFrameRate()) {
        float dt=_frameActor.GetProgressTime();
        _frameActor.Refresh();
        cout << dt << "\n";
    }
}

void renderCallback() {

}