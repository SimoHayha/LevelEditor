#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

#include <vector>
#include <string>

class Entity;

class UNMANAGED_API UnmanagedEntity
{
public:
	class UNMANAGED_API FeedbackEntity
	{
	public:
		virtual void OnValueChanged(int value) = 0;
		virtual void OnTotalTimeChanged(double value) = 0;
		virtual void OnCurrentTimeChanged(double value) = 0;
		virtual void OnCameraMoved() = 0;
		virtual void OnTransXChanged() = 0;
		virtual void OnTransYChanged() = 0;
		virtual void OnTransZChanged() = 0;
		virtual void OnRotXChanged() = 0;
		virtual void OnRotYChanged() = 0;
		virtual void OnRotZChanged() = 0;
		virtual void OnScaleXChanged() = 0;
		virtual void OnScaleYChanged() = 0;
		virtual void OnScaleZChanged() = 0;
		virtual void OnTransformationUpdate() = 0;
		virtual void OnAnimationUpdate() = 0;
		virtual void OnAnimationSpeedChanged() = 0;
		virtual void OnSelectedChanged() = 0;
		virtual void OnScriptRunningChanged() = 0;
	};

	struct UNMANAGED_API Transform
	{
		float scaleX, scaleY, scaleZ;
		float rotX, rotY, rotZ;
		float transX, transY, transZ;
	};

	UnmanagedEntity(FeedbackEntity* feedback);

	void	setEntity(size_t index, Entity* entity);

	void	setValue(int pValue);
	int		getValue(void);
	size_t	getIndex(void);
	Transform&	getTransform(void);
	void	setTransX(float);
	void	setTransY(float);
	void	setTransZ(float);
	void	setRotX(float);
	void	setRotY(float);
	void	setRotZ(float);
	void	setScaleX(float);
	void	setScaleY(float);
	void	setScaleZ(float);
	float	getTransX() const;
	float	getTransY() const;
	float	getTransZ() const;
	float	getRotX() const;
	float	getRotY() const;
	float	getRotZ() const;
	float	getScaleX() const;
	float	getScaleY() const;
	float	getScaleZ() const;
	size_t	getChildCount(void);
	Entity*	getEntity(void);
	int		getAnimNumber();
	char *	getAnimNameAt(int index);
	void	setAnimate(bool b);
	bool	isAnimate();
	bool	canAnimate();
	void	setCurrentAnimation(int animation);
	int		getCurrentAnimation();
	double	getCurrentAnimationTotalTime();
	double	getCurrentAnimationCurrentTime();
	void	setCurrentAnimationCurrentTime(double d);
	size_t	getMaterialsCount(void);
	int		getIndexViewport(void);
	std::string&	getMaterialName(size_t index);
	std::string&	getName(void);
	void			setName(std::string &);
	std::string&	getPath(void);
	std::string&	getTextureName(size_t index);
	void			setTexture(size_t index, const std::string& name);
	std::string&	getScriptName(void);
	void			setScript(const std::string& name);
	FeedbackEntity*	_feedback;
	bool	isAnimationLoop() const;
	void	setAnimationLoop(bool);
	bool	isAnimationMoveNextOnEnd() const;
	void	setAnimationMoveNextOnEnd(bool);
	float	getAnimationSpeed() const;
	void	setAnimationSpeed(float);
	bool	isSelected() const;
	void	setSelected(bool);
	bool	isScriptRunning() const;
	void	setScriptRunning(bool b);

private:
	Entity*			_entity;
	int				_value;
	size_t			_index;
};

