#include <crazines.h>
#include <memory.h>

nes::MMU *mmu = new nes::MMU();

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_MENU(wxID_EXIT,  MainWindow::OnExit)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(wxcrazines);

bool wxcrazines::OnInit()
{
    frame = new MainWindow("crazines", wxPoint(20, 20), wxSize(640, 480));
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

    try
    {
        mmu->write_byte(0x2008, 1);
    }
    catch(std::string e)
    {
        SetStatusText(e, 0);
    }
}
