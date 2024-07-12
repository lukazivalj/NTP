#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include <queue>
#include <fstream>

#include <IdContext.hpp>
#include <IdGlobal.hpp>
#include <IdTCPClient.hpp>

#include "NumberConversion.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "frCoreClasses"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma link "frxSmartMemo"
#pragma link "frxExportBaseDialog"
#pragma link "frxExportPDF"
#pragma resource "*.dfm"
#include <System.JSON.hpp>
#include <memory>
#include <System.SysUtils.hpp>

TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
    : TForm(Owner)
{
    Timer1 = new TTimer(this);
    Timer1->Interval = 5000;
    Timer1->OnTimer = Timer1Timer;
    Timer1->Enabled = true;

    CheckUserCount();
}

//---------------------------------------------------------------------------
void __fastcall TForm4::Timer1Timer(TObject *Sender)
{
    CheckUserCount();
}

//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
    RESTRequest1->Execute();
    CheckUserCount();
}

//---------------------------------------------------------------------------
void __fastcall TForm4::ReportButtonClick(TObject *Sender)
{
    frxReport1->ShowReport();
}

//---------------------------------------------------------------------------
void __fastcall TForm4::ExportReportToPDF()
{
    if (frxReport1->PrepareReport())
    {
        frxPDFExport1->FileName = "C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\Izvjestaj.pdf";
        frxReport1->Export(frxPDFExport1);
    }
}

void __fastcall TForm4::CheckUserCount()
{
    FDQuery1->SQL->Text = "SELECT COUNT(*) AS UserCount FROM ntp.korisnici";

    class QueryGuard {
    public:
        QueryGuard(TFDQuery* query) : query_(query) {}
        ~QueryGuard() { query_->Close(); }
    private:
        TFDQuery* query_;
    } queryGuard(FDQuery1);

    try
    {
        FDQuery1->Open();

        if (!FDQuery1->IsEmpty())
        {
            int userCount = FDQuery1->FieldByName("UserCount")->AsInteger;
            Memo2->Lines->Clear();
            Memo2->Lines->Add("Number of users: " + IntToStr(userCount));

            _di_NumberConversionSoapType NumberConversionService = GetNumberConversionSoapType();
            UnicodeString userCountInWords = NumberConversionService->NumberToWords(userCount);
            Memo2->Lines->Add("Number of users in words: " + userCountInWords);

            userCountQueue.push(userCount);
        }
        else
        {
            Memo2->Lines->Add("No records found in ntp.korisnici.");
        }
    }
    catch (Exception &e)
    {
        ShowMessage("Error while querying database: " + e.Message);
        return;
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm4::Button2Click(TObject *Sender)
{
    ExportReportToPDF();
}

//---------------------------------------------------------------------------
void __fastcall TForm4::Button3Click(TObject *Sender)
{
    UnicodeString filePath = L"C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\korisnici.json";

    std::ifstream jsonFile(filePath.c_str());
    if (!jsonFile.is_open()) {
        ShowMessage("Failed to open file: " + filePath);
        return;
    }
    std::string content((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
    jsonFile.close();

    UnicodeString contentUnicode = content.c_str();

    std::unique_ptr<TJSONObject> jsonObject(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(contentUnicode)));
    if (jsonObject)
    {
        Memo3->Lines->Text = jsonObject->ToString();
    }
    else
    {
        Memo3->Lines->Text = "Failed to parse JSON content.";
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm4::Button4Click(TObject *Sender)
{
    UnicodeString filePath = L"C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\korisnici.json";

    std::ifstream jsonFile(filePath.c_str());
    if (!jsonFile.is_open()) {
        ShowMessage("Failed to open file: " + filePath);
        return;
    }
    std::string content((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
    jsonFile.close();

    UnicodeString contentUnicode = content.c_str();

    std::unique_ptr<TJSONObject> jsonObject(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(contentUnicode)));
    if (jsonObject)
    {
        TJSONArray *usersArray = static_cast<TJSONArray*>(jsonObject->GetValue("users"));
        if (usersArray)
        {
            std::ofstream sqlFile("C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\korisnici.sql");
            if (!sqlFile.is_open()) {
                ShowMessage("Failed to create SQL file.");
                return;
            }
            try
            {
                for (int i = 0; i < usersArray->Count; i++)
                {
                    TJSONObject *userObj = static_cast<TJSONObject*>(usersArray->Items[i]);
                    if (userObj)
                    {
                        UnicodeString username = userObj->GetValue("username")->Value();
                        UnicodeString password = userObj->GetValue("password")->Value();
                        UnicodeString name = userObj->GetValue("name")->Value();
                        UnicodeString surname = userObj->GetValue("surname")->Value();
                        UnicodeString email = userObj->GetValue("email")->Value();
                        UnicodeString datumRodenja = userObj->GetValue("datumRodenja")->Value();

                        sqlFile << "INSERT INTO korisnici (username, PASSWORD) VALUES ('" << username.c_str() << "', '" << password.c_str() << "');\n";
                        sqlFile << "INSERT INTO osobni_podatci (username, ime, prezime, datum_rodenja, mail) VALUES ('" << username.c_str() << "', '" << name.c_str() << "', '" << surname.c_str() << "', STR_TO_DATE('" << datumRodenja.c_str() << "', '%d.%m.%Y.'), '" << email.c_str() << "');\n";
                    }
                }
                sqlFile.close();

                ShowMessage("Users successfully written to the SQL file.");
            }
            catch (Exception &e)
            {
                ShowMessage("Error while writing users to SQL file: " + e.Message);
            }
        }
        else
        {
            ShowMessage("No users found in JSON file.");
        }
    }
    else
    {
        Memo3->Lines->Text = "Failed to parse JSON content.";
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm4::TCP_posluziteljExecute(TIdContext *AContext)
{
    int a = AContext->Connection->Socket->ReadLongInt();
    int b = AContext->Connection->Socket->ReadLongInt();
    AContext->Connection->Socket->Write(a + b);
    AContext->Connection->Disconnect();
}
