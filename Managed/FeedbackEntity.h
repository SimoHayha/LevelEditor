#pragma once

#include <vcclr.h>
#include "..\DirectX_Engine\UnmanagedEntity.h"
#include "ManagedEntity.h"

using namespace ManagedLib;

class FeedbackEntity : public UnmanagedEntity::FeedbackEntity
{
public:
	FeedbackEntity(ManagedEntity^ p);
	void OnValueChanged(int pValue);
	void OnTotalTimeChanged(double pValue);
	void OnCurrentTimeChanged(double pValue);
	void OnCameraMoved();

	/* Update */
	void OnTransXChanged();
	void OnTransYChanged();
	void OnTransZChanged();
	void OnRotXChanged();
	void OnRotYChanged();
	void OnRotZChanged();
	void OnScaleXChanged();
	void OnScaleYChanged();
	void OnScaleZChanged();
	void OnTransformationUpdate();
	void OnAnimationUpdate();
	void OnAnimationSpeedChanged();
	void OnSelectedChanged();
	void OnScriptRunningChanged();

private:
	gcroot<ManagedEntity^> _Managed;
};
