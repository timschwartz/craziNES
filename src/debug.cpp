#include <craziNES.h>
#include <cpu_6502.h>
#include <memory.h>

DECLARE_APP(wxcraziNES)

wxBEGIN_EVENT_TABLE(MemoryWindow, wxFrame)
    EVT_BUTTON(BUTTON_cpu_step, MemoryWindow::OnCPUStep)
    EVT_BUTTON(BUTTON_cpu_steps, MemoryWindow::OnCPUSteps)
wxEND_EVENT_TABLE()

extern nes::cpu_6502 *cpu;

void open_memory()
{
    if(!wxGetApp().memory) 
    {
        wxGetApp().memory = new MemoryWindow(wxGetApp().frame, 
                              "NES memory", wxPoint(150, 50), wxSize(700, 480));
    }

    wxGetApp().memory->SetBackgroundColour(wxColour(*wxWHITE));
    wxGetApp().memory->Show(true);

    wxFont font(wxFontInfo(11).Bold());
    wxStaticText *label_memory_address = new wxStaticText(wxGetApp().memory, 
                            wxID_ANY, wxT("Address    Data"), wxPoint(20, 5));
    label_memory_address->SetFont(font);

    wxFont mem_font(wxFontInfo(12).FaceName("Courier"));
    wxGetApp().memory->memory_listbox = new wxListBox(wxGetApp().memory, ID_MEMORY_LISTBOX, 
      wxPoint(20, 30), wxSize(550, 380)); 
    wxGetApp().memory->memory_listbox->SetFont(mem_font);

    wxGetApp().memory->debugger_pc = new wxTextCtrl(wxGetApp().memory, ID_DEBUGGER_PC, "0x0000 0000",
                                                          wxPoint(580, 30), wxSize(90, 25), 0);

    wxGetApp().memory->cpu_step = new wxButton(wxGetApp().memory, BUTTON_cpu_step, _T("Step"), 
                                                 wxPoint(580, 60), wxDefaultSize, 0);

    wxGetApp().memory->cpu_steps_count = new wxTextCtrl(wxGetApp().memory, ID_CPU_STEP_COUNT, "2",
                                                          wxPoint(580, 110), wxSize(90, 25), 0);

    wxGetApp().memory->cpu_steps = new wxButton(wxGetApp().memory, BUTTON_cpu_steps, _T("Multi-step"),
                                                  wxPoint(580, 140), wxDefaultSize, 0);
}

void MemoryWindow::view(uint16_t start_addr, uint16_t end_addr)
{
    char message[1024];
    std::string msg;
    uint16_t addr = start_addr;

    sprintf(message, "0x%.4X", addr);
    debugger_pc->Clear();
    debugger_pc->AppendText(message);

    nes::opcode_t *op;

    while(addr < end_addr)
    {
        try
        {
            op = new nes::opcode_t(cpu->mmu, addr); 

            if(op->sz == 1) sprintf(message, "%.4X    %.2X      ", addr, op->opcode);

            if(op->sz == 2) sprintf(message, "%.4X    %.2X %.2X   ", addr, op->opcode, op->imm & 0xFF);
         
            if(op->sz == 3) sprintf(message, "%.4X    %.2X %.2X %.2X", addr, op->opcode, op->imm >> 8, op->imm & 0xFF);

            addr += op->sz;
            msg = message + op->to_string();
            delete op;
            memory_listbox->Append(msg.c_str());
        }
        catch(std::string e)
        {
            memory_listbox->Append(e.c_str());
            return;
        }
    }
}

void open_registers()
{
}

MemoryWindow::MemoryWindow(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, wxID_ANY, title, pos, size)
{
}

MemoryWindow::~MemoryWindow()
{
    wxGetApp().memory = NULL;
}

void MemoryWindow::OnCPUStep(wxCommandEvent& event)
{
    if(wxGetApp().registers != NULL) open_registers();
}

void MemoryWindow::OnCPUSteps(wxCommandEvent& event)
{
    if(wxGetApp().registers != NULL) open_registers();
}
