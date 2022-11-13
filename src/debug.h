#ifndef _CRAZINES_DEBUG_H
#define _CRAZINES_DEBUG_H

#include <wx/wx.h>
#include "nes/cpu_6502.hpp"

void open_memory(nes::cpu_6502 *);
void open_registers();
void open_log();

class LogWindow: public wxFrame
{
  public:
    LogWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~LogWindow();
    wxListBox *log_listbox = NULL;
};

class RegistersWindow: public wxFrame
{
  public:
    RegistersWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~RegistersWindow();
    wxListBox *registers_listbox = NULL;
};

class MemoryWindow: public wxFrame
{
  private:
    uint16_t start_addr = 0, end_addr = 0;
  public:
    MemoryWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~MemoryWindow();
    wxListBox *memory_listbox = NULL;
    void view(uint16_t start_addr, uint16_t end_addr);
    wxTextCtrl *debugger_pc = NULL;
    wxButton *cpu_step = NULL;
    void OnCPUStep(wxCommandEvent &event);
    wxTextCtrl *cpu_steps_count = NULL;
    wxButton *cpu_steps = NULL;
    void OnCPUSteps(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};

enum
{
    BUTTON_cpu_step = 20,
    BUTTON_cpu_steps = 21
};

const int ID_MEMORY_LISTBOX = 100;
const int ID_REGISTERS_LISTBOX = 101;
const int ID_CPU_STEP_COUNT = 102;
const int ID_DEBUGGER_PC = 103;
const int ID_LOG_LISTBOX = 104;

#endif
