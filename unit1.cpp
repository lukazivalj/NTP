#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "reinit.hpp"
#include <Windows.h> // Include Windows.h for DLL loading functions
#include <Graphics.hpp> // Include Graphics.hpp for TImage
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
#include <Windows.h> 
#include <Graphics.hpp> 
//---------------------------------------------------------------------------
void __fastcall TForm1::registerButtonClick(TObject *Sender)
{
    TForm2 *form2 = new TForm2(this);
    form2->ShowModal();
    delete form2; 
}
void TForm1::FillLoginFields(const String& username, const String& password)
{
    eUsername->Text = username;
    ePassword->Text = password;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::loginButtonClick(TObject *Sender)
{
    // Provjera jesu li polja za korisničko ime i lozinku popunjena
    if (eUsername->Text != "" && ePassword->Text != "")
    {
        // SQL upit za provjeru korisničkog imena i lozinke
        FDQuery1->SQL->Text = "SELECT * FROM korisnici WHERE username = :UsernameParam AND password = :PasswordParam";
		FDQuery1->ParamByName("UsernameParam")->AsString = eUsername->Text;
		 TIdHashSHA1 * sha1 = new TIdHashSHA1;
        String saltedPassword = ePassword->Text + "rossi";
        FDQuery1->ParamByName("PasswordParam")->AsString = sha1->HashStringAsHex(saltedPassword);
        delete sha1;
        try
        {
            // Izvršavanje upita
            FDQuery1->Open();
            // Provjera jesu li pronađeni rezultati
            if (!FDQuery1->IsEmpty())
            {
                // Sačuvaj korisničko ime u lokalnoj varijabli
                String loggedInUsername = GetLoggedInUsername();
                // Prikazi Form3
                Form3->SetLoggedInUsername(loggedInUsername);
                Form3->Show();
                // Sakrij Form1
                this->Hide();
            }
            else
            {
                // Ako nema rezultata, prikaži poruku o pogrešnim podacima
                ShowMessage("Pogrešno korisničko ime ili lozinka.");
            }
        }
        catch(Exception &E)
        {
            ShowMessage("Došlo je do greške prilikom provjere podataka: " + E.Message);
        }
    }
    else
    {
        // Ako su polja prazna, prikaži poruku o popunjavanju
        ShowMessage("Molimo popunite korisničko ime i lozinku.");
    }
}
//---------------------------------------------------------------------------
void TForm1::CloseForm1()
{
    Close(); // Close Form1
}
String TForm1::GetLoggedInUsername()
{
    // Provjerimo je li upit pronašao rezultate
    if (!FDQuery1->IsEmpty()) {
        // Ako je pronađen rezultat, dohvatimo korisničko ime iz rezultata upita
        return FDQuery1->FieldByName("username")->AsString;
    } else {
        // Ako upit nije pronašao rezultate, vratimo prazan string
        return "";
    }
}
// Implementacija globalne funkcije
String getLoggedinUsername() {
    if (Form1 != nullptr) {
        return Form1->GetLoggedInUsername();
    }
    return "";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
   HINSTANCE ResourceDll;
    if ((ResourceDll = LoadLibrary(L"logo.dll")) == NULL) {
        ShowMessage("Cannot load dll");
		return;
	}
  Image1->Picture->Bitmap->LoadFromResourceID((int)ResourceDll, 1);
  FreeLibrary(ResourceDll);
}
void __fastcall TForm1::Button1Click(TObject *Sender)
{
// Croatian (Croatia)
    if (LoadNewResourceModule(MAKELANGID(LANG_CROATIAN, SUBLANG_CROATIAN_CROATIA))) {
        ReinitializeForms();
	}
//---------------------------------------------------------------------------
}
void __fastcall TForm1::Button2Click(TObject *Sender)
{
// English (United Kingdom)
 if (LoadNewResourceModule(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK))) {
        ReinitializeForms();
	}
//---}
//---------------------------------------------------------------------------
}
