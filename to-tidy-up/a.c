#include <exec/types.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/graphics_protos.h>

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

int main() {
    IntuitionBase = (struct IntuitionBase *)OpenLibrary("intuition.library", 0);
    GfxBase = (struct GfxBase *)OpenLibrary("graphics.library", 0);

    if (!IntuitionBase || !GfxBase) return 1;

    struct Screen *scr = OpenScreenTags(NULL,
        SA_Width, 320,
        SA_Height, 256,
        SA_Depth, 2,  // 2 bitplanes = 4 colores
        SA_Title, (ULONG)"Mi Pantalla",
        TAG_DONE
    );

    if (!scr) return 1;

    struct RastPort *rp = &scr->RastPort;
    SetAPen(rp, 1);          // Selecciona color 1
    WritePixel(rp, 100, 100); // Dibuja un pixel

    Delay(200); // Espera ~4 segundos

    CloseScreen(scr);

    CloseLibrary((struct Library *)GfxBase);
    CloseLibrary((struct Library *)IntuitionBase);

    return 0;
}

