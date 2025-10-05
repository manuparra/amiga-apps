#include <exec/types.h>
#include <exec/memory.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <graphics/rastport.h>
#include <workbench/workbench.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>
#include <clib/dos_protos.h>

struct GfxBase *GfxBase;
struct IntuitionBase *IntuitionBase;

#define WIDTH  320
#define HEIGHT 256

int main() {
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0);
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    if (!GfxBase || !IntuitionBase) return 1;

    CloseWorkBench();

    struct NewScreen ns = {
        0, 0, WIDTH, HEIGHT,
        2, // 2 bitplanes = 4 colores
        0, 1,
        HIRES,
        CUSTOMSCREEN,
        NULL, NULL, NULL
    };

    struct Screen *screen = OpenScreen(&ns);
    if (!screen) {
        OpenWorkBench();
        return 1;
    }

    // Crear una ventana clásica con OpenWindow()
    struct NewWindow nw = {
        0, 0, WIDTH, HEIGHT,
        0, 1, // detail, block pen
        IDCMP_MOUSEBUTTONS,
        WFLG_SIMPLE_REFRESH | WFLG_SMART_REFRESH | WFLG_ACTIVATE,
        NULL, NULL, NULL, NULL,
        NULL,
        screen, // asociamos la pantalla creada
        NULL,
        0, 0, 0, 0,
        CUSTOMSCREEN
    };

    struct Window *win = OpenWindow(&nw);
    if (!win) {
        CloseScreen(screen);
        OpenWorkBench();
        return 1;
    }

    struct RastPort *rp = win->RPort;

    // Establecer paleta: índice 0 = negro, los demás colores
    SetRGB4(&screen->ViewPort, 0, 0, 0, 0);       // negro
    SetRGB4(&screen->ViewPort, 1, 15, 0, 0);      // rojo
    SetRGB4(&screen->ViewPort, 2, 0, 15, 0);      // verde
    SetRGB4(&screen->ViewPort, 3, 0, 0, 15);      // azul

    // Dibujar 5 píxeles con distintos colores
    for (int i = 0; i < 4; i++) {
        SetAPen(rp, i);
        WritePixel(rp, 10 + i * 10, 10);
    }

    // Dibujar "botón" rojo
    SetAPen(rp, 1);
    RectFill(rp, 100, 200, 220, 220);
    SetAPen(rp, 0);
    Move(rp, 130, 214);
    Text(rp, "Salir", 5);

    // Esperar clic del ratón
    ULONG sigmask = 1L << win->UserPort->mp_SigBit;
    BOOL salir = FALSE;
    while (!salir) {
        Wait(sigmask);
        struct IntuiMessage *msg;
        while ((msg = (struct IntuiMessage *)GetMsg(win->UserPort))) {
            if (msg->Class == IDCMP_MOUSEBUTTONS) {
                WORD x = msg->MouseX;
                WORD y = msg->MouseY;
                if (x >= 100 && x <= 220 && y >= 200 && y <= 220) {
                    salir = TRUE;
                }
            }
            ReplyMsg((struct Message *)msg);
        }
    }

    // Cerrar ventana y pantalla
    CloseWindow(win);
    CloseScreen(screen);

    // Restaurar el Workbench
    OpenWorkBench();

    // Cerrar librerías
    CloseLibrary((struct Library *)IntuitionBase);
    CloseLibrary((struct Library *)GfxBase);
    return 0;
}

