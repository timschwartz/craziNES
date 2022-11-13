#include "RegistersWindow.hpp"

wxDECLARE_APP(wxcraziNES);

RegistersWindow::RegistersWindow(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(parent, wxID_ANY, title, pos, size)
{   
}

RegistersWindow::~RegistersWindow()
{
    wxGetApp().registers = NULL;
}
