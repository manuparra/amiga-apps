# Amiga 500 C experiments ( Motorola 68000 ) 

This is an experiment on applications for the **AMIGA 500** platform (Motorola 68000) written in **C**, targeting **Kickstart 1.3**, **Workbench1.3**, **standalone apps/games** and so on, and built with **vbcc**.  

The goal is to explore CLI-Shell apps, low-level graphics (screens/bitplanes/sprites), and simple tooling on real hardware or emulators.

## Motivation

This project was born from a question that has been in my mind for the last 25 years:

> *"Could I use C to create something useful and usable for a computer that is now over 40 years old?, I mean ,in 2025?"*

Part of the challenge is twofold:
1. **Exploring feasibility** — seeing if modern C skills can be applied effectively to such a vintage platform.
2. **Pushing the limits** — get the constraints of an obsolete architecture (m68k) and chipset to think in terms of *optimised (or adjusted) C programming*, where every byte and every cycle matters.

Beyond the technical side, it’s also a personal experiment in creativity, nostalgia, and the art of programming when hardware limitations force you to be both efficient and inventive.


# Platform limits (Amiga 500, KS/WB 1.3)

Here a few notes of this journey: 

**CPU, alignment & data types**
- CPU: Motorola 68000 @ ~7.14 MHz (PAL). No MMU/FPU by default.
- Alignment: 68000 traps on misaligned word/long accesses (e.g., reading a UWORD from an odd address causes an address error / Guru). Keep structures and buffers even-aligned when accessing as words/longs.
- C sizes (typical with vbcc on m68k): char=8 bit, short=16 bit, int=32 bit, long=32 bit, pointers=32 bit. Prefer Amiga types for clarity: UBYTE/UWORD/ULONG.
- Custom logic: Bitplanes, sprites, Copper and Blitter are controlled by registers like BPLCON0/1/2, etc.

**Chipset & memory model**
- OCS custom chips: Agnus (DMA/Copper/Blitter), Denise (video/sprites), Paula (audio/I/O). Agnus arbitrates DMA over Chip RAM.
- Chip RAM vs Fast RAM: Chip RAM is shared with the custom chips (required for bitplanes, sprites, audio). A500 usually ships with 512 KB Chip, often upgraded to 1 MB (A500+ can reach 2 MB). Fast RAM (expansions) is CPU-only and much faster. Use Chip RAM only when the chipset must see it.
- Mem Allocators: Prefer AllocMem(size, MEMF_ANY) for general buffers; reserve MEMF_CHIP strictly for display/audio data. Check with AvailMem(MEMF_ANY/MEMF_CHIP) before allocating.

**Workbench/Intuition 1.3 specifics**
- Screens & windows: On WB 1.3, use classic APIs (OpenScreen, OpenWindow). Tag-based calls like OpenWindowTags() are 2.0+, not available on 1.3.
- Fullscreen look: A CUSTOMSCREEN may show a title strip/gadgets unless you suppress system decor (common trick: CUSTOMSCREEN | SCREENQUIET) and/or manage View/Copper yourself.
- Graphics depth: 1–5 bitplanes (2–32 colors) in LORES; HIRES vales per pixel width. Control planes/modes via BPLCON0; fine scroll via BPLCON1.

**Stack & large local buffers**
- Small default stack: CLI/Workbench programs on 1.3 often start with a small stack (commonly a few kilobytes). Large local arrays (e.g., 10–50 KB) can crash you. Prefer heap (AllocMem) for big buffers. You can raise stack via Stack command or icon tooltype on WB.

**AmigaDOS & file I/O (BCPL heritage)**
- Handles: AmigaDOS uses file handles from Open(); many structures use BCPL-style pointers (e.g., BPTR = pointer >> 2).
- I/O API: Use Open/Read/Seek/Close (proto/dos.h). Strings like BSTR (BCPL strings) still appear in APIs.

**Performance notes**
- Blitter vs CPU: Use the Blitter for block copies, line draws, and masked blits into Chip RAM; free the 68000 for game logic.
- DMA contention: Heavy DMA (bitplanes + sprites + audio) eats bandwidth; keep bitplane count modest when you need CPU cycles. (Agnus arbitrates DMA.)

**Other things ...**
- Misaligned word/long access → Guru (address error).
- Calling 2.0+ APIs on KS 1.3 → link-time undefined symbol.
- Using %zu in printf on vbcc → format warning; cast size_t to unsigned long and use %lu.
- Allocating big local arrays → stack overflow and Software failure without more information; use AllocMem.
- Putting display buffers in FAST → nothing appears; the chipset can’t see FAST RAM—use MEMF_CHIP for visible graphics.

