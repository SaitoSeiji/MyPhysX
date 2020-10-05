#include "FrameActor.h"

FrameActor::FrameActor(float frameRate) {
	_frameRate = frameRate;
	_dtSum=0;
	_myculcrator.Init();
}

void FrameActor::CalcDt() {
	_dtSum += _myculcrator.CalcDt();
}

bool FrameActor::IsOverFrame() {

	return _dtSum > _frameRate;
}

float FrameActor::GetDt() {
	return _dtSum;
}

void FrameActor::Reset() {
	_dtSum = 0;
	_myculcrator.Init();
}