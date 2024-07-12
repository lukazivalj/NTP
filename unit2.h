//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
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
#include <Vcl.WinXPickers.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <IdHashSHA.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:    // IDE-managed Components
    TLabel *Ime;
	TLabel *Label2;
    TLabel *Label3;
    TEdit *name;
    TEdit *surname;
    TLabel *Label4;
    TLabel *Label5;
    TEdit *username;
    TEdit *password1;
    TEdit *password2;
    TLabel *Label6;
    TButton *Button1;
    TFDConnection *FDConnection1;
    TFDQuery *FDQuery1;
    TFDTable *FDTable1;
    TLabel *mailLabel;
    TEdit *eMail;
	TDatePicker *datumRodenja;
    TLabel *Label8;
    TXMLDocument *XMLDocument1;
    TOpenDialog *OpenDialog1;
    TFDQuery *FDQuery2;
	void __fastcall Button1Click(TObject *Sender);

private:    // User declarations
	TXMLDocument *xmlDoc; // Deklaracija xmlDoc varijable
    void __fastcall AddUserToJson(const String& username);


public:        // User declarations
    __fastcall TForm2(TComponent* Owner);
	bool IsEmailValid(const String& email);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
