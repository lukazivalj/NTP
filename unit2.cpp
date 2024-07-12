#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include "reinit.hpp"
#include<IdHashSHA.hpp>>
#include <RegularExpressions.hpp> // Dodajte ovaj include
#include <System.JSON.hpp>
#include <System.IOUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button1Click(TObject *Sender)
{
  // Provjera da li su sva polja popunjena
    if (name->Text == "" || surname->Text == "" || username->Text == "" || password1->Text == "" || password2->Text == "" || eMail->Text == "" || datumRodenja->Date == TDateTime())
    {
        ShowMessage("Molimo popunite sva polja.");
        return;
    }
    // Provjera da li su lozinke jednake
    if (password1->Text != password2->Text)
    {
        ShowMessage("Lozinke se ne podudaraju.");
        return;
    }
    // Provjera da li korisničko ime već postoji u bazi
    FDQuery1->SQL->Text = "SELECT COUNT(*) FROM korisnici WHERE username = :UsernameParam";
    FDQuery1->ParamByName("UsernameParam")->AsString = username->Text;
    try
    {
        FDQuery1->Open();
        int count = FDQuery1->Fields->Fields[0]->AsInteger;
        FDQuery1->Close();
        if (count > 0)
        {
            ShowMessage("Korisničko ime već postoji. Molimo odaberite drugo korisničko ime.");
            return;
        }
    }
    catch (Exception &E)
    {
        ShowMessage("Došlo je do greške prilikom provjere korisničkog imena: " + E.Message);
        return;
    }
    if (!IsEmailValid(eMail->Text))
    {
        ShowMessage("Unesite valjanu email adresu.");
        return;
    }
    // Declare xmlDoc outside the try block
    _di_IXMLDocument xmlDoc;
    // Unos novog korisnika u bazu podataka "korisnici"
    FDQuery1->SQL->Text = "INSERT INTO korisnici (username, password) VALUES (:UsernameParam, :PasswordParam)";
    FDQuery1->ParamByName("UsernameParam")->AsString = username->Text;
    TIdHashSHA1 * sha1 = new TIdHashSHA1;
    // Dodavanje soli "rossi" u lozinku prije hashiranja
    String saltedPassword = password1->Text + "rossi";
    FDQuery1->ParamByName("PasswordParam")->AsString = sha1->HashStringAsHex(saltedPassword);
    delete sha1;
    try
    {
        FDQuery1->ExecSQL();
        // Unos osobnih podataka novog korisnika u bazu podataka "osobni_podatci"
        FDQuery1->SQL->Text = "INSERT INTO osobni_podatci (username, ime, prezime, datum_rodenja, mail) VALUES (:UsernameParam, :ImeParam, :PrezimeParam, :DatumParam, :MailParam)";
        FDQuery1->ParamByName("UsernameParam")->AsString = username->Text;
        FDQuery1->ParamByName("ImeParam")->AsString = name->Text;
        FDQuery1->ParamByName("PrezimeParam")->AsString = surname->Text;
        FDQuery1->ParamByName("DatumParam")->AsDateTime = datumRodenja->Date;
        FDQuery1->ParamByName("MailParam")->AsString = eMail->Text;
        try
        {
            FDQuery1->ExecSQL();
            // Dodavanje korisnika u JSON datoteku
			AddUserToJson(username->Text);
            // Stvaranje XML datoteke
            xmlDoc = NewXMLDocument();
			xmlDoc->Active = true;
            // Kreiranje čvorova XML strukture
            _di_IXMLNode managerNode = xmlDoc->AddChild("manager");
            _di_IXMLNode credentialsNode = managerNode->AddChild("credentials");
            credentialsNode->AddChild("website")->Text = "Password manager";
            credentialsNode->AddChild("username")->Text = username->Text;
            credentialsNode->AddChild("password")->Text = password1->Text;
            // Snimanje XML datoteke
            String directory = "C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\Projekt";
            String fileName = username->Text + ".xml";
            xmlDoc->SaveToFile(fileName);
            // Popunjavanje polja u Unit1
            Form1->FillLoginFields(username->Text, password1->Text);
            ShowMessage("Uspješno ste se registrirali.");
            Close(); // Zatvori Form2 nakon registracije
        }
        catch (Exception &E)
        {
            ShowMessage("Došlo je do greške prilikom registracije: " + E.Message);
        }
    }
    catch (Exception &E)
    {
        ShowMessage("Došlo je do greške prilikom unosa korisnika: " + E.Message);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <System.RegularExpressions.hpp>
bool TForm2::IsEmailValid(const String& email)
{
	// Regularni izraz za provjeru valjanosti emaila
	TRegEx regex("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b", TRegExOptions() << roIgnoreCase);
	return regex.IsMatch(email);
}
void __fastcall TForm2::AddUserToJson(const String& username)
{
    // Formiranje putanje do JSON datoteke
    String jsonFilePath = "C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\korisnici.json";

    // Učitavanje trenutnog sadržaja JSON datoteke
    TStringList *jsonContent = new TStringList;
    if (TFile::Exists(jsonFilePath))
    {
        jsonContent->LoadFromFile(jsonFilePath);
    }
    else
    {
        // Ako datoteka ne postoji, kreiraj novi JSON objekat
        jsonContent->Text = "{ \"users\": [] }";
    }

    try
    {
        // Parsiranje JSON-a
        TJSONObject *jsonObject = static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(jsonContent->Text));

        if (jsonObject)
        {
            try
            {
                // Dodavanje novog korisnika
                TJSONArray *usersArray = static_cast<TJSONArray*>(jsonObject->Get("users")->JsonValue);
                TJSONObject *newUser = new TJSONObject;
                newUser->AddPair("username", new TJSONString(username));
                usersArray->Add(newUser);

                // Čuvanje promjena u JSON datoteku
                jsonContent->Text = jsonObject->ToString();
                jsonContent->SaveToFile(jsonFilePath);
            }
            __finally
            {
                delete jsonObject;
            }
        }
    }
    __finally
    {
        delete jsonContent;
    }
}
