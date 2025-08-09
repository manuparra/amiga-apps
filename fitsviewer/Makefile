# The Makefile ;)
# =========================
#  Makefile — Amiga 500 (vbcc +kick13 con Docker)
# =========================

# ---- Config Docker/vbcc ----
DOCKER_IMAGE ?= vintagecomputingcarinthia/vbcc4vcc
USER_MAP     ?= 1000:uucp
WORKDIR      ?= /host
DOCKER_RUN    = docker run -u $(USER_MAP) -v $(PWD):$(WORKDIR) -w $(WORKDIR) --rm $(DOCKER_IMAGE)

# ---- Flags vbcc ----
KICK     ?= +kick13
INCLUDES ?= -I/opt/NDK_3.9/Include/include_h
LIBS     ?= -lamiga
VERBOSE  ?= # pon -v si quieres verboso

VC       = $(DOCKER_RUN) vc
VCFLAGS  = $(VERBOSE) $(KICK) $(INCLUDES)

PROGS = fitsviewer

all: $(PROGS)

%: %.c
	$(VC) $(VCFLAGS) -o $@ $< $(LIBS)

fitsviewer: fitsviewer.c
 	$(VC) $(VCFLAGS) -o $@ $< $(LIBS)

.PHONY: clean veryclean
clean:
	@echo "Cleaning ... "
	@rm -f $(PROGS)

veryclean: clean
	@echo "Cleaning all ..."
	@rm -f *.o *.asm *.map core

help:
	@echo "Targets:"
	@echo "  all            -> compile: $(PROGS)"
	@echo "  fitsviewer     -> compile fitsviewver
	@echo "  clean          -> clean binaries"
	@echo "  veryclean      -> clean all !"
	@echo ""
	@echo "Vars:"
	@echo "  KICK=+kick13            (default)"
	@echo "  INCLUDES='-I/opt/NDK_3.9/Include/include_h'"
	@echo "  LIBS='-lamiga'"
	@echo "  VERBOSE='-v'            (default)"
	@echo ""
	@echo "Examples:"
	@echo "  make                     # compile all"
	@echo "  make readfits            # compile fitsviewer"
	@echo "  make VERBOSE=-v          # add VERBOSE"
	@echo "  make KICK=+kick13        # force kickstart 1.3"