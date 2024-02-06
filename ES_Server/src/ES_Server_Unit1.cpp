//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ES_Shared_Header.h"
#include "ES_Server_Unit1.h"

#include <System.SysUtils.hpp> // for encoding/decoding
#include <system.hash.hpp> // for Hash
#include <Registry.hpp> // for ini
#include "MyDLL.h"



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uTPLb_BaseNonVisualComponent"
#pragma link "uTPLb_Codec"
#pragma link "uTPLb_CryptographicLibrary"
#pragma link "uTPLb_Signatory"
#pragma resource "*.dfm"
TForm1 *Form1;

AnsiString s_privateKeyFileName = "private_key_S.bin";
AnsiString s_publicKeyFileName = "public_key_S.bin";
AnsiString c_privateKeyFileName = "private_key_C.bin";
AnsiString c_publicKeyFileName = "public_key_C.bin";


std::unique_ptr<AES_KeyCache> aesKeyCachePtr(new AES_KeyCache(10));  // cache for AES keys


Logger logger("logs.txt"); // File for logs

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
			// Check if keys were generated
	if (!FileExists(s_privateKeyFileName) || !FileExists(s_publicKeyFileName))
	{
		ShowMessage("First time connecting on this PC.\n Wait about 30-45 sec after clicking OK");
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
			privateKey->SaveToFile("private_key_S.bin");
			publicKey->SaveToFile("public_key_S.bin");
		}
	}


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

}


//---------------------------------------------------------------------------
void __fastcall TForm1::TCPServerExecute(TIdContext *AContext)
{
	TThread::Synchronize(NULL, [=]()
	{
		LServerStatus->Caption = "........WORKING......";
	});

		// Reseting ALL Filters && flags
	TPasswords->Filter = "";
	TGrades->Filter = "";
	TProfessorNames->Filter = "";
	TSubjects->Filter = "";
	TStudentNames->Filter = "";
	int PROFESSOR_STATUS = IS_NOT_PROFESSOR;


		// Read Clients Flag
	int RECEIVED_FLAG = AContext->Connection->Socket->ReadInt32(); 			// Receive #1




								// LOGIN Procedure
	if (RECEIVED_FLAG == LOGIN_FLAG)
	{
			// Recieve clients  public_key_C.bin
		UnicodeString fileName = AContext->Connection->Socket->ReadLn();  	 					 // Receive #2
		int fileSize = AContext->Connection->Socket->ReadInt64();  			 					 // Receive #3
		{
		std::unique_ptr<TFileStream> fileStream(new TFileStream(fileName, fmCreate));
		AContext->Connection->Socket->ReadStream(fileStream.get(), fileSize); // Receive #4
		}

			// Send SERVER public Key File
		{
		std::unique_ptr<TFileStream> fileStream(new TFileStream(s_publicKeyFileName,fmOpenRead));
		AContext->Connection->Socket->WriteLn(ExtractFileName(fileStream->FileName)); 			// Send #1 - name
		AContext->Connection->Socket->Write(fileStream->Size); 					   				// Send #2 - size
		AContext->Connection->Socket->Write(fileStream.get()); 			   		   				// Send #3 - stream itself
		}

			// generate AES KEY
		int lenght = 32;
		char* buffer = new char[lenght + 1];
		RandomString(lenght, buffer);

		AnsiString AES_SecretKey(buffer);
		delete[] buffer;
		//ShowMessage(AES_SecretKey);


			// Send AES Secret key Securely using RSA
		String encryptedKey;
		// load key
		std::unique_ptr<TMemoryStream> publicKey_C(new TMemoryStream);
		publicKey_C->LoadFromFile(fileName);
		// encrypt
		Signatory1->LoadKeysFromStream(publicKey_C.get(), TKeyStoragePartSet() << partPublic);
		CodecRSA->EncryptString(AES_SecretKey, encryptedKey, TEncoding::UTF8);
		// Send
		AContext->Connection->Socket->WriteLn(encryptedKey); 		    						// Send #4 - AES Key



			// Receive user info
		String enc_r_FullName = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); 	// Receive #5
		String enc_r_Password = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); 	// Receive #6

			// Decrypt user info
        CodecAES->Password = AES_SecretKey;
		String r_FullName;
			CodecAES->DecryptString(r_FullName, enc_r_FullName, TEncoding::UTF8);
		String r_Password;
			CodecAES->DecryptString(r_Password, enc_r_Password, TEncoding::UTF8);

			// Hash & Filter
		bool found = false;
		for (int i = 0; i < TPasswords->RecordCount; i++)
		{
			std::unique_ptr<THashSHA2> sha2(new THashSHA2);
			for (char papar = 'a'; papar <= 'z'; papar++)
			{
					UnicodeString hashedPass = sha2->GetHashString(
						sha2->GetHashString(r_Password) +
						r_Password +
						papar
					 );
				FilterTable(TPasswords, "Password", hashedPass);
				String passDatabase =  TPasswords->FieldByName("Password")->AsString;
				if (hashedPass == passDatabase )
					{
						found = true;
						//ShowMessage("Password in database");
						break;
					}
				UnfilterTable(TPasswords);
			}
			if (found == true)  break;
		}

		//if (found == false) ShowMessage("Password Not in database!");


			// Read from Table
		String tempProfStatus = IS_NOT_PROFESSOR;
		String FullName = "";
		if (TPasswords->RecordCount > 0)
		{
			FullName = TPasswords->FieldByName("Full Name")->AsString;
			tempProfStatus = TPasswords->FieldByName("Is Professor?")->AsString;
		}
		else tempProfStatus = IS_NOT_PROFESSOR;

			// Check prof status
		if (tempProfStatus == "True")
		{
			PROFESSOR_STATUS = IS_PROFESSOR;
		}
		else PROFESSOR_STATUS = IS_NOT_PROFESSOR;

			// Send Data Back (and encrypt AES)
		if(FullName == r_FullName)
		{
			String enc_DataCorrect;
			CodecAES->EncryptString(IntToStr(DATA_CORRECT), enc_DataCorrect, TEncoding::UTF8);
			AContext->Connection->Socket->WriteLn(enc_DataCorrect); 								// Send #5

			String enc_ProfessorStatus;
			CodecAES->EncryptString(IntToStr(PROFESSOR_STATUS), enc_ProfessorStatus, TEncoding::UTF8);
			AContext->Connection->Socket->WriteLn(enc_ProfessorStatus); 							// Send #6


			AnsiString msg = r_FullName + " " + r_Password + " .UserDataCorrect: " + IntToStr(DATA_CORRECT) + ", ProfStatus:" + IntToStr(PROFESSOR_STATUS) + " | Login success : YES.";
			logger.logLogin(msg);

			/*    Connected Propery of ADOConnection wont turn back on, so I cant send
				// pošalji datoteku
			//std::unique_ptr<TFileStream> fileStream(new TFileStream("ES_Database.mdb",fmOpenRead));

			//AContext->Connection->Socket->WriteLn(ExtractFileName(fileStream->FileName));   // Send #7 - name
			//AContext->Connection->Socket->Write(fileStream->Size); 						  // Send #8 - size
			//AContext->Connection->Socket->Write(fileStream.get()); 						  // Send #9 - stream itself
			*/
			aesKeyCachePtr->Add(AES_SecretKey);
		}

		else
		{
			String enc_DataNotCorrect;
			CodecAES->EncryptString(IntToStr(DATA_NOT_CORRECT), enc_DataNotCorrect, TEncoding::UTF8);
			AContext->Connection->Socket->WriteLn(enc_DataNotCorrect); 								// Send #5

			String enc_ProfessorStatus;
			CodecAES->EncryptString(IntToStr(PROFESSOR_STATUS), enc_ProfessorStatus, TEncoding::UTF8);
			AContext->Connection->Socket->WriteLn(enc_ProfessorStatus); 							// Send #6

			AnsiString msg = r_FullName + " " + r_Password + " .UserDataCorrect: " + IntToStr(DATA_NOT_CORRECT) + ", ProfStatus:" + IntToStr(PROFESSOR_STATUS) + " | Login success : NO.";
			logger.logLogin(msg);
		}
	}





								// WRITE Procedure
	else if (RECEIVED_FLAG == WRITE_FLAG)
	{
			// Receive
		int  studentID = AContext->Connection->Socket->ReadInt32(); 	// Read #2
		int  subjectID = AContext->Connection->Socket->ReadInt32();     // Read #3
		int  grade = AContext->Connection->Socket->ReadInt32();        	// Read #4
		String  gradeDate = AContext->Connection->Socket->ReadLn();     // Read #5
		int  professorID = AContext->Connection->Socket->ReadInt32();   // Read #6
		String  comment = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); // Read #7
		//String  hashPlusRSA = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); // Read #7


			//Lockbox wont decrypt
		/*

			ShowMessage(hashPlusRSA);
			// Verify the sender
		String calcHash = CombineMessage(studentID,subjectID,grade,gradeDate,professorID,comment);
		std::unique_ptr<THashSHA2> sha2(new THashSHA2);
		calcHash = sha2->GetHashString(calcHash);
		//	ShowMessage(calcHash);


		CodecRSA->BlockCipherId = "native.RSA";
		CodecRSA->ChainModeId = "native.CBC";

		ShowMessage("Trying to decrypt");
		String decryptedHash;
		// load key
		std::unique_ptr<TMemoryStream> publicKey_C(new TMemoryStream);
		publicKey_C->LoadFromFile("public_key_C.bin");
		// encrypt
		Signatory1->LoadKeysFromStream(publicKey_C.get(), TKeyStoragePartSet() << partPublic);
		CodecRSA->DecryptString(decryptedHash, hashPlusRSA, TEncoding::UTF8);
		// Send
		ShowMessage("It worked");


		if(calcHash != decryptedHash)
		{
			AContext->Connection->Socket->Write(DATA_NOT_CORRECT);
			return;
		}
		*/

			// prepare
		FilterTable(TStudentNames, "Student ID", IntToStr(studentID));
		String studentName = TStudentNames->FieldByName("Full name")->AsString;

		FilterTable(TSubjects, "Subject ID", IntToStr(subjectID));
		String subject = TSubjects->FieldByName("Subject name")->AsString;

		FilterTable(TProfessorNames, "Professor ID", IntToStr(professorID));
		String professorName = TProfessorNames->FieldByName("Full name")->AsString;

			// Check if anything empty
		if (studentID < 0 || subjectID < 0 || grade < 0 || gradeDate == "" || professorID < 0)
		{
			AContext->Connection->Socket->Write(DATA_NOT_CORRECT);

			AnsiString msg = studentName + " - " + grade + " - " + subject + " - " + professorName + " - '" + comment  + "' | SUCCESS : NO. |";
			logger.logWrite(msg);

			return;
		}
		else // Add to DB
		{
			TGrades->Append(); // dodaj novi zapis
			TGrades->FieldByName("Full Name")->AsString = studentName;
			TGrades->FieldByName("Subject")->AsString = subject;
			TGrades->FieldByName("Grade")->AsString = grade;
			TGrades->FieldByName("Grade date")->AsString = gradeDate;
			TGrades->FieldByName("Professor name")->AsString = professorName;
			TGrades->FieldByName("Comment")->AsString = comment;
			TGrades->Post(); // pohrani zapis

			AContext->Connection->Socket->Write(DATA_CORRECT); 			// Write #1

			AnsiString msg = studentName + " - " + grade + " - " + subject + " - " + professorName + " - '" + comment  + "' | SUCCESS : YES. |";
			logger.logWrite(msg);
		}
	}





								// DELETE Procedure
	else if (RECEIVED_FLAG == DELETE_FLAG)
	{
			// Receive
		int  studentID = AContext->Connection->Socket->ReadInt32();     // Read #2
		int  subjectID = AContext->Connection->Socket->ReadInt32();     // Read #3
		int  grade = AContext->Connection->Socket->ReadInt32();        	// Read #4
		String  gradeDate = AContext->Connection->Socket->ReadLn();     // Read #5
		String  comment = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); // Read #6
		String  professorName = AContext->Connection->Socket->ReadLn(IndyTextEncoding_UTF8()); // Read #7

			// Prepare
		FilterTable(TStudentNames, "Student ID", IntToStr(studentID));
		String  studentName = TStudentNames->FieldByName("Full name")->AsString;

		FilterTable(TSubjects, "Subject ID", IntToStr(subjectID));
		String  subject = TSubjects->FieldByName("Subject name")->AsString;

			// Filter
		String filter = "[Full name] = '" + studentName + "' AND [Subject] = '" + subject + "' AND [Grade] = ' " + grade + "'" + "AND [Grade Date] = '" + gradeDate + "'" + " AND [Comment] = '" + comment + "'";
		TGrades->Filter = filter;

			// LOG the delete
		// log.delete(gradeID, fullName, subject, grade, gradeDate, professorName, comment, reason)

			// Check and DELETE
		if (TGrades->RecordCount > 0)
		{
			TGrades->Delete();
			AContext->Connection->Socket->Write(DATA_CORRECT); 		  // Write #1

			AnsiString msg = studentName + " - " + grade + " - " + subject + " - " + professorName + " - '" + comment  + "' | SUCCESS : YES. |";
			logger.logDelete(msg);
		}
		else
		{
			AContext->Connection->Socket->Write(DATA_NOT_CORRECT); 	  // Write #1

			AnsiString msg = studentName + " - " + grade + " - " + subject + " - " + professorName + " - '" + comment  + "' | SUCESS : NO. |";
			logger.logDelete(msg);
		}

	}

		// Reseting ALL Filters
	TPasswords->Filter = "";
	TGrades->Filter = "";
	TProfessorNames->Filter = "";
	TSubjects->Filter = "";
	TStudentNames->Filter = "";

		// Disconnect
	AContext->Connection->Disconnect();

	TThread::Synchronize(NULL, [=]()
	{
        LServerStatus->Caption = "SERVER CHILLING";
	});
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
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
}
//---------------------------------------------------------------------------


