#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include "../Vehicle/Car.hpp"
#define MAX_STATIONS 8



class Itinerary {
private:
    int id;
    Car* car;
    std::vector<std::vector<Component*>> ComponentsToRepair;
    std::vector<std::vector<Component*>> ComponentsRepaired;

public:
    //getters and setters
    void setId(int id);

    int getId();

    void setCar(Car* car);

    Car* getCar();

    void setComponentsToRepair(const std::vector<std::vector<Component*>>& ComponentsToRepair);

    std::vector<std::vector<Component*>>* getComponentsToRepair();

    void setComponentsRepaired(const std::vector<std::vector<Component*>>& ComponentsRepaired);

    std::vector<std::vector<Component*>>* getComponentsRepaired();

    //constructors

    Itinerary();

    Itinerary(int id, Car* car);

    Itinerary(int id, Car* car, std::vector<std::vector<Component*>> ComponentsToRepair);

    //methods

    void printItinerary();

    /** @brief verifies if every vector of components is empty. If so, returns true.  */
    bool isFullyRepaired();

    void addComponentToRepair(Component* component, int stationId);

};

class WorkStation {
    
private:
    std::mutex mutex;
    int id;
    std::queue<Itinerary*> carQueue;

    int repairTime = 0;
    int repairTimeCounter = 0;
    Itinerary* currentItinerary = nullptr;
    Failure* currentFailure = nullptr;

public:
    //constructors

    WorkStation();

    WorkStation(int id);



    //getters and setters

    void setId(int id);

    int getId();

    std::queue<Itinerary*>* getCarQueue();

    void setCarQueue(std::queue<Itinerary*> carQueue);

    int getRepairTime();

    int getRepairTimeCounter();

    Itinerary* getCurrentItinerary();

    Failure* getCurrentFailure();








    //methods

    void addCar(Itinerary* car);

    void removeCar();


    void printStation();

    void repairCar();
};




/** @brief a queue of cars coming from client apps */
extern std::queue<Car*> incomingCars;
/** @brief diagnoses the cars coming from incomingCars */
void diagnose();


/** @brief a vector of itineraries coming from the diagnosis module */
extern std::vector<Itinerary*> itineraries;
/** @brief schedules the itineraries, dispatching to every workstation, optimizing their use */
void schedule();
/** @brief a mutex for the itineraries vector */
extern std::mutex itinerariesMutex;


/** @brief a vector of workstations */
extern std::vector<WorkStation*> stations;
extern std::mutex stationsMutex;

extern void startSystem();


extern int diagnoseTimeCounter;
extern int diagnose_diagnoseTime;

extern Car* diagnose_currentCar;
extern int diagnose_totalFailures;
extern int diagnose_failuresDiagnosed;
