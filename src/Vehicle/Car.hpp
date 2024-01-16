#pragma once 

#include <string>
#include <iostream>
#include <vector>


class Failure {
private:
    int id;

    std::string name;
    std::string description;

    /** @brief the id of the station that can repair it*/
    int stationId;

    int repairTime;
    int checkTime;

    bool repaired;
public:
    Failure();

    Failure(const std::string& failureName);

    Failure(int id, int stationId, const std::string& failureName, const std::string& failureDescription, const int& failureRepairTime);

    //getters and setters
    int getId();
    void setId(int failureId);

    void setName(const std::string& failureName);
    std::string getName();

    void setDescription(const std::string& failureDescription);
    std::string getDescription();

    void setRepairTime(const int& failureRepairTime);
    int getRepairTime();

    void setRepaired(const bool& failureRepaired);
    bool getRepaired();

    void setStationId(const int& failureStationId);
    int getStationId();

    //methods

    /**
     * @brief Prints the failure.
     *
     */
    void printFailure();
};

class Component {
private:
    std::string name;
    std::vector<Failure*> failures;
    bool checked;
public:

    //constructor 1
    Component();

    //constructor 2
    Component(const std::string& componentName);

    //constructor 3
    Component(const std::string& componentName, const std::vector<Failure*>& componentFailuresList);

    //getters and setters
    std::string getName();
    void setName(const std::string& componentName);

    std::vector<Failure*> getFailures();
    void setFailures(const std::vector<Failure*>& componentFailuresList);

    bool getChecked();
    void setChecked(const bool& componentChecked);

    //methods

    void fail();

    void addFailure(Failure* failure);

    void removeFailure(Failure* failure);

    std::vector<Component*> check();
};

class System {
private:
    std::string name;
    std::vector<Component> components;
    bool checked;

public:
    //constructor 1
    System();

    //constructor 2
    System(const std::string& componentName);

    //constructor 3
    System(const std::string& componentName, const std::vector<Component>& subcomponentsList);


    //getters and setters
    std::vector<Component> getComponents();
    void setSubcomponents(const std::vector<Component>& subcomponentsList);


    std::string getName();
    void setName(const std::string& componentName);

    bool getChecked();
    void setChecked(const bool& componentChecked);

    //methods


    void fail();

    /**
     *@brief adds a list of sub-components to the system.
     *
     * @param subcomponentsList The list of sub-components to be added.
     */
    void addComponents(std::vector<Component> subcomponentsList);


    /**
     * @brief Adds a sub-component to the system
     *
     * @param subComponent The sub-component to be added.
     */
    void addComponent(Component component);

    /**
    * @brief Checks the component and its sub-components for failures.
    *
    * @return A vector of Failure objects representing the failures found.
    */

    std::vector<Component*> check();
};

class Car {
private:
    std::string model;
    //el id se actualiza por cada auto que es asinado un itinerario. Este se guarda en la base de datos.
    int id;
    std::vector<System> systems;
    bool checked;
public:
    //constructor 1
    Car();

    //constructor 2
    Car(std::string model, int id, std::vector<System> systems);

    //getters and setters

    std::string getModel();
    void setModel(const std::string& carModel);

    int getId();
    void setId(const int& carId);

    std::vector<System> getSystems();
    void setSystems(const std::vector<System>& carSystems);

    bool getChecked();
    void setChecked(const bool& carChecked);

    //methods

    void fail();

    /**
     * @brief Adds a system to the car.
     *
     * @param system The system to be added.
     */
    void addSystem(System system);

    /**
     * @brief Adds a list of systems to the car.
     *
     * @param systems The list of systems to be added.
     */
    void addSystems(std::vector<System> systems);

    /**
     * @brief Checks the car and its systems for failures.
     *
     * @return A vector of Component* objects representing the failures found.
     */
    std::vector<Component*> check();
};

//TODO #3 hacer que lea de sde archivo cuantos van para mantener secencia cada que se cree un auto
extern int actualCarID;

/**
 * @brief Generates a car with no failures.
 *
 * @return A car with no failures.
*/
Car generateCar();
Car *generateCarPointer();

    /**
     * @brief Generates a car with random failures.
     *
     * @return A car with random failures.
     */
    // Car generateRandomCar();
