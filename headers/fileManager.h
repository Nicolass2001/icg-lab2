#include <filesystem>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

namespace fs = std::filesystem;

std::string obtenerFechaHoraActual()
{
    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
#ifdef _WIN32
    localtime_s(&localTime, &timeNow); // Windows
#else
    localtime_r(&timeNow, &localTime); // Linux/macOS
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%d-%m_%H-%M-%S");
    return oss.str();
}

std::pair<std::string, std::string> splitOnce(const std::string &str, char delimiter)
{
    size_t pos = str.find(delimiter);
    if (pos != std::string::npos)
    {
        std::string first = str.substr(0, pos);
        std::string rest = str.substr(pos + 1);
        return {first, rest};
    }
    return {str, ""};
}

std::string getPathToFile()
{
    // AYUDADO POR EL CHAT
    std::string dirPath = "./historial/";
    std::string timestamp = obtenerFechaHoraActual();

    char delimitador = '_';
    auto partes = splitOnce(timestamp, delimitador);

    dirPath = dirPath + partes.first;

    if (!fs::exists(fs::absolute(dirPath)))
    {
        std::cout << "Directorio no existe. Creando...\n";
        if (fs::create_directories(fs::absolute(dirPath)))
        {
            std::cout << "Directorio creado con exito.\n";
        }
        else
        {
            std::cerr << "Error al crear el directorio.\n";
        }
    }

    std::string file = dirPath + "/" + partes.second + ".png";

    return file;
}

#endif // FILE_MANAGER_H