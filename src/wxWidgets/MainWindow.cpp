#include "MainWindow.hpp"
#include "../nes/palette.hpp"
#include <cstdlib>
#include <ctime>

wxDECLARE_APP(wxcraziNES);

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(ID_open_rom,   MainWindow::OnOpenROM)
    EVT_MENU(ID_debug_rom, MainWindow::OnDebugROM)
    EVT_MENU(ID_debug_registers, MainWindow::OnDebugRegisters)
    EVT_MENU(ID_debug_log, MainWindow::OnDebugLog)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
    EVT_PAINT(MainWindow::OnPaint)
wxEND_EVENT_TABLE()

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
        wxGetApp().cpu->load_rom(filename);
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        return;
    }
}

void MainWindow::OnDebugROM(wxCommandEvent& event)
{
    wxGetApp().open_memory();
    wxGetApp().memory->view(wxGetApp().cpu->get_PC(), wxGetApp().cpu->get_PC() + 0x400);
}

void MainWindow::OnDebugRegisters(wxCommandEvent &event)
{
    wxGetApp().open_registers();
}

void MainWindow::OnDebugLog(wxCommandEvent &event)
{
    wxGetApp().open_log();
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
//    this->SetBackgroundColour(wxColour(*wxBLACK));
    wxGetApp().cpu = new nes::cpu_6502(this->screen);

    std::srand(std::time(nullptr));
    uint8_t color_index = std::rand() % 64;
    for(uint32_t counter = 0; counter < (256 * 240 * 3); counter += 3)
    {
        this->screen[counter + 0] = nes::palette[color_index].x;
        this->screen[counter + 1] = nes::palette[color_index].y;
        this->screen[counter + 2] = nes::palette[color_index].z;
    }

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_open_rom, "&Open ROM...\tCtrl-H", "");
    menuFile->Append(wxID_EXIT);

    wxMenu *menuDebug = new wxMenu;
    menuDebug->Append(ID_debug_rom, "View &ROM", "");
    menuDebug->Append(ID_debug_registers, "View R&egisters", "");
    menuDebug->Append(ID_debug_log, "View &Log", "");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuDebug, "&Debug");
    SetMenuBar(menuBar);

    CreateStatusBar(1);

    if(wxGetApp().argc < 2) return;

    try
    {
        std::string argv1 = wxGetApp().argv[1].ToStdString();
        wxGetApp().cpu->load_rom(wxGetApp().argv[1].ToStdString());
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        wxGetApp().frame->SetStatusText(e, 0);
        return;
    }

    std::string message = "Loaded " + wxGetApp().argv[1].ToStdString();
    std::cout << message << std::endl;
    SetStatusText(message, 0);    

    uint32_t counter = 0;
    std::string arg2 = wxGetApp().argv[2].ToStdString();
    uint32_t max = atoi(arg2.c_str());

    std::cout << "Running " << arg2 << " instructions." << std::endl;
    
    while(counter < max)
    {
        try
        {
            std::string status = wxGetApp().cpu->step();
            std::cout << status << std::endl;
//            SetStatusText(status.c_str(), 0);
        }
        catch(std::string e)
        {
            std::cout << e << std::endl;
            SetStatusText(e, 0);
            std::cout << "Clock cycles: " << std::dec << +(wxGetApp().cpu->get_cycles()) << std::endl;
            exit(0);
            return;
        }
        counter++;
    }

    std::cout << "Clock cycles: " << std::dec << +(wxGetApp().cpu->get_cycles()) << std::endl;
//    exit(0);
}

void MainWindow::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    wxImage img(256, 240, this->screen, true);
    wxBitmap bmp(img, 24);
    dc.DrawBitmap(bmp, 0, 0, false);
}
