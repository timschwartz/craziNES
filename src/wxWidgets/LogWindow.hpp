#pragma once

#include <wx/wx.h>
#include "nes/cpu_6502.hpp"

void open_log();

class LogWindow: public wxFrame
{
  public:
    LogWindow(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size);
    ~LogWindow();
    wxListBox *log_listbox = NULL;
};
