#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"
#include "reinit.hpp"
#include "xml.h"
#include <IdSSLOpenSSLHeaders.hpp>
#include <jpeg.hpp>
#include <pngimage.hpp>
#include <System.Classes.hpp>
#include <IdHTTP.hpp>
#include <Vcl.Forms.hpp>
#include "uTPLb_CryptographicLibrary.hpp"
#include <System.Net.HttpClient.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Forms.hpp>
#include <System.SysUtils.hpp>

#include <IdSSLOpenSSL.hpp> // Include the OpenSSL header
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdHTTP.hpp>
#include <memory>
#include <System.SysUtils.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uTPLb_BaseNonVisualComponent"
#pragma link "uTPLb_Codec"
#pragma link "uTPLb_CryptographicLibrary"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
		  ReadSettingsFromIni();
		  ReadSettingsFromRegistry();
}
void TForm3::SetLoggedInUsername(const String& username)
{
	loggedInUsername = username;
}

void TForm3::LoadProfilePicture()
{
   try
	{
		FDQuery2->Close();
        FDQuery2->SQL->Clear();
        FDQuery2->SQL->Add("SELECT profilna_slika FROM profilna_slika WHERE username = :username");
        FDQuery2->ParamByName("username")->AsString = loggedInUsername;
        FDQuery2->Open();
        if (!FDQuery2->FieldByName("profilna_slika")->IsNull)
        {
            TMemoryStream *imageStream = new TMemoryStream;
            try
            {
                static_cast<TBlobField*>(FDQuery2->FieldByName("profilna_slika"))->SaveToStream(imageStream);
                imageStream->Position = 0;
                // Učitana slika
                TJPEGImage *loadedImage = new TJPEGImage;
                loadedImage->LoadFromStream(imageStream);
                // Prilagođavanje veličine slike
                TBitmap *resizedBitmap = new TBitmap;
                try
                {
                    resizedBitmap->SetSize(Image1->Width, Image1->Height);
                    resizedBitmap->Canvas->StretchDraw(Rect(0, 0, Image1->Width, Image1->Height), loadedImage);
                    // Prikazivanje prilagođene slike u Image1 komponenti
                    Image1->Picture->Bitmap->Assign(resizedBitmap);
                }
                __finally
                {
                    delete resizedBitmap;
                }
            }
            __finally
            {
                delete imageStream;
            }
        }
        else
        {
            Image1->Picture->Bitmap->Assign(NULL); // Clear the image if no profile picture is found
        }
    }
    catch (Exception &e)
    {
        ShowMessage("Greška prilikom učitavanja profilne slike: " + e.Message);
    }
}
void __fastcall TForm3::Button1Click(TObject *Sender)
{
			// Generiranje naziva XML datoteke na osnovu korisničkog imena
	String fileName = loggedInUsername + ".xml";
	// Učitavanje XML dokumenta
	_di_IXMLDocument xmlDoc = LoadXMLDocument(fileName);
	if (!xmlDoc)
	{
		ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
		return;
	}
	// Dobijanje menadžera iz XML dokumenta
	_di_IXMLmanagerType manager = Getmanager(xmlDoc);
	if (!manager)
	{
		ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
		return;
	}
	// Očišćenje ListView-a
	ListView1->Items->Clear();
	// Popunjavanje ListView-a sa podacima iz XML-a
	for (int i = 0; i < manager->Count; i++)
	{
		TListItem *ListItem = ListView1->Items->Add();
		ListItem->Caption = manager->credentials[i]->website;
		ListItem->SubItems->Add(manager->credentials[i]->username);
		ListItem->SubItems->Add(manager->credentials[i]->password);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button3Click(TObject *Sender)
{
	// Dobijanje putanje XML datoteke na osnovu korisničkog imena
	String fileName = loggedInUsername + ".xml";
	// Učitavanje XML dokumenta
	_di_IXMLDocument xmlDoc = LoadXMLDocument(fileName);
	if (!xmlDoc)
	{
		ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
		return;
	}
	// Dobijanje menadžera iz XML dokumenta
	_di_IXMLmanagerType manager = Getmanager(xmlDoc);
	if (!manager)
	{
		ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
		return;
	}
    // Provjera da li je odabran neki red u ListView-u
    if (ListView1->ItemIndex == -1)
    {
        ShowMessage("Molimo odaberite red za brisanje.");
        return;
    }
    // Dobijanje indeksa odabranog reda
    int selectedIndex = ListView1->ItemIndex;
    // Brisanje odabranog reda iz XML dokumenta
    if (selectedIndex >= 0 && selectedIndex < manager->Count)
    {
        manager->Delete(selectedIndex);
        // Sačuvajte promjene u XML datoteci
        xmlDoc->SaveToFile(fileName);
        // Osvježite prikaz ListView-a
        Button1Click(Sender);
        // Clear values in editWebsite, editUsername, and editPassword
        editWebsite->Text = "";
        editUsername->Text = "";
        editPassword->Text = "";
        ShowMessage("Odabrani red je uspješno izbrisan iz XML dokumenta.");
    }
    else
    {
        ShowMessage("Nije moguće izbrisati odabrani red. Odabir nije valjan.");
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender)
{
	// Dobijanje putanje XML datoteke na osnovu korisničkog imena
	String fileName = loggedInUsername + ".xml";
	// Učitavanje XML dokumenta
	_di_IXMLDocument xmlDoc = LoadXMLDocument(fileName);
	if (!xmlDoc)
	{
		ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
		return;
	}
	// Dobijanje menadžera iz XML dokumenta
	_di_IXMLmanagerType manager = Getmanager(xmlDoc);
	if (!manager)
	{
		ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
		return;
	}
	// Dobijanje vrijednosti iz Edit polja
	String newWeb = newWebsite->Text;
	String newUser = newUsername->Text;
	String newPass = newPassword->Text;
	// Provjera da li su sva polja popunjena
    if (newWeb.IsEmpty() || newUser.IsEmpty() || newPass.IsEmpty())
	{
		ShowMessage("Sva polja moraju biti popunjena.");
		return;
    }
    // Dodavanje novog reda u manager
    _di_IXMLcredentialsType newCredentials = manager->Add();
	// Postavljanje vrijednosti novog reda
    newCredentials->website = newWeb;
    newCredentials->username = newUser;
    newCredentials->password = newPass;
    // Sačuvajte promjene u XML datoteci
    xmlDoc->SaveToFile(fileName);
    // Osvježite prikaz ListView-a
    Button1Click(Sender);
}
void __fastcall TForm3::ListView1SelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
	// Provjera da li je selektiran neki red
	 if (Selected && Item != NULL)
    {
        int selectedIndex = Item->Index;
        // Dobijanje putanje XML datoteke na osnovu korisničkog imena
        String fileName = loggedInUsername + ".xml";
        // Učitavanje XML dokumenta
        _di_IXMLDocument xmlDoc = LoadXMLDocument(fileName);
        if (!xmlDoc)
        {
            ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
            return;
        }
        // Dobijanje menadžera iz XML dokumenta
        _di_IXMLmanagerType manager = Getmanager(xmlDoc);
        if (!manager)
        {
            ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
            return;
        }
        // Provjera da li je indeks validan
        if (selectedIndex >= 0 && selectedIndex < manager->Count)
        {
            // Dobijanje selektiranog reda
            _di_IXMLcredentialsType selectedCredentials = manager->credentials[selectedIndex];
            // Postavljanje vrijednosti selektiranog reda u edit boxove
            editWebsite->Text = selectedCredentials->website;
			editUsername->Text = selectedCredentials->username;
            editPassword->Text = selectedCredentials->password;
        }
    }
}

void __fastcall TForm3::Button4Click(TObject *Sender)
{
	// Provjera da li je odabran neki red u ListView-u
	if (ListView1->ItemIndex == -1)
    {
        ShowMessage("Molimo odaberite red za ažuriranje.");
        return;
    }
    // Dobijanje indeksa odabranog reda
    int selectedIndex = ListView1->ItemIndex;
    // Dobijanje putanje XML datoteke na osnovu korisničkog imena
    String fileName = loggedInUsername + ".xml";
    // Učitavanje XML dokumenta
    _di_IXMLDocument xmlDoc = LoadXMLDocument(fileName);
    if (!xmlDoc)
    {
        ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
        return;
	}
    // Dobijanje menadžera iz XML dokumenta
    _di_IXMLmanagerType manager = Getmanager(xmlDoc);
    if (!manager)
    {
        ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
        return;
    }
    // Provjera da li je indeks validan
    if (selectedIndex >= 0 && selectedIndex < manager->Count)
    {
        // Dobijanje selektiranog reda
        _di_IXMLcredentialsType selectedCredentials = manager->credentials[selectedIndex];
        // Ažuriranje vrijednosti selektiranog reda na temelju unesenih vrijednosti u edit boxove
        selectedCredentials->website = editWebsite->Text;
        selectedCredentials->username = editUsername->Text;
        selectedCredentials->password = editPassword->Text;
        // Sačuvajte promjene u XML datoteci
        xmlDoc->SaveToFile(fileName);
        // Osvježite prikaz ListView-a
        Button1Click(Sender);
        //ShowMessage("Vrijednosti selektiranog reda su uspješno ažurirane.");
    }
    else
    {
        ShowMessage("Nije moguće ažurirati selektirani red. Odabir nije valjan.");
    }
}
//---------------------------------------------------------------------------
void TForm3::ClearXMLDocument(System::DelphiInterface<Xml::Xmlintf::IXMLDocument> xmlDoc)
{
    // Implementacija funkcije će biti prazna
	// Ovdje možete dodati bilo kakve druge potrebne korake za čišćenje XML dokumenta
}
void __fastcall TForm3::LogOutButtonClick(TObject *Sender)
{
	// Generiranje naziva XML datoteke na osnovu korisničkog imena
	String fileName = loggedInUsername + ".xml";
	// Učitavanje XML dokumenta
	_di_IXMLDocument xmlDoc = LoadXMLDocument("prazni.xml");
	if (!xmlDoc)
	{
		ShowMessage("Nije moguće učitati XML dokument za korisnika: " + loggedInUsername);
		return;
	}
	// Dobijanje menadžera iz XML dokumenta
	_di_IXMLmanagerType manager = Getmanager(xmlDoc);
	if (!manager)
	{
		ShowMessage("Nije moguće pronaći menadžera u XML dokumentu za korisnika: " + loggedInUsername);
		return;
	}
	// Očišćenje ListView-a
	ListView1->Items->Clear();
	// Popunjavanje ListView-a sa podacima iz XML-a
	for (int i = 0; i < manager->Count; i++)
	{
		TListItem *ListItem = ListView1->Items->Add();
		ListItem->Caption = manager->credentials[i]->website;
		ListItem->SubItems->Add(manager->credentials[i]->username);
		ListItem->SubItems->Add(manager->credentials[i]->password);
	}
this->Close();

    TForm1 *form1 = new TForm1(Application);
	form1->Show();
	isLoggedOut = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::adminButtonClick(TObject *Sender)
{
    // Provjerite je li prijavljeni korisnik "admin"
    if (loggedInUsername == "admin")
	{
        // Otvaranje Forma5 ako je prijavljeni korisnik "admin"
		Form4->Show(); 
    }
    else
    {
        ShowMessage("Samo korisnici s administratorskim privilegijama mogu pristupiti ovoj funkciji."); // Ako prijavljeni korisnik nije "admin"
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm3::ChangeAppFont(const String& fontName)
{
    // Čuvanje postojeće boje i veličine teksta za ListView1
    TColor currentListViewTextColor = ListView1->Font->Color;
    int currentListViewTextSize = ListView1->Font->Size;
    // Čuvanje postojeće boje i veličine teksta za Label1
    TColor currentLabelTextColor = Label1->Font->Color;
    int currentLabelTextSize = Label1->Font->Size;
    TFont* newFont = new TFont;
    newFont->Name = fontName;
    // Postavljanje fonta za ListView1
    ListView1->Font = newFont;
    // Postavljanje fonta za Label1
    Label1->Font = newFont;
    // Postavljanje prethodno odabrane boje i veličine teksta za ListView1
    ListView1->Font->Color = currentListViewTextColor;
    ListView1->Font->Size = currentListViewTextSize;
    // Postavljanje prethodno odabrane boje i veličine teksta za Label1
    Label1->Font->Color = currentLabelTextColor;
    Label1->Font->Size = currentLabelTextSize;
    delete newFont;
}

void __fastcall TForm3::FontArialClick(TObject *Sender)
{
	ChangeAppFont("Arial");
	WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FontVerdanaClick(TObject *Sender)
{
	   ChangeAppFont("Verdana");
	   WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FontTNRClick(TObject *Sender)
{
	ChangeAppFont("Times New Roman");
	WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ChangeAppFontSize(int newSize)
{
    // Postavi novu veličinu fonta za sve relevantne komponente
    ListView1->Font->Size = newSize;
    Label1->Font->Size = newSize;
    Button1->Font->Size = newSize;
    Button2->Font->Size = newSize;
    Button3->Font->Size = newSize;
    Button4->Font->Size = newSize;
    LogOutButton->Font->Size = newSize;
    adminButton->Font->Size = newSize;
}
void __fastcall TForm3::Size10Click(TObject *Sender)
{
   ChangeAppFontSize(10);
   WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Size12Click(TObject *Sender)
{
	   ChangeAppFontSize(12);
	   WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Size14Click(TObject *Sender)
{
	ChangeAppFontSize(14);
	WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Size16Click(TObject *Sender)
{
							 ChangeAppFontSize(16);
							 WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ChangeAppTextColor(TColor color)
{
	// Postavljamo boju teksta za sve relevantne komponente
	ListView1->Font->Color = color;
	Label1->Font->Color = color;
	Button1->Font->Color = color;
	Button2->Font->Color = color;
	Button3->Font->Color = color;
	Button4->Font->Color = color;
	LogOutButton->Font->Color = color;
	adminButton->Font->Color = color;
}
void __fastcall TForm3::Plava1Click(TObject *Sender)
{
	   ChangeAppTextColor(clBlue);
  WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Crvena1Click(TObject *Sender)
{
			   ChangeAppTextColor(clRed);
 WriteSettingsToIni();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Crna1Click(TObject *Sender)
{
ChangeAppTextColor(clBlack);
 WriteSettingsToIni();
}
//---------------------------------------------------------------------------
 #include "Unit3.h"
  #include "Unit5.h"
#include <IniFiles.hpp>
void TForm3::WriteSettingsToIni()
{
    TIniFile *iniFile = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
    iniFile->WriteString("Settings", "FontName", ListView1->Font->Name);
    iniFile->WriteInteger("Settings", "FontSize", ListView1->Font->Size);
    iniFile->WriteInteger("Settings", "FontColor", ListView1->Font->Color);
	delete iniFile;
}
void TForm3::ReadSettingsFromIni()
{
	TIniFile *iniFile = new TIniFile(ChangeFileExt(Application->ExeName, ".ini"));
	AnsiString fontName = iniFile->ReadString("Settings", "FontName", "Arial");
	int fontSize = iniFile->ReadInteger("Settings", "FontSize", 10);
	TColor fontColor = (TColor)iniFile->ReadInteger("Settings", "FontColor", clBlack);
	delete iniFile;
	// Postavljanje fonta i veličine teksta iz postavki u .ini datoteci
	ChangeAppFont(fontName);
	ChangeAppFontSize(fontSize);
	ChangeAppTextColor(fontColor);
}
// Metoda za čitanje postavki iz Windows registra
void TForm3::ReadSettingsFromRegistry() {
    TRegistry *registry = new TRegistry;
    try {
        registry->RootKey = HKEY_CURRENT_USER;
        if (registry->OpenKey("\\Software\\YourAppName\\Settings", false)) {
            this->Left = registry->ReadInteger("Left");
            this->Top = registry->ReadInteger("Top");
            this->Width = registry->ReadInteger("Width");
            this->Height = registry->ReadInteger("Height");
            bool isFullScreen = registry->ReadBool("FullScreen");
            if (isFullScreen) {
                this->WindowState = wsMaximized;
			}
            registry->CloseKey();
        }
    }
    __finally {
        delete registry;
	}
}
// Metoda za spremanje postavki u Windows registar
void TForm3::WriteSettingsToRegistry() {
    TRegistry *registry = new TRegistry;
    try {
        registry->RootKey = HKEY_CURRENT_USER;
        if (registry->OpenKey("\\Software\\YourAppName\\Settings", true)) {
            registry->WriteInteger("Left", this->Left);
            registry->WriteInteger("Top", this->Top);
            registry->WriteInteger("Width", this->Width);
            registry->WriteInteger("Height", this->Height);
            bool isFullScreen = (this->WindowState == wsMaximized);
            registry->WriteBool("FullScreen", isFullScreen);
			registry->CloseKey();
        }
    }
    __finally {
        delete registry;
    }
}
void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
WriteSettingsToRegistry();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::ChangePasswod1Click(TObject *Sender)
{
	Form5->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Editpersonalinfo1Click(TObject *Sender)
{
	Form6->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Deleteaccount1Click(TObject *Sender)
{
  // Direktno izbrišemo korisnika
    DeleteUser();
	ShowMessage("User successfully deleted.");
    this->Close();
    TForm1 *form1 = new TForm1(Application);
    form1->Show();
}
//---------------------------------------------------------------------------
void TForm3::DeleteUser()
{
	// Implementirajte brisanje korisnika iz baze
    // Brišemo korisnika iz tabele osobni_podatci prvo
    try
    {
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("DELETE FROM osobni_podatci WHERE username = :username");
		FDQuery1->ParamByName("username")->AsString = getLoggedinUsername(); 
        FDQuery1->ExecSQL();
    }
    catch (Exception &e)
    {
		ShowMessage("An error occurred while deleting user details: " + e.Message);
        return;
    }
    // Zatim brišemo korisnika iz tabele korisnici
    try
    {
        FDQuery1->SQL->Clear();
        FDQuery1->SQL->Add("DELETE FROM korisnici WHERE username = :username");
        FDQuery1->ParamByName("username")->AsString = getLoggedinUsername(); 
        FDQuery1->ExecSQL();
    }
    catch (Exception &e)
    {
        ShowMessage("An error occurred while deleting user: " + e.Message);
        return;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm3::Button5Click(TObject *Sender)
{
	 String encodedMessage;
    SymetricCodec->Password = SymetricKey->Text;
    SymetricCodec->EncryptString(editPassword->Text, encodedMessage, TEncoding::UTF8);
	editPassword->Text = encodedMessage;
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button6Click(TObject *Sender)
{
 UnicodeString encodedMessage = editPassword->Text;
    UnicodeString decodedMessage;
	SymetricCodec->Password = SymetricKey->Text;
	SymetricCodec->DecryptString(decodedMessage, editPassword->Text, TEncoding::UTF8);
	editPassword->Text = decodedMessage;
}
//-----
void __fastcall TForm3::Jezik2Click(TObject *Sender)
{
// Croatian (Croatia)
    if (LoadNewResourceModule(MAKELANGID(LANG_CROATIAN, SUBLANG_CROATIAN_CROATIA))) {
        ReinitializeForms();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm3::English1Click(TObject *Sender)
{
 if (LoadNewResourceModule(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_UK))) {
		ReinitializeForms();
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Uploadprofilepicture1Click(TObject *Sender)
{
 if (OpenDialog1->Execute())
    {
        String filePath = OpenDialog1->FileName;
        TFileStream *fileStream = new TFileStream(filePath, fmOpenRead);
		try
		{
            FDQuery2->Close();
            FDQuery2->SQL->Clear();
            FDQuery2->SQL->Add("INSERT INTO profilna_slika (username, profilna_slika) VALUES (:username, :profilna_slika) ON DUPLICATE KEY UPDATE profilna_slika = :profilna_slika");
            FDQuery2->ParamByName("username")->AsString = loggedInUsername;
            FDQuery2->ParamByName("profilna_slika")->LoadFromStream(fileStream, ftBlob);
            FDQuery2->ExecSQL();
            ShowMessage("Profilna slika je uspješno ažurirana.");
        }
        __finally
        {
            delete fileStream;
        }
        LoadProfilePicture(); // Load the updated profile picture
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Deleteprofilepicture1Click(TObject *Sender)
{
    try
    {
        // Ensure the FDQuery2 is properly set up and connected
        FDQuery2->Close();
        FDQuery2->SQL->Clear();
        FDQuery2->SQL->Add("UPDATE profilna_slika SET profilna_slika = NULL WHERE username = :username");
        FDQuery2->ParamByName("username")->AsString = loggedInUsername;
        FDQuery2->ExecSQL();
        ShowMessage("Profilna slika je uspješno izbrisana.");
    }
    catch (Exception &e)
    {
        ShowMessage("Greška prilikom brisanja profilne slike: " + e.Message);
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TForm3::FormShow(TObject *Sender)
{
	LoadProfilePicture();

}

//---------------------------------------------------------------------------

void __fastcall TForm3::CheckPassword1Click(TObject *Sender)
{
  Form7->Show();
}
//---------------------------------------------------------------------------

// Define a global variable to hold the desired download speed in kbps
int CurrentDownloadSpeed = 100;  // Example: 100 kbps

// Calculate the equivalent bytes per second (Bps) for throttling
int BytesPerSecond = CurrentDownloadSpeed * 1024 / 8;

// Additional variables
__int64 TotalBytesRead = 0;

void __fastcall TForm3::NetHTTPClient1ReceiveData(TObject * const Sender, __int64 AContentLength,
	__int64 AReadCount, bool &AAbort)
{
	TotalBytesRead = AReadCount;

	// Update progress bar
	ProgressBar1->Max = AContentLength;
	ProgressBar1->Position = AReadCount;

	// Update percentage label
	int percentage = static_cast<int>((static_cast<double>(AReadCount) / AContentLength) * 100);
	LabelPostotak->Caption = IntToStr(percentage) + "%";
}

void __fastcall TForm3::Timer1Timer(TObject *Sender)
{
	if (TotalBytesRead < ProgressBar1->Max) {
		int BytesToRead = BytesPerSecond * Timer1->Interval / 1000;

		ProgressBar1->Position = TotalBytesRead;

		// Update percentage label
		int percentage = static_cast<int>((static_cast<double>(TotalBytesRead) / ProgressBar1->Max) * 100);
		LabelPostotak->Caption = IntToStr(percentage) + "%";
	}
	else {
		Timer1->Enabled = false; // Zaustavite timer kada je preuzimanje završeno
	}
}

void __fastcall TForm3::Button9Click(TObject *Sender)
{
	Timer1->Interval = 1000;  // Adjust interval as needed
	Timer1->Enabled = true;
	TotalBytesRead = 0;

	UnicodeString filePath = L"C:\\Users\\User\\Documents\\Embarcadero\\Studio\\Projects\\ProjektNTP\\upute.pdf";
	fs = new TFileStream(filePath, fmCreate);

	try {
		UnicodeString url = L"https://www.claflin.edu/docs/default-source/guides-and-help-docs/it/claflin-password-tips.pdf?sfvrsn=6981380e_2";
		NetHTTPClient1->Get(url, fs);
	}
	__finally {
		delete fs;
	}
}

void __fastcall TForm3::NetHTTPClient1ReceiveData(TObject * const Sender, TStream * const AStream,
	__int64 AContentLength, __int64 AReadCount, bool &AAbort)
{
	TotalBytesRead = AReadCount;
	ProgressBar1->Max = AContentLength;
	ProgressBar1->Position = AReadCount;

	// Update percentage label
	int percentage = static_cast<int>((static_cast<double>(AReadCount) / AContentLength) * 100);
	LabelPostotak->Caption = IntToStr(percentage) + "%";

	fs->CopyFrom(AStream, 0); 
}

void __fastcall TForm3::SetDownloadSpeed(int speedKbps)
{
	CurrentDownloadSpeed = speedKbps;
	BytesPerSecond = CurrentDownloadSpeed * 1024 / 8; 
}

void __fastcall TForm3::Button7Click(TObject *Sender)
{
	SetDownloadSpeed(50);  // Postavi brzinu preuzimanja na 50 kbps
}

void __fastcall TForm3::Button8Click(TObject *Sender)
{
	SetDownloadSpeed(100);  // Postavi brzinu preuzimanja na 100 kbps
}

void __fastcall TForm3::Button10Click(TObject *Sender)
{
	SetDownloadSpeed(200);  // Postavi brzinu preuzimanja na 200 kbps
}

