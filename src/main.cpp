#include <iostream>
#include <unistd.h>
#include <ctime>

#include "Workshop/Workshop.hpp"
#include "Vehicle/FailureLibrary.hpp"
#include "Vehicle/Car.hpp"
#include "Graphics/Graphics.hpp"
#include "BaseDatos/FileManager.hpp"

#include <ncurses.h>

int main()
{

    //!!!!!! SIMEMPRE COLOCAR AL INICIO
    srand(time(0));

    std::thread graphics(initGraphics);
    std::thread system(startSystem);

    

    graphics.join();
    system.join();
    return 0;
}