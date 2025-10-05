#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <proto/exec.h>  // <-- aquí están los prototipos de AllocMem y FreeMem

extern struct ExecBase *SysBase;

int main() {
    char *address;

    address = (char *)AllocMem(300, MEMF_CHIP);
    if (address == NULL) {
        return 1;
    }

    FreeMem(address, 300);
    return 0;
}

