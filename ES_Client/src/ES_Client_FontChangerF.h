//---------------------------------------------------------------------------

#ifndef ES_Client_FontChangerFH
#define ES_Client_FontChangerFH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFFontChanger : public TForm
{
__published:	// IDE-managed Components
	TLabel *LGradesFont;
	TEdit *EGradesFont;
	TLabel *Label1;
	TEdit *EButtonsFont;
	TLabel *LOthersFont;
	TEdit *EFieldsFont;
	TButton *Button1;
	TButton *Button2;
	TEdit *ELabelsFont;
	TLabel *Label2;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFFontChanger(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFFontChanger *FFontChanger;
//---------------------------------------------------------------------------
#endif
