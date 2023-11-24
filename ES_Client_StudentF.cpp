//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ES_Shared_Header.h"

#include "ES_Client_StudentF.h"

#include "ES_Client_LoginF.h"

#include <Registry.hpp> // for ini

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFMainStudent *FMainStudent;

extern UnicodeString FULLNAME;


//---------------------------------------------------------------------------
__fastcall TFMainStudent::TFMainStudent(TComponent* Owner)
	: TForm(Owner)
{
		// caption change
	FMainStudent->Caption += FULLNAME;

		// Filtering Grades by Student name
	FilterTable(TGrades, "Full name", FULLNAME);
	FilterTableExtend(TGrades, "Subject", "");

		// Get the subjects into Listbox
	for(int i = 0 ; i < TSubjects->RecordCount; i++)
	{
		TSubjects->RecNo = i + 1;  // Set the current record

		// Access the elements of the current row
		String subject = TSubjects->FieldByName("Subject name")->AsString;

		// Do something with the accessed data
		LBSubjects->AddItem(subject,FMainStudent);

		// Move to the next record
		TSubjects->Next();
	}

		// INI stuff
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\STUDENT_STARTUP.INI");
	// read from [Main Window]
	Left = ini->ReadInteger("Main Window", "LEFT", 900);
	Top = ini->ReadInteger("Main Window", "TOP", 450);
	Width = ini->ReadInteger("Main Window", "WIDTH", 802);
	Height = ini->ReadInteger("Main Window", "HEIGHT", 473);
	delete ini;

}



 	// Filter by Table COLUMN
//---------------------------------------------------------------------------

void __fastcall TFMainStudent::DBGrid1TitleClick(TColumn *Column)
{
	String columnName = Column->FieldName;

	if (columnName == "Grade")
	{
		if (TGrades->IndexFieldNames == "[Grade] DESC")
			TGrades->IndexFieldNames = "[Grade] ASC";
		else
			TGrades->IndexFieldNames = "[Grade] DESC";
	}
	else if (columnName == "Comment")
	{
		if (TGrades->IndexFieldNames == "[Comment] DESC")
			TGrades->IndexFieldNames = "[Comment] ASC";
		else
			TGrades->IndexFieldNames = "[Comment] DESC";
	}
	else if (columnName == "Grade date")
	{
		if (TGrades->IndexFieldNames == "[Grade date] DESC")
			TGrades->IndexFieldNames = "[Grade date] ASC";
		else
			TGrades->IndexFieldNames = "[Grade date] DESC";
	}

}


//---------------------------------------------------------------------------

void __fastcall TFMainStudent::LBSubjectsClick(TObject *Sender)
{
		// Filter by Subject
	for(int i=0 ; i < LBSubjects->Count; i++)
	{
		if(LBSubjects->Selected[i] == true)
		{
			String subject = LBSubjects->Items->Strings[i];
				FilterTable(TGrades, "Full name", FULLNAME);
				FilterTableExtend(TGrades, "Subject", subject);
		}
	}

		// Calculate AVG for That Subject
	int sumGrades = 0;
	int grade = 0;
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	int c4 = 0;
	int c5 = 0;
;
	for (int i = 0; i < TGrades->RecordCount; i++)
	{
		TGrades->RecNo = i + 1;  // Set the current record

		// Access the elements of the current row
		grade = TGrades->FieldByName("Grade")->AsInteger;

		// Do something with the accessed data
		sumGrades += grade;
		switch(grade)
		{
			case 1: c1++;
			break;
			case 2: c2++;
			break;
			case 3: c3++;
			break;
			case 4: c4++;
			break;
			case 5: c5++;
			break;
		}

		// Move to the next record
		TGrades->Next();
	}
	float avg = (float)sumGrades / TGrades->RecordCount;

	UnicodeString formattedAvg = FormatFloat("#0.00", avg); // Format the average with two decimal places

	// update captions
	LAVG->Caption = formattedAvg;

	L1s->Caption = IntToStr(c1);
	L2s->Caption = IntToStr(c2);
	L3s->Caption = IntToStr(c3);
	L4s->Caption = IntToStr(c4);
	L5s->Caption = IntToStr(c5);

	// reset
	TGrades->RecNo = 1;

}

//---------------------------------------------------------------------------
/*
void __fastcall TFMainStudent::Button1Click(TObject *Sender)
{

	// Calculate AVG of ALL Subjects Combined
	float sumOfAVGs = 0;
	LBSubjects->ItemIndex = 0;
	int subjectsAmount = TSubjects->RecordCount;
	for(int i = LBSubjects->ItemIndex ; i < subjectsAmount; i++)
	{
		sumOfAVGs += StrToFloat(LAVG->Caption);
		LBSubjects->ItemIndex += 1;
		ShowMessage(sumOfAVGs);
	}
	float avg = (float)sumOfAVGs / TSubjects->RecordCount;

	UnicodeString formattedAvg = FormatFloat("#0.00", avg); // Format the average with two decimal places

	LAllSubjectsAVG->Caption = formattedAvg;

	// reset
	TGrades->RecNo = 1;
	LBSubjects->ItemIndex = -1;

}
//---------------------------------------------------------------------------
*/
void __fastcall TFMainStudent::FormClose(TObject *Sender, TCloseAction &Action)
{
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\STUDENT_STARTUP.INI");
	// write to [Main Window]
	ini->WriteInteger("Main Window", "LEFT", Left);
	ini->WriteInteger("Main Window", "TOP", Top);
	ini->WriteInteger("Main Window", "WIDTH", Width);
	ini->WriteInteger("Main Window", "HEIGHT", Height);
	delete ini;

	int answer = Application->MessageBoxW(L"You're about to close the application.",L"Exit?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
	if (answer == mrNo)
	{   // stop the app from closing
		Action = caNone;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFMainStudent::FormResize(TObject *Sender)
{

	// responsive columns
	if (FMainStudent->Width > 400)
	{
		int colCount = DBGrid1->Columns->Count;

		DBGrid1->Columns->Items[0]->Width = ((DBGrid1->Width - 40) / colCount) * 0.5;
		DBGrid1->Columns->Items[1]->Width = ((DBGrid1->Width - 40) / colCount) * 1.25;
		DBGrid1->Columns->Items[2]->Width = ((DBGrid1->Width - 40) / colCount) * 0.75;
		DBGrid1->Columns->Items[3]->Width = ((DBGrid1->Width - 40) / colCount) *1.5;
	}



	if (FMainStudent->Width < 623)
	{
		FMainStudent->Width = 630;
		ShowMessage("Minimum width allowed!");
	}

	if (FMainStudent->Height < 344)
	{
		FMainStudent->Height = 350;
		ShowMessage("Minimum height allowed!");
	}

}
//---------------------------------------------------------------------------


	// Mainmenu Window Size
void __fastcall TFMainStudent::ExtrasmallClick(TObject *Sender)
{
	this->Width = 630;
	this->Height = 347;
}
//---------------------------------------------------------------------------
void __fastcall TFMainStudent::SmallClick(TObject *Sender)
{
	this->Width = 630 * 1.25;
	this->Height = 347 * 1.4;
}
//---------------------------------------------------------------------------
void __fastcall TFMainStudent::MediumClick(TObject *Sender)
{
	this->Width = 630 * 1.5;
	this->Height = 347 * 1.75;
}
//---------------------------------------------------------------------------
void __fastcall TFMainStudent::LargeClick(TObject *Sender)
{
	this->Width = 630 * 1.75;
	this->Height = 347 * 2;
}
//---------------------------------------------------------------------------
void __fastcall TFMainStudent::FullscreenClick(TObject *Sender)
{
	this->WindowState = wsMaximized;
}
//---------------------------------------------------------------------------



