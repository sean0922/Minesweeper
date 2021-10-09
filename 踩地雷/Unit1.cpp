//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
#define Layout_Size 15//�w�]�����j�p
int LayoutSize=10;//�w�]�����j�p
int BombNumber=10;
int Time_Clock=0;//�ɶ��p��
bool Game_Start=false;//���I�F�Ĥ@���I��~�|�}�l
TImage *img[Layout_Size][Layout_Size];//�Ϥ����
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Label2->Caption=IntToStr(LayoutSize)+"x"+IntToStr(LayoutSize);
	Label4->Caption=IntToStr(BombNumber)+"��";
    time_t t;
	srand((unsigned)time(&t));    //�üƺؤl
}
//---------------------------------------------------------------------------
void TForm1::Print_Label()//���Label
{
	Label2->Caption=IntToStr(LayoutSize)+"x"+IntToStr(LayoutSize);
	Label4->Caption=IntToStr(BombNumber)+"��";
}
void __fastcall TForm1::Button2Click(TObject *Sender)//�W�[�����j�p
{
	if(LayoutSize<15)
		LayoutSize+=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)//��֪����j�p
{
	if(LayoutSize>10)
		LayoutSize-=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)//�W�[���u
{
	if(BombNumber<(LayoutSize*LayoutSize)/5)
		BombNumber+=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)//��֬��u
{
	if(BombNumber>(LayoutSize*LayoutSize)/10)
		BombNumber-=1;
 	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)//�}�l�C��
{
	#define Layout_Size LayoutSize

	Label6->Caption=IntToStr(Layout_Size);

	for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
		{
			if(img[i][j]!=NULL)
			{
				delete img[i][j];
				img[i][j]=NULL;
			}
		}
	}
	for(int i=0;i<Layout_Size;i++)
	{
		for(int j=0;j<Layout_Size;j++)
		{
			img[i][j]=new TImage(this);
			img[i][j]->Top=60+(i*22);
			img[i][j]->Left=60+(j*22);
			img[i][j]->Height=20;
			img[i][j]->Width=20;
			img[i][j]->Stretch=true;
			img[i][j]->Parent=this;
			img[i][j]->Hint=0;//���� ���u99 �� �P�򬵼u��0~8
			img[i][j]->Tag=(i*Layout_Size)+j;//�]�w��m�s�� 0~99
			img[i][j]->HelpContext=0;//���i�}=0 �i�}=1 �кX=2
			img[i][j]->OnMouseUp=whenimgclk;
			img[i][j]->Enabled=true;
			img[i][j]->Picture->LoadFromFile("block.bmp");
		}
	}
	Time_Clock=0;
    Game_Start=false;
	Timer1->Enabled=true;
	Button1->Visible=false;
	Button2->Visible=false;
	Button3->Visible=false;
	Button4->Visible=false;
	Button5->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
   for(int i=0;i<15;i++)
	{
		for(int j=0;j<15;j++)
		{
			if(img[i][j]!=NULL)
			{
			delete img[i][j];
			img[i][j]=NULL;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	Label6->Caption=IntToStr(Time_Clock);
	Time_Clock+=1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::whenimgclk(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)//�Ϥ����U��
{
  TImage *img2;
  img2=(TImage *)Sender;
  int imgtag= img2->Tag;//�Ϥ����s��
  if(Game_Start==false)//���]�m���u
  {
	img2->HelpContext=1;//��ܤw���}
	Set_Bomb(imgtag);//�]�m���u
	Set_Number();//�]�m�Ʀr
	Game_Start=true;
	img2->Picture->LoadFromFile(img2->Hint+".jpg");
	Spread();
  }
  else
  {
	if (Button == mbLeft&&img2->HelpContext!=2&&img2->HelpContext!=1)//���U����p�G�S���кX�]�S���i�}�N�|�X��
	{
		//ShowMessage("LEFT button"+IntToStr(img2->Tag)+IntToStr(img2->HelpContext));
		if(img2->Hint=="99")
		{
			Game_Over_Execute();
			ShowMessage("�Ĩ쬵�u�C������");
		}
		else
		{
			img2->HelpContext=1;//��ܤw���}
			img2->Picture->LoadFromFile(img2->Hint+".jpg");
			Spread();
		}
	}

	else if (Button == mbRight&&img2->HelpContext!=1)//���U�k��p�G�S���i�}�N�|�кX�Τ��кX
	{
		//ShowMessage("RIGHT button"+IntToStr(img2->Tag));
		if(img2->HelpContext!=2)
		{
			img2->HelpContext=2;
			img2->Picture->LoadFromFile("flag.jpg");
		}
		else
		{
			img2->HelpContext=0;
			img2->Picture->LoadFromFile("block.bmp");
		}

	}
	Game_Over_Check();//�P�_�C���O�_����
  }
}
//---------------------------------------------------------------------------
void TForm1::Set_Bomb(int imgtag)//���U�Ĥ@�ӫث� �]�m���u
{
	int Bomb_Arr[BombNumber];//�]�m�h�֭Ӭ��u���}�C
	int rd=(rand()%((LayoutSize*LayoutSize-1)-1+1));

	for(int i=0;i<BombNumber;i++)
	{
		rd=(rand()%((LayoutSize*LayoutSize-1)-1+1));
		Bomb_Arr[i]=rd;
		for(int j=0;j<i;j++)
		{
			while(Bomb_Arr[i]==Bomb_Arr[j]||Bomb_Arr[i]==imgtag)
			{
				rd=(rand()%((LayoutSize*LayoutSize-1)-1+1));
				Bomb_Arr[i]=rd;
                j=0;
            }
		}
		img[Bomb_Arr[i]/LayoutSize][Bomb_Arr[i]%LayoutSize]->Hint=99;
		//img[Bomb_Arr[i]/LayoutSize][Bomb_Arr[i]%LayoutSize]->Picture->LoadFromFile("Bomb.jpg");
	}
}
//---------------------------------------------------------------------------
void TForm1::Set_Number()//���U�Ĥ@�ӫث� �]�m�Ʀr
{
	int CountBomb=0;//�p��P�򬵼u
	int direct[8][2] = {{-1,0},{0,-1},{1,0},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};
	int dx=0;
	int dy=0;
	for(int i=0;i<LayoutSize;i++)//x�b
	{
		for(int j=0;j<LayoutSize;j++)//y�b
		{
			if(img[i][j]->Hint!=99)
			{
                CountBomb=0;
				for(int c=0;c<8;c++)//�P�_�P��8��
				{
					dx=i+direct[c][0];
					dy=j+direct[c][1];
					if((dx>=0&&dy>=0)&&(dx<LayoutSize&&dy<LayoutSize))
					{
						if(img[dx][dy]->Hint==99)
							CountBomb+=1;
                    }
				}

				img[i][j]->Hint=CountBomb;
				//img[i][j]->Picture->LoadFromFile(IntToStr(CountBomb)+".jpg");
            }
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::Spread()//�P�_�O�_�i�H�X��
{
	bool KeepGoing=true;//�O�_�~��P�_�X��

	int dx=0;
	int dy=0;
	int direct[8][2] = {{-1,0},{0,-1},{1,0},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};
	int CountBomb=0;//�Ȯɭp��P�򬵼u

	while(KeepGoing==true)//���٦��i�H�P�_���٨S�]����
	{
		KeepGoing=false;
		for(int i=0;i<LayoutSize;i++)//x�b
		{
			for(int j=0;j<LayoutSize;j++)//y�b
			{
				CountBomb=0;
				if(img[i][j]->HelpContext==1)//�i�}=1
				{
					for(int c=0;c<8;c++)//�P�_�P��8��
					{
						dx=i+direct[c][0];
						dy=j+direct[c][1];
						if((dx>=0&&dy>=0)&&(dx<LayoutSize&&dy<LayoutSize))
						{
							if(img[dx][dy]->Hint==99)
								CountBomb+=1;
						}
					}
					if(CountBomb==0)//�p�G�S���u�N�i�}
					{
						for(int c=0;c<8;c++)//�P�_�P��8��
						{
							dx=i+direct[c][0];
							dy=j+direct[c][1];
							if((dx>=0&&dy>=0)&&(dx<LayoutSize&&dy<LayoutSize))
							{
								if(img[dx][dy]->HelpContext==0)
								{
									img[dx][dy]->HelpContext=1;//���i�}=0 �i�}=1 �кX=2
									img[dx][dy]->Picture->LoadFromFile(img[dx][dy]->Hint+".jpg");
									KeepGoing=true;
								}
							}
						}

					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void TForm1::Game_Over_Check()//�P�_�C���O�_����
{
	int NotOpen=0;//�٨S���}
	int BombOk=0;
	for(int i=0;i<LayoutSize;i++)//x�b
	{
		for(int j=0;j<LayoutSize;j++)//y�b
		{
			if(img[i][j]->Hint==99&&img[i][j]->HelpContext==2)//�T�{�O���u//���i�}=0 �i�}=1 �кX=2
            {
				BombOk+=1;
			}
			if(img[i][j]->Hint!=99&&img[i][j]->HelpContext==0)
			{
				NotOpen+=1;
			}
        }
	}
	if(BombOk==BombNumber&&NotOpen==0)
	{
		Game_Over_Execute();
		ShowMessage("WIN�C������ ���Z:"+IntToStr(Time_Clock)+"��");
    }
}
//---------------------------------------------------------------------------
void TForm1::Game_Over_Execute()//����C������
{
	for(int i=0;i<Layout_Size;i++)
	{
		for(int j=0;j<Layout_Size;j++)
		{
			if(img[i][j]->Hint=="99")
				img[i][j]->Picture->LoadFromFile("Bomb.jpg");
			else
				img[i][j]->Picture->LoadFromFile(img[i][j]->Hint+".jpg");
			img[i][j]->Enabled=false;
		}
	}
	Timer1->Enabled=false;
	Button1->Visible=true;
	Button2->Visible=true;
	Button3->Visible=true;
	Button4->Visible=true;
	Button5->Visible=true;
}



