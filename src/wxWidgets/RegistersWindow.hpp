#pragma once

#include "wxcraziNES.hpp"

class RegistersWindow: public wxFrame
{
  public:
    RegistersWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~RegistersWindow();
    wxListBox *registers_listbox = NULL;
};
