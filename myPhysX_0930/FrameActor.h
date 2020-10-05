#pragma once
#include"DtCulcurator.h"
class FrameActor
{
	float _frameRate;
	float _dtSum;
	DtCulcurator _myculcrator;
public:
	FrameActor(float frameRate);
	void CalcDt();
	bool IsOverFrame();
	float GetDt();
	void Reset();
};

