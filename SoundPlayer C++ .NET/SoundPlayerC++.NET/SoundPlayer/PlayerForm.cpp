#include "PlayerForm.h"
 
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace System::Diagnostics;

[STAThreadAttribute]
void Main()
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
    SoundPlayer::PlayerForm plForm;
	Application::Run(%plForm);	
	 
}