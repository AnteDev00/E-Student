//---------------------------------------------------------------------------

#ifndef ES_Client_ProfessorFH
#define ES_Client_ProfessorFH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ComCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.Menus.hpp>
#include "uTPLb_BaseNonVisualComponent.hpp"
#include "uTPLb_Codec.hpp"
#include "uTPLb_CryptographicLibrary.hpp"
#include "uTPLb_Signatory.hpp"
#include <System.SysUtils.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.StdActns.hpp>
//---------------------------------------------------------------------------
class TFMainProfessor : public TForm
{
__published:	// IDE-managed Components
	TADOConnection *CMainDB;
	TDBGrid *DBGrid1;
	TDataSource *GradesDS;
	TADOTable *TGrades;
	TDBLookupComboBox *CBSubject;
	TADOTable *TStudentNames;
	TADOTable *TSubjects;
	TDataSource *SubjectsDS;
	TDataSource *StudentNamesDS;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TDBLookupComboBox *CBStudentName;
	TDBEdit *DBEditProfessorName;
	TLabel *Label6;
	TComboBox *CBGrade;
	TDateTimePicker *DTPGradeDate;
	TEdit *EComment;
	TButton *BAddGrade;
	TButton *BDeleteSelGrade;
	TIdTCPClient *TCPClient;
	TLabel *LTitleStudentGrades;
	TDBGrid *DBGrid2;
	TDBGrid *DBGrid3;
	TADOTable *TProfessorNames;
	TDataSource *ProfessorNamesDS;
	TDBGrid *DBGrid4;
	TButton *BFilterBy;
	TButton *BFilterNot;
	TMainMenu *MainMenu1;
	TMenuItem *Application1;
	TMenuItem *Application2;
	TGroupBox *GroupBox1;
	TCodec *CodecRSA;
	TSignatory *Signatory1;
	TCryptographicLibrary *CryptographicLibrary1;
	TMenuItem *heme1;
	TMenuItem *DarkMode;
	TMenuItem *LightMode;
	TMenuItem *Edit1;
	TMenuItem *Logout2;
	TActionList *ActionList1;
	TFileExit *FileExit1;
	TMenuItem *Window1;
	TMenuItem *Window2;
	TMenuItem *Small;
	TMenuItem *Medium;
	TMenuItem *Large;
	TMenuItem *Fullscreen;
	TMenuItem *ExtraSmall;
	void __fastcall BAddGradeClick(TObject *Sender);
	void __fastcall BDeleteSelGradeClick(TObject *Sender);
	void __fastcall BFilterByClick(TObject *Sender);
	void __fastcall DBGrid1TitleClick(TColumn *Column);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall MMFontChangeDialog(TObject *Sender);
	void __fastcall DarkModeClick(TObject *Sender);
	void __fastcall LightModeClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ExtraSmallClick(TObject *Sender);
	void __fastcall SmallClick(TObject *Sender);
	void __fastcall MediumClick(TObject *Sender);
	void __fastcall LargeClick(TObject *Sender);
	void __fastcall FullscreenClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFMainProfessor(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFMainProfessor *FMainProfessor;
//---------------------------------------------------------------------------
#endif
