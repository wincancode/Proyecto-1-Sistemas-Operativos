// definiciones y importes
#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include "./Workshop/Workshop.hpp"
#include "./Vehicle/Car.hpp"
#define NAME_FILE "data/constants.dat"
#define WORSTATION_DATA "workstation.dat"

bool ExistenciaFichero();
void CrearHistorial(Itinerary *itinerario);
void ReadData();

void ReadData();

// Historial de reparaciones Terminadas
//  - Vehiculo
//  - Estaciones
