#include <exec/types.h>
#include <intuition/intuition.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>

struct IntuitionBase *IntuitionBase;

int main() {
    // Abrir la librería de ventanas
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    if (!IntuitionBase) return 1;

    // Abrir una ventana
    struct Window *win = OpenWindowTags(NULL,
        WA_Left,   100,
        WA_Top,    50,
        WA_Width,  200,
        WA_Height, 100,
        WA_Title,  (ULONG)"Mi Ventana",
        WA_Flags,  WFLG_DRAGBAR | WFLG_CLOSEGADGET | WFLG_DEPTHGADGET | WFLG_ACTIVATE | WFLG_SMART_REFRESH,
        TAG_DONE
    );

    if (!win) {
        CloseLibrary((struct Library *)IntuitionBase);
        return 1;
    }

    // Esperar unos segundos (aprox. 5 segundos)
    Delay(250);

    // Cerrar ventana y liberar recursos
    CloseWindow(win);
    CloseLibrary((struct Library *)IntuitionBase);

    return 0;
}

