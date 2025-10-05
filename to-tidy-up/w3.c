#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <intuition/screens.h>
#include <intuition/intuition.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

int main() {
    // Abrimos librerías del sistema
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0);
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);

    if (!GfxBase || !IntuitionBase)
        return 1;

    // Abrimos una pantalla nueva (propia)
    struct NewScreen ns = {
        0, 0,              // LeftEdge, TopEdge
        320, 256,          // Width, Height
        2,                 // Depth (2 bitplanes = 4 colores)
        0, 1,              // DetailPen, BlockPen
        HIRES | LACE,      // ViewModes (usa HIRES y entrelazado)
        CUSTOMSCREEN,      // Tipo de pantalla
        NULL,              // No título
        NULL,              // No fuente
        NULL               // No BitMap
    };

    struct Screen *screen = OpenScreen(&ns);
    if (!screen) {
        CloseLibrary((struct Library *)IntuitionBase);
        CloseLibrary((struct Library *)GfxBase);
        return 1;
    }

    // Dibujar un píxel en la pantalla
    struct RastPort *rp = &screen->RastPort;
    SetAPen(rp, 1);             // Selecciona color 1
    WritePixel(rp, 100, 100);   // Dibuja píxel en (100,100)

    Delay(250); // espera ~5 segundos

    // Cierra pantalla y limpia
    CloseScreen(screen);
    CloseLibrary((struct Library *)IntuitionBase);
    CloseLibrary((struct Library *)GfxBase);

    return 0;
}

