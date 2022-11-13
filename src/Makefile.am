AM_CPPFLAGS = -std=c++17 -Wall -Werror -O0 ${WX_CPPFLAGS} ${GLM_CFLAGS}

bin_PROGRAMS = craziNES
craziNES_SOURCES = nes/rom.cpp nes/cpu_6502.cpp nes/ppu.cpp \
		nes/mappers/mapper.cpp nes/mappers/mapper0.cpp \
		wxWidgets/MainWindow.cpp wxWidgets/LogWindow.cpp wxWidgets/RegistersWindow.cpp \
		wxWidgets/MemoryWindow.cpp wxWidgets/wxcraziNES.cpp
craziNES_LDADD = ${WX_LIBS}
