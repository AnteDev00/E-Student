//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ES_Shared_Header.h"
#include "ES_Client_ProfessorF.h"
#include "ES_Client_LoginF.h"
#include "ES_Client_FontChangerF.h"

#include <System.SysUtils.hpp> // for encoding/decoding  && curr datev
#include <Registry.hpp> // for ini
#include <system.hash.hpp> // for Hash


	#include "ES_Client_LoginF.h"
		#include <memory>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uTPLb_BaseNonVisualComponent"
#pragma link "uTPLb_Codec"
#pragma link "uTPLb_CryptographicLibrary"
#pragma link "uTPLb_Signatory"
#pragma resource "*.dfm"
TFMainProfessor *FMainProfessor;

extern UnicodeString FULLNAME;

extern AnsiString c_privateKeyFileName;
extern AnsiString c_publicKeyFileName;
extern AnsiString s_privateKeyFileName;
extern AnsiString s_publicKeyFileName;



int THEME_FLAG = 1;

int BASE_WIDTH = 764;
int	BASE_HEIGHT = 503;

int FILTER_FLAG = 0;

//---------------------------------------------------------------------------
__fastcall TFMainProfessor::TFMainProfessor(TComponent* Owner)
	: TForm(Owner)
{
		// Caption change
	FMainProfessor->Caption += FULLNAME;

		// Setting the Professor name
	DBEditProfessorName->Text = FULLNAME;
	DBEditProfessorName->ReadOnly = True;

		// Filtering Grades by Proffesor name
	FilterTable(TGrades, "Professor name", FULLNAME);


		// Making the Date Current
	TDateTime currentDate = Date();

	Word year, month, day;
	DecodeDate(currentDate, year, month, day);

	TDateTime dateOnly = EncodeDate(year, month, day);

	DTPGradeDate->Date = dateOnly;

		// INI Stuff
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\PROFESSOR_STARTUP.INI");

	Left = ini->ReadInteger("Main Window", "mainL", 900);
	Top = ini->ReadInteger("Main Window", "mainT", 450);
	Width = ini->ReadInteger("Main Window", "mainW", 728);
	Height = ini->ReadInteger("Main Window", "mainH", 550);


		// Fonts
	DBGrid1->Font->Size = ini->ReadInteger("Main Window", "tableFont", 9);

	BAddGrade->Font->Size = ini->ReadInteger("Main Window", "buttonsFont", 9);
	BDeleteSelGrade->Font->Size = ini->ReadInteger("Main Window", "buttonsFont", 9);

	CBStudentName->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);
	CBGrade->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);
	DTPGradeDate->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);
	EComment->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);
	CBSubject->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);
	DBEditProfessorName->Font->Size = ini->ReadInteger("Main Window", "fieldsFont", 9);

	Label1->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);
	Label2->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);
	Label3->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);
	Label4->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);
	Label5->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);
	Label6->Font->Size = ini->ReadInteger("Main Window", "labelsFont", 9);


		// Last state of Table Columns Widths
	for(int i=0; i<7; i++)
		DBGrid1->Columns->Items[i]->Width = ini->ReadInteger("Main Window", "TableColWidth" + IntToStr(0), 75);

		// Theme flag
	THEME_FLAG = ini->ReadInteger("Main Window", "THEME_FLAG", 1);
	if (THEME_FLAG == DARK_MODE_FLAG)
		DarkModeClick(this);
	else
		LightModeClick(this);

	delete ini;


}

//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::BAddGradeClick(TObject *Sender)
{
		// Check if Any Field is EMPTY
	if(CBStudentName->Text == "" || CBSubject->Text == ""
	|| CBGrade->ItemIndex == -1 || DTPGradeDate->Date.toAnsiString() == "")
	{
		ShowMessage("Input all fields");
		return;
	}

		// CONNECT
	if (!TCPClient->Connected())
	{
		TCPClient->Host = "127.0.0.1";
		TCPClient->Port = 8001;
		TCPClient->Connect();
	}

		// Prepare
	int studentID = TStudentNames->FieldByName("Student ID")->AsInteger;
	int subjectID = TSubjects->FieldByName("Subject ID")->AsInteger;
	int grade = CBGrade->Text.ToInt();
	String gradeDate = DTPGradeDate->Date.toAnsiString();

	FilterTable(TProfessorNames, "Full name", FULLNAME);
	int professorID = TProfessorNames->FieldByName("Professor ID")->AsInteger;
    UnfilterTable(TProfessorNames);

	String comment = EComment->Text;
	//String hash = CombineMessage(studentID,subjectID,grade,gradeDate,professorID,comment);


	/*
	std::unique_ptr<THashSHA2> sha2(new THashSHA2);
	hash = sha2->GetHashString(hash);

	CodecRSA->BlockCipherId = "native.RSA";
	CodecRSA->ChainModeId = "native.CBC";

		// This part exists because LockBox sucks
//---------------------------------------------------
		// Load Clients public key
	std::unique_ptr<TMemoryStream> publicKey_C(new TMemoryStream);
	publicKey_C->LoadFromFile("public_key_C.bin");

	Signatory1->LoadKeysFromStream(publicKey_C.get(), TKeyStoragePartSet() << partPublic);
	// Encrypt using clients public key
	String hashPlusRSA2;
	CodecRSA->EncryptString(hash, hashPlusRSA2, TEncoding::UTF8);
//------------------------------------------------------

		// Load Clients priv key
	std::unique_ptr<TMemoryStream> privateKey_C(new TMemoryStream);
	privateKey_C->LoadFromFile("private_key_C.bin");

	Signatory1->LoadKeysFromStream(privateKey_C.get(), TKeyStoragePartSet() << partPrivate);
	// Encrypt using clients private key
	String hashPlusRSA;
	CodecRSA->EncryptString(hash, hashPlusRSA, TEncoding::UTF8);
	*/
		// Send To Database !
	TCPClient->Socket->Write  (WRITE_FLAG); 	// Send #1

	TCPClient->Socket->Write  (studentID);      // Send #2
	TCPClient->Socket->Write  (subjectID);  	// Send #3
	TCPClient->Socket->Write  (grade);  		// Send #4
	TCPClient->Socket->WriteLn(gradeDate);  	// Send #5
	TCPClient->Socket->Write  (professorID);    // Send #6
	TCPClient->Socket->WriteLn(comment, IndyTextEncoding_UTF8());  // Send #7
	//TCPClient->Socket->WriteLn(hashPlusRSA, IndyTextEncoding_UTF8());  // Send #7


		// Receive answer
	int WRITE_SUCCESS = 0;
	WRITE_SUCCESS = TCPClient->Socket->ReadInt32(); // Receive #1

		// Disconnect
	TCPClient->Disconnect();

		// Add to local DB
	if(WRITE_SUCCESS == 1)
	{
			// Add
		TGrades->Append(); // dodaj novi zapis
		TGrades->FieldByName("Full Name")->AsString = CBStudentName->Text;
		TGrades->FieldByName("Subject")->AsString = CBSubject->Text;
		TGrades->FieldByName("Grade")->AsString = grade;
		TGrades->FieldByName("Grade date")->AsString = gradeDate;
		TGrades->FieldByName("Professor name")->AsString = FULLNAME;
		TGrades->FieldByName("Comment")->AsString = comment;
		TGrades->Post(); // pohrani zapis
		ShowMessage("Grade added.");
	}
	else
		ShowMessage("Server side error! \nGrade NOT added.");

		// Partial clean up
	CBGrade->ItemIndex = -1 ;

		// Filters
	TStudentNames->Filter = "";
	TSubjects->Filter = "";
	TProfessorNames->Filter = "";
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::BDeleteSelGradeClick(TObject *Sender)
{
		// Check if empty
	if(!(TGrades->RecordCount > 0))
	{
		ShowMessage("There is nothing to delete.");
		return;
	}

	int answer = Application->MessageBoxW(L"Are you sure you want to Delete Current Grade? \nAll changes are recorded.",L"Delete Current Grade?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);

	if(answer == IDYES)
	{
			// CONNECT
		if (!TCPClient->Connected())
		{
			TCPClient->Host = "127.0.0.1";
			TCPClient->Port = 8001;
			TCPClient->Connect();
		}
			//Prepare
			FilterTable(TStudentNames, "Full name", TGrades->FieldByName("Full name")->AsString);
		int studentID = TStudentNames->FieldByName("Student ID")->AsInteger;
			TStudentNames->Filter = "";

			FilterTable(TSubjects, "Subject name", TGrades->FieldByName("Subject")->AsString);
		int subjectID = TSubjects->FieldByName("Subject ID")->AsInteger;
			TSubjects->Filter = "";

		int grade = TGrades->FieldByName("Grade")->AsInteger;
		String gradeDate = TGrades->FieldByName("Grade date")->AsString;
		String comment = TGrades->FieldByName("Comment")->AsString;
		String professorName = FULLNAME;

			// Send
		TCPClient->Socket->Write(DELETE_FLAG);      // Send #1
		TCPClient->Socket->Write(studentID);        // Send #2
		TCPClient->Socket->Write(subjectID);        // Send #3
		TCPClient->Socket->Write(grade);        	// Send #4
		TCPClient->Socket->WriteLn(gradeDate);      // Send #5
		TCPClient->Socket->WriteLn(comment, IndyTextEncoding_UTF8()); // Send #6
		TCPClient->Socket->WriteLn(professorName, IndyTextEncoding_UTF8()); // Send #7

			// Receive
		int DELETE_SUCCESS = 0;
		DELETE_SUCCESS = TCPClient->Socket->ReadInt32(); // Receive #1

			// Disconnect
		TCPClient->Disconnect();

			// Delete Locally
		if(DELETE_SUCCESS == 1)
		{
			TGrades->Delete();
			ShowMessage("Grade DELETED successfuly");
		}
		else
		{
			ShowMessage("Grade was NOT deleted successfuly");
		}

	}
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::BFilterByClick(TObject *Sender)
{
	FILTER_FLAG++;
	if (FILTER_FLAG % 2 == true)
	{
		FilterTable(TGrades, "Full name", CBStudentName->Text);
		FilterTableExtend(TGrades, "Professor name", FULLNAME);
		BFilterBy->Font->Style = BFilterBy->Font->Style << fsBold << fsUnderline;
		BFilterBy->Caption = "Unfilter";
	}
	else
	{
		TGrades->Filter = "";
		FilterTable(TGrades, "Professor name", FULLNAME);
		BFilterBy->Font->Style = BFilterBy->Font->Style >> fsBold >> fsUnderline;
		BFilterBy->Caption = "Filter";
	}

}



void __fastcall TFMainProfessor::DBGrid1TitleClick(TColumn *Column)
{
	String columnName = Column->FieldName;

	if (columnName == "Full name")
	{
		if (TGrades->IndexFieldNames == "[Full name] DESC")
			TGrades->IndexFieldNames = "[Full name] ASC";
		else
			TGrades->IndexFieldNames = "[Full name] DESC";
	}
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

void __fastcall TFMainProfessor::FormClose(TObject *Sender, TCloseAction &Action)
{
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\PROFESSOR_STARTUP.INI");
	// write to [Main Window]
	ini->WriteInteger("Main Window", "mainL", Left);
	ini->WriteInteger("Main Window", "mainT", Top);
	ini->WriteInteger("Main Window", "mainW", Width);
	ini->WriteInteger("Main Window", "mainH", Height);

	ini->WriteInteger("Main Window", "tableFont", DBGrid1->Font->Size);
	ini->WriteInteger("Main Window", "buttonsFont", BAddGrade->Font->Size);
	ini->WriteInteger("Main Window", "fieldsFont", CBStudentName->Font->Size);
	ini->WriteInteger("Main Window", "labelsFont", Label1->Font->Size);


	for(int i=0 ; i < 7; i++)
		ini->WriteInteger("Main Window", "TableColWidth" + IntToStr(i), DBGrid1->Columns->Items[i]->Width);

	ini->WriteInteger("Main Window", "THEME_FLAG", THEME_FLAG);

	delete ini;

	int answer = Application->MessageBoxW(L"You're about to close the application.",L"Exit?",MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
	if (answer == mrNo)
	{   // stop the app from closing
		Action = caNone;
	}

}

//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::MMFontChangeDialog(TObject *Sender)
{
	Application->CreateForm(__classid(TFFontChanger), &FFontChanger);
	FFontChanger->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::DarkModeClick(TObject *Sender)
{
	THEME_FLAG = 0;

	FMainProfessor->Color = clGray;

	for(int i = 0; i < 7; i++)
		DBGrid1->Columns->Items[i]->Color = clMedGray;

	LTitleStudentGrades->Font->Color = clCream;
	GroupBox1->Color = clMedGray;
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::LightModeClick(TObject *Sender)
{
    THEME_FLAG = 1;

	FMainProfessor->Color = clBtnFace;

	for(int i = 0; i < 7; i++)
		DBGrid1->Columns->Items[i]->Color = clWhite;

	LTitleStudentGrades->Font->Color = clWindowText;

	GroupBox1->Color =  clBtnFace;

}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::FormResize(TObject *Sender)
{
	// responsive columns
	if (FMainProfessor->Width > 736)
	{
		int colCount = DBGrid1->Columns->Count;
		DBGrid1->Columns->Items[0]->Width = ((DBGrid1->Width - 40) / colCount) * 0.5;
		DBGrid1->Columns->Items[1]->Width = (DBGrid1->Width - 40) / colCount;
		DBGrid1->Columns->Items[2]->Width = ((DBGrid1->Width - 40) / colCount) * 0.5;
		DBGrid1->Columns->Items[3]->Width = (DBGrid1->Width - 40) / colCount;
		DBGrid1->Columns->Items[4]->Width = (DBGrid1->Width - 40) / colCount;
		DBGrid1->Columns->Items[5]->Width = (DBGrid1->Width - 40) / colCount;
		DBGrid1->Columns->Items[6]->Width = ((DBGrid1->Width - 40) / colCount) * 2;
	}


	
	if (FMainProfessor->Width < 736)
	{
		FMainProfessor->Width = BASE_WIDTH;
		ShowMessage("Minimum width allowed!");
	}

	if (FMainProfessor->Height < 482)
	{
		FMainProfessor->Height = BASE_HEIGHT;
		ShowMessage("Minimum height allowed!");
	}


}
//---------------------------------------------------------------------------

//	BASE_WIDTH = 764;
//	BASE_HEIGHT = 503; 

void __fastcall TFMainProfessor::ExtraSmallClick(TObject *Sender)
{
	FMainProfessor->Width = 879;
	FMainProfessor->Height = 576;

	FMainProfessor->Width++;	
	TFMainProfessor::FormResize(this);
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::SmallClick(TObject *Sender)
{
	FMainProfessor->Width = 1014;
	FMainProfessor->Height = 721;
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::MediumClick(TObject *Sender)
{
	FMainProfessor->Width = 1222;
	FMainProfessor->Height = 887;	
}                                
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::LargeClick(TObject *Sender)
{
	FMainProfessor->Width = 1540;
	FMainProfessor->Height = 1130;	
}
//---------------------------------------------------------------------------

void __fastcall TFMainProfessor::FullscreenClick(TObject *Sender)
{
	FMainProfessor->WindowState = wsMaximized;	
}
//---------------------------------------------------------------------------



/*
	// spread evenly new amount of width (positive or negative)
		int widthDelta = DBGrid1->Width - dbGridWidth;
		for(int i = 0; i < 7; i++)
		{
			if(DBGrid1->Columns->Items[i]->Width < 66)
				DBGrid1->Columns->Items[i]->Width = 67;
			else
				DBGrid1->Columns->Items[i]->Width += widthDelta / 7;
		}

			//calculate the dif between Dbgrid width and Sum of all Columns
		int colsWidthSum = 0;
		for(int i = 0; i < 7; i++)
			colsWidthSum += DBGrid1->Columns->Items[i]->Width;

			 // if dbgrid bigger -> last element bigger,    else make all smaller
		if (colsWidthSum < dbGridWidth)
			DBGrid1->Columns->Items[6]->Width += (DBGrid1->Width - colsWidthSum) -45;
		else
			for(int i = 0; i < 7; i++)
				if(DBGrid1->Columns->Items[i]->Width < 66)
					DBGrid1->Columns->Items[i]->Width = 67;
				else
					DBGrid1->Columns->Items[i]->Width += (DBGrid1->Width - colsWidthSum ) / 7;					
	
		dbGridWidth = DBGrid1->Width;
*/
void __fastcall TFMainProfessor::Button1Click(TObject *Sender)
{
    DBGrid1->Color = clBlack;

}
//---------------------------------------------------------------------------

