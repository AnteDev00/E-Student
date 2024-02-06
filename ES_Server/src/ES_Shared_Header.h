#pragma once

#include <vector>

#include <vcl.h> // Include VCL headers
#include <fstream>

#include <Data.Win.ADODB.hpp>  // a lot of stuff from builder
#pragma hdrstop


	// Shared flags between SERVER and CLIENT
int LOGIN_FLAG = 0;
int WRITE_FLAG = 1;
int DELETE_FLAG = 2;

int DATA_CORRECT = 1;
int DATA_NOT_CORRECT = 0;

int IS_PROFESSOR = 1;
int IS_NOT_PROFESSOR = 0;

	// My functions for shortening code
void FilterTable (TADOTable* Table, String rowName, String cellValue)
{
	String filter;
	filter = "[" + rowName + "] = '" + cellValue + "'";
	Table->Filter = filter;
	Table->Refresh();
}

void FilterTableExtend(TADOTable* Table, String rowName, String cellValue)
{
	String filter;
	filter = " AND [" + rowName + "] = '" + cellValue + "'";
	Table->Filter += filter;
	Table->Refresh();
}

void UnfilterTable(TADOTable* Table)
{
    Table->Filter = "";
}

//--------------------------------------------------------------------------
class AES_KeyCache
{
private:
	std::vector<String> keyCacheV;
	int capacity;

public:

	AES_KeyCache(int _capacity)
	{
	   if(capacity <= 0) capacity = 1;
	   capacity = _capacity;
	   InitCache(capacity);
	}
	void InitCache(int capacity)
	{
		keyCacheV.reserve(capacity);
	}

	void Add(String key)
	{
	   keyCacheV.push_back(key);
	}

};
//--------------------------------------------------------------------------
String CombineMessage(int studentID, int subjectID, int grade, String gradeDate, int professorID, String comment)
{
	String combinedMessage;

	String _studentID = IntToStr(studentID);
	String _subjectID = IntToStr(subjectID);
	String _professorID = IntToStr(professorID);
	String _grade = IntToStr(grade);

	combinedMessage = _studentID + _subjectID + _grade + gradeDate + _professorID + comment;

	return combinedMessage;
}

//--------------------------------------------------------------------------

#include <fstream>
#include <vcl.h> // Include VCL headers
#include <Data.Win.ADODB.hpp>  // a lot of stuff from builder

class Logger {
private:
	String filename; // Name of the log file
	std::ofstream file; // File stream object for writing logs

public:
	Logger(const String& filename) : filename(filename) {
		file.open(filename.c_str(), std::ios::app); // Open the file in append mode
	}

	~Logger() {
		file.close(); // Close the file when the logger is destroyed
	}


	void log(const String& message)
	{
		std::time_t now = std::time(nullptr);
		std::string timestamp = std::ctime(&now);
		timestamp.pop_back(); // Removes the newline characte

		AnsiString ansiMessage = AnsiString(message).c_str();

		file << timestamp << ": " << ansiMessage.c_str() << std::endl;
	}

	void logLogin(const String& loginInfo)
	{
		String formattedLoginMessage = "[LOGIN] " + loginInfo;
		log(formattedLoginMessage);
	}

	void logWrite(const String& loginInfo)
	{
		String formattedLoginMessage = "[WRITE] " + loginInfo;
		log(formattedLoginMessage);
	}

	void logDelete(const String& loginInfo)
	{
		String formattedLoginMessage = "[DELETE] " + loginInfo;
		log(formattedLoginMessage);
	}




	void logError(const String& errorMessage)
	{
		String formattedErrorMessage = "[ERROR] " + errorMessage;
		log(formattedErrorMessage);
	}

};

