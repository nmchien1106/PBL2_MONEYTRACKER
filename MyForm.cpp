#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

using namespace MyNamespace;
[STAThreadAttribute]
int main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MyForm form;
	Application::Run(%form);
	return 0;
}