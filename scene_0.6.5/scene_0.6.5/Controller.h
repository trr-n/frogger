#pragma once

static enum class Type { Press, Down, Release };
class Controller
{
private:
	s3d::detail::XInput_impl pad = XInput(0);
public:
	//Vec2 axis;
	double play;
	int index;
	bool forward, back, left, right; // all in one
	bool kforward, kback, kleft, kright; // wasd
	bool dforward, dback, dleft, dright; // d-key
	bool a, b, x, y;
	bool rb, lb;
	double zr, zl; // triggers

	//Controller(void);

	Controller(int index = 0, double play = 0.8)
	{
		this->index = index;
		this->play = play;
		pad = XInput(index);
		forward = back = left = right =
			kforward = kleft = kback = kright =
			dforward = dback = dleft = dright =
			a = b = x = y = rb = lb = false;
	}

	Vec2 LStick()
	{
		return Vec2(pad.leftThumbX, pad.leftThumbY);
	}

	Vec2 RStick()
	{
		return Vec2(pad.rightThumbX, pad.rightThumbY);
	}

	void Update(Type type)
	{
		switch (type)
		{
		case Type::Press:
			kforward = KeyW.pressed();
			kleft = KeyA.pressed();
			kback = KeyS.pressed();
			kright = KeyS.pressed();
			dforward = KeyUp.pressed();
			dback = KeyDown.pressed();
			dleft = KeyLeft.pressed();
			dright = KeyRight.pressed();
			forward = (KeyUp | KeyW | pad.buttonUp).pressed() || LStick().y > play;
			back = (KeyDown | KeyS | pad.buttonDown).pressed() || LStick().y < -play;
			left = (KeyLeft | KeyA | pad.buttonLeft).pressed() || LStick().x < -play;
			right = (KeyRight | KeyD | pad.buttonRight).pressed() || LStick().x > play;
			rb = pad.buttonRB.pressed();
			lb = pad.buttonLB.pressed();
			a = pad.buttonA.pressed();
			b = pad.buttonB.pressed();
			x = pad.buttonX.pressed();
			y = pad.buttonY.pressed();
			break;
		case Type::Down:
			kforward = KeyW.down();
			kleft = KeyA.down();
			kback = KeyS.down();
			kright = KeyS.down();
			dforward = KeyUp.down();
			dback = KeyDown.down();
			dleft = KeyLeft.down();
			dright = KeyRight.down();
			forward = (KeyUp | KeyW | pad.buttonUp).down() || LStick().y > play;
			back = (KeyDown | KeyS | pad.buttonDown).down() || LStick().y < -play;
			left = (KeyLeft | KeyA | pad.buttonLeft).down() || LStick().x < -play;
			right = (KeyRight | KeyD | pad.buttonRight).down() || LStick().x > play;
			rb = pad.buttonRB.down();
			lb = pad.buttonLB.down();
			a = pad.buttonA.down();
			b = pad.buttonB.down();
			x = pad.buttonX.down();
			y = pad.buttonY.down();
			break;
		case Type::Release:
			kforward = KeyW.up();
			kleft = KeyA.up();
			kback = KeyS.up();
			kright = KeyS.up();
			dforward = KeyUp.up();
			dback = KeyDown.up();
			dleft = KeyLeft.up();
			dright = KeyRight.up();
			forward = (KeyUp | KeyW | pad.buttonUp).up() || LStick().y > play;
			back = (KeyDown | KeyS | pad.buttonDown).up() || LStick().y < -play;
			left = (KeyLeft | KeyA | pad.buttonLeft).up() || LStick().x < -play;
			right = (KeyRight | KeyD | pad.buttonRight).up() || LStick().x > play;
			rb = pad.buttonRB.up();
			lb = pad.buttonLB.up();
			a = pad.buttonA.up();
			b = pad.buttonB.up();
			x = pad.buttonX.up();
			y = pad.buttonY.up();
			break;
		default:
			throw std::exception();
		}
	}
};

void Example()
{
	auto controller = Controller(0);
	controller.Update(Type::Press);
	if (controller.kforward)
		std::cout << "just pressed W key";
}
