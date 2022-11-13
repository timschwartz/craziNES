#include "MemoryWindow.hpp"
#include "nes/cpu_6502.hpp"

DECLARE_APP(wxcraziNES)

wxBEGIN_EVENT_TABLE(MemoryWindow, wxFrame)
    EVT_BUTTON(BUTTON_cpu_step, MemoryWindow::OnCPUStep)
    EVT_BUTTON(BUTTON_cpu_steps, MemoryWindow::OnCPUSteps)
wxEND_EVENT_TABLE()

void MemoryWindow::view(uint16_t start_addr, uint16_t end_addr)
{
    char message[1024];
    std::string msg;
    uint16_t addr = start_addr;
    this->start_addr = start_addr;
    this->end_addr = end_addr;

    sprintf(message, "0x%.4X", addr);
    debugger_pc->Clear();
    debugger_pc->AppendText(message);
    memory_listbox->Clear();

    nes::opcode_t *op;

    while(addr < end_addr)
    {
        try
        {
            op = new nes::opcode_t(wxGetApp().cpu, addr); 

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
    if(wxGetApp().registers != NULL) wxGetApp().open_registers();
    std::string status;
    try
    {
        status = wxGetApp().cpu->step();
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
    }
    if(wxGetApp().log) wxGetApp().log->log_listbox->Append(status);
    std::cout << status << std::endl;
    view(start_addr, end_addr);

    char message[1024];
    sprintf(message, "0x%.4X", wxGetApp().cpu->get_PC());
    debugger_pc->Clear();
    debugger_pc->AppendText(message);
}

void MemoryWindow::OnCPUSteps(wxCommandEvent& event)
{
    if(wxGetApp().registers != NULL) wxGetApp().open_registers();
    std::string steps_str = wxGetApp().memory->cpu_steps_count->GetValue().ToStdString();
    uint64_t steps = atoi(steps_str.c_str());

    std::string status;

    while(steps--)
    {
        try
        {
            status = wxGetApp().cpu->step();
        }
        catch(std::string e)
        {
            std::cout << e << std::endl;
        }
        if(wxGetApp().log) wxGetApp().log->log_listbox->Append(status);
        std::cout << status << std::endl;
    }
    view(start_addr, end_addr);

    char message[1024];
    sprintf(message, "0x%.4X", wxGetApp().cpu->get_PC());
    debugger_pc->Clear();
    debugger_pc->AppendText(message);
}
