#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>

struct IntuitionBase *IntuitionBase;

int main() {
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    if (!IntuitionBase) return 1;

    struct NewWindow win = {
        10, 10,          // LeftEdge, TopEdge
        200, 100,        // Width, Height
        0, 1,            // DetailPen, BlockPen
        IDCMP_CLOSEWINDOW, // IDCMP Flags
        WFLG_CLOSEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET |
        WFLG_ACTIVATE | WFLG_SMART_REFRESH | WFLG_GIMMEZEROZERO,
        NULL,            // No title
        NULL,            // No screen
        NULL,            // No bitmap
        5, 5, 195, 95,   // MinWidth, MinHeight, MaxWidth, MaxHeight
        CUSTOMSCREEN     // Type
    };

    struct Window *window = OpenWindow(&win);
    if (window) {
        Delay(250); // Wait ~5 seconds
        CloseWindow(window);
    }

    CloseLibrary((struct Library *)IntuitionBase);
    return 0;
}

