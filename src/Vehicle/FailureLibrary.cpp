#include "FailureLibrary.hpp"
#include "Car.hpp"
#include <random>

//Lubricacion
std::vector<Failure> BombaDeAceite_Failures = {
    Failure(1, 0, "falla Bomba de aceite 1", "Falta de presión de aceite en el circuito", 15),
    Failure(2, 0, "falla Bomba de aceite 2", "Fuga en la bomba de aceite", 15),
};

std::vector<Failure> FiltroDeAciete_Failures = {
    Failure(3, 0, "falla Filtro de aceite 1", "El filtro de aceite requiere reemplazo", 8),
};

//motor
std::vector<Failure> Bloque_Failures = {
    Failure(5, 1, "falla Bloque 1", "Se ha filtrado el aceite en la cámara de combustión", 20),
    Failure(6, 1, "falla Bloque 2", "El bloque posee una fisura", 25),
};

std::vector<Failure> Culata_Failures = {
    Failure(7, 1, "falla Culata 1", "Culata del motor agrietada", 10),
    Failure(8, 1, "falla Culata 2", "Fuga de refrigerante, avería por sobrecalentamiento", 10),
};

std::vector<Failure> Piston_Failures = {
    Failure(9, 2, "falla Piston 1", "Pistón dañado por mala reprogramación de la computadora", 15),
    Failure(10, 2, "falla Piston 2", "Pistón requiere reemplazo", 15),
};

std::vector<Failure> ArbolDeLevas_Failures = {
    Failure(11, 2, "falla Arbol de levas 1", "Se ha gripado el arbol de levas", 25),
};

std::vector<Failure> CadenaDeTiempo_Failures = {
    Failure(13, 3, "falla Cadena de tiempo 1", "Se ha roto la cadena de tiempo", 15),
    Failure(14, 3, "falla Cadena de tiempo 2", "El motor está fuera de tiempo", 15),
};

std::vector<Failure> Admision_Failures = {
    Failure(15, 3, "falla Admision 1", "Fuga de aire", 5),
    Failure(16, 3, "falla Admision 2", "Suciedad acumulada en el sistema", 10),
};

std::vector<Failure> Escape_Failures = {
    Failure(17, 4, "falla Escape 1", "Acumulación de óxido", 10),
    Failure(18, 4, "falla Escape 2", "Fuga de escape", 10),
};

//transmision
std::vector<Failure> EjeDeTransmision_Failures = {
    Failure(19, 4, "falla Eje de transmision 1", "Ruido anormal en el eje de transmisión", 15),
    Failure(20, 4, "falla Eje de transmision 2", "Falta de lubricación en los componentes", 8),
};

std::vector<Failure> Diferencial_Failures = {
    Failure(21, 5, "falla Diferencial 1", "Rotura de dientes del diferencial", 15),
    Failure(22, 5, "falla Diferencial 2", "Ruido anormal en el diferencial", 10),
};

//direccion
std::vector<Failure> CajaDeDireccion_Failures = {
    Failure(23, 5, "falla Caja de direccion 1", "Fuga de aceite en la caja de dirección", 20),
    Failure(24, 5, "falla Caja de direccion 2", "Dirección errante", 20),
};

std::vector<Failure> BrazoDeDireccion_Failures = {
    Failure(25, 6, "falla Brazo de direccion 1", "Juego en la direccion", 10),
    Failure(26, 6, "falla Brazo de direccion 2", "Ruido en el brazo de dirección", 15),
};

std::vector<Failure> BarraEstabilizadora_Failures = {
    Failure(27, 6, "falla Barra estabilizadora 1", "Bieletas sueltas", 10),
    Failure(28, 6, "falla Barra estabilizadora 2", "Soportes de la barra estabilizadora dañados", 10),
};

//combustion
std::vector<Failure> Inyector_Failures = {
    Failure(29, 7, "falla Inyector 1", "Inyectores requieren limpieza", 12),
    Failure(30, 7, "falla Inyector 2", "Inyectores en mal estado", 10),
};

std::vector<Failure> BombaDeCombustible_Failures = {
    Failure(31, 7, "falla Bomba de combustible 1", "Falla en la pila de la bomba de combustible", 5),
    Failure(32, 7, "falla Bomba de combustible 2", "Bomba dañada por octanaje incorrecto", 8),
};

//Suspension
std::vector<Failure> Suspension_Component = {
    Failure(33, 8, "falla Suspension 1", "Amortiguadores dañados", 15),
    Failure(34, 8, "falla Suspension 2", "Resortes dañados", 15),
};

//Component names
std::vector<Failure> getFailuresByComponent(std::string componentName) {
    if (componentName == "Bomba de aceite") return BombaDeAceite_Failures;
    if (componentName == "Filtro de aceite") return FiltroDeAciete_Failures;
    if (componentName == "Bloque") return Bloque_Failures;
    if (componentName == "Culata") return Culata_Failures;
    if (componentName.find("Pistón") != std::string::npos) return Piston_Failures;
    if (componentName == "Arbol de levas") return ArbolDeLevas_Failures;
    if (componentName == "Cadena de tiempo") return CadenaDeTiempo_Failures;
    if (componentName == "Admisión") return Admision_Failures;
    if (componentName == "Escape") return Escape_Failures;
    if (componentName == "Eje de transmisión") return EjeDeTransmision_Failures;
    if (componentName == "Diferencial") return Diferencial_Failures;
    if (componentName == "Caja de dirección") return CajaDeDireccion_Failures;
    if (componentName.find("Brazo de dirección") != std::string::npos) return BrazoDeDireccion_Failures;
    if (componentName == "Barra estabilizadora") return BarraEstabilizadora_Failures;
    if (componentName.find("Inyector") != std::string::npos) return Inyector_Failures;
    if (componentName == "Bomba de combustible") return BombaDeCombustible_Failures;
    if (componentName.find("Amortiguador") != std::string::npos) return Suspension_Component;

    // Return an empty vector if the component name is not found
    return std::vector<Failure>();
}
