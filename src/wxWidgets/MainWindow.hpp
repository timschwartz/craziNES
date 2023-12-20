#pragma once

#include "wxcraziNES.hpp"
#include "../nes/cpu_6502.hpp"

class MainWindow: public wxFrame
{
  public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
  private:
    void OnOpenROM(wxCommandEvent &event);
    void OnDebugROM(wxCommandEvent &event);
    void OnDebugRegisters(wxCommandEvent &event);
    void OnDebugLog(wxCommandEvent &event);
    void OnExit(wxCommandEvent &event);
    void OnPaint(wxPaintEvent& event);
    //wxDECLARE_EVENT_TABLE();
    uint8_t screen[256 * 240 * 3];
    nes::cpu_6502 *cpu;
};

enum
{
    ID_open_rom = 0,

    ID_debug_rom = 10,
    ID_debug_registers = 11,
    ID_debug_log = 12
};
