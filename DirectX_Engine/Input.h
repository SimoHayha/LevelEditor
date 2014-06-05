#pragma once

#include <vector>

class Input
{
public:
	enum
	{
		EMOUSELEFT,
		EMOUSERIGHT,
		EMOUSEMIDDLE
	};

	void	initialize(void);
	void	keyDown(size_t key);
	void	keyUp(size_t key);
	void	mouseMove(int x, int y);
	void	mouseButtonDown(size_t button);
	void	mouseButtonUp(size_t button);
	bool	isKeyDown(size_t key) const;
	bool	isMouseButtonDown(size_t button);
	int		getMousePosX(void);
	int		getMousePosY(void);
	void	setWheel(short);
	short	getWheel();

private:
	std::vector<bool>	keys;
	std::vector<bool>	mouseButton;
	int					mousePosX;
	int					mousePosY;
	short				wheel;
};