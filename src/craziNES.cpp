#include <craziNES.h>
#include <memory.h>
#include <rom.h>

nes::MMU *mmu = new nes::MMU();

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(wxcraziNES);

bool wxcraziNES::OnInit()
{
    frame = new MainWindow("craziNES", wxPoint(20, 20), wxSize(640, 480));
    frame->Show(true);
    return true;
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
    menuFile->Append(wxID_EXIT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    CreateStatusBar(1);

    if(wxGetApp().argc < 2) return;

    try
    {
        mmu->load_rom(wxGetApp().argv[1].ToStdString());
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        wxGetApp().frame->SetStatusText(e, 0);
        return;
    }

    std::string message = "Loaded " + wxGetApp().argv[1].ToStdString();
    SetStatusText(message, 0);    
}
