//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>

#include "ES_Client_LoginF.h"
#include <memory>

//---------------------------------------------------------------------------
USEFORM("ES_Client_ProfessorF.cpp", FMainProfessor);
USEFORM("ES_Client_StudentF.cpp", FMainStudent);
USEFORM("ES_Client_FontChangerF.cpp", FFontChanger);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		std::unique_ptr<TFLogin> FLogin(new TFLogin(NULL));
		int loginResult = FLogin->ShowModal();  // Store the modal result

		if (loginResult == mrYes)
		{
			// User is a professor
			Application->CreateForm(__classid(TFMainProfessor), &FMainProfessor);
			Application->Run();
		}
		else if (loginResult == mrNo)
		{
			Application->CreateForm(__classid(TFMainStudent), &FMainStudent);
			Application->Run();
		}
		//else ShowMessage("Login aborted");

	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
