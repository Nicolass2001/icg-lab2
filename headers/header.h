#include <iostream>
#include <FreeImage.h>

void copiarImagen()
{
    // Inicializar FreeImage
    FreeImage_Initialise();

    // Detectar tipo de archivo
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType("chart.png", 0);
    if (format == FIF_UNKNOWN)
    {
        std::cerr << "Formato desconocido o archivo no encontrado" << std::endl;
        FreeImage_DeInitialise();
        return;
    }

    // Cargar la imagen
    FIBITMAP *image = FreeImage_Load(format, "chart.png");
    if (!image)
    {
        std::cerr << "Error al cargar la imagen" << std::endl;
        FreeImage_DeInitialise();
        return;
    }

    // Guardar la imagen en otro formato
    if (FreeImage_Save(FIF_BMP, image, "chart.bmp"))
    {
        std::cout << "Imagen guardada correctamente" << std::endl;
    }
    else
    {
        std::cerr << "Error al guardar la imagen" << std::endl;
    }

    // Liberar memoria
    FreeImage_Unload(image);

    // Finalizar FreeImage
    FreeImage_DeInitialise();
}