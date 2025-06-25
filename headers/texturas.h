#include <FreeImage.h>

#ifndef TEXTURAS_H
#define TEXTURAS_H

struct datosTextura {
    FIBITMAP* datos;
    unsigned width;
    unsigned height;
};

class Texturas {
public:
    int cantTexturas;
    std::vector<datosTextura> texturasCargadas;
    Texturas(int cantTexturas);
    void liberarTexturas();
private:
};

void Texturas::liberarTexturas() {
    for (int i = 0; i < this->cantTexturas; i++) {
        FreeImage_Unload(this->texturasCargadas[i].datos);
    }
}

Texturas::Texturas(int cantTexturas) {

    // Depende de cuantas texturas cargamos, iniciamos con una sola
    this->cantTexturas = cantTexturas;

    // Inicializar FreeImage
    FreeImage_Initialise();

    FREE_IMAGE_FORMAT format;
    FIBITMAP* image = NULL;
    FIBITMAP* image32;

    unsigned width;
    unsigned height;
    int bpp;

    this->texturasCargadas.reserve(cantTexturas);

    for (int i = 0; i < cantTexturas; i++) {

        switch (i) {
        case 0:
            format = FreeImage_GetFileType("dep/ladrillos.png", 0);
            image = FreeImage_Load(format, "dep/ladrillos.png");
            break;
        case 1:
            format = FreeImage_GetFileType("dep/madera.png", 0);
            image = FreeImage_Load(format, "dep/madera.png");
            break;
        case 2:
            format = FreeImage_GetFileType("dep/techo.png", 0);
            image = FreeImage_Load(format, "dep/techo.png");
            break;
        }

        image32 = FreeImage_ConvertTo32Bits(image);

        FreeImage_Unload(image);

        width = FreeImage_GetWidth(image32);
        height = FreeImage_GetHeight(image32);
        bpp = FreeImage_GetBPP(image32); // Por ahora no utilizo bpp, queda por si es necesario despues

        this->texturasCargadas[i].datos = image32;
        this->texturasCargadas[i].width = width;
        this->texturasCargadas[i].height = height;
    }
}

#endif