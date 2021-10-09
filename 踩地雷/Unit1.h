//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TImage *Image1;
	TTimer *Timer1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
void Print_Label();//���Label
void Set_Bomb(int);//�]�m���u
void Set_Number();//�]�m�Ʀr
void Spread();//�P�_�O�_�i�H�X��
void Game_Over_Check();//�P�_�O�_����
void Game_Over_Execute();//��������
public:		// User declarations
void __fastcall whenimgclk(TObject *Sender, TMouseButton , TShiftState ,
		  int , int );//�I���Ϥ���
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
