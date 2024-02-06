//---------------------------------------------------------------------------

#ifndef ES_Client_LoginFH
#define ES_Client_LoginFH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include "uTPLb_BaseNonVisualComponent.hpp"
#include "uTPLb_Codec.hpp"
#include "uTPLb_CryptographicLibrary.hpp"
#include "uTPLb_Signatory.hpp"
#include <System.SysUtils.hpp>
#include <Vcl.Menus.hpp>

#include <map>

//---------------------------------------------------------------------------
class TFLogin : public TForm
{
__published:	// IDE-managed Components
	TLabel *LFullName;
	TEdit *EFullName;
	TLabel *LPassword;
	TEdit *EPassword;
	TButton *BLogin;
	TIdTCPClient *TCPClient;
	TCheckBox *CBSaveCredentials;
	TCodec *CodecRSA;
	TCryptographicLibrary *CryptographicLibrary1;
	TSignatory *Signatory1;
	TCodec *CodecAES;
	TMainMenu *MainMenu1;
	TMenuItem *Language1;
	TMenuItem *Croatian;
	TMenuItem *English;
	void __fastcall BLoginClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CroatianClick(TObject *Sender);
	void __fastcall EnglishClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFLogin(TComponent* Owner);

	std::map<String, std::map<String, String>> translation;

};
//---------------------------------------------------------------------------
extern PACKAGE TFLogin *FLogin;
//---------------------------------------------------------------------------
#endif
