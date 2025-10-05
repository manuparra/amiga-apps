#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <intuition/screens.h>
#include <clib/exec_protos.h>
#include <clib/graphics_protos.h>
#include <clib/intuition_protos.h>
#include <workbench/workbench.h>
#include <clib/wb_protos.h>

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;
struct Library *WorkbenchBase;

int main() {
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0);
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    WorkbenchBase = OpenLibrary("workbench.library", 0);

    if (!GfxBase || !IntuitionBase || !WorkbenchBase)
        return 1;

    // Cerramos el Workbench visualmente
    CloseWorkBench();

    // Abrimos pantalla personalizada (fullscreen)
    struct NewScreen screenInfo = {
        0, 0,             // LeftEdge, TopEdge
        320, 256,         // Width, Height
        2,                // Depth (2 bitplanes = 4 colores)
        0, 1,             // DetailPen, BlockPen
        HIRES,
        CUSTOMSCREEN,
        NULL, NULL, NULL
    };

    struct Screen *screen = OpenScreen(&screenInfo);
    if (!screen) {
        OpenWorkBench();
        return 1;
    }

    // Dibujar "HOLA" con DrawText()
    struct RastPort *rp = &screen->RastPort;
    SetAPen(rp, 1);  // Color 1 (normalmente blanco)
    Move(rp, 100, 100);
    Text(rp, "HOLA", 4);

    Delay(250); // Espera ~5 segundos

    CloseScreen(screen);
    OpenWorkBench();

    CloseLibrary(WorkbenchBase);
    CloseLibrary((struct Library *)IntuitionBase);
    CloseLibrary((struct Library *)GfxBase);

    return 0;
}


