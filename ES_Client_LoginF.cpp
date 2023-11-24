//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "ES_Shared_Header.h"

#include "ES_Client_LoginF.h"

#include <Registry.hpp> // for ini

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uTPLb_BaseNonVisualComponent"
#pragma link "uTPLb_Codec"
#pragma link "uTPLb_CryptographicLibrary"
#pragma link "uTPLb_Signatory"
#pragma resource "*.dfm"
TFLogin *FLogin;

String FULLNAME;  // Users Full name

bool UAPKeyDeleted = false; // for caching Pass and Username


AnsiString c_privateKeyFileName = "private_key_C.bin";
AnsiString c_publicKeyFileName = "public_key_C.bin";

AnsiString s_privateKeyFileName = "private_key_S.bin";
AnsiString s_publicKeyFileName = "public_key_S.bin";

//---------------------------------------------------------------------------
__fastcall TFLogin::TFLogin(TComponent* Owner)
	: TForm(Owner)
{
		// ini stuff for Window
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\LOGIN_STARTUP.INI");

	// read from [Main Window]
	Caption = ini->ReadString("Main Window", "Title", "E-Student Login");
	Left = ini->ReadInteger("Main Window", "X1", 1000);
	Top = ini->ReadInteger("Main Window", "Y1", 500);
	Width = ini->ReadInteger("Main Window", "X2", 636);
	Height = ini->ReadInteger("Main Window", "Y2", 479);
	delete ini;

		// Regedit stuff for Pass and Username
	TRegistry *Registry = new TRegistry;
	Registry->RootKey = HKEY_CURRENT_USER;
	AnsiString KEY = "SOFTWARE\\EStudentCredentials";

	// try to access KEY if it exists
	if(Registry->KeyExists(KEY))
		if(Registry->OpenKey(KEY, true)){
			EFullName->Text = Registry->ReadString("Full name " );
			EPassword->Text = Registry->ReadString("Password " );
			CBSaveCredentials->Checked = Registry->ReadBool("Save credentials ");
			Registry->CloseKey();
		}
	delete Registry;

		// Init Languages
	translation["LFullName"] = {
	  {
		{"EN", "Your full name:"},
		{"HR", "Vaše ime i prezime:"},
	  }
	};
	translation["EFullName"] = {
	  {
		{"EN", "John Doe"},
		{"HR", "Ivan Horvat"},
	  }
	};
	translation["LPassword"] = {
	  {
		{"EN", "Your password"},
		{"HR", "Vaša lozinka"},
	  }
	};
	translation["CBSaveCredentials"] = {
	  {
		{"EN", "Remember me"},
		{"HR", "Zapamti me"},
	  }
	};
	translation["BLogin"] = {
	  {
		{"EN", "Log in"},
		{"HR", "Prijavi se"},
	  }
	};


}


//---------------------------------------------------------------------------
void __fastcall TFLogin::BLoginClick(TObject *Sender)
{
		// Clean up broken connection
	TCPClient->Disconnect();


		// Check if keys were generated
	if (!FileExists(c_privateKeyFileName) || !FileExists(c_publicKeyFileName))
	{
		ShowMessage("First time connecting on this PC.\n Please Wait about 30-45 sec after clicking OK");
		CodecRSA->BlockCipherId = "native.RSA";
		CodecRSA->ChainModeId = "native.CBC";

		std::unique_ptr<TMemoryStream> privateKey (new TMemoryStream);
		std::unique_ptr<TMemoryStream> publicKey (new TMemoryStream);
			// Generate and Save keys - RSA
		if(Signatory1->GenerateKeys())
		{
			Signatory1->StoreKeysToStream(privateKey.get(),
										  TKeyStoragePartSet() << partPrivate);
			Signatory1->StoreKeysToStream(publicKey.get(),
										  TKeyStoragePartSet() << partPublic);
			privateKey->SaveToFile("private_key_C.bin");
			publicKey->SaveToFile("public_key_C.bin");
		}
	}

		// CONNECT
	if (!TCPClient->Connected())
	{
		TCPClient->Host = "127.0.0.1";
		TCPClient->Port = 8001;
		TCPClient->Connect();
	}

		// Send Flag
	TCPClient->Socket->Write(LOGIN_FLAG);  								// Send #1

		// Send client public Key File
	{
	std::unique_ptr<TFileStream> fileStream(new TFileStream(c_publicKeyFileName,fmOpenRead));
	TCPClient->Socket->WriteLn(ExtractFileName(fileStream->FileName)); // Send #2 - name
	TCPClient->Socket->Write(fileStream->Size); 					   // Send #3 - size
	TCPClient->Socket->Write(fileStream.get()); 			   		   // Send #4 - stream itself
	}

		// Recieve servers public_key_S.bin
	UnicodeString fileName = TCPClient->Socket->ReadLn();  	  	  // Receive #1 - name
	int fileSize = TCPClient->Socket->ReadInt64();  			  // Receive #2 - size
	{
	std::unique_ptr<TFileStream> fileStream(new TFileStream(fileName, fmCreate));
	TCPClient->Socket->ReadStream(fileStream.get(), fileSize);    // Receive #3 - stream itself
	}

		// Receive AES_SecretKey
	String enc_AES_SecretKey = TCPClient->Socket->ReadLn();      // Receive #4 - AES KEY

		//Decrypt AES_SecretKey
	std::unique_ptr<TMemoryStream> privateKey_C(new TMemoryStream);
	std::unique_ptr<TMemoryStream> publicKey_C(new TMemoryStream);
	// Load Clients keys
	privateKey_C->LoadFromFile(c_privateKeyFileName);
	publicKey_C->LoadFromFile(c_publicKeyFileName);

	Signatory1->LoadKeysFromStream(privateKey_C.get(), TKeyStoragePartSet() << partPrivate);
	// Decrypt using Clients Private key
	String AES_SecretKey;
	CodecRSA->DecryptString(AES_SecretKey, enc_AES_SecretKey, TEncoding::UTF8);

		// Prepare User data
	FULLNAME = EFullName->Text;
	String password = EPassword->Text;

		// Send User data with AES Encryption
	CodecAES->Password = AES_SecretKey;

	String fullNameCIPHER;
	CodecAES->EncryptString(FULLNAME, fullNameCIPHER, TEncoding::UTF8);

	String passwordCIPHER;
	CodecAES->EncryptString(password, passwordCIPHER, TEncoding::UTF8);

		// Send encrpyted data
	TCPClient->Socket->WriteLn(fullNameCIPHER, IndyTextEncoding_UTF8());  // Send #5
	TCPClient->Socket->WriteLn(passwordCIPHER, IndyTextEncoding_UTF8());  // Send #6

		// Receive answers
	String enc_DataCorrect = TCPClient->Socket->ReadLn(); // Receive #5
	String enc_IsProfessor = TCPClient->Socket->ReadLn(); // Receive #6
		// Decrypt
	String dec_DataCorrect;
	String dec_IsProfessor;

	CodecAES->DecryptString(dec_DataCorrect, enc_DataCorrect, TEncoding::UTF8);
	CodecAES->DecryptString(dec_IsProfessor, enc_IsProfessor, TEncoding::UTF8);

	DATA_CORRECT = StrToInt(dec_DataCorrect);
	IS_PROFESSOR = StrToInt(dec_IsProfessor);

		// Verify user and data && Receive the DB
	if(DATA_CORRECT == 1)
	{
		/* 			c++ builder sucks
			// Pohrani naziv
		// UnicodeString fileName = TCPClient->Socket->ReadLn();  // Receive #7

			// Pohrani veličinu
		// int fileSize = TCPClient->Socket->ReadInt64();  // Receive #8

			// pohrani sadržaj datoteke
		// std::unique_ptr<TFileStream> fileStream(new TFileStream(fileName, fmCreate));
		// TCPClient->Socket->ReadStream(fileStream.get(), fileSize); // Receive #9
		*/
			// Open correct form
		if(IS_PROFESSOR == 1)
			ModalResult = mrYes;
		else
			ModalResult = mrNo; //if they are not Professor (they are a student)
	}
	else
		 ShowMessage("Incorrect name or Password");

		// Disconnect
	TCPClient->Disconnect();

}
//---------------------------------------------------------------------------

void __fastcall TFLogin::Button1Click(TObject *Sender)
{
	EFullName->Text = "Ante Pišković";
	EPassword->Text = "123";
}
//---------------------------------------------------------------------------

void __fastcall TFLogin::FormClose(TObject *Sender, TCloseAction &Action)
{
		// ini stuff
	TIniFile *ini;
	ini = new TIniFile(GetCurrentDir() + "\\LOGIN_STARTUP.INI");
	// write to [Main Window]
	ini->WriteString ("Main Window", "Title", Caption);
	ini->WriteInteger("Main Window", "X1", Left);
	ini->WriteInteger("Main Window", "Y1", Top);
	ini->WriteInteger("Main Window", "X2", Width);
	ini->WriteInteger("Main Window", "Y2", Height);
	delete ini;

    		// Regedit stuff
	if(UAPKeyDeleted) return;

	TRegistry *Registry = new TRegistry;
	Registry->RootKey = HKEY_CURRENT_USER;
	AnsiString KEY = "SOFTWARE\\EStudentCredentials";

   	Registry->CreateKey(KEY);
	if(Registry->OpenKey(KEY, true))
	{
		if( CBSaveCredentials->Checked == true )
		{
			Registry->WriteString("Full name " , EFullName->Text);
			Registry->WriteString("Password " , EPassword->Text);
			Registry->WriteBool("Save credentials ", CBSaveCredentials->Checked);
			Registry->CloseKey();
		}
		else
		{
			Registry->WriteString("Full name " , "");
			Registry->WriteString("Password " , "");
			Registry->WriteBool("Save credentials ", CBSaveCredentials->Checked);
			Registry->CloseKey();
		}

	}
	delete Registry;


}
//---------------------------------------------------------------------------

void __fastcall TFLogin::CroatianClick(TObject *Sender)
{
	translateForm(this, "HR", translation);
}
//---------------------------------------------------------------------------

void __fastcall TFLogin::EnglishClick(TObject *Sender)
{
	translateForm(this, "EN", translation);
}
//---------------------------------------------------------------------------

