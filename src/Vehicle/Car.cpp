#include "Car.hpp"
#include "FailureLibrary.hpp"
#include <mutex>


#pragma region Failure Implementation

std::mutex FailureMutex;

//Constructors
Failure::Failure() {
    int id;
    this->name = "";
    this->description = "";
    this->repairTime = 0;
    this->repaired = false;
}

Failure::Failure(const std::string& failureName) : name(failureName) {
    this->description = "";
    this->repairTime = 0;
    this->repaired = false;
}

Failure::Failure(int id, int stationId, const std::string& failureName, const std::string& failureDescription, const int& failureRepairTime)
    : name(failureName), description(failureDescription), repairTime(failureRepairTime), id(id), stationId(stationId) {
    this->repaired = false;
}

//getters and setters

void Failure::setId(int id) { this->id = id; }
int Failure::getId() { return this->id; }

void Failure::setName(const std::string& failureName) { this->name = failureName; }
std::string Failure::getName() { return this->name; }

void Failure::setDescription(const std::string& failureDescription) { this->description = failureDescription; }
std::string Failure::getDescription() { return this->description; }

void Failure::setRepairTime(const int& failureRepairTime) { this->repairTime = failureRepairTime; }
int Failure::getRepairTime() { return this->repairTime; }

void Failure::setRepaired(const bool& failureRepaired) { this->repaired = failureRepaired; }
bool Failure::getRepaired() { return this->repaired; }

void Failure::setStationId(const int& failureStationId) { this->stationId = failureStationId; }
int Failure::getStationId() { return this->stationId; }


//methods

void Failure::printFailure() {
    std::cout << "id" << this->id << std::endl;
    std::cout << "Station id: " << this->stationId << std::endl;
    std::cout << "Failure: " << this->name << std::endl;
    std::cout << "Description: " << this->description << std::endl;
    std::cout << "Repair time: " << this->repairTime << std::endl;
    std::cout << "Repaired: " << this->repaired << std::endl;
}

#pragma endregion

#pragma region Component Implementation

std::mutex ComponentMutex;
//Constructors
Component::Component() {
    this->name = "";
    this->failures = std::vector<Failure*>();
    this->checked = false;
}

Component::Component(const std::string& componentName) : name(componentName) {
    this->failures = std::vector<Failure*>();
    this->checked = false;
}

Component::Component(const std::string& componentName, const std::vector<Failure*>& componentFailuresList)
    : name(componentName), failures(componentFailuresList) {
    this->checked = false;
}

//getters and setters
std::string Component::getName() { return this->name; }
void Component::setName(const std::string& componentName) { this->name = componentName; }

std::vector<Failure*> Component::getFailures() { return this->failures; }
void Component::setFailures(const std::vector<Failure*>& componentFailuresList) { this->failures = componentFailuresList; }

bool Component::getChecked() { return this->checked; }
void Component::setChecked(const bool& componentChecked) { this->checked = componentChecked; }

//methods

/** @brief Makes the component fail from a random failure */
void Component::fail() {
    // Get a random failure from the failure library
    std::vector<Failure> failures = getFailuresByComponent(this->name);

    int randomFailureIndex = (rand() + 1) % failures.size();


    Failure failure = failures[randomFailureIndex];

    //verify if the failure already exists 
    for (int i = 0; i < this->failures.size(); i++) {
        if (this->failures[i]->getName() == failure.getName()) {
            return;
        }
    }

    // Add the failure to the component
    this->addFailure(new Failure(failure.getId(), failure.getStationId(), failure.getName(), failure.getDescription(), failure.getRepairTime()));
};

/** @brief Adds a failure to the component.*/
void Component::addFailure(Failure* failure) {
    ComponentMutex.lock();
    this->failures.push_back(failure);
    ComponentMutex.unlock();
};

/** * @brief Removes a failure. */
void Component::removeFailure(Failure* failure) {
    ComponentMutex.lock();
    for (int i = 0; i < this->failures.size(); i++) {
        if (this->failures[i]->getName() == failure->getName()) {
            this->failures.erase(this->failures.begin() + i);
            break;
        }
    }
    ComponentMutex.unlock();
};

/** @brief Checks the component for failures.
 * @return A vector of Failure objects representing the failures found.*/
std::vector<Component*> Component::check() {
    std::vector<Component*> failuresFound;

    // If the component has already been checked, return an empty vector.
    if (this->checked) {
        return failuresFound;
    }

    // If the component has not been checked, mark it as checked.
    if (this->failures.size() > 0) {
        for (int i = 0; i < this->failures.size(); i++) {
            if (!this->failures[i]->getRepaired()) {
                failuresFound.push_back(this);
                break;
            }
        }
    }

    this->checked = true;

    return failuresFound;
};

#pragma endregion

#pragma region System Implementation 

//Constructors
System::System() {
    this->name = "";
    this->components = std::vector<Component>();
    this->checked = false;
}

System::System(const std::string& componentName) : name(componentName) {
    this->components = std::vector<Component>();
    this->checked = false;
}

System::System(const std::string& componentName, const std::vector<Component>& subcomponentsList)
    : name(componentName), components(subcomponentsList) {
    this->checked = false;
}

//getters and setters
std::vector<Component> System::getComponents() { return this->components; }
void System::setSubcomponents(const std::vector<Component>& subcomponentsList) { this->components = subcomponentsList; }


std::string System::getName() { return this->name; }
void System::setName(const std::string& componentName) { this->name = componentName; }

bool System::getChecked() { return this->checked; }
void System::setChecked(const bool& componentChecked) { this->checked = componentChecked; }

//methods

/** @brief Takes random systems of the car and makes at least one to fail*/
void System::fail() {

    bool AtLeastOneFailed = false;


    while (!AtLeastOneFailed)
        for (int i = 0; i < this->components.size(); i++)
            if ((rand() + 1) % 2 == 0) {
                this->components[i].fail();
                AtLeastOneFailed = true;
            }
}



/** @brief adds a list of sub-components to the system.
 * @param subcomponentsList The list of sub-components to be added.*/
void System::addComponents(std::vector<Component> subcomponentsList) {
    this->components.insert(this->components.end(), subcomponentsList.begin(), subcomponentsList.end());
};


/**
 * @brief Adds a sub-component to the system
 *
 * @param subComponent The sub-component to be added.
 */
void System::addComponent(Component component) {
    this->components.push_back(component);
};

/**
* @brief Checks the component and its sub-components for failures.
*
* @return A vector of Failure objects representing the failures found.
*/
std::vector<Component*> System::check() {

    std::vector<Component*> failuresFound;

    // If the component has already been checked, return an empty vector.
    if (this->checked) {
        return failuresFound;
    }

    // If cthe system has not been checked, mark it as checked.
    this->checked = true;

    // Check the component for failures.
    if (this->components.size() > 0) {
        for (int i = 0; i < this->components.size(); i++) {
            std::vector<Component*> failuresFoundInSubComponent = this->components[i].check();
            if (failuresFoundInSubComponent.size() > 0) {
                failuresFound.insert(failuresFound.end(), failuresFoundInSubComponent.begin(), failuresFoundInSubComponent.end());
            }
        }
    }
    return failuresFound;
};

#pragma endregion

#pragma region Car Implementation

//Constructors
Car::Car() {
    this->model = "Explorer";
    this->id = 0;
    this->systems = std::vector<System>();
    this->checked = false;
}

//constructor 2
Car::Car(std::string model, int id, std::vector<System> systems) {
    this->model = model;
    this->id = id;
    this->systems = systems;
    this->checked = false;
};

//getters and setters

std::string Car::getModel() { return this->model; }
void Car::setModel(const std::string& carModel) { this->model = carModel; }

int Car::getId() { return this->id; }
void Car::setId(const int& carId) { this->id = carId; }

std::vector<System> Car::getSystems() { return this->systems; }
void Car::setSystems(const std::vector<System>& carSystems) { this->systems = carSystems; }

bool Car::getChecked() { return this->checked; }
void Car::setChecked(const bool& carChecked) { this->checked = carChecked; }

//methods

/** @brief takes a random set of systems and makes them fail. The probability of a system failing is
 *less than the probability of a component failing, but at least one system will fail */
void Car::fail() {
    bool AtLeastOneFailed = false;


    while (!AtLeastOneFailed)
        for (int i = 0; i < this->systems.size(); i++)
            if ((rand() + 1) % 100 <= 15) {
                this->systems[i].fail();
                AtLeastOneFailed = true;
            }
};

/**
 * @brief Adds a system to the car.
 *
 * @param system The system to be added.
 */
void Car::addSystem(System system) {
    this->systems.push_back(system);
};

/**
 * @brief Adds a list of systems to the car.
 *
 * @param systems The list of systems to be added.
 */
void Car::addSystems(std::vector<System> systems) {
    this->systems.insert(this->systems.end(), systems.begin(), systems.end());
};

/**
 * @brief Checks the car and its systems for failures.
 *
 * @return A vector of Component* objects representing the failures found.
 */
std::vector<Component*> Car::check() {
    std::vector<Component*> failuresFound;

    // If the car has already been checked, return an empty vector.
    if (this->checked) {
        return failuresFound;
    }

    // If the car has not been checked, mark it as checked.
    this->checked = true;

    // Check the car for failures.
    if (this->systems.size() > 0) {
        for (int i = 0; i < this->systems.size(); i++) {
            std::vector<Component*> failuresFoundInSystem = this->systems[i].check();
            if (failuresFoundInSystem.size() > 0) {
                failuresFound.insert(failuresFound.end(), failuresFoundInSystem.begin(), failuresFoundInSystem.end());
            }
        }
    }
    return failuresFound;
};
#pragma endregion


//TODO #3 needs to read from file
int actualCarID = 0;


Car* generateCarPointer() {



    // Creación de sistemas y componentes

    //lubricación
    std::vector<Component> lubricationSystemComponents = {
        Component("Bomba de aceite"),
         Component("Filtro de aceite")
    };

    //motor
    std::vector<Component> engineSystemComponents = {
        Component("Bloque"),
        Component("Culata"),
        Component("Arbol de levas"),
        Component("Cadena de tiempo"),
        Component("Pistón 1"),
        Component("Pistón 2"),
        Component("Pistón 3"),
        Component("Pistón 4"),
        Component("Pistón 5"),
        Component("Pistón 6"),
        Component("Admisión"),
        Component("Escape")
    };

    //transmisión
    std::vector<Component> transmisionSystemComponents = {
        Component("Eje de transmisión"),
        Component("Diferencial")
    };

    //dirección
    std::vector<Component> steeringSystemComponents = {
            Component("Caja de dirección"),
            Component("Brazo de dirección izquierdo"),
            Component("Brazo de dirección derecho"),
            Component("Barra estabilizadora")
    };

    //combustión
    std::vector<Component> fuelSystemComponents = {
            Component("Inyector 1"),
            Component("Inyector 2"),
            Component("Inyector 3"),
            Component("Inyector 4"),
            Component("Inyector 5"),
            Component("Inyector 6"),
            Component("Bomba de combustible")
    };

    //suspensión
    std::vector<Component> suspensionSystemComponents = {
                Component("Amortiguador 1"),
                Component("Amortiguador 2"),
                Component("Amortiguador 3"),
                Component("Amortiguador 4")
    };

    //vector de sistemas
    std::vector<System> systems = {
                System("Sistema de Lubricación", lubricationSystemComponents),
                System("Sistema de Motor", engineSystemComponents),
                System("Sistema de transmisión", transmisionSystemComponents),
                System("Sistema de Dirección", steeringSystemComponents),
                System("Sistema de combustible", fuelSystemComponents),
                System("Sistema de suspensión", suspensionSystemComponents)
    };

    //creación del auto
    //!Cambiar para agregarle un id
    Car* car = new Car("Explorer", actualCarID++, systems);

    return car;

}

/**
 * @brief Generates a car with no failures.
 *
 * @return A car with no failures.
*/
Car generateCar() {


    // Creación de sistemas y componentes

    //lubricación
    std::vector<Component> lubricationSystemComponents = {
        Component("Bomba de aceite"),
         Component("Filtro de aceite")
    };

    //motor
    std::vector<Component> engineSystemComponents = {
        Component("Bloque"),
        Component("Culata"),
        Component("Arbol de levas"),
        Component("Cadena de tiempo"),
        Component("Pistón 1"),
        Component("Pistón 2"),
        Component("Pistón 3"),
        Component("Pistón 4"),
        Component("Pistón 5"),
        Component("Pistón 6"),
        Component("Admisión"),
        Component("Escape")
    };

    //transmisión
    std::vector<Component> transmisionSystemComponents = {
        Component("Eje de transmisión"),
        Component("Diferencial")
    };

    //dirección
    std::vector<Component> steeringSystemComponents = {
            Component("Caja de dirección"),
            Component("Brazo de dirección izquierdo"),
            Component("Brazo de dirección derecho"),
            Component("Barra estabilizadora")
    };

    //combustión
    std::vector<Component> fuelSystemComponents = {
            Component("Inyector 1"),
            Component("Inyector 2"),
            Component("Inyector 3"),
            Component("Inyector 4"),
            Component("Inyector 5"),
            Component("Inyector 6"),
            Component("Bomba de combustible")
    };

    //suspensión
    std::vector<Component> suspensionSystemComponents = {
                Component("Amortiguador 1"),
                Component("Amortiguador 2"),
                Component("Amortiguador 3"),
                Component("Amortiguador 4")
    };

    //vector de sistemas
    std::vector<System> systems = {
                System("Sistema de Lubricación", lubricationSystemComponents),
                System("Sistema de Motor", engineSystemComponents),
                System("Sistema de transmisión", transmisionSystemComponents),
                System("Sistema de Dirección", steeringSystemComponents),
                System("Sistema de combustible", fuelSystemComponents),
                System("Sistema de suspensión", suspensionSystemComponents)
    };

    //creación del auto
    //!Cambiar para agregarle un id
    Car car = Car("Explorer", actualCarID++, systems);

    return car;
};