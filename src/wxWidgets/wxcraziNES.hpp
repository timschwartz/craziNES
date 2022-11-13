#pragma once

#include <wx/wx.h>
#include "MainWindow.hpp"
#include "MemoryWindow.hpp"
#include "LogWindow.hpp"
#include "RegistersWindow.hpp"
#include "../nes/cpu_6502.hpp"

class MainWindow;
class MemoryWindow;
class RegistersWindow;

class wxcraziNES: public wxApp
{
  public:
    virtual bool OnInit();
    MainWindow *frame = nullptr;
    MemoryWindow *memory = nullptr;
    RegistersWindow *registers = nullptr;
    LogWindow *log = nullptr;
    nes::cpu_6502 *cpu = nullptr;

    void open_log();
    void open_registers();
    void open_memory();
};

const int ID_MEMORY_LISTBOX = 100;
const int ID_REGISTERS_LISTBOX = 101;
const int ID_CPU_STEP_COUNT = 102;
const int ID_DEBUGGER_PC = 103;
const int ID_LOG_LISTBOX = 104;

enum
{
    BUTTON_cpu_step = 20,
    BUTTON_cpu_steps = 21
};
