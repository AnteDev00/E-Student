//---------------------------------------------------------------------------

#ifndef ES_Client_StudentFH
#define ES_Client_StudentFH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.StdActns.hpp>
//---------------------------------------------------------------------------
class TFMainStudent : public TForm
{
__published:	// IDE-managed Components
	TListBox *LBSubjects;
	TLabel *Label1;
	TADOTable *TGrades;
	TADOConnection *ADOConnection1;
	TDataSource *GradesDS;
	TDBGrid *DBGrid1;
	TLabel *Label2;
	TDataSource *SubjectsDS;
	TADOTable *TSubjects;
	TLabel *LAVG;
	TLabel *LAllSubjAvg;
	TLabel *LForThisSubj;
	TLabel *LAllSubjectsAVG;
	TMainMenu *MainMenu1;
	TMenuItem *Application1;
	TMenuItem *Exit1;
	TActionList *ActionList1;
	TFileExit *FileExit1;
	TMenuItem *Window1;
	TMenuItem *Window2;
	TMenuItem *Small;
	TMenuItem *Medium;
	TMenuItem *Large;
	TMenuItem *Fullscreen;
	TMenuItem *Extrasmall;
	TGroupBox *GroupBox1;
	TLabel *L5s;
	TLabel *L4s;
	TLabel *L3s;
	TLabel *L2s;
	TLabel *L1s;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	void __fastcall LBSubjectsClick(TObject *Sender);
	void __fastcall DBGrid1TitleClick(TColumn *Column);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ExtrasmallClick(TObject *Sender);
	void __fastcall SmallClick(TObject *Sender);
	void __fastcall MediumClick(TObject *Sender);
	void __fastcall LargeClick(TObject *Sender);
	void __fastcall FullscreenClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFMainStudent(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFMainStudent *FMainStudent;
//---------------------------------------------------------------------------
#endif
