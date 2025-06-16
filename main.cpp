#include <iostream>
#include <FreeImage.h>
#include "escena.h"
#include "fileManager.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Color_RR traza_RR(Rayo_RR rayo, int profundidad);

Color_RR traza_RR_coeficientes(Rayo_RR rayo, int profundidad, int eleccion);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad)
{
    Color_RR color;
    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());

    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);

    if (profundidad < PROFUNDIDAD_MAXIMA)
    {
        // Calcular Reflexion
        if (objeto->getCoeficienteReflexion() > 0.0f)
        {
            Vector direccionReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            Rayo_RR rayoReflexion(punto + normal * EPSILON, direccionReflexion.normalize(), rayo.getIndiceRefraccion());
            Color_RR colorReflexion = traza_RR(rayoReflexion, profundidad + 1);
            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
        }
        // Calcular Refraccion
        if (objeto->getCoeficienteTransparencia() > 0.0f)
        {
            Vector puntoRefraccion;
            float indiceRefraccionIncidente = rayo.getIndiceRefraccion();

            // Si estoy dentro de la esfera, entonces corro el punto hacia afuera,
            // no hacia adentro
            if (indiceRefraccionIncidente == 1.5f) {
                puntoRefraccion = punto + normal * EPSILON;
            }
            else {
                puntoRefraccion = punto - normal * EPSILON;
            }

            float indiceRefraccionTransmitido = escena.indiceRefraccion(puntoRefraccion);
            float indiceRefraccionRelativo = indiceRefraccionIncidente / indiceRefraccionTransmitido;
            Vector direccionRefraccion;
            Color_RR colorRefraccion(0, 0, 0);

            float productoPuntoRayoNormal = rayo.getDireccion().dot(normal);

            float segundoTerminoRaiz = 1 - ((1 - powf(productoPuntoRayoNormal, 2.0f)) * (powf(indiceRefraccionRelativo, 2.0f)));
            
            if (segundoTerminoRaiz < 0.0f) {
                // Reflexion interna total
                int i = 0;
            }
            else {
                Vector primerTermino = (rayo.getDireccion() - normal * productoPuntoRayoNormal) * (indiceRefraccionRelativo);
                Vector segundoTermino = (normal * sqrt(segundoTerminoRaiz)) * (- 1.0f);

                direccionRefraccion = primerTermino + segundoTermino;
                
                /*
                std::cout << "Indice de refraccion de origen del rayo: " << indiceRefraccionIncidente << std::endl;
                std::cout << "Indice de refraccion de punto de corte: " << indiceRefraccionTransmitido << std::endl;
                std::cout << "Punto de corte (con epsilon): " << puntoRefraccion << std::endl;
                std::cout << "Punto direccion calculado: " << direccionRefraccion << std::endl;
                std::cout << "Coeficiente de refraccion del objeto: " << objeto->getCoeficienteTransparencia() << std::endl;
                std::cout << "Profundidad traza: " << profundidad << std::endl;
                std::cout << std::endl;
                */

                Rayo_RR rayoRefraccion(puntoRefraccion, direccionRefraccion.normalize(), indiceRefraccionTransmitido);
                colorRefraccion = traza_RR(rayoRefraccion, profundidad + 1);

            }

            /*
            if (indiceRefraccionRelativo == 1)
            {
                direccionRefraccion = rayo.getDireccion();
                if (test == 0)
                {
                    std::cout << "Indice de refracción incidente: " << indiceRefraccionIncidente << std::endl;
                    std::cout << "direccionRefraccion: " << direccionRefraccion << std::endl;
                }
            }
            else
            {
                std::cout << "Indice de refracción incidente: " << indiceRefraccionIncidente << std::endl;
                // Ley de Snell
                float cosenoIncidencia = -rayo.getDireccion().dot(normal);
                float senoCuadradoRefraccion = indiceRefraccionRelativo * indiceRefraccionRelativo * (1.0f - cosenoIncidencia * cosenoIncidencia);
                if (senoCuadradoRefraccion > 1.0f)
                {
                    // Total internal reflection
                    color.setComponenteTransparencia(ColorRGB(0, 0, 0), 0.0f);
                    return color;
                }
                float cosenoRefraccion = sqrt(1.0f - senoCuadradoRefraccion);
                direccionRefraccion = rayo.getDireccion() * indiceRefraccionRelativo + normal * (indiceRefraccionRelativo * cosenoIncidencia - cosenoRefraccion);
            }
            Rayo_RR rayoRefraccion(puntoRefraccion, direccionRefraccion.normalize(), indiceRefraccionTransmitido);
            Color_RR colorRefraccion = traza_RR(rayoRefraccion, profundidad + 1);
            if (test == 0)
            {
                std::cout << "colorRefraccion: " << colorRefraccion << std::endl;
            }
            test = 1;
            */
            color.setComponenteTransparencia(colorRefraccion.getColorTotal(), objeto->getCoeficienteTransparencia());
        }
    }
    return color;
}

Color_RR traza_RR(Rayo_RR rayo, int profundidad)
{
    Vector interseccion;
    Vector normal;
    ObjetoPtr objMasCercano = escena.calcularInterseccionMasCercana(rayo, &interseccion, &normal);
    if (objMasCercano == nullptr)
    {
        return escena.getFondo();
    }

    return sombra_RR(objMasCercano, rayo, interseccion, normal, profundidad);
}

Color_RR traza_RR_coeficientes(Rayo_RR rayo, int profundidad, int eleccion)
{
    Vector interseccion;
    Vector normal;
    ObjetoPtr objMasCercano = escena.calcularInterseccionMasCercana(rayo, &interseccion, &normal);
    if (objMasCercano == nullptr)
    {
        return escena.getFondo();
    }

    Color_RR color;
    ColorRGB colorFigura(255, 255, 255);

    switch (eleccion) {
    case 2:
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteReflexion());
        break;
    case 3:
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteTransparencia());
        break;
    case 4:
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteAmbiente());
        break;
    case 5:
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteReflexionDifusa());
        break;
    case 6:
        color.setComponenteAmbiente(colorFigura, objMasCercano->getCoeficienteReflexionEspecular());
        break;
    }

    return color;
}

int main(int argc, char* argv[])
{
    // SDL
    // PAGINA DE REFERENCIA: https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    uint8_t* rojo = 0;
    uint8_t* verde = 0;
    uint8_t* azul = 0;

    // Inicializar FreeImage
    FreeImage_Initialise();

    // Crear una imagen vacía de 24 bits
    FIBITMAP *bitmap = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    if (!bitmap)
    {
        std::cerr << "No se pudo crear la imagen." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }

    int eleccion;

    std::cout << "Elija que imagen desea generar: " << std::endl;
    std::cout << "1 - Escena | 2 - Coeficientes reflexion | 3 - Coeficientes refraccion" << std::endl;
    std::cout << "4 - Coeficientes ambiente | 5 - Coeficientes difuso | 6 - Coeficientes especular" << std::endl;
    std::cin >> eleccion;

    Camara_RR camara = escena.getCamara();
    Color_RR colorPixel;
    // Llenar la imagen utilizando la función `colorFunction`
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            if (eleccion != 1) {
                colorPixel = traza_RR_coeficientes(camara.generarRayo(x, y), 1, eleccion);
            }
            else {
                colorPixel = traza_RR(camara.generarRayo(x, y), 1);
            }
            // Convertir Color_RR a RGBQUAD
            RGBQUAD color = colorPixel.toRGBQUAD();
            FreeImage_SetPixelColor(bitmap, x, y, &color);

            // SDL
            rojo = &color.rgbRed;
            verde = &color.rgbGreen;
            azul = &color.rgbBlue;
            SDL_SetRenderDrawColor(renderer, *rojo, *verde, *azul, 255);
            SDL_RenderDrawPoint(renderer, x, 600 - y);
            if (x == 0)
                SDL_RenderPresent(renderer);

        }
    }

    // Guardar la imagen en un archivo
    std::string outputPath = getPathToFile();
    /*
    if (FreeImage_Save(FIF_PNG, bitmap, outputPath.c_str(), 0))
    {
        std::cout << "Imagen guardada en: " << outputPath << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar la imagen." << std::endl;
    }
    */

    // Liberar memoria y finalizar FreeImage
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
