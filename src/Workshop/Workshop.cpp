
#include "Workshop.hpp"

#include <functional>
#include "./BaseDatos/FileManager.hpp"

#pragma region Global Variables

//! Deberia ser 1000
#define UPDATES_PER_SECOND 1000 / 1

#define DEFAULT_DIAGNOSE_TIME 1

// Global variables

std::mutex itinerariesMutex = std::mutex();

/** @brief cars coming from diagnose or stations that ended their work, going to re-schedule*/
std::vector<Itinerary *> itineraries = std::vector<Itinerary *>();

/** @brief incoming cars from client servers*/
std::queue<Car *> incomingCars = std::queue<Car *>();

std::mutex incomingCarsMutex = std::mutex();


std::vector<WorkStation*> stations = {
    new WorkStation(0),
    new WorkStation(1),
    new WorkStation(2),
    new WorkStation(3),
    new WorkStation(4),
    new WorkStation(5),
    new WorkStation(6),
    new WorkStation(7),
    new WorkStation(8),
};

/** @brief cars coming from scheduler that are totally repaired*/
std::queue<Car *> outcomingCars = std::queue<Car *>();

/** @brief is used by diagnose and schedule threads to prevent run conditions in itinerary array*/
Itinerary *useItinerariesQueue(Itinerary *itinerary)
{
    itinerariesMutex.lock();

    // getter
    if (itinerary == nullptr)
    {
        Itinerary *frontItinerary = itineraries.back();
        itineraries.pop_back();

        itinerariesMutex.unlock();
        return frontItinerary;
    }

    // setter
    itineraries.push_back(itinerary);

    itinerariesMutex.unlock();
    return nullptr;
}

std::mutex stationsMutex = std::mutex();

#pragma endregion

#pragma region Itinerary Definition

// Constructors
Itinerary::Itinerary()
{
    this->id = 0;
    this->car = nullptr;
    this->ComponentsToRepair = std::vector<std::vector<Component *>>();
    this->ComponentsRepaired = std::vector<std::vector<Component *>>();

    for (int i = 0; i < stations.size(); i++)
    {
        this->ComponentsRepaired.push_back(std::vector<Component *>());
    }
}

Itinerary::Itinerary(int id, Car *car, std::vector<std::vector<Component *>> ComponentsToRepair)
{
    this->id = id;
    this->car = car;
    this->ComponentsToRepair = ComponentsToRepair;
    this->ComponentsRepaired = std::vector<std::vector<Component *>>();

    for (int i = 0; i < stations.size(); i++)
    {
        this->ComponentsRepaired.push_back(std::vector<Component *>());
    }
}

Itinerary::Itinerary(int id, Car *car)
{
    this->id = id;
    this->car = car;
    this->ComponentsToRepair = std::vector<std::vector<Component *>>();
    this->ComponentsRepaired = std::vector<std::vector<Component *>>();

    for (int i = 0; i < stations.size(); i++)
    {
        this->ComponentsRepaired.push_back(std::vector<Component *>());
    }
}

// Getters
int Itinerary::getId() { return this->id; }
void Itinerary::setId(int id) { this->id = id; }

Car *Itinerary::getCar() { return this->car; }
void Itinerary::setCar(Car *car) { this->car = car; }

std::vector<std::vector<Component *>> *Itinerary::getComponentsToRepair() { return &(this->ComponentsToRepair); }
void Itinerary::setComponentsToRepair(const std::vector<std::vector<Component *>> &ComponentsToRepair) { this->ComponentsToRepair = ComponentsToRepair; }

std::vector<std::vector<Component *>> *Itinerary::getComponentsRepaired() { return &(this->ComponentsRepaired); }
void Itinerary::setComponentsRepaired(const std::vector<std::vector<Component *>> &ComponentsRepaired) { this->ComponentsRepaired = ComponentsRepaired; }
// Methods
bool Itinerary::isFullyRepaired()
{
    for (int i = 0; i < this->ComponentsToRepair.size(); i++)
    {
        if (this->ComponentsToRepair[i].size() > 0)
            return false;
    }
    return true;
}

void Itinerary::printItinerary()
{
    std::cout << "Itinerary #" << this->car->getId() << std::endl;
    // show failures id to repair by station
    for (int i = 0; i < this->ComponentsToRepair.size(); i++)
    {
        std::cout << "Station #" << i << ": ";
        for (int j = 0; j < this->ComponentsToRepair[i].size(); j++)
        {
            for (int k = 0; k < this->ComponentsToRepair[i][j]->getFailures().size(); k++)
            {
                if (!this->ComponentsToRepair[i][j]->getFailures()[k]->getRepaired())
                    std::cout << this->ComponentsToRepair[i][j]->getFailures()[k]->getId() << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Fallos reparados: ";
    for (int i = 0; i < this->ComponentsRepaired.size(); i++)
    {
        for (int j = 0; j < this->ComponentsRepaired[i].size(); j++)
        {
            for (int k = 0; k < this->ComponentsRepaired[i][j]->getFailures().size(); k++)
            {
                if (this->ComponentsRepaired[i][j]->getFailures()[k]->getRepaired())
                    std::cout << this->ComponentsRepaired[i][j]->getFailures()[k]->getId() << " ";
            }
        }
    }
    std::cout << std::endl;
}

void Itinerary::addComponentToRepair(Component *component, int stationId)
{
    this->ComponentsToRepair[stationId].push_back(component);
}
#pragma endregion

#pragma region WorkStation Definition

// constructors

WorkStation::WorkStation()
{
    this->id = 0;
    this->carQueue = std::queue<Itinerary *>();
}

WorkStation::WorkStation(int id)
{
    this->id = id;
    this->carQueue = std::queue<Itinerary *>();
}

// getters and setters

void WorkStation::setId(int id) { this->id = id; }
int WorkStation::getId() { return this->id; }

//* these in particular are thread safe

std::queue<Itinerary *> *WorkStation::getCarQueue()
{
    this->mutex.lock();
    std::queue<Itinerary *> *temp = &(this->carQueue);
    this->mutex.unlock();
    return temp;
}

void WorkStation::setCarQueue(std::queue<Itinerary *> carQueue)
{
    this->mutex.lock();
    this->carQueue = carQueue;
    this->mutex.unlock();
}

int WorkStation::getRepairTime()
{
    this->mutex.lock();
    int temp = this->repairTime;
    this->mutex.unlock();
    return temp;
}

int WorkStation::getRepairTimeCounter()
{
    this->mutex.lock();
    int temp = this->repairTimeCounter;
    this->mutex.unlock();
    return temp;
}

Itinerary *WorkStation::getCurrentItinerary()
{
    this->mutex.lock();
    Itinerary *temp = this->currentItinerary;
    this->mutex.unlock();
    return temp;
}

Failure *WorkStation::getCurrentFailure()
{
    this->mutex.lock();
    Failure *temp = this->currentFailure;
    this->mutex.unlock();
    return temp;
}

// methods

void WorkStation::addCar(Itinerary *car)
{
    this->mutex.lock();
    this->carQueue.push(car);
    this->mutex.unlock();
}

void WorkStation::removeCar()
{
    this->carQueue.pop();
}

void WorkStation::printStation()
{
    std::cout << "Station #" << this->id << std::endl;
    std::cout << "Cars: " << std::endl;
    std::queue<Itinerary *> temp = this->carQueue;
    while (!temp.empty())
    {
        std::cout << temp.front()->getId() << std::endl;
        temp.pop();
    }
}

void WorkStation::repairCar()
{
    while (true)    {
        if (!this->carQueue.empty())
        {
            Itinerary *itinerary = this->carQueue.front();
            this->carQueue.pop();

            this->currentItinerary = itinerary;

            // Se obtienen los componentes a reparar en esta estacion

            // Se simula la reparación para cada componente
            while (!itinerary->getComponentsToRepair()->at(this->id).empty())
            {

                // Se obtiene el componente y se elimina de la lista de componentes por reparar

                Component *component = itinerary->getComponentsToRepair()->at(this->id).back();
                itinerary->getComponentsToRepair()->at(this->id).pop_back();

                std::vector<Failure *> failures = component->getFailures();

                // Se simula la reparación para cada falla
                for (int j = 0; j < failures.size(); j++)
                {
                    // Se obtiene la falla
                    Failure *failure = failures[j];

                    this->currentFailure = failure;
                    this->repairTime = failure->getRepairTime();
                    this->repairTimeCounter = 0;

                    // Se simula la reparación
                    // TODO #1 agregar notificacion de reparacion
                    for (int i = 0; i < failure->getRepairTime(); i++)
                    {
                        this->repairTimeCounter++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATES_PER_SECOND));
                    }

                    // Se marca la falla como reparada

                    failure->setRepaired(true);

                    // TODO #2 agregar reparacion a la historia de reparaciones del vehiculo
                }

                // Se agrega el componente a la lista de componentes reparados
                itinerary->getComponentsRepaired()->at(this->getId()).push_back(component);
            }

            // Se devuelve el auto a la cola de itinerarios

            this->currentItinerary = nullptr;
            this->currentFailure = nullptr;
            this->repairTime = 0;
            this->repairTimeCounter = 0;

            useItinerariesQueue(itinerary);
        }
    }
}

#pragma endregion

#pragma region Workshop Definition

int diagnoseTimeCounter = 0;
int diagnose_diagnoseTime = 0;

Car *diagnose_currentCar = nullptr;
int diagnose_totalFailures = 0;
int diagnose_failuresDiagnosed = 0;

void diagnose()
{
    while (true)
    {
        if (!incomingCars.empty())
        {

            Car *car = incomingCars.front();
            incomingCars.pop();

            diagnose_currentCar = car;

            std::vector<Component *> ComponentsToRepair = car->check();

            diagnose_totalFailures = ComponentsToRepair.size();
            diagnose_diagnoseTime = ComponentsToRepair.size() * DEFAULT_DIAGNOSE_TIME;

            std::vector<std::vector<Component *>> ComponentsToRepairByStation;

            // initialize the vector

            for (int i = 0; i < stations.size(); i++)
            {
                ComponentsToRepairByStation.push_back(std::vector<Component *>());
            }

            for (int i = 0; i < ComponentsToRepair.size(); i++)
            {
                for (int j = 0; j < ComponentsToRepair[i]->getFailures().size(); j++)
                {

                    ComponentsToRepairByStation[ComponentsToRepair[i]->getFailures()[j]->getStationId()].push_back(ComponentsToRepair[i]);

                    for (int k = 0; k < DEFAULT_DIAGNOSE_TIME; k++)
                    {
                        diagnoseTimeCounter++;
                        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATES_PER_SECOND));
                    }

                    diagnose_failuresDiagnosed++;
                }
            }

            Itinerary *itinerary = new Itinerary(0, car, ComponentsToRepairByStation);

            diagnose_currentCar = nullptr;
            diagnose_totalFailures = 0;
            diagnose_failuresDiagnosed = 0;
            diagnoseTimeCounter = 0;
            diagnose_diagnoseTime = 0;

            useItinerariesQueue(itinerary);
        }
    }
}

void schedule()
{
    while (true)
    {

        if (itineraries.empty())
        {
            continue;
        }

        Itinerary *itinerary = useItinerariesQueue(nullptr);

        // verifies if the actual car is ready
        if (itinerary->isFullyRepaired())
        {
            // TODO #3 agregar notificacion de vehiculo listo y devolver al cliente
            CrearHistorial(itinerary);
            //  std::cout << "\n-------------------------------\nauto " << itinerary->getCar()->getId() << " listo\n------------------------------------" << std::endl;
            continue;
        }

        bool carAdded = false;

        // search for empty stations and add the car to the queue if it has failures to repair in that station
        for (int i = 0; i < stations.size(); i++)
        {
            if (stations[i]->getCarQueue()->empty())
            {
                if (itinerary->getComponentsToRepair()->at(i).size() > 0)
                {
                    stations[i]->addCar(itinerary);
                    carAdded = true;
                    break;
                }
            }
        }

        if (carAdded)
        {
            continue;
        }

        // search for stations with cars in queue and add the car to the queue if it has failures to repair in that station
        for (int i = 0; i < itinerary->getComponentsToRepair()->size(); i++)
            if (itinerary->getComponentsToRepair()[i].size() > 0)
            {
                stations[i]->addCar(itinerary);
                break;
            }
    }
}

void startSystem()
{

    std::thread diagnoseThread(diagnose);
    std::thread scheduleThread(schedule);

    std::vector<std::thread> repairThreads = std::vector<std::thread>();

    for (int i = 0; i < stations.size(); i++)
    {
        repairThreads.push_back(std::thread(&WorkStation::repairCar, stations[i]));
    }

    diagnoseThread.join();
    scheduleThread.join();

    for (int i = 0; i < repairThreads.size(); i++)
    {
        repairThreads[i].join();
    }
}

#pragma endregion