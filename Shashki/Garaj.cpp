#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <windows.h> 

// Абстрактный базовый класс для транспортных средств
class Vehicle {
public:
    virtual ~Vehicle() {}
    virtual std::string getType() const = 0;
    virtual void printDetails() const = 0;
};

// Класс для автомобилей
class Car : public Vehicle {
public:
    Car(const std::string& model, int year) : model(model), year(year) {}

    std::string getType() const override {
        return "Car";
    }

    void printDetails() const override {
        std::cout << "Car Model: " << model << ", Year: " << year << std::endl;
    }

private:
    std::string model;
    int year;
};

// Класс для велосипедов
class Bicycle : public Vehicle {
public:
    Bicycle(const std::string& brand, bool hasGears) : brand(brand), hasGears(hasGears) {}

    std::string getType() const override {
        return "Bicycle";
    }

    void printDetails() const override {
        std::cout << "Bicycle Brand: " << brand << ", Has Gears: " << (hasGears ? "Yes" : "No") << std::endl;
    }

private:
    std::string brand;
    bool hasGears;
};

// Шаблонный класс для гаража
template <typename T>
class Garage {
public:
    void addVehicle(std::shared_ptr<T> vehicle) {
        vehicles.push_back(vehicle);
    }

    void showVehicles() const {
        for (const auto& vehicle : vehicles) {
            std::cout << vehicle->getType() << ": ";
            vehicle->printDetails();
        }
    }

private:
    std::vector<std::shared_ptr<T>> vehicles;
};

int garaj() {
    setlocale(LC_ALL, "russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Garage<Vehicle> garage;

    std::shared_ptr<Vehicle> car1 = std::make_shared<Car>("Toyota Camry", 2020);
    std::shared_ptr<Vehicle> car2 = std::make_shared<Car>("Honda Accord", 2018);
    std::shared_ptr<Vehicle> bike1 = std::make_shared<Bicycle>("Trek FX", true);
    std::shared_ptr<Vehicle> bike2 = std::make_shared<Bicycle>("Giant Escape", false);

    garage.addVehicle(car1);
    garage.addVehicle(car2);
    garage.addVehicle(bike1);
    garage.addVehicle(bike2);

    garage.showVehicles();

    return 0;
}
