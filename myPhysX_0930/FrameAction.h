#pragma once
#include <string>
#include "DtCulcurator.h"

using namespace std;
class FrameAction
{
	DtCulcurator _dtCulc;
	float _frameRate = 0;
	float _dtSum = 0;
public:
	FrameAction(float frameRate);
	//経過時間の更新
	void Update();
	//設定したフレーム呼び出し間隔を超えているかの確認
	bool IsOverFrameRate();
	//現在の経過時間の取得
	float GetProgressTime();
	//経過時間をリセット
	void Refresh();
};

