#pragma once
#include <Data.Win.ADODB.hpp>


	// Shared flags between SERVER and CLIENT
int LOGIN_FLAG = 0;
int WRITE_FLAG = 1;
int DELETE_FLAG = 2;

int DATA_CORRECT = 1;
int DATA_NOT_CORRECT = 0;

int IS_PROFESSOR = 1;
int IS_NOT_PROFESSOR = 0;



int DARK_MODE_FLAG = 0;
int LIGHT_MODE_FLAG = 1;




//(TStudentNames, "Full name", StudentName)
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



//-----------------------------------------------------------------------------
 #include <vcl.h>
#include "ES_Client_LoginF.h"
#include <map>


void translateForm(TForm* FLogin, String Language, const std::map<String, std::map<String, String>>& translation){
	for(int i = 0; i < FLogin->ComponentCount; i++) // iterate though all components on the form
		for(auto it_ComponentName = translation.begin(); it_ComponentName != translation.end(); it_ComponentName++)
			if(FLogin->Components[i]->Name == it_ComponentName->first) // find component by name
				for(auto it_Language = it_ComponentName->second.begin(); it_Language != it_ComponentName->second.end(); it_Language++)
					if(it_Language->first == Language) // find translation for the target language
						{
							if(IsPublishedProp(FLogin->Components[i], "Caption"))
								SetPropValue(FLogin->Components[i], "Caption", it_Language->second);
							if(IsPublishedProp(FLogin->Components[i], "TextHint"))
								SetPropValue(FLogin->Components[i], "TextHint", it_Language->second);
						}
}


