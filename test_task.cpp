#include <vector>

const int initialCarsCount = 10;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define CAR_WIDTH 100
#define CAR_HEIGHT 100
#define CAR_SPEED 1
#define CAR_CHARGE 1100
#define CAR_FUEL 1100

struct sPos {
	sPos() { x = 0; y = 0; }
	sPos(int aX, int aY) { x = aX; y = aY; }
	int x;
	int y;
};

struct sSize
{
	sSize() { width = 0; height = 0; }
	sSize(int aW, int aH) { width = aW; height = aW; }
	int width;
	int height;
};

struct sRect
{
	sRect() {};
	sRect(int x, int y, int w, int h) { pos.x = x; pos.y = y; size.width = w; size.height = h; }
	sPos pos;
	sSize size;
	//возможна ошибка, связанная с привязкой координат
	bool intersects(const sRect& other) {
		return !((other.pos.x + other.size.width <= pos.x) ||
			(other.pos.y + other.size.height <= pos.y) ||
			(other.pos.x >= pos.x + size.width) ||
			(other.pos.y >= pos.y + size.height));
	}
};

enum class eDirection {
	UP,
	LEFT,
	RIGHT,
	DOWN
};

struct sCar {
	sRect rect;
	eDirection dir;
	int speed;
	void move() {
		switch (dir) {
		case eDirection::UP:
			rect.pos.y += speed;
		case eDirection::DOWN:
			rect.pos.y -= speed;
		case eDirection::RIGHT:
			rect.pos.x += speed;
		case eDirection::LEFT:
			rect.pos.x -= speed;
		}
	}

	sRect getFuturePos() {
		switch (dir) {
		case eDirection::UP:
			return sRect(rect.pos.x, rect.pos.y + speed, rect.size.width, rect.size.height);
			break;
		case eDirection::DOWN:
			return sRect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
			break;
		case eDirection::RIGHT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height); 
			break;
		case eDirection::LEFT:
			return sRect(rect.pos.x - speed, rect.pos.y, rect.size.width, rect.size.height);
			break;
		}
	}

	bool needPassOtherCar(sCar* otherCar) {
		bool result;
		auto otherdir = otherCar->dir;
		switch (dir) {
		case eDirection::UP:
			if (otherdir == eDirection::LEFT)
				result = true;
			else
				result = false;
			break;
		case eDirection::DOWN:
			if (otherdir == eDirection::RIGHT)
				result = true;
			else
				result = false;
			break;
		case eDirection::RIGHT:
			if (otherdir == eDirection::UP)
				result = true;
			else
				result = false;
			break;
		case eDirection::LEFT:
			if (otherdir == eDirection::DOWN)
				result = true;
			else
				result = false;
			break;
		}
		return result;
	}

	virtual int getFuel() = 0;
	virtual void refill(int count) = 0;
};

struct sGasEngine : sCar {
	int getFuel() { return fuel; }
	void refill(int count) { fuel += count; }
	void move() {
		if (fuel != 0) { fuel--; sCar::move(); }
	}
	int fuel = CAR_FUEL;
};

struct sElectroCar : sCar {
	int getFuel() { return charge; }
	void refill(int count) { charge += count; }
	void move() {
		if (charge != 0) { charge--; sCar::move(); }
	}
	int charge = CAR_CHARGE;
};

struct sHybrid : sCar {
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		if (fuel != 0) { fuel--; sCar::move(); }
		else if (charge != 0) { charge--; sCar::move(); }
	}
	int fuel = CAR_FUEL / 2;
	int charge = CAR_CHARGE / 2;
};

void spawnCar() {
	sCar* car;
	int carType = rand() % 3;
	switch (carType) {
	case 0:
		car = new sGasEngine();
		break;
	case 1:
		car = new sElectroCar();
		break;
	case 2:
		car = new sHybrid();
		break;
	}

	int carDirection = rand() % 4;
	switch (carDirection) {
	case 0:
		car->dir = eDirection::UP;
		car->rect = sRect(SCREEN_WIDTH / 2 + CAR_SPEED + CAR_HEIGHT / 2, 0, CAR_HEIGHT, CAR_WIDTH);
		break;
	case 1:
		car->dir = eDirection::DOWN;
		car->rect = sRect(SCREEN_WIDTH / 2 - CAR_SPEED - CAR_HEIGHT / 2, SCREEN_HEIGHT, CAR_HEIGHT, CAR_WIDTH);
		break;
	case 2:
		car->dir = eDirection::LEFT;
		car->rect = sRect(SCREEN_WIDTH, SCREEN_HEIGHT / 2 + CAR_SPEED + CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT);
		break;
	case 3:
		car->dir = eDirection::RIGHT;
		car->rect = sRect(0, SCREEN_HEIGHT / 2 - CAR_SPEED - CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT);
		break;
	}
	car->speed = CAR_SPEED;
	//добавить сохранение в вектор sCarVector
}

std::vector<sCar*> sCarVector;

bool main_loop() {
	for (auto car1 : sCarVector) {
		for (auto car2 : sCarVector) {
			//ошибка условий, переписать в соответствии с ТЗ
			if (car1->getFuturePos().intersects(car2->getFuturePos())) {
				if (car1->needPassOtherCar(car2))
					car1->move();
			}
			else {
				car2->move();
			}
		}
		if (car1->rect.pos.x <= 0 || car1->rect.pos.x >= SCREEN_WIDTH || car1->rect.pos.y <= 0 || car1->rect.pos.y >= SCREEN_HEIGHT)
			spawnCar();
	}
	//добавить визуальное отображение
	return main_loop(); //бесконечный цикл?
}

int main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; i++) {
		spawnCar(); //отредактировать
	}
	main_loop();
	return 0;
}
