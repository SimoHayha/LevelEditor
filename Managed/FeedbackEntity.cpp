#include "stdafx.h"
#include "FeedbackEntity.h"

using namespace ManagedLib;
FeedbackEntity::FeedbackEntity(ManagedEntity^ p)
{
	_Managed = p;
}

void	FeedbackEntity::OnValueChanged(int pValue)
{
	_Managed->raiseValueChanged(pValue);
}

void	FeedbackEntity::OnTotalTimeChanged(double pValue)
{
	_Managed->raiseTotalTimeChanged(pValue);
}

void	FeedbackEntity::OnCurrentTimeChanged(double pValue)
{
	_Managed->raiseCurrentTimeChanged(pValue);
}

void	FeedbackEntity::OnCameraMoved()
{
	_Managed->raiseCameraMoved();
}

void	FeedbackEntity::OnTransXChanged()
{
	_Managed->raiseTransXChanged();
}

void	FeedbackEntity::OnTransYChanged()
{
	_Managed->raiseTransYChanged();
}

void	FeedbackEntity::OnTransZChanged()
{
	_Managed->raiseTransZChanged();
}

void	FeedbackEntity::OnRotXChanged()
{
	_Managed->raiseRotXChanged();
}

void	FeedbackEntity::OnRotYChanged()
{
	_Managed->raiseRotYChanged();
}

void	FeedbackEntity::OnRotZChanged()
{
	_Managed->raiseRotZChanged();
}

void	FeedbackEntity::OnScaleXChanged()
{
	_Managed->raiseScaleXChanged();
}

void	FeedbackEntity::OnScaleYChanged()
{
	_Managed->raiseScaleYChanged();
}

void	FeedbackEntity::OnScaleZChanged()
{
	_Managed->raiseScaleZChanged();
}

void	FeedbackEntity::OnTransformationUpdate()
{
	_Managed->raiseTransformationUpdate();
}

void	FeedbackEntity::OnAnimationUpdate()
{
	_Managed->raiseAnimationUpdate();
}

void	FeedbackEntity::OnAnimationSpeedChanged()
{
	_Managed->raiseAnimationSpeedChanged();
}

void	FeedbackEntity::OnSelectedChanged()
{
	_Managed->raiseSelectedChanged();
}

void	FeedbackEntity::OnScriptRunningChanged()
{
	_Managed->raiseOnScriptRunningChanged();
}