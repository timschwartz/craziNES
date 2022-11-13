#include "wxWidgets/wxcraziNES.hpp"
#include "nes/cpu_6502.hpp"
#include "nes/rom.hpp"
#include <cstdlib>
#include <ctime>

wxIMPLEMENT_APP(wxcraziNES);

bool wxcraziNES::OnInit()
{
    frame = new MainWindow("craziNES", wxPoint(20, 20), wxSize(256, 240));
    frame->Show(true);
    return true;
}

void wxcraziNES::open_log()
{
    if(!this->log)
    {
        this->log = new LogWindow(this->frame,
            "Log", wxPoint(150, 50), wxSize(700, 480));
    }

    this->log->SetBackgroundColour(wxColour(*wxWHITE));
    this->log->Show(true);

    if(!this->log->log_listbox)
    {
        this->log->log_listbox = new wxListBox(this->log, ID_LOG_LISTBOX,
            wxPoint(20, 30), wxSize(550, 380));
    }
    wxFont log_font(wxFontInfo(12).FaceName("Courier"));
    this->log->log_listbox->SetFont(log_font);
}

void wxcraziNES::open_registers()
{
    if(!this->registers)
    {
        this->registers = new RegistersWindow(this->frame,
            "6502 Registers", wxPoint(150, 50), wxSize(700, 480));
    }

    this->registers->SetBackgroundColour(wxColour(*wxWHITE));
    this->registers->Show(true);
}

void wxcraziNES::open_memory()
{
    if(!this->memory) 
    {
        this->memory = new MemoryWindow(this->frame, 
            "NES memory", wxPoint(150, 50), wxSize(700, 480));
    }

    this->memory->SetBackgroundColour(wxColour(*wxWHITE));
    this->memory->Show(true);

    wxFont font(wxFontInfo(11).Bold());
    wxStaticText *label_memory_address = new wxStaticText(this->memory, 
        wxID_ANY, wxT("Address    Data"), wxPoint(20, 5));
    label_memory_address->SetFont(font);

    wxFont mem_font(wxFontInfo(12).FaceName("Courier"));
    this->memory->memory_listbox = new wxListBox(this->memory, ID_MEMORY_LISTBOX, 
        wxPoint(20, 30), wxSize(550, 380)); 
    this->memory->memory_listbox->SetFont(mem_font);

    this->memory->debugger_pc = new wxTextCtrl(this->memory, ID_DEBUGGER_PC, "0x0000 0000",
        wxPoint(580, 30), wxSize(90, 25), 0);

    this->memory->cpu_step = new wxButton(this->memory, BUTTON_cpu_step, _T("Step"), 
        wxPoint(580, 60), wxDefaultSize, 0);

    this->memory->cpu_steps_count = new wxTextCtrl(this->memory, ID_CPU_STEP_COUNT, "2",
        wxPoint(580, 110), wxSize(90, 25), 0);

    this->memory->cpu_steps = new wxButton(this->memory, BUTTON_cpu_steps, _T("Multi-step"),
        wxPoint(580, 140), wxDefaultSize, 0);
}
