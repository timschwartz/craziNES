#include <craziNES.h>
#include <cpu_6502.h>
#include <memory.h>
#include <rom.h>

nes::cpu_6502 *cpu = new nes::cpu_6502();

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_open_rom,   MainWindow::OnOpenROM)
    EVT_MENU(ID_debug_rom, MainWindow::OnDebugROM)
    EVT_MENU(ID_debug_registers, MainWindow::OnDebugRegisters)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(wxcraziNES);

bool wxcraziNES::OnInit()
{
    frame = new MainWindow("craziNES", wxPoint(20, 20), wxSize(640, 480));
    frame->Show(true);
    return true;
}

void MainWindow::OnOpenROM(wxCommandEvent& event)
{
    wxFileDialog rom_dialog(this, _("Open NES ROM file"), "", "",
                                "NES ROM files (*.nes;*.bin)|*.nes;*.bin|All files (*.*)|*.*",
                                wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if(wxID_CANCEL == rom_dialog.ShowModal()) return;

    std::string filename = rom_dialog.GetPath().ToStdString();
    if(!filename.size()) return;

    try
    {
        cpu->load_rom(filename);
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        return;
    }
}

void MainWindow::OnDebugROM(wxCommandEvent& event)
{
    open_memory();
    wxGetApp().memory->view(0xC5F5, 0xFFFF);
}

void MainWindow::OnDebugRegisters(wxCommandEvent &event)
{
    open_registers();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    this->SetBackgroundColour(wxColour(*wxBLACK));

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_open_rom, "&Open ROM...\tCtrl-H", "");
    menuFile->Append(wxID_EXIT);

    wxMenu *menuDebug = new wxMenu;
    menuDebug->Append(ID_debug_rom, "View &ROM", "");
    menuDebug->Append(ID_debug_registers, "View R&egisters", "");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuDebug, "&Debug");
    SetMenuBar(menuBar);

    CreateStatusBar(1);

    if(wxGetApp().argc < 2) return;

    try
    {
        cpu->load_rom(wxGetApp().argv[1].ToStdString());
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        wxGetApp().frame->SetStatusText(e, 0);
        return;
    }

    std::string message = "Loaded " + wxGetApp().argv[1].ToStdString();
    SetStatusText(message, 0);    

    uint32_t counter = 0;
    std::string arg2 = wxGetApp().argv[2].ToStdString();
    uint32_t max = atoi(arg2.c_str());

    while(counter < max)
    {
        try
        {
            SetStatusText(cpu->step(), 0);
        }
        catch(std::string e)
        {
            SetStatusText(e, 0);
            return;
        }
        counter++;
    }
}
