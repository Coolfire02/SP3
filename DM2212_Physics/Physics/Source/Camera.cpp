#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera() : mode(EDGE)
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;

	
	newTarget = target;
	view_locked = true;
	auto_lock = true;

	SLIDE_SPEED = 100.0f;
	defaultMAXD_X = 50.0f;
	defaultNEWD_X = 45.0f;
	defaultMAXD_Y = 30.0f;
	defaultNEWD_Y = 25.0f;
	MAXD_X = defaultMAXD_X;
	NEWD_X = defaultNEWD_X;
	MAXD_Y = defaultMAXD_Y;
	NEWD_Y = defaultNEWD_Y;
	
	if (NEWD_X >= MAXD_X || NEWD_Y >= MAXD_Y)
		std::cout << "Camera new distance values must be smaller than max distance values!" << std::endl;

	screenWidth = screenHeight = worldWidth = worldHeight = 0;

	//Creleased = false;
	keyboard = Keyboard::GetInstance();

}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(Vector3 focusTarget, double dt)
{

	// use C to unlock view and move camera freely (for editing level/just viewing level purposes etc)
	if (keyboard->IsKeyPressed('C'))
	{		
		ToggleAutoLock();
	}
	
	// example of switching mode;; to be removed in final product zz
	if (keyboard->IsKeyPressed('M'))
	{
		SetMode(CENTER);
	}
	if (keyboard->IsKeyPressed('N'))
	{
		SetMode(EDGE);
	}

	
	// move camera using arrow keys
	if (Application::IsKeyPressed(VK_LEFT) 
		&& target.x - screenWidth * 0.5 > 0)
	{
		newTarget.x = target.x - 1;
	}
	else if (Application::IsKeyPressed(VK_RIGHT)
		&& target.x + screenWidth * 0.5 < worldWidth)
	{
		newTarget.x = target.x + 1;
	}
	if (Application::IsKeyPressed(VK_UP)
		&& target.y + screenHeight * 0.5 < worldHeight)
	{
		newTarget.y = target.y + 1;
	}
	else if (Application::IsKeyPressed(VK_DOWN)
		&& target.y - screenHeight * 0.5 > 0)
	{
		newTarget.y = target.y - 1;
	}

	// update target based on new/next target
	// if focus target exceeds maxd, set new target 
	if (view_locked) // if view is locked to follow focustarget
	{
		if (focusTarget.x - target.x > MAXD_X) // move camera to RIGHT
		{
			newTarget.x = focusTarget.x + NEWD_X;
		}
		else if (target.x - focusTarget.x > MAXD_X) // move camera to LEFT
		{
			newTarget.x = focusTarget.x - NEWD_X;
		}

		if (focusTarget.y - target.y > MAXD_Y) // move camera UP
		{
			newTarget.y = focusTarget.y + NEWD_Y;
		}
		else if (target.y - focusTarget.y > MAXD_Y) // move camera DOWN
		{
			newTarget.y = focusTarget.y - NEWD_Y;
		}
	}

	Constraint();

	if (target != newTarget)
	{
		UpdateTarget(dt);
	}
	// update cam pos based on target
	position.x = target.x;
	position.y = target.y;

}

void Camera::SetFocusTarget(Vector3 focustarget, bool slide)
{
	newTarget = focustarget;
	if (!slide)
	{
		target = newTarget;
		position.x = focustarget.x;
		position.y = focustarget.y;
	}
	else
	{
		view_locked = false;
	}
}

void Camera::SetLimits(float screenx, float screeny, float worldx, float worldy)
{
	screenWidth = screenx;
	screenHeight = screeny;
	worldWidth = worldx;
	worldHeight = worldy;
}

void Camera::SetSlidingSpeed(float spd)
{
	SLIDE_SPEED = spd;
}

void Camera::SetMode(MODE mode)
{
	this->mode = mode;
	switch (mode)
	{
	case CENTER:
		MAXD_X = 0;
		NEWD_X = 0;
		MAXD_Y = 0;
		NEWD_Y = 0;
		std::cout << "Mode has been set to CENTER." << std::endl;
		break;
	case EDGE:
		MAXD_X = defaultMAXD_X;
		NEWD_X = defaultNEWD_X;
		MAXD_Y = defaultMAXD_Y;
		NEWD_Y = defaultNEWD_Y;
		std::cout << "Mode has been set to EDGE." << std::endl;
		break;
	default:
		break;
	}
}

void Camera::ToggleAutoLock()
{
	auto_lock = !auto_lock;
	if (auto_lock)
	{
		std::cout << "Auto Lock has been turned on, camera will follow target upon release." << std::endl;
		view_locked = true;
	}
	else
	{
		std::cout << "Auto Lock has been turned off. Camera no longer follows target and can be more freely controlled." << std::endl;
		view_locked = false; 
	}
}

void Camera::ToggleAutoLock(bool on)
{
	auto_lock = on;
	if (auto_lock)
	{
		std::cout << "Auto Lock has been turned on, camera will follow target upon release." << std::endl;
		view_locked = true;
	}
	else
	{
		std::cout << "Auto Lock has been turned off. Camera no longer follows target and can be more freely controlled." << std::endl;
		view_locked = false;
	}
}

void Camera::Constraint()
{
	if (newTarget.x + screenWidth * 0.5 > worldWidth)
	{
		newTarget.x = worldWidth - screenWidth * 0.5;
	}
	else if (newTarget.x - screenWidth * 0.5 < 0)
	{
		newTarget.x = screenWidth * 0.5;
	}

	if (newTarget.y + screenHeight * 0.5 > worldHeight)
	{
		newTarget.y = worldHeight - screenHeight * 0.5;
	}
	else if (newTarget.y - screenHeight * 0.5 < 0)
	{
		newTarget.y = screenHeight * 0.5;
	}
}

void Camera::UpdateTarget(double dt)
{
	float offset = 3.0f;
	if ((newTarget - target).Length() < offset)
	{
		target = newTarget;
		if (auto_lock)
		{
			view_locked = true;
		}
		return;
	}
	
	target += (newTarget - target).Normalized() * SLIDE_SPEED * dt;
}
