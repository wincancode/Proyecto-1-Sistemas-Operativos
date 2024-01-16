#include "FileManager.hpp"

bool ExistenciaFichero(std::string nombreFichero)
{
    std::ifstream archivo(nombreFichero.c_str());
    return archivo.good();
}

void WriteDataWorkStation()
{
}

void CrearHistorial(Itinerary *itinerario)
{

    std::ofstream archivo(WORSTATION_DATA, std::ios::app);

    archivo << "Identificador de itinerario: " << itinerario->getId() << std::endl;
    archivo << "Identificador del carro" << itinerario->getCar()->getId() << std::endl;

    archivo << "Componentes Reparados: " << std::endl;
    for (int i = 0; i < itinerario->getComponentsRepaired()->size(); i++)
    {
        archivo << "Estacion: " << i << std::endl;
        for (int j = 0; j < itinerario->getComponentsRepaired()->at(i).size(); j++)
        {
            archivo << "  Componente: " << itinerario->getComponentsRepaired()->at(i).at(j)->getName() << "  Falla: " <<itinerario->getComponentsRepaired()->at(i)[j]->getFailures()[0]->getDescription()<< std::endl;
        }
    }
    archivo << std::endl;
    archivo.close();
}

void WriteCarHistory()
{
    // Recibir el objeto itinerario listo
}

void ReadData()
{
    FILE *arch;
    arch = fopen(WORSTATION_DATA, "rb");
    if (arch == NULL)
        exit(1);
    for (int i = 0; i < MAX_STATIONS; i++)
    {
        fread(&stations[i], sizeof(stations[i]), 1, arch);
    }
    fclose(arch);
}