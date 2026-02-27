BUILDDIR	  = build

SHELL 		  = /bin/sh
MKDIR         = mkdir -p
QMAKE		  = qmake

QMAKE_ARGS	  = -spec linux-g++
QMAKE_DEBUG_ARGS	  = $(QMAKE_ARGS) CONFIG+=debug CONFIG+=qml_debug

.PHONY: default debug generate generate_debug build clean distclean
default: ensure_build_dir generate build
debug: ensure_build_dir generate_debug build

all: default

ensure_build_dir:
	@$(MKDIR) $(BUILDDIR)
build:
	@$(MAKE) -C $(BUILDDIR) -j$$(nproc)

generate:
	@cd $(BUILDDIR) && $(QMAKE) .. $(QMAKE_ARGS)
generate_debug:
	@cd $(BUILDDIR) && $(QMAKE) .. $(QMAKE_DEBUG_ARGS)

clean:
	@$(MAKE) -C $(BUILDDIR) clean
distclean: clean
	@rm -rf $(BUILDDIR)