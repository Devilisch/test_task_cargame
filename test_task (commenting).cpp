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
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.width); // width => height
		case eDirection::LEFT:
			return sRect(rect.pos.x + speed, rect.pos.y, rect.size.width, rect.size.height); // rect.pos.x + speed => rect.pos.x - speed
		}
	}

	//ошибки в выполнении функции, нужно правильно вписать правила из задания
	bool needPassOtherCar(sCar* otherCar) {
		bool result;
		switch (dir) {
		case eDirection::UP:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::LEFT)
				result = true;
			break;
		case eDirection::DOWN:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::RIGHT)
				result = true;
			break;
		case eDirection::RIGHT:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::UP)
				result = true;
			break;
		case eDirection::LEFT:
			auto otherdir = otherCar->dir;
			if (otherdir == eDirection::LEFT)
				result = false;
			else
				result = false;
			break;
		}
		return result;
	}

	virtual int getFuel() = 0;
	virtual void refill(int count) = 0; //присваивание значения для функции типа void
};

struct sGasEngine : sCar {
	int getFuel() { return fuel; }
	void refill(int count) { fuel += count; }
	void move() { fuel--; sCar::move(); }
	int fuel;
};

struct sElectroCar : sCar {
	int getFuel() { return charge; }
	void refill(int count) { charge += count; }
	void move() { charge--; sCar::move(); }
	int charge;
};

struct sHybrid : sGasEngine, sElectroCar {
	void refill(int count) { charge += count / 2; fuel += count / 2; }
	int getFuel() { return charge + fuel; }
	void move() {
		//сомнительная схема сжигания топлива, с учётом принципа пополнения топлива
		if (rand() % 2 == 0)
			charge--;
		else
			fuel--;
		sCar::move();
	}
};

std::vector<sCar*> asdasd; //некорректное название, не отображает сути объекта, тяжело воспринимать в коде
const int initialCarsCount = 10;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
//объявить через константы размеры для машин
#define CAR_
//объявить через константу начальную скорость машины


//неправильное использование функции rand, нужно выделить под её значение отдельную переменную, значения начинаются с 0
void spawnCar() {
	if (rand() % 4 == 1)
		spawnCarFromRight();
	else if (rand() % 4 == 2)
		spawnCarFromTop();
	else if (rand() % 4 == 3)
		spawnCarFromBot();
	else if (rand() % 4 == 4)
		SpawnCarFromLeft();
}

//повторяющиеся функции, можно объединить в одну +++
void spawnCarFromTop() {
	sCar* car;
	//повторяющаяся часть кода, можно выделить в отдельную функцию ---
	int carType = rand();
	if (carType % 3 == 0) {
		car = new sGasEngine();
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
	}
	//---
	car->rect = sRect(SCREEN_WIDTH / 2, 0, 100, 100);
	car->speed = 1;
	car->dir = eDirection::DOWN; //инициализация направления, должна производится в каждой функции спавна
}

void spawnCarFromBot() {
	sCar* car;
	//повторяющаяся часть кода, можно выделить в отдельную функцию ---
	int carType = rand();
	if (carType % 3 == 0) {
		car = new sGasEngine();
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
	}
	//---
	car->rect = sRect(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 100, 100);
	car->speed = 1;
}

void SpawnCarFromLeft() {
	sCar* car;
	//повторяющаяся часть кода, можно выделить в отдельную функцию ---
	int carType = rand();
	if (carType % 3 == 0) {
		car = new sGasEngine();
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
	}
	//---
	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->speed = 1;
}

void spawnCarFromRight() {
	sCar* car;
	//повторяющаяся часть кода, можно выделить в отдельную функцию ---
	int carType = rand();
	if (carType % 3 == 0) {
		car = new sGasEngine();
	}
	else if (carType % 3 == 1) {
		car = new sElectroCar();
	}
	else if (carType % 3 == 2) {
		car = new sHybrid();
	}
	//---
	car->rect = sRect(0, SCREEN_HEIGHT / 2, 100, 100);
	car->speed = 1;
}
//+++

bool main_loop() {
	for (auto car : asdasd) {
		for (auto car22 : asdasd) {
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