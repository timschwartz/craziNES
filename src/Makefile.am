AM_CPPFLAGS = -std=c++17 -I. -Wall -Werror ${WX_CPPFLAGS} ${GLM_CFLAGS}

bin_PROGRAMS = craziNES
craziNES_SOURCES = craziNES.cpp rom.cpp debug.cpp cpu_6502.cpp ppu.cpp \
		mappers/mapper.cpp mappers/mapper0.cpp
craziNES_LDADD = ${WX_LIBS}
