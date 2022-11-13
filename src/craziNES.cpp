#include "wxWidgets/wxcraziNES.hpp"
#include <cpu_6502.h>
#include "nes/rom.hpp"
#include <cstdlib>
#include <ctime>

nes::cpu_6502 *cpu = nullptr;

wxIMPLEMENT_APP(wxcraziNES);

bool wxcraziNES::OnInit()
{
    frame = new MainWindow("craziNES", wxPoint(20, 20), wxSize(256, 240));
    frame->Show(true);
    return true;
}
