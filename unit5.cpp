#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
#include "Unit1.h"  // Uključite Unit1 kako bi koristili getLoggedinUsername
#include "Unit3.h" 
#include <IdHashSHA.hpp>

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;

__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TForm5::Button1Click(TObject *Sender)
{
    // Read user inputs
    String oldPwd = oldPassword->Text;
    String newPwd = newPassword->Text;
    String newPwd2 = newPassword2->Text;

    if (newPwd != newPwd2) {
        ShowMessage("New passwords do not match!");
        return;
    }

	String username = getLoggedinUsername(); // Get the logged in username
    ShowMessage("Logged in username: " + username); // Debugging

    try {
        FDConnection1->Connected = true;

		// Prepare and execute query to check the old password
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("SELECT password FROM korisnici WHERE username = :username");
        FDQuery1->ParamByName("username")->AsString = username;
        FDQuery1->Open();

        if (FDQuery1->RecordCount == 0) {
            ShowMessage("Username not found!");
            FDConnection1->Connected = false;
            return;
        }

        // Hash the entered old password with the same salt
        String saltedOldPassword = oldPwd + "rossi";
        TIdHashSHA1 *sha1 = new TIdHashSHA1;
        String hashedOldPassword = sha1->HashStringAsHex(saltedOldPassword);
        delete sha1;

        // Check if the hashed entered old password matches the stored hashed password
        String storedHashedPassword = FDQuery1->FieldByName("password")->AsString;
        if (hashedOldPassword != storedHashedPassword) {
            ShowMessage("Old password is incorrect!");
            FDConnection1->Connected = false;
            return;
        }

        // Hash the new password with the same salt
        String saltedNewPassword = newPwd + "rossi";
        sha1 = new TIdHashSHA1;
        String hashedNewPassword = sha1->HashStringAsHex(saltedNewPassword);
        delete sha1;

        // Prepare and execute query to update the password
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("UPDATE korisnici SET password = :newPassword WHERE username = :username");
        FDQuery1->ParamByName("newPassword")->AsString = hashedNewPassword;
        FDQuery1->ParamByName("username")->AsString = username;
        FDQuery1->ExecSQL();

        ShowMessage("Password updated successfully!");
    }
    catch (Exception &e) {
        ShowMessage("An error occurred: " + e.Message);
    }

    // Ensure the connection is closed after the try block
    FDConnection1->Connected = false;
}
