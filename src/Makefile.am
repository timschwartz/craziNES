AM_CPPFLAGS = -std=c++17 -Wall -Werror ${WX_CPPFLAGS} ${GLM_CFLAGS}

bin_PROGRAMS = craziNES
craziNES_SOURCES = craziNES.cpp nes/rom.cpp debug.cpp cpu_6502.cpp ppu.cpp \
		nes/mappers/mapper.cpp nes/mappers/mapper0.cpp wxWidgets/MainWindow.cpp
craziNES_LDADD = ${WX_LIBS}
