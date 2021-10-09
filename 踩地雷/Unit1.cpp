//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;
#define Layout_Size 15//預設版面大小
int LayoutSize=10;//預設版面大小
int BombNumber=10;
int Time_Clock=0;//時間計時
bool Game_Start=false;//當點了第一個點後才會開始
TImage *img[Layout_Size][Layout_Size];//圖片顯示
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Label2->Caption=IntToStr(LayoutSize)+"x"+IntToStr(LayoutSize);
	Label4->Caption=IntToStr(BombNumber)+"個";
    time_t t;
	srand((unsigned)time(&t));    //亂數種子
}
//---------------------------------------------------------------------------
void TForm1::Print_Label()//顯示Label
{
	Label2->Caption=IntToStr(LayoutSize)+"x"+IntToStr(LayoutSize);
	Label4->Caption=IntToStr(BombNumber)+"個";
}
void __fastcall TForm1::Button2Click(TObject *Sender)//增加版面大小
{
	if(LayoutSize<15)
		LayoutSize+=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)//減少版面大小
{
	if(LayoutSize>10)
		LayoutSize-=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)//增加炸彈
{
	if(BombNumber<(LayoutSize*LayoutSize)/5)
		BombNumber+=1;
	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)//減少炸彈
{
	if(BombNumber>(LayoutSize*LayoutSize)/10)
		BombNumber-=1;
 	Print_Label();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)//開始遊戲
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
			img[i][j]->Hint=0;//提示 炸彈99 或 周圍炸彈數0~8
			img[i][j]->Tag=(i*Layout_Size)+j;//設定位置編號 0~99
			img[i][j]->HelpContext=0;//未展開=0 展開=1 標旗=2
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
          int X, int Y)//圖片按下時
{
  TImage *img2;
  img2=(TImage *)Sender;
  int imgtag= img2->Tag;//圖片的編號
  if(Game_Start==false)//先設置炸彈
  {
	img2->HelpContext=1;//表示已打開
	Set_Bomb(imgtag);//設置炸彈
	Set_Number();//設置數字
	Game_Start=true;
	img2->Picture->LoadFromFile(img2->Hint+".jpg");
	Spread();
  }
  else
  {
	if (Button == mbLeft&&img2->HelpContext!=2&&img2->HelpContext!=1)//按下左鍵如果沒有標旗也沒有展開就會擴散
	{
		//ShowMessage("LEFT button"+IntToStr(img2->Tag)+IntToStr(img2->HelpContext));
		if(img2->Hint=="99")
		{
			Game_Over_Execute();
			ShowMessage("採到炸彈遊戲結束");
		}
		else
		{
			img2->HelpContext=1;//表示已打開
			img2->Picture->LoadFromFile(img2->Hint+".jpg");
			Spread();
		}
	}

	else if (Button == mbRight&&img2->HelpContext!=1)//按下右鍵如果沒有展開就會標旗或不標旗
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
	Game_Over_Check();//判斷遊戲是否結束
  }
}
//---------------------------------------------------------------------------
void TForm1::Set_Bomb(int imgtag)//按下第一個建後 設置炸彈
{
	int Bomb_Arr[BombNumber];//設置多少個炸彈的陣列
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
void TForm1::Set_Number()//按下第一個建後 設置數字
{
	int CountBomb=0;//計算周圍炸彈
	int direct[8][2] = {{-1,0},{0,-1},{1,0},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};
	int dx=0;
	int dy=0;
	for(int i=0;i<LayoutSize;i++)//x軸
	{
		for(int j=0;j<LayoutSize;j++)//y軸
		{
			if(img[i][j]->Hint!=99)
			{
                CountBomb=0;
				for(int c=0;c<8;c++)//判斷周圍8格
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
void TForm1::Spread()//判斷是否可以擴散
{
	bool KeepGoing=true;//是否繼續判斷擴散

	int dx=0;
	int dy=0;
	int direct[8][2] = {{-1,0},{0,-1},{1,0},{0,1},{-1,-1},{1,1},{-1,1},{1,-1}};
	int CountBomb=0;//暫時計算周圍炸彈

	while(KeepGoing==true)//當還有可以判斷或還沒跑完時
	{
		KeepGoing=false;
		for(int i=0;i<LayoutSize;i++)//x軸
		{
			for(int j=0;j<LayoutSize;j++)//y軸
			{
				CountBomb=0;
				if(img[i][j]->HelpContext==1)//展開=1
				{
					for(int c=0;c<8;c++)//判斷周圍8格
					{
						dx=i+direct[c][0];
						dy=j+direct[c][1];
						if((dx>=0&&dy>=0)&&(dx<LayoutSize&&dy<LayoutSize))
						{
							if(img[dx][dy]->Hint==99)
								CountBomb+=1;
						}
					}
					if(CountBomb==0)//如果沒炸彈就展開
					{
						for(int c=0;c<8;c++)//判斷周圍8格
						{
							dx=i+direct[c][0];
							dy=j+direct[c][1];
							if((dx>=0&&dy>=0)&&(dx<LayoutSize&&dy<LayoutSize))
							{
								if(img[dx][dy]->HelpContext==0)
								{
									img[dx][dy]->HelpContext=1;//未展開=0 展開=1 標旗=2
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
void TForm1::Game_Over_Check()//判斷遊戲是否結束
{
	int NotOpen=0;//還沒打開
	int BombOk=0;
	for(int i=0;i<LayoutSize;i++)//x軸
	{
		for(int j=0;j<LayoutSize;j++)//y軸
		{
			if(img[i][j]->Hint==99&&img[i][j]->HelpContext==2)//確認是炸彈//未展開=0 展開=1 標旗=2
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
		ShowMessage("WIN遊戲結束 成績:"+IntToStr(Time_Clock)+"秒");
    }
}
//---------------------------------------------------------------------------
void TForm1::Game_Over_Execute()//執行遊戲結束
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



