STRIP ?= strip
PKGCONFIG ?= pkg-config

CFLAGS = -I${TOOLCHAIN}/include -I${TOOLCHAIN}/usr/include $(shell ${PKGCONFIG} --cflags MagickWand)
LDFLAGS = -L${TOOLCHAIN}/lib -L${TOOLCHAIN}/usr/lib $(shell ${PKGCONFIG} --static --libs MagickWand)

BINARIES = spud_from_header_png${EXTENSION} spud_to_header_png${EXTENSION}

all: ${BINARIES}

spud_from_header_png${EXTENSION}: spud_from_header_png.c
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@
	${STRIP} $@

spud_to_header_png${EXTENSION}: spud_to_header_png.c
	${CC} ${CFLAGS} $< ${LDFLAGS} -o $@
	${STRIP} $@

.PHONY: clean

clean:
	${RM} ${BINARIES}
