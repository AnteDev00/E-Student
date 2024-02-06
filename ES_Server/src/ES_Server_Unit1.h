//---------------------------------------------------------------------------

#ifndef ES_Server_Unit1H
#define ES_Server_Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdContext.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "uTPLb_BaseNonVisualComponent.hpp"
#include "uTPLb_Codec.hpp"
#include "uTPLb_CryptographicLibrary.hpp"
#include "uTPLb_Signatory.hpp"
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TADOConnection *CPasswordsDB;
	TDataSource *DSPasswords;
	TDBGrid *DBGrid1;
	TADOTable *TPasswords;
	TIdTCPServer *TCPServer;
	TLabel *LServerStatus;
	TLabel *Label1;
	TDataSource *DSGrades;
	TADOTable *TGrades;
	TDBGrid *DBGrid2;
	TLabel *LGrades;
	TDataSource *DSStudents;
	TADOTable *TStudentNames;
	TDBGrid *DBGrid3;
	TDataSource *DSProfessorNames;
	TADOTable *TProfessorNames;
	TDBGrid *DBGrid4;
	TDataSource *DSSubjects;
	TADOTable *TSubjects;
	TDBGrid *DBGrid5;
	TDBNavigator *DBNavigator1;
	TDBNavigator *DBNavigator2;
	TADOConnection *CMainDB;
	TCryptographicLibrary *CryptographicLibrary1;
	TCodec *CodecRSA;
	TSignatory *Signatory1;
	TCodec *CodecAES;
	void __fastcall TCPServerExecute(TIdContext *AContext);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
