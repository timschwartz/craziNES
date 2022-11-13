#pragma once

#include <wx/wx.h>
#include "../debug.h"
#include "MainWindow.hpp"

class MainWindow;

class wxcraziNES: public wxApp
{
  public:
    virtual bool OnInit();
    MainWindow *frame = NULL;
    MemoryWindow *memory = NULL;
    RegistersWindow *registers = NULL;
    LogWindow *log = NULL;
};
