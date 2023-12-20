#pragma once

#include "wxcraziNES.hpp"
#include <cstdint>

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
