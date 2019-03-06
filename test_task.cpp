std::vector<sCar*> sCarVector;
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
		case eDirection::DOWN:
			return sRect(rect.pos.x, rect.pos.y - speed, rect.size.width, rect.size.height);
		case eDirection::RIGHT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height); 
		case eDirection::LEFT:
			return sRect(rect.pos.x - speed, rect.pos.y, rect.size.width, rect.size.height);
		}
	}

	bool needPassOtherCar(sCar* otherCar) {
		bool result;
		switch (dir) {
		case eDirection::UP:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::LEFT)
				result = true;
			else
				result = false;
			break;
		case eDirection::DOWN:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::RIGHT)
				result = true;
			else
				result = false;
			break;
		case eDirection::RIGHT:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::UP)
				result = true;
			else
				result = false;
			break;
		case eDirection::LEFT:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::DOWN)
				result = true;
			else
				result = false;
			break;
		}
		return result;
	}

	virtual int getFuel() = 0;
	virtual void refill(int count); //присваивание значения для функции типа void
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

struct sHybrid : sGasEngine, sElectroCar {
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		if (fuel != 0) { fuel--; sCar::move(); }
		else if (charge != 0) { charge--; sCar::move(); }	
	}
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
}

bool main_loop() {
	for (auto car : sCarVector) {
		for (auto car22 : sCarVector) {
			//ошибка условий, переписать в соответствии с ТЗ
			if (car->getFuturePos().intersects(car22->getFuturePos())) {
				if (car->needPassOtherCar(car22))
					car->move();
			}
			else {
				car22->move();
			}
		}
		if (car->rect.pos.x <= 0 || car->rect.pos.x >= SCREEN_WIDTH || car->rect.pos.y <= 0 || car->rect.pos.y >= SCREEN_HEIGHT)
			spawnCar();
	}
	//добавить визуальное отображение
	return main_loop(); //бесконечный цикл?
}

int main(int argc, char** argv) {
	for (auto i = 0; i < initialCarsCount; ++i) {
		spawnCar();
	}
	main_loop();
	return 0;
}