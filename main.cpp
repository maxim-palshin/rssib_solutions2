#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>
#include <random>

#include "config.h"

enum MotorTransportType{
    BIKE = 0,
    SCOOTER,
    CAR,
    BUS,
};

class MotorTransport{
public:

    virtual ~MotorTransport(){}

    virtual std::size_t countWheels()const = 0;

    virtual  std::string toString() = 0;

protected:
    float _maxSpeed;

    template<typename D,typename ...ARG>
    void _generateMaxSpeed(ARG ...arg){
        auto seed = std::random_device{}();
        std::mt19937  rg(seed);
        _maxSpeed =  std::round(D(arg...)(rg)*100)/100;
    }


private:
};

class MotorTransportFactory{
public:
    virtual  ~MotorTransportFactory(){}

    virtual  std::shared_ptr<MotorTransport> instanceCreate() = 0;




};

class Bike final : public MotorTransport{
public:

    Bike(){
        _generateMaxSpeed<std::uniform_real_distribution<float>>(12.f,50.f);
    }

    std::size_t countWheels()const override{
        return 2;
    }

    std::string toString() override{
        std::stringstream  ss;
        ss << "Наименование транспорта: Велосипед\n"
        << "Кол-во колес: " << countWheels() << '\n'
        << "Максимальная скорость: " << _maxSpeed<<'\n'
        << "Максимальное количество пассажиров: " << 1<<'\n';

        return ss.str();
    }

private:

};

class Scooter final : public MotorTransport{
public:


    Scooter(){
        _generateMaxSpeed<std::uniform_real_distribution<float>>(5.f,10.f);
    }

    std::size_t countWheels()const override{
        return 2;
    }

    std::string toString() override{
        std::stringstream  ss;
        ss << "Наименование транспорта: Скутер\n"
           << "Кол-во колес: " << countWheels() << '\n'
           << "Максимальная скорость: " << _maxSpeed<<'\n'
           << "Максимальное количество пассажиров: " << 1 <<'\n';

        return ss.str();
    }

private:
};

class Car final : public MotorTransport{
public:

    Car(){
        _generateMaxSpeed<std::uniform_real_distribution<float>>(90.f,220.f);
    }

    std::size_t countWheels()const override{
        return 4;
    }

    std::string toString() override{
        std::stringstream  ss;
        ss << "Наименование транспорта: Машина\n"
           << "Кол-во колес: " << countWheels() << '\n'
           << "Максимальная скорость: " << _maxSpeed<<'\n'
           << "Максимальное количество пассажиров: " << 4 <<'\n';

        return ss.str();
    }

private:
};

class Bus final : public MotorTransport{
public:


    Bus(){
        _generateMaxSpeed<std::uniform_real_distribution<float>>(50.f,120.f);

        std::mt19937 rg(std::random_device{}());
        _countPassengers = std::uniform_int_distribution(6,35)(rg);

    }

    std::size_t countWheels()const override{
        return 6;
    }



    std::string toString() override{
        std::stringstream  ss;
        ss << "Наименование транспорта: Автобус\n"
           << "Кол-во колес: " << countWheels() << '\n'
           << "Максимальная скорость: " << _maxSpeed<<'\n'
           << "Максимальное количество пассажиров: " << _countPassengers <<'\n';

        return ss.str();
    }

private:
    std::size_t  _countPassengers;
};


class BikeFactory final  : public  MotorTransportFactory{
public:

    std::shared_ptr<MotorTransport> instanceCreate()override{
        return std::make_shared<Bike>();
    }

};

class ScooterFactory final  : public  MotorTransportFactory{
public:

    std::shared_ptr<MotorTransport> instanceCreate()override{
        return std::make_shared<Scooter>();
    }

};

class CarFactory final  : public  MotorTransportFactory{
public:

    std::shared_ptr<MotorTransport> instanceCreate()override{
        return std::make_shared<Car>();
    }

};

class BusFactory final  : public  MotorTransportFactory{
public:

    std::shared_ptr<MotorTransport> instanceCreate()override{
        return std::make_shared<Bus>();
    }

};


std::unique_ptr<MotorTransportFactory> getFactoryByType(MotorTransportType type){

    switch (type) {
        case BIKE: return std::make_unique<BikeFactory>();
        case SCOOTER: return std::make_unique<ScooterFactory>();
        case CAR: return std::make_unique<CarFactory>();
        case BUS: return std::make_unique<BusFactory>();
    }

    throw std::invalid_argument("unknown motor transport Type");
}




int main(int argc, char **argv) {

    if constexpr (gWindows) {
        setlocale(LC_ALL, "");
    }

    try {

    if(argc == 1){
        std::cerr << "Введите тип транспорта\n";

        std::cout << "Велосипед = 0\n"
                     "Самокат = 1\n"
                     "Машина = 2\n"
                     "Автобус = 3\n";


        return EXIT_FAILURE;
    }


    for(int i = 1; i != argc; ++i)try{
        MotorTransportType type = static_cast<MotorTransportType>(std::stoull(std::string(argv[i])));
        std::cout << getFactoryByType(static_cast<MotorTransportType>(type))->instanceCreate()->toString() << '\n';

    }catch(std::invalid_argument _){
        std::cerr << "Тип транспорта("<<argv[i]<<") указан неверно.\n";
    }catch(...){
            std::cerr << "Неправильный формат ввода\n";
    }




    } catch (const std::exception &exp) {
        std::cerr << exp.what() << '\n';
        return EXIT_FAILURE;
    }
}
