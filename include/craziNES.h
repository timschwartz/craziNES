#ifndef _CRAZINES_H
#define _CRAZINES_H

#include <wx/wx.h>

class MainWindow: public wxFrame
{
  public:
    MainWindow(const wxString &title, const wxPoint &pos, const wxSize &size);
  private:
    void OnExit(wxCommandEvent &event);
    wxDECLARE_EVENT_TABLE();
};

class wxcraziNES: public wxApp
{
  public:
    virtual bool OnInit();
    MainWindow *frame = NULL;
};

#endif
