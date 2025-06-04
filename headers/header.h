#include <iostream>
#include <FreeImage.h>

void copiarImagen() {
    // Inicializar FreeImage
    FreeImage_Initialise();

    // Cargar una imagen
    FIBITMAP* image = FreeImage_Load(FIF_PNG, "chart.png", PNG_DEFAULT);
    if (!image) {
        std::cerr << "Error al cargar la imagen" << std::endl;
        return;
    }

    // Guardar la imagen en otro formato
    if (FreeImage_Save(FIF_BMP, image, "chart.bmp", BMP_DEFAULT)) {
        std::cout << "Imagen guardada correctamente" << std::endl;
    }
    else {
        std::cerr << "Error al guardar la imagen" << std::endl;
    }

    // Liberar memoria
    FreeImage_Unload(image);

    // Finalizar FreeImage
    FreeImage_DeInitialise();
}