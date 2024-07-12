//---------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Vcl.Menus.hpp>
#include <IniFiles.hpp>
#include <Registry.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Phys.MySQL.hpp>
#include <FireDAC.Phys.MySQLDef.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include "uTPLb_BaseNonVisualComponent.hpp"
#include "uTPLb_Codec.hpp"
#include "uTPLb_CryptographicLibrary.hpp"
#include <System.SysUtils.hpp>
#include "uTPLb_CryptographicLibrary.hpp"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdAuthentication.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
#include <IdIntercept.hpp>
#include <IdInterceptThrottler.hpp>#
#include <IdTCPConnection.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdHTTP.hpp>p
#include <IdTCPClient.hpp>r
#include <IdTCPConnection.hpp>agma package(smart_init)
#pragma link "uTPLb_BaseNonVisualComponent"
#pragma link "uTPLb_Codec"
#pragma link "uTPLb_CryptographicLibrary"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:    // IDE-managed Components
    TLabel *Label1;
    TListView *ListView1;
    TButton *Button1;
    TXMLDocument *XMLDocument1;
    TButton *Button2;
    TButton *Button3;
    TEdit *newWebsite;
    TEdit *newUsername;
	TEdit *newPassword;
    TEdit *editWebsite;
    TEdit *editUsername;
    TEdit *editPassword;
    TButton *Button4;
    TButton *LogOutButton;
    TButton *adminButton;
    TMainMenu *MainMenu1;
    TMenuItem *Font1;
    TMenuItem *FontArial;
    TMenuItem *FontVerdana;
    TMenuItem *FontTNR;
    TMenuItem *Size1;
    TMenuItem *Size10;
    TMenuItem *Size12;
    TMenuItem *Size14;
    TMenuItem *Size16;
    TMenuItem *Boja1;
    TMenuItem *Plava1;
    TMenuItem *Crvena1;
    TMenuItem *Crna1;
    TMenuItem *AccountSettinga1;
    TMenuItem *ChangePasswod1;
    TMenuItem *Editpersonalinfo1;
    TMenuItem *Deleteaccount1;
    TFDConnection *FDConnection1;
    TFDQuery *FDQuery1;
	TEdit *SymetricKey;
	TButton *Button5;
	TButton *Button6;
	TCodec *SymetricCodec;
	TCryptographicLibrary *CryptographicLibrary1;
	TMenuItem *Jezik1;
	TMenuItem *Jezik2;
	TMenuItem *English1;
	TImage *Image1;
	TFDQuery *FDQuery2;
	TOpenDialog *OpenDialog1;
	TMenuItem *Uploadprofilepicture1;
	TMenuItem *Deleteprofilepicture1;
	TButton *Button9;
	TNetHTTPClient *NetHTTPClient1;
	TProgressBar *ProgressBar1;
	TProgressBar *ProgressBar2;
	TButton *Button7;
	TButton *Button8;
	TButton *Button10;
	TTimer *Timer1;
	TLabel *Label2;
	TLabel *LabelPostotak;

    void __fastcall Button1Click(TObject *Sender);
    void __fastcall Button3Click(TObject *Sender);
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall ListView1SelectItem(TObject *Sender, TListItem *Item, bool Selected);
    void __fastcall Button4Click(TObject *Sender);
    void __fastcall LogOutButtonClick(TObject *Sender);
    void __fastcall adminButtonClick(TObject *Sender);
    void __fastcall FontArialClick(TObject *Sender);
    void __fastcall FontVerdanaClick(TObject *Sender);
    void __fastcall FontTNRClick(TObject *Sender);
    void __fastcall Size10Click(TObject *Sender);
    void __fastcall Size12Click(TObject *Sender);
    void __fastcall Size14Click(TObject *Sender);
    void __fastcall Size16Click(TObject *Sender);
    void __fastcall Plava1Click(TObject *Sender);
    void __fastcall Crvena1Click(TObject *Sender);
    void __fastcall Crna1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ChangePasswod1Click(TObject *Sender);
    void __fastcall Editpersonalinfo1Click(TObject *Sender);
	void __fastcall Deleteaccount1Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall Jezik2Click(TObject *Sender);
	void __fastcall English1Click(TObject *Sender);
	void __fastcall Uploadprofilepicture1Click(TObject *Sender);
	void __fastcall Deleteprofilepicture1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckPassword1Click(TObject *Sender);
	void __fastcall Button9Click(TObject *Sender);
	void __fastcall NetHTTPClient1ReceiveData(TObject * const Sender, __int64 AContentLength,
          __int64 AReadCount, bool &AAbort);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);

private:    // User declarations
    String loggedInUsername;
    bool isLoggedOut;
    void __fastcall ChangeAppFont(const String& fontName);
    void __fastcall ChangeAppFontSize(int newSize);
    void __fastcall ChangeAppTextColor(TColor color);
	void ReadSettingsFromIni();
	void WriteSettingsToIni();
	void ReadSettingsFromRegistry();
    void WriteSettingsToRegistry();
	void DeleteUser();
	void __fastcall downloadFile(const UnicodeString& url, TStream* outputStream, int delayMs);
	int CurrentDownloadSpeed; 
	TFileStream *fs; 
    void __fastcall SetDownloadSpeed(int speedKbps);

public:        // User declarations
	__fastcall TForm3(TComponent* Owner);
	void SetLoggedInUsername(const String& username);
	void ClearXMLDocument(System::DelphiInterface<Xml::Xmlintf::IXMLDocument> xmlDoc);
	 void LoadProfilePicture();
	   void __fastcall Timer1Timer(TObject *Sender); // Declaration of Timer1Timer function
       void __fastcall NetHTTPClient1ReceiveData(TObject * const Sender, TStream * const AStream,
		__int64 AContentLength, __int64 AReadCount, bool &AAbort);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
