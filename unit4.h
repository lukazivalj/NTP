#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <REST.Client.hpp>
#include <REST.Types.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
#include <Data.DB.hpp>
#include <FireDAC.Comp.Client.hpp>
#include <FireDAC.Phys.hpp>
#include <FireDAC.Phys.Intf.hpp>
#include <FireDAC.Stan.Async.hpp>
#include <FireDAC.Stan.Def.hpp>
#include <FireDAC.Stan.Error.hpp>
#include <FireDAC.Stan.Intf.hpp>
#include <FireDAC.Stan.Option.hpp>
#include <FireDAC.Stan.Pool.hpp>
#include <FireDAC.UI.Intf.hpp>
#include <FireDAC.VCLUI.Wait.hpp>
#include <FireDAC.Comp.DataSet.hpp>
#include <FireDAC.DApt.hpp>
#include <FireDAC.DApt.Intf.hpp>
#include <FireDAC.DatS.hpp>
#include <FireDAC.Stan.Param.hpp>
#include <FireDAC.Phys.MySQL.hpp>
#include <FireDAC.Phys.MySQLDef.hpp>
#include <Data.Bind.DBScope.hpp>
#include "frCoreClasses.hpp"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "frxSmartMemo.hpp"
#include "frxExportBaseDialog.hpp"
#include "frxExportPDF.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdCustomTCPServer.hpp>
#include <IdTCPServer.hpp>
#include <IdContext.hpp>
#include <queue>

#include <System.Generics.Collections.hpp> // Include for TQueue
//---------------------------------------------------------------------------

class TForm4 : public TForm
{
__published:    // IDE-managed Components
    TRESTClient *RESTClient1;
    TRESTRequest *RESTRequest1;
    TRESTResponse *RESTResponse1;
    TMemo *Memo1;
    TBindingsList *BindingsList1;
    TLinkControlToField *LinkControlToField1;
    TButton *Button1;
    TEdit *Edit1;
    TLinkControlToField *LinkControlToField2;
    TEdit *Edit2;
    TLinkControlToField *LinkControlToField3;
    TMemo *Memo2;
    TFDConnection *FDConnection1;
    TFDQuery *FDQuery1;
    TFDTable *FDTable1;
    TBindSourceDB *BindSourceDB1;
	TFDTable *FDTable2;
	TfrxDBDataset *frxKorisnici;
	TfrxDBDataset *frxOsobniPodatci;
	TfrxDBDataset *frxSlika;
	TFDTable *FDTable3;
	TfrxReport *frxReport1;
	TButton *ReportButton;
	TfrxPDFExport *frxPDFExport1;
	TButton *Button2;
	TButton *Button3;
	TMemo *Memo3;
	TButton *Button4;
	TIdTCPClient *TCP_klijent;
	TIdTCPServer *TCP_posluzitelj;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ReportButtonClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall TCP_posluziteljExecute(TIdContext *AContext);

private:    // User declarations
	void __fastcall CheckUserCount(); // Deklaracija CheckUserCount metode
	void __fastcall Timer1Timer(TObject *Sender); // Deklaracija Timer1Timer metode
	TTimer *Timer1; // Deklaracija tajmera
	std::queue<int> userCountQueue; // Declaration of userCountQueue
     void __fastcall ExportReportToPDF();


public:     // User declarations
    __fastcall TForm4(TComponent* Owner);
};

extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
