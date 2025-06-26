#include "color_rgb.h"
#include "globalVariables.h"

#ifndef PROPIEDADES_COLOR_OBJETO_H
#define PROPIEDADES_COLOR_OBJETO_H

class PropiedadesObjeto
{
public:
	// Tipo objeto agregado, del 1 al 5 corresponden a las paredes fondo, izquierda,
	// superior, derecha e inferior respectivamente. Su identificador se carga
	// en el XML. El 6 corresponde a la esfera y el 7 al cilindro, cargados
	// estaticamente en xmlHandler.h. El 8 corresponde a la malla.
	// Se necesita el tipoObjeto para las texturas en la interseccion para saber
	// si se alcanzo alguna de las paredes, y en tal caso, setear su color segun
	// la coordenada de textura correspondiente.
	//
	// Predeterminado en 0 para (por ahora) no romper el constructor predeterminado.
	int tipoObjeto = 0;

	float coeficienteAmbiente;
	ColorRGB colorAmbiente;
	float coeficienteReflexionDifusa;
	ColorRGB colorReflexionDifusa;
	float coeficienteReflexionEspecular;
	int brilloEspecular;
	ColorRGB colorReflexionEspecular;
	float coeficienteTransparencia;
	float indiceRefraccion;
	float coeficienteReflexion;

	void setTipoObjeto(int tipo) {
		this->tipoObjeto = tipo;
	};

	PropiedadesObjeto()
		: coeficienteAmbiente(DEFAULT_AMBIENT_COEFFICIENT),
		  colorAmbiente(ColorRGB(0, 0, 0)),
		  coeficienteReflexionDifusa(DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT),
		  colorReflexionDifusa(ColorRGB(0, 0, 0)),
		  coeficienteReflexionEspecular(DEFAULT_SPECULAR_REFLECTION_COEFFICIENT),
		  brilloEspecular(DEFAULT_SPECULAR_BRIGHTNESS),
		  colorReflexionEspecular(ColorRGB(255, 255, 255)),
		  coeficienteTransparencia(DEFAULT_TRANSPARENCY_COEFFICIENT),
		  indiceRefraccion(DEFAULT_REFRACTION_INDEX),
		  coeficienteReflexion(DEFAULT_REFLECTION_COEFFICIENT) {}

	PropiedadesObjeto(float coefAmbiente,
					  ColorRGB colorAmbiente,
					  float coefRefDifusa,
					  ColorRGB colorRefDifusa,
					  float coefRefEspecular,
					  int brilloEspecular,
					  ColorRGB colorRefEspecular,
					  float coefTransparencia,
					  float indiceRefraccion,
					  float coefReflexion)
		: coeficienteAmbiente(coefAmbiente),
		  colorAmbiente(colorAmbiente),
		  coeficienteReflexionDifusa(coefRefDifusa),
		  colorReflexionDifusa(colorRefDifusa),
		  coeficienteReflexionEspecular(coefRefEspecular),
		  brilloEspecular(brilloEspecular),
		  colorReflexionEspecular(colorRefEspecular),
		  coeficienteTransparencia(coefTransparencia),
		  indiceRefraccion(indiceRefraccion),
		  coeficienteReflexion(coefReflexion) {}
	PropiedadesObjeto(const ColorRGB &color)
		: coeficienteAmbiente(DEFAULT_AMBIENT_COEFFICIENT),
		  colorAmbiente(color),
		  coeficienteReflexionDifusa(DEFAULT_DIFFUSE_REFLECTION_COEFFICIENT),
		  colorReflexionDifusa(color),
		  coeficienteReflexionEspecular(DEFAULT_SPECULAR_REFLECTION_COEFFICIENT),
		  brilloEspecular(DEFAULT_SPECULAR_BRIGHTNESS),
		  colorReflexionEspecular(ColorRGB(255, 255, 255)),
		  coeficienteTransparencia(DEFAULT_TRANSPARENCY_COEFFICIENT),
		  indiceRefraccion(DEFAULT_REFRACTION_INDEX),
		  coeficienteReflexion(DEFAULT_REFLECTION_COEFFICIENT) {}
};

#endif // PROPIEDADES_COLOR_OBJETO_H