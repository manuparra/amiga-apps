#include <exec/types.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <workbench/workbench.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <dos/dos.h>
#include <clib/dos_protos.h>


struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

#define WIDTH  320
#define HEIGHT 256

int main() {
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0);
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    if (!GfxBase || !IntuitionBase) return 1;


    // Ocultar el Workbench
    CloseWorkBench();

    struct NewScreen ns = {
    .LeftEdge = 0,
    .TopEdge = 0,
    .Width = WIDTH,
    .Height = HEIGHT,
    .Depth = 2,
    .DetailPen = 0,
    .BlockPen = 0,
    .ViewModes = 0,  // o LORESLACE si quieres entrelazado
    .Type = CUSTOMSCREEN,
    .Font = NULL,
    .DefaultTitle = NULL,
    .Gadgets = NULL
};

    struct Screen *screen = OpenScreen(&ns);
    if (!screen) {
        OpenWorkBench();
        return 1;
    }

    struct RastPort *rp = &screen->RastPort;

    // Establecer paleta de colores
    //SetRGB4(&screen->ViewPort, 0, 0, 0, 0);    // negro
    //SetRGB4(&screen->ViewPort, 1, 15, 0, 0);   // rojo
    //SetRGB4(&screen->ViewPort, 2, 0, 15, 0);   // verde
    //SetRGB4(&screen->ViewPort, 3, 0, 0, 15);   // azul
    for (int i = 0; i < 32; i++) {
        SetRGB4(&screen->ViewPort, i, (i * 2) % 16, (i * 3) % 16, (i * 5) % 16);
    }


    // Pintar 5 píxeles en diferentes colores
    for (int i = 0; i < 12; i++) {
        SetAPen(rp, i);
        WritePixel(rp, 20 + i * 10, 20);
    }

    // Esperar un poco (por ejemplo, 5 segundos)
    Delay(5 * 50); // 50 ticks/segundo

    // Cerrar pantalla y restaurar Workbench
    CloseScreen(screen);
    OpenWorkBench();

    // Cerrar librerías
    CloseLibrary((struct Library *)IntuitionBase);
    CloseLibrary((struct Library *)GfxBase);

    return 0;
}

