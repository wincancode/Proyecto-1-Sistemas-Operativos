#include "./Workshop/Workshop.hpp"
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

//!!! SHOULD BE 1
#define UPDATES_PER_SECOND 1
#define MAX_WIDTH 140
#define MAX_HEIGHT 20

#define DISPLAY_WIDTH 90
#define DISPLAY_HEIGHT 20

#define MENU_WIDTH 35
#define MENU_HEIGHT 20

#define DATA_WIDTH 50
#define DATA_HEIGHT 20

#define MAX_CARS_IN_INCOMING_QUEUE 14

#define STATIONS_COLUMNS 2
#define STATIONS_ROWS 3

// headers
void initGraphics();
void endGraphics();
void update();
void drawMenu();
void drawMargins();
void manageInput(int ch);
void goToWindow();
void drawPrincipalDisplay();
void InformacionEstacion(int id);
void CrearAutoFallaAleatoria();
void wdrawrectangle(WINDOW* win, int x, int y, int width, int height);
void drawDisplay();
void drawData();

std::vector<std::string> actualMenuOptions_Y = std::vector<std::string>();
std::vector<std::string> ActualMenuOptions_X = std::vector<std::string>();

std::vector<std::string> stationsSelection = {
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "salir",

};

std::vector<std::string> menuPrincipalOptions = {
    "1. Agregar autos",
    "2. Revisar estaciones",
    "3. Revisar el historial global",
    "4. Parar todo", // Colocar para reanudar
    "5. Salir" };

std::vector<std::string> menuIngresarAutoOptions = {
    "1. Agregar autos",
    "2. Agregar auto aleatorio",
    "3. Regresar",
};

std::vector<std::string> menuCrearAutoOptions = {
    "1. Agregar auto personalizado",
    "2. Agregar auto aleatorio",
    "3. Regresar",
};

enum Pantallas
{
    MENU_PRINCIPAL,
    INGRESO_AUTO,
    REVISION_ESTACIONES,
    REVISION_COLA_ENTRADA,
    REVISION_COLA_SALIDA,
    REVISION_HISTORIAL,
    PARAR_TODO,
    SALIR
};

Pantallas pantallaActual;

int menuSelection_Y = 0;
int menuSelection_X = 0;

int timer = 0;

WINDOW* menuWin = NULL;
WINDOW* displayWin = NULL;
WINDOW* dataWin = NULL;

void initGraphics()
{
    initscr();

    actualMenuOptions_Y = menuPrincipalOptions;
    pantallaActual = MENU_PRINCIPAL;

    // create a window
    menuWin = newwin(MENU_HEIGHT, MENU_WIDTH, 0, 0);
    displayWin = newwin(DISPLAY_HEIGHT, DISPLAY_WIDTH, 0, MENU_WIDTH);
    dataWin = newwin(DATA_HEIGHT, DATA_WIDTH, 0, DISPLAY_WIDTH + MENU_WIDTH);

    // Set the cursor to invisible
    curs_set(0);
    // Set the input to non-echoing
    noecho();
    // Set the input to non-buffering
    cbreak();
    // Enable the keypad
    keypad(menuWin, TRUE);
    // refresh the screen
    refresh();
    // Draw a box as the margin
    drawMargins();

    // make a thread to update the display
    std::thread displayThread(drawDisplay);
    std::thread dataThread(drawData);

    update();
}

void endGraphics()
{
    endwin();
}

void update()
{
    while (true)
    {

        wclear(menuWin);
        // Draw a box as the margin
        drawMargins();

        // refresh the screen
        drawMenu();

        // Get the input
        int ch = wgetch(menuWin);

        // manage the input
        manageInput(ch);
    }
}

void CrearAutoFallaAleatoria()
{
    Car* car = generateCarPointer();
    car->fail();
    incomingCars.push(car);
}


void selectFailure() {



}

void goToWindow()
{
    if (pantallaActual == MENU_PRINCIPAL)
    {
        Car car;
        switch (menuSelection_Y)
        {
        case 0:
            actualMenuOptions_Y = menuIngresarAutoOptions;

            // codigo de auto aleatorio
            pantallaActual = INGRESO_AUTO;


            break;
        case 1:
            pantallaActual = REVISION_ESTACIONES;
            actualMenuOptions_Y = stationsSelection;
            break;
        case 2:
            pantallaActual = REVISION_HISTORIAL;
            break;
        case 3:
            pantallaActual = PARAR_TODO;
            break;
        case 4:
            endwin();
            exit(0);
            break;
        default:
            break;
        }
    }

    if (pantallaActual == INGRESO_AUTO)
    {
        switch (menuSelection_Y)
        {
        case 0:
            selectFailure();
            break;
        case 1:
            CrearAutoFallaAleatoria();
            break;
        case 2:
            actualMenuOptions_Y = menuPrincipalOptions;
            pantallaActual = MENU_PRINCIPAL;
            break;
        default:
            break;
        }
    }

    if (pantallaActual == REVISION_ESTACIONES)
    {
        switch (menuSelection_Y)
        {

        case 8:
            actualMenuOptions_Y = menuPrincipalOptions;
            pantallaActual = MENU_PRINCIPAL;
            break;
        default:
            break;
        }
    }
}

void manageInput(int ch)
{
    switch (ch)
    {
    case KEY_DOWN:
        if (menuSelection_Y < actualMenuOptions_Y.size() - 1)
            menuSelection_Y++;
        break;
    case KEY_UP:
        if (menuSelection_Y > 0)
            menuSelection_Y--;
        break;
    case KEY_ENTER:

    case '\n':

        goToWindow();
        break;

    default:
        break;
    }
}

void drawMenu()
{

    // Draw the menu options
    for (int i = 0; i < actualMenuOptions_Y.size(); i++)
    {
        if (i == menuSelection_Y)
        {
            wattron(menuWin, A_REVERSE);
        }
        mvwprintw(menuWin, i + 1, 1, "%s", actualMenuOptions_Y[i].c_str());
        wattroff(menuWin, A_REVERSE);
    }

    wrefresh(menuWin);
}

void drawMargins()
{
    // Draw a box as the margin
    box(menuWin, 0, 0);
    box(displayWin, 0, 0);

    box(dataWin, 0, 0);

    wrefresh(menuWin);
    wrefresh(displayWin);
    wrefresh(dataWin);
}

void drawData()
{

    while (true)
    {
        wclear(dataWin);

        switch (pantallaActual)
        {
        case MENU_PRINCIPAL:
            mvwprintw(dataWin, 1, 1, "Menu principal");
            break;
        case INGRESO_AUTO:
            mvwprintw(dataWin, 1, 1, "Ingreso de autos");
            break;
        case REVISION_ESTACIONES:
            if (menuSelection_Y < MAX_STATIONS)
                InformacionEstacion(menuSelection_Y);
            break;
        case PARAR_TODO:
            mvwprintw(dataWin, 1, 1, "Parar todo");
            break;
        case SALIR:
            mvwprintw(dataWin, 1, 1, "Salir");
            break;
        default:
            break;
        }

        drawMargins();

        wrefresh(dataWin);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / UPDATES_PER_SECOND));
    }
}

void drawDisplay()
{
    while (true)
    {
        wclear(displayWin);

        drawPrincipalDisplay();
        drawMargins();

        wrefresh(displayWin);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / UPDATES_PER_SECOND));
    }
}

void InformacionEstacion(int id)
{

    Itinerary* currentItinerary = stations.at(id)->getCurrentItinerary();
    wclear(dataWin);
    mvwprintw(dataWin, 1, 1, "Revision de estacion %i", id + 1);
    if (currentItinerary != nullptr)
    {
        int elapsedRepairTime = stations.at(id)->getRepairTimeCounter();
        int totalRepairTime = stations.at(id)->getRepairTime();
        int repairedFailures = currentItinerary->getComponentsRepaired()->at(id).size() + 1;
        int toRepairFailures = currentItinerary->getComponentsToRepair()->at(id).size() + 1;

        mvwprintw(dataWin, 2, 1, "Carro id: %i", currentItinerary->getCar()->getId());
        mvwprintw(dataWin, 3, 1, "Falla %i/%i", repairedFailures, toRepairFailures);
        mvwprintw(dataWin, 4, 1, "Tiempo total: %i", totalRepairTime);
        mvwprintw(dataWin, 4, 1, "Tiempo restante: %i", totalRepairTime - elapsedRepairTime);

        return;
    }

    mvwprintw(dataWin, 2, 1, "Desocupado");
}

void wdrawrectangle(WINDOW* win, int x, int y, int width, int height)
{
    for (int i = 0; i < width; i++)
    {
        mvwprintw(win, y, x + i, "-");
        mvwprintw(win, y + height, x + i, "-");
    }

    for (int i = 0; i < height; i++)
    {
        mvwprintw(win, y + i, x, "|");
        mvwprintw(win, y + i, x + width, "|");
    }

    mvwprintw(win, y, x, "+");
    mvwprintw(win, y, x + width, "+");
    mvwprintw(win, y + height, x, "+");
    mvwprintw(win, y + height, x + width, "+");
}

void drawPrincipalDisplay()
{
    // write incomingCars queue

    mvwprintw(displayWin, 1, 1, "Cola de entrada: ");

    std::queue<Car*> incomingCarsToDraw = incomingCars;
    int i = 2;

    while (!incomingCarsToDraw.empty())
    {
        if (i > MAX_CARS_IN_INCOMING_QUEUE - 2)
            break;
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE - i, 1, "id: %i", incomingCarsToDraw.front()->getId());
        incomingCarsToDraw.pop();
        i++;
    };

    // write diagnostic

    wdrawrectangle(displayWin, 1, MAX_CARS_IN_INCOMING_QUEUE - 1, 14, 5);
    mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE, 2, "Diagnostico");

    // write actual diagnostic

    if (diagnose_currentCar != nullptr)
    {

        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 1, 2, "Auto: %i", diagnose_currentCar->getId());
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 2, 2, "Falla %i/%i", diagnose_failuresDiagnosed, diagnose_totalFailures);
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 3, 2, "Tiempo: %i", diagnose_diagnoseTime - diagnoseTimeCounter);
    }
    else
    {
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 1, 2, "Auto: ninguno");
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 2, 2, "Falla %i/%i", 0, 0);
        mvwprintw(displayWin, MAX_CARS_IN_INCOMING_QUEUE + 3, 2, "Tiempo: %i", 0);
    }
    // draw stations tittle
    mvwprintw(displayWin, 1, 35, "Estaciones: ");

    // draw stations
    for (i = 0; i < MAX_STATIONS / 2; i++)
    {

        wdrawrectangle(displayWin, 22, (i * 4) + 3, 25, 3);
        Itinerary* currentItinerary = stations.at(i)->getCurrentItinerary();
        if (currentItinerary != nullptr)
        {
            int elapsedRepairTime = stations.at(i)->getRepairTimeCounter();
            int totalRepairTime = stations.at(i)->getRepairTime();
            int repairedFailures = currentItinerary->getComponentsRepaired()->at(i).size() + 1;
            int toRepairFailures = currentItinerary->getComponentsToRepair()->at(i).size() + 1;

            int totalFailures = repairedFailures + toRepairFailures - 1;

            mvwprintw(displayWin, (i * 4) + 4, 23, "Estacion %i: auto %i", i + 1, currentItinerary->getCar()->getId());
            mvwprintw(displayWin, (i * 4) + 5, 23, "falla %i/%i, tiempo: %i", repairedFailures, totalFailures, totalRepairTime - elapsedRepairTime);
        }
        else
        {
            mvwprintw(displayWin, (i * 4) + 4, 23, "Estacion %i: desocupada", i + 1);
        }
    }

    for (i = 0; i < MAX_STATIONS / 2; i++)
    {
        wdrawrectangle(displayWin, 49, (i * 4) + 3, 25, 3);
        Itinerary* currentItinerary = stations.at(i + MAX_STATIONS / 2)->getCurrentItinerary();
        if (currentItinerary != nullptr)
        {
            int elapsedRepairTime = stations.at(i + MAX_STATIONS / 2)->getRepairTimeCounter();
            int totalRepairTime = stations.at(i + MAX_STATIONS / 2)->getRepairTime();
            int repairedFailures = currentItinerary->getComponentsRepaired()->at(i + MAX_STATIONS / 2).size() + 1;
            int toRepairFailures = currentItinerary->getComponentsToRepair()->at(i + MAX_STATIONS / 2).size() + 1;

            int totalFailures = repairedFailures + toRepairFailures - 1;

            mvwprintw(displayWin, (i * 4) + 4, 50, "Estacion %i: auto %i", i + 1 + MAX_STATIONS / 2, currentItinerary->getCar()->getId());
            mvwprintw(displayWin, (i * 4) + 5, 50, "falla %i/%i, tiempo: %i", repairedFailures, totalFailures, totalRepairTime - elapsedRepairTime);
        }
        else
        {
            mvwprintw(displayWin, (i * 4) + 4, 50, "Estacion %i: desocupada", i + 1 + MAX_STATIONS / 2);
        }
    }
}
