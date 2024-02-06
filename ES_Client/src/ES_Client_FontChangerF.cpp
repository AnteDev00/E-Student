//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ES_Client_FontChangerF.h"
#include "ES_Client_ProfessorF.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFFontChanger *FFontChanger;
//---------------------------------------------------------------------------
__fastcall TFFontChanger::TFFontChanger(TComponent* Owner)
	: TForm(Owner)
{
	EGradesFont->Text = FMainProfessor->DBGrid1->Font->Size;

	EButtonsFont->Text = FMainProfessor->BAddGrade->Font->Size;

	EFieldsFont->Text = FMainProfessor->CBStudentName->Font->Size;

	ELabelsFont->Text = FMainProfessor->Label1->Font->Size;
}
//---------------------------------------------------------------------------
void __fastcall TFFontChanger::Button2Click(TObject *Sender)
{
	FFontChanger->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFFontChanger::Button1Click(TObject *Sender)
{
		//table
	FMainProfessor->DBGrid1->Font->Size = StrToInt(EGradesFont->Text);
		//buttons
	FMainProfessor->BAddGrade->Font->Size = StrToInt(EButtonsFont->Text);
	FMainProfessor->BDeleteSelGrade->Font->Size = StrToInt(EButtonsFont->Text);
		//fields
	FMainProfessor->CBStudentName->Font->Size = StrToInt(EFieldsFont->Text);
	FMainProfessor->CBGrade->Font->Size = StrToInt(EFieldsFont->Text);
	FMainProfessor->DTPGradeDate->Font->Size = StrToInt(EFieldsFont->Text);
	FMainProfessor->EComment->Font->Size = StrToInt(EFieldsFont->Text);
	FMainProfessor->CBSubject->Font->Size = StrToInt(EFieldsFont->Text);
	FMainProfessor->DBEditProfessorName->Font->Size = StrToInt(EFieldsFont->Text);
    		//labels
	FMainProfessor->Label1->Font->Size = StrToInt(ELabelsFont->Text);
	FMainProfessor->Label2->Font->Size = StrToInt(ELabelsFont->Text);
	FMainProfessor->Label3->Font->Size = StrToInt(ELabelsFont->Text);
	FMainProfessor->Label4->Font->Size = StrToInt(ELabelsFont->Text);
	FMainProfessor->Label5->Font->Size = StrToInt(ELabelsFont->Text);
	FMainProfessor->Label6->Font->Size = StrToInt(ELabelsFont->Text);

	FFontChanger->Hide();
}
//---------------------------------------------------------------------------
