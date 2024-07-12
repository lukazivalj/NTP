//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit6.h"
#include <RegularExpressions.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm6 *Form6;
//---------------------------------------------------------------------------
__fastcall TForm6::TForm6(TComponent* Owner)
	: TForm(Owner)
{
}
bool TForm6::IsEmailValid(const String& email)
{
    // Regularni izraz za provjeru valjanosti emaila
    TRegEx regex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b", TRegExOptions() << roIgnoreCase);

    return regex.IsMatch(email);
}
//---------------------------------------------------------------------------
void __fastcall TForm6::Button1Click(TObject *Sender)
{
    // Read user inputs
    String newIme = ime->Text;
	String newPrezime = prezime->Text;
    String newEmail = email->Text;
    TDateTime newDatumRodenja = datumRodenja->Date;

    // Get the logged in username
	String username = getLoggedinUsername();
	 if (!IsEmailValid(newEmail))
    {
		ShowMessage("Unesite valjanu email adresu.");
		return;
	}
    try {
        FDConnection1->Connected = true;

        // Prepare and execute query to update the user information
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("UPDATE osobni_podatci SET ime = :ime, prezime = :prezime, mail = :mail, datum_rodenja = :datumRodenja WHERE username = :username");
        FDQuery1->ParamByName("ime")->AsString = newIme;
        FDQuery1->ParamByName("prezime")->AsString = newPrezime;
        FDQuery1->ParamByName("mail")->AsString = newEmail;
        FDQuery1->ParamByName("datumRodenja")->AsDateTime = newDatumRodenja;
        FDQuery1->ParamByName("username")->AsString = username;
        FDQuery1->ExecSQL();

        ShowMessage("User information updated successfully!");
    }
    catch (Exception &e) {
        ShowMessage("An error occurred: " + e.Message);
    }

    // Ensure the connection is closed after the try block
    FDConnection1->Connected = false;

}
//---------------------------------------------------------------------------

void __fastcall TForm6::FormShow(TObject *Sender)
{
// Get the logged in username
    String username = getLoggedinUsername();

    try {
        FDConnection1->Connected = true;

        // Prepare and execute query to retrieve user information
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("SELECT * FROM osobni_podatci WHERE username = :username");
        FDQuery1->ParamByName("username")->AsString = username;
        FDQuery1->Open();

        // Populate edit boxes and date picker with retrieved values
        if (!FDQuery1->IsEmpty()) {
            ime->Text = FDQuery1->FieldByName("ime")->AsString;
            prezime->Text = FDQuery1->FieldByName("prezime")->AsString;
            email->Text = FDQuery1->FieldByName("mail")->AsString;
            datumRodenja->Date = FDQuery1->FieldByName("datum_rodenja")->AsDateTime;
        }
    }
    catch (Exception &e) {
        ShowMessage("An error occurred: " + e.Message);
    }

    // Ensure the connection is closed after the try block
	FDConnection1->Connected = false;
}
//---------------------------------------------------------------------------
bool IsEmailValid(const String& email)
{
    // Regularni izraz za provjeru valjanosti emaila
    TRegEx regex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b", TRegExOptions() << roIgnoreCase);

    return regex.IsMatch(email);
}
