#include <iostream>
#include <FreeImage.h>
#include "fileManager.h"
#include "texturas.h"
#include "escena.h"
#include <SDL.h>

Escena_RR escena("config.xml");

Texturas texturas(3);

Color_RR traza_RR(Rayo_RR rayo, int profundidad);

Color_RR sombra_RR(ObjetoPtr objeto, Rayo_RR rayo, Vector punto, Vector normal, int profundidad)
{
    Color_RR color; // Color a devolver

    // OPCIONAL TEXTURAS
    if (TEXTURAS_ACTIVADAS)
    {
        if (objeto->getTipoObjeto() == 4)
            objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
        else if (objeto->getTipoObjeto() == 2)
            objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
        else if (objeto->getTipoObjeto() == 5)
            objeto->setColoresTextura(punto, texturas.texturasCargadas[1]);
        else if (objeto->getTipoObjeto() == 3)
            objeto->setColoresTextura(punto, texturas.texturasCargadas[2]);
        else if (objeto->getTipoObjeto() == 1)
            objeto->setColoresTextura(punto, texturas.texturasCargadas[0]);
    }

    // Color = termino de ambiente
    color.setComponenteAmbiente(objeto->getColorAmbiente(), objeto->getCoeficienteAmbiente());

    // for (cada luz)
    // rayos = rayo desde punto a la luz;
    // if (el producto punto de normal y la dirección a la luz es positivo) {
    // calcular cuánta luz es bloqueada por superficie opacas y transparentes, y usarla
    // para escalar los términos difusos y especulares antes de añadirlos a color;

    escena.calcularColorIluminacion(objeto, rayo, punto, normal, color);

    if (profundidad < PROFUNDIDAD_MAXIMA)
    {

        // If objeto es reflejante
        if (objeto->getCoeficienteReflexion() > 0.0f)
        {

            Vector dirReflexion = rayo.getDireccion() - normal * 2.0f * (rayo.getDireccion().dot(normal));
            dirReflexion = dirReflexion.normalize();

            Rayo_RR rayo_r(punto + normal * EPSILON, dirReflexion, rayo.getIndiceRefraccion());

            Color_RR colorReflexion;

            colorReflexion = traza_RR(rayo_r, profundidad + 1);

            color.setComponenteReflexion(colorReflexion.getColorTotal(), objeto->getCoeficienteReflexion());
        }

        // If objeto es transparente

        // Si el indice de refraccion es distinto de 0, el objeto es transparente
        if (objeto->getIndiceRefraccion() > 0.0f)
        {
            // rayo en la direccion de refraccion

            float n1 = 1;                             // indice de refraccion del medio
            float n2 = objeto->getIndiceRefraccion(); // ind de refracc del objeto, en este caso vidrio 1.5

            bool entrando = (normal.dot(rayo.getDireccion()) < 0.0f);

            if (!entrando)
            { // Rayo saliente
                std::swap(n1, n2);
                normal = normal * -1.0f;
            }
            // La ley de Snell me dice que: sen(o1)/sen(o2) = n2/n1
            float n = n1 / n2;

            // DEL LIBRO: El vector de refraccion es: T = sen(o2)*M - cos(o1)*N
            // Con M un vector unidad perpendicular a N en el plano del rayo incidente T
            // SIGO LOS PASOS DEL LIBRO PARA CALCULAR T, que lo simplifica a algo asi:
            // T = (η(N.I) − sqrt[1 − η^2 (1 − (N.I)^2)]) * N - η*I

            Vector I = rayo.getDireccion().normalize() * (-1.0f);

            // calculo: (N.I)
            float expr1 = normal.dot(I);

            // calculo: η^2 (1 − (N.I)^2)
            float expr2 = n * n * (1.0f - (expr1 * expr1));

            // En el libro dice que "...La reflexión interna total ocurre cuando la raíz
            // cuadrada en la ecuación (14.30) es imaginaria."

            if (expr2 > 1) // Reflexión interna total
                return color;

            // calculo: sqrt[1 − η^2 (1 − (N.I)^2)]
            float expr3 = sqrt(1.0f - expr2);

            // A esta altura tengo:
            // T = (η(N.I) − expr3) * N - η*I

            // Calculo η(N.I)
            float expr4 = (normal.dot(I)) * n;

            // Calculo (η(N.I) − expr3) * N
            Vector expr5 = normal * (expr4 - expr3);

            Vector T = expr5 - (I * n);
            T = T.normalize();

            Vector origenRefraccion = punto - normal * EPSILON;

            float indiceRefraccionTransmitido = escena.indiceRefraccion(origenRefraccion);
            Rayo_RR rayo_t(origenRefraccion, T, indiceRefraccionTransmitido);

            Color_RR colorRefraccion = traza_RR(rayo_t, profundidad + 1);
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

int main(int argc, char *argv[])
{
    // SDL
    // PAGINA DE REFERENCIA: https://stackoverflow.com/questions/20579658/how-to-draw-pixels-in-sdl-2-0

    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    uint8_t *rojo = 0;
    uint8_t *verde = 0;
    uint8_t *azul = 0;

    // Inicializo FreeImage en texturas.h

    // Crear una imagen vacía de 24 bits
    FIBITMAP *bitmap = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapCoeficienteReflexion = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapCoeficienteRefraccion = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapComponentesAmbiente = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapComponentesDifuso = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapComponentesEspecular = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapComponentesReflexion = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    FIBITMAP *bitmapComponentesRefraccion = FreeImage_Allocate(IMAGE_WIDTH, IMAGE_HEIGHT, 24);
    if (!bitmap || !bitmapCoeficienteReflexion || !bitmapCoeficienteRefraccion ||
        !bitmapComponentesAmbiente || !bitmapComponentesDifuso || !bitmapComponentesEspecular ||
        !bitmapComponentesReflexion || !bitmapComponentesRefraccion)
    {
        std::cerr << "No se pudo crear la imagen." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }

    Camara_RR camara = escena.getCamara();
    Color_RR colorPixel;
    // Llenar la imagen utilizando la función `colorFunction`
    for (int y = 0; y < IMAGE_HEIGHT; ++y)
    {
        for (int x = 0; x < IMAGE_WIDTH; ++x)
        {
            colorPixel = traza_RR(camara.generarRayo(x, y), 1);

            RGBQUAD color = colorPixel.getColorTotal().toRGBQUAD();
            FreeImage_SetPixelColor(bitmap, x, y, &color);
            RGBQUAD colorCoeficienteReflexion = colorPixel.getColorCoeficienteReflexion().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapCoeficienteReflexion, x, y, &colorCoeficienteReflexion);
            RGBQUAD colorCoeficienteRefraccion = colorPixel.getColorCoeficienteTransparencia().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapCoeficienteRefraccion, x, y, &colorCoeficienteRefraccion);
            RGBQUAD colorComponentesAmbiente = colorPixel.getComponenteAmbiente().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapComponentesAmbiente, x, y, &colorComponentesAmbiente);
            RGBQUAD colorComponentesDifuso = colorPixel.getComponenteDifusa().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapComponentesDifuso, x, y, &colorComponentesDifuso);
            RGBQUAD colorComponentesEspecular = colorPixel.getComponenteEspecular().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapComponentesEspecular, x, y, &colorComponentesEspecular);
            RGBQUAD colorComponentesReflexion = colorPixel.getComponenteReflexion().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapComponentesReflexion, x, y, &colorComponentesReflexion);
            RGBQUAD colorComponentesRefraccion = colorPixel.getComponenteTransparencia().toRGBQUAD();
            FreeImage_SetPixelColor(bitmapComponentesRefraccion, x, y, &colorComponentesRefraccion);

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
    bool exito = true;
    exito = FreeImage_Save(FIF_PNG, bitmap, (outputPath + ".png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapCoeficienteReflexion, (outputPath + "_coeficiente_reflexion.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapCoeficienteRefraccion, (outputPath + "_coeficiente_refraccion.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapComponentesAmbiente, (outputPath + "_componentes_ambiente.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapComponentesDifuso, (outputPath + "_componentes_difuso.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapComponentesEspecular, (outputPath + "_componentes_especular.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapComponentesReflexion, (outputPath + "_componentes_reflexion.png").c_str(), 0) || exito;
    exito = FreeImage_Save(FIF_PNG, bitmapComponentesRefraccion, (outputPath + "_componentes_refraccion.png").c_str(), 0) || exito;
    if (exito)
    {
        std::cout << "Imagen guardada en: " << outputPath << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar la imagen." << std::endl;
    }

    // Liberar memoria y finalizar FreeImage
    texturas.liberarTexturas();
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
