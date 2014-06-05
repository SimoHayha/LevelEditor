#include "SystemClass.h"
#include "LuaInput.h"
#include "BspFile.h"

#include <fstream>
#include <sstream>

namespace
{
	SystemClass*	appHandle = nullptr;
}

SystemClass::SystemClass(void) : _feedback(nullptr), _dico(nullptr), oldX(0), oldY(0), locked(false), active(false)
{
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		switch (msg)
		{
			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			default:
				return appHandle->messageHandler(hwnd, msg, wParam, lParam);
		}
}

HWND
SystemClass::initialize(HWND parent, size_t width, size_t height)
{
	std::string		str;

	_log = log_implementation::Log::get();
	logCall(log_level_t::LOG_DEBUG) << "DU GROS DEBUG MWAHAHAH" << Elog::flush;
	_width = width;
	_height = height;
	switchFill = true;
	if (!initializeWindows(_width, _height, parent))
	{
		MessageBox(hwnd, L"Could not initialize windows", L"Error", MB_OK);
		return false;
	}
	input.initialize();
	_dico = Dictionnary::getInstance();
	if (_dico == nullptr)
	{
		MessageBox(hwnd, L"Could not initialize/create dico", L"Error", MB_OK);
		return false;
	}
	graphic.reset(new (std::nothrow) Graphic);
	if (!graphic)
	{
		MessageBox(hwnd, L"Could not create graphics", L"Error", MB_OK);
		return false;
	}
	graphic->setFbxManager(&fbxManager);
	if (!graphic->initialize(_width, _height, hwnd))
	{
		MessageBox(hwnd, L"Could not initialize graphic", L"Error", MB_OK);
		return false;
	}
	logCall(log_level_t::LOG_INFO) << "Engine Succesfully load" << Elog::flush;
	return hwnd;
}

void
SystemClass::shutdown(void)
{
	shutdownWindows();
	if (graphic)
		graphic->shutdown();
	if (_dico != nullptr)
		_dico->kill();
}

void
SystemClass::run(void)
{
	Frame();
}

bool
SystemClass::initializeWindows(size_t& width, size_t& height, HWND parent)
{
		WNDCLASSEX	wc;
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

		appName = L"DirectX";
		appHandle = this;
		hInstance = GetModuleHandle(nullptr);
		wc.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
		wc.hIconSm = wc.hIcon;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = appName;
		wc.cbSize = sizeof(wc);
		RegisterClassEx(&wc);
		/*if (parent == nullptr)
		{
			_hasParent = false;
			hwnd = CreateWindowEx(WS_EX_APPWINDOW, appName, appName,
								  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
								  posX, posY, width, height,
								  nullptr, nullptr, hInstance, nullptr);
		}
		else*/
		{
			_hasParent = true;
			hwnd = CreateWindowEx(0, appName, appName,
			WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_BORDER,
			0, 0, width, height, parent, (HMENU)0x00000001,
			nullptr, nullptr);
		}
		ShowWindow(hwnd, SW_SHOW);
		SetForegroundWindow(hwnd);
		SetFocus(hwnd);
		
		return true;
}

bool
SystemClass::Frame(void)
{
	int	mousePosX = input.getMousePosX();
	int	mousePosY = input.getMousePosY();

	if (input.isKeyDown('F'))
	{
		if (switchFill)
		{
			graphic->switchFillMode();
			switchFill = false;
		}
	}
	else
		switchFill = true;

	if (this->active)
	{
		this->applyMouseInput(mousePosX, mousePosY);
		this->applyKeyboardInput();
	}
	else
	{
		this->stopCamera();
	}

	LuaScript::loadFromInput(&this->input);

	if (graphic->frame() == false)
		return false;

	return true;
}

void	SystemClass::stopCamera()
{
	MovementController *	cam = this->graphic->getCamera();

	if (nullptr == cam)
		return ;

	cam->reset();

	FpsCam *		fpsCam = dynamic_cast<FpsCam *>(cam);

	if (nullptr != fpsCam)
	{
		fpsCam->setSpeed(0.0f);
		fpsCam->setSlideSpeed(0.0f);
		fpsCam->setCrouchSpeed(0.0f);
	}
}

void	SystemClass::applyMouseInput(int mouseX, int mouseY)
{
	if (this->input.isMouseButtonDown(Input::EMOUSELEFT))
	{
		graphic->selectCamera(mouseX, mouseY);
		graphic->pick(mouseX, mouseY);
		this->locked = true;
	}

	if (!this->input.isMouseButtonDown(Input::EMOUSELEFT))
	{
		graphic->onMouseUp();
	}

	if (this->oldX != mouseX ||
		this->oldY != mouseY)
		this->graphic->onMouseMoved(mouseX, mouseY);

	if (!this->locked)
		return ;

	if (nullptr == this->graphic->getCamera())
	{
		this->oldX = mouseX;
		this->oldY = mouseY;
		return ;
	}

	if (!this->input.isMouseButtonDown(Input::EMOUSEMIDDLE))
	{
		this->oldX = mouseX;
		this->oldY = mouseY;
		this->graphic->getCamera()->reset();
	}

	FreeCamera *	freeCam = dynamic_cast<FreeCamera *>(this->graphic->getCamera());
	FpsCam *		fpsCam = dynamic_cast<FpsCam *>(this->graphic->getCamera());

	if (nullptr != freeCam)
		freeCam->setRotationSpeed((float)(this->oldY - mouseY), (float)(this->oldX - mouseX), 0.0f);
	else if (nullptr != fpsCam)
	{
		fpsCam->setRotationSpeedX((float)(-(this->oldY - mouseY)));
		fpsCam->setRotationSpeedY((float)(-(this->oldX - mouseX)));
	}

	this->oldX = mouseX;
	this->oldY = mouseY;
}

void	SystemClass::applyKeyboardInput()
{
	if (nullptr == this->graphic->getCamera())
		return ;

	float	thrust = 0.0f;
	float	slide = 0.0f;
	float	crouch = 0.0f;

	if (this->input.isKeyDown('W'))
		thrust = 1.0f;
	else if (this->input.isKeyDown('S'))
		thrust = -1.0f;

	if (this->input.isKeyDown('A'))
		slide = -1.0f;
	else if (this->input.isKeyDown('D'))
		slide = 1.0f;

	if (this->input.isKeyDown(VK_SPACE))
		crouch = 1.0f;
	else if (this->input.isKeyDown(VK_CONTROL))
		crouch = -1.0f;

	if (this->input.isKeyDown(VK_SHIFT))
		thrust *= 5.0f;

	if (this->input.isKeyDown(VK_ESCAPE))
	{
		this->locked = false;
		this->graphic->selectCamera(0, 0);
	}

	this->graphic->getCamera()->setThrust(thrust);

	FpsCam *		fpsCam = dynamic_cast<FpsCam *>(this->graphic->getCamera());

	/* If fps cam, also set the slide speed */
	if (nullptr != fpsCam)
	{
		fpsCam->setSpeed(thrust * 5);
		fpsCam->setSlideSpeed(slide * 5);
		fpsCam->setCrouchSpeed(crouch * 5);
	}
}

void
SystemClass::shutdownWindows(void)
{
	ShowCursor(true);
	DestroyWindow(hwnd);
	hwnd = nullptr;
	UnregisterClass(appName, hInstance);
	appHandle = nullptr;
}

LRESULT CALLBACK
SystemClass::messageHandler(HWND hwnd , UINT msg, WPARAM wParam, LPARAM lParam)
{

	//if (!_hasParent)
	//{
		switch(msg)
		{
		case WM_KILLFOCUS:
			active = false;
			break;
		case WM_SETFOCUS:
			active = true;
			break;
		case WM_KEYDOWN:
			input.keyDown((size_t)wParam);
		break;
		case WM_KEYUP:
			input.keyUp((size_t)wParam);
		break;
		case WM_MOUSEMOVE:
			input.mouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
		case WM_MBUTTONDOWN:
			input.mouseButtonDown(Input::EMOUSEMIDDLE);
		break;
		case WM_MBUTTONUP:
			input.mouseButtonUp(Input::EMOUSEMIDDLE);
		break;
		case WM_LBUTTONDOWN:
			if (_feedback != nullptr)
				_feedback->OnClick();
			input.mouseButtonDown(Input::EMOUSELEFT);
		break;
		case WM_LBUTTONUP:
			input.mouseButtonUp(Input::EMOUSELEFT);
		break;
		case WM_RBUTTONDOWN:
			input.mouseButtonDown(Input::EMOUSERIGHT);
		break;
		case WM_RBUTTONUP:
			input.mouseButtonUp(Input::EMOUSERIGHT);
		break;
		case WM_MOUSEWHEEL:
			input.setWheel(GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		case WM_MOUSEHWHEEL:
			input.setWheel(GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	//	return 0;
	//}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void
SystemClass::setFeedback(CUnmanaged::Feedback* feedback)
{
	_feedback = feedback;
}

bool SystemClass::ResizeWindow(int height, int width)
{
	if (!graphic)
		return true;
	if (width < 10 || height < 10)
		return true;
	Renderer*	renderer = graphic->getRenderer();
	graphic->getViewportManager()->init(width, height, false);
	return renderer->Resize(height, width);
}

void	SystemClass::unselect()
{
	this->graphic->unselect();
}

void	SystemClass::actionMode(std::string & mode)
{
	this->graphic->actionMode(mode);
}

void	SystemClass::wireframe(int cam, bool wireframe)
{
	this->graphic->wireframe(cam, wireframe);
}

void	SystemClass::loadVmf(std::string & filename)
{
	if (false == this->file.load(filename))
		return ;

	Entity *	container = this->graphic->getEntityManager()->addEntity(Entity::E_CONTAINER, hwnd, this->graphic->getRenderer()->getDevice());
	Linelist *	line      = dynamic_cast<Linelist *>(this->graphic->getEntityManager()->addEntity(Entity::E_LINELIST, hwnd, this->graphic->getRenderer()->getDevice(), container));

	line->setVertex(this->file.getVertices(), this->file.getVerticesCount());
	line->setIndices(this->file.getIndices(), this->file.getIndicesCount());

	line->initializeBuffers(this->graphic->getRenderer()->getDevice());
	line->setRotateX((3.0f * DirectX::XM_PI) / 2.0f);
}