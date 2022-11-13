#include "wxcraziNES.hpp"
#include "LogWindow.hpp"

wxDECLARE_APP(wxcraziNES);


LogWindow::LogWindow(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, wxID_ANY, title, pos, size)
{   
}

LogWindow::~LogWindow()
{
    wxGetApp().log = NULL;
}
