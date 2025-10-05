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
        2,              // 2 bitplanes = 4 colores
        0, 1,
        HIRES,
        CUSTOMSCREEN,
        NULL, NULL, NULL
    };

    struct Screen *screen = OpenScreen(&ns);
    if (!screen) {
        OpenWorkBench(); // Reabrir Workbench si falla
        return 1;
    }

    struct Window *win = OpenWindowTags(NULL,
        WA_CustomScreen, (ULONG)screen,
        WA_Left, 0,
        WA_Top, 0,
        WA_Width, WIDTH,
        WA_Height, HEIGHT,
        WA_Flags, WFLG_SIMPLE_REFRESH | WFLG_SMART_REFRESH | WFLG_ACTIVATE,
        WA_IDCMP, IDCMP_MOUSEBUTTONS,
        WA_Title, (ULONG)"Demo",
        TAG_END);

    if (!win) {
        CloseScreen(screen);
        OpenWorkBench();
        return 1;
    }

    struct RastPort *rp = win->RPort;

    SetRGB4(&screen->ViewPort, 0, 0, 0, 0);    // color 0: negro
    SetRGB4(&screen->ViewPort, 1, 15, 0, 0);   // color 1: rojo
    SetRGB4(&screen->ViewPort, 2, 0, 15, 0);   // color 2: verde
    SetRGB4(&screen->ViewPort, 3, 0, 0, 15);   // color 3: azul

    for (int i = 0; i < 4; i++) {
        SetAPen(rp, i);
        WritePixel(rp, 10 + i * 10, 10);
    }

    SetAPen(rp, 1); // fondo rojo
    RectFill(rp, 100, 200, 220, 220);
    SetAPen(rp, 0); // texto negro
    Move(rp, 130, 214);
    Text(rp, "Salir", 5);

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

    CloseWindow(win);
    CloseScreen(screen);

    OpenWorkBench();

    CloseLibrary((struct Library *)IntuitionBase);
    CloseLibrary((struct Library *)GfxBase);
    return 0;
}

