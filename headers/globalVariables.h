#include "vector.h"

#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

// Dimensiones de la imagen
int IMAGE_WIDTH = 800;
int IMAGE_HEIGHT = 600;

float EPSILON = 0.001f;

int PROFUNDIDAD_MAXIMA = 5;

float DEFAULT_AMBIENT_COEFFICIENT = 0.1f;
float DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT = 0.7f;
float DEFAULT_SPECULAR_REFLECTION_COEFFICIENT = 0.2f;
int DEFAULT_SPECULAR_BRIGHTNESS = 10;
Vector DEFAULT_TRANSPARENCY_COEFFICIENT(0.0f, 0.0f, 0.0f);
float DEFAULT_REFRACTION_INDEX = 1.0f;
Vector DEFAULT_REFLECTION_COEFFICIENT(0.0f, 0.0f, 0.0f);

float CAMERA_REFRACTION_INDEX = 1.0f; // Índice de refracción del medio de la cámara

int test = 0; // Variable de prueba para depuración

#endif // GLOBAL_VARIABLES_H