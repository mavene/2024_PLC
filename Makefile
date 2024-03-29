# Makefile for Cygwin64

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/image_processor
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC := C:/cygwin64/bin/x86_64-w64-mingw32-gcc.exe
INCLUDE := -Iinclude
GTKINCLUDE := $$(PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig/ pkg-config --cflags gtk+-3.0)
CPPFLAGS := -MMD -MP
CFLAGS := -Wall -Werror -pedantic -ansi 
GTKLIBS := C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgtk-3.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgdk-3.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgdi32.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libimm32.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libshell32.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libole32.a -Wl,C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libuuid.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libwinmm.a -ldwmapi C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libsetupapi.a -lcfgmgr32 C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libz.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libpangowin32-1.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libpangocairo-1.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libpango-1.0.dll.a -l:libatk-1.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libcairo.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libcairo-gobject.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgdk_pixbuf-2.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgio-2.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libglib-2.0.dll.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libintl.a C:/cygwin64/usr/x86_64-w64-mingw32/sys-root/mingw/lib/libgobject-2.0.dll.a
# $$(PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/sys-root/mingw/lib/pkgconfig pkg-config --libs gtk+-3.0 | sed -E 's/-l([a-z0-9._\-]*)/-l:lib\1.dll.a/g')
LDLIBS := -lm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(GTKINCLUDE) $^ -o $@ $(GTKLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) --static $(CPPFLAGS) $(CFLAGS) -c -std=c99 $(INCLUDE) $(GTKINCLUDE) $< -o $@ $(LDLIBS) $(GTKLIBS)
#$(CC) $(INCLUDE) $(CPPFLAGS) $(CFLAGS) -c $< -o $@ $(LDLIBS)

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(EXE) $(OBJ_DIR)
	
-include $(OBJ:.o=.d)