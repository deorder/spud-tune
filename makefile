7Z ?= 7z
GIT ?= git
CP ?= cp -ra
STRIP ?= strip
MKDIR ?= mkdir -p
PKGCONFIG ?= pkg-config

GITVERSION := $(shell ${GIT} describe --abbrev=4 --dirty --always --tags)

CFLAGS = -I${TOOLCHAIN}/include -I${TOOLCHAIN}/usr/include $(shell ${PKGCONFIG} --cflags MagickWand)
LDFLAGS = -L${TOOLCHAIN}/lib -L${TOOLCHAIN}/usr/lib $(shell ${PKGCONFIG} --libs MagickWand)

BINARIES = spud_from_header_png${EXTENSION} spud_to_header_png${EXTENSION}

all: ${BINARIES}

spud_from_header_png${EXTENSION}: spud_from_header_png.c
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@
	${STRIP} $@

spud_to_header_png${EXTENSION}: spud_to_header_png.c
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@
	${STRIP} $@

.PHONY: clean archive-windows
 
archive-windows: ${BINARIES}
	${MKDIR} spudtune.windows.64.${GITVERSION}
	${CP} ${BINARIES} spudtune.windows.64.${GITVERSION}/
	${CP} ${TOOLCHAIN}/usr/bin/*.dll spudtune.windows.64.${GITVERSION}/
	${7Z} a spudtune.windows.64.${GITVERSION}.7z spudtune.windows.64.${GITVERSION}/
 
clean:
	${RM} ${BINARIES} 
