#include "FrameAction.h"

FrameAction::FrameAction(float frameRate) {
	_frameRate = frameRate;
}
//経過時間の更新
void FrameAction::Update() {
	if (!_dtCulc.IsStarted()) {
		_dtCulc.Start();
		return;
	}
	float dt = _dtCulc.CalcDt_sec();
	_dtSum += dt;
}
//設定したフレーム呼び出し間隔を超えているかの確認
bool FrameAction::IsOverFrameRate() {
	return _dtSum >= _frameRate;
}
//現在の経過時間の取得
float FrameAction::GetProgressTime() {
	return _dtSum;
}
//経過時間をリセット
void FrameAction::Refresh() {
	_dtSum = 0;
	_dtCulc.Reset();
}