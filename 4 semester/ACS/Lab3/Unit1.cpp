//---------------------------------------------------------------------------

#include <vcl.h>
//#include <intrin.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TEdit *edits[40];
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  edits[0] = Edit1;    edits[1] = Edit2;    edits[2] = Edit3;
  edits[3] = Edit4;    edits[4] = Edit5;    edits[5] = Edit6;
  edits[6] = Edit7;    edits[7] = Edit8;    edits[8] = Edit9;
  edits[9] = Edit10;   edits[10] = Edit11;  edits[11] = Edit12;
  edits[12] = Edit13;  edits[13] = Edit14;  edits[14] = Edit15;
  edits[15] = Edit16;  edits[16] = Edit17;  edits[17] = Edit18;
  edits[18] = Edit19;  edits[19] = Edit20;  edits[20] = Edit21;
  edits[21] = Edit22;  edits[22] = Edit23;  edits[23] = Edit24;
  edits[24] = Edit25;  edits[25] = Edit26;  edits[26] = Edit27;
  edits[27] = Edit28;  edits[28] = Edit29;  edits[29] = Edit30;
  edits[30] = Edit31;  edits[31] = Edit32;

  for (int i = 0; i < 32; i++)
  {
    edits[i]->Text = 0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
  for (int i = 0; i < 32; i++)
  {
    edits[i]->Clear();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  for (int i = 0; i < 32; i++)
  {
    edits[i]->Text = StrToInt(rand() % 10 + 1);
  }  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  for (int i = 0; i < 32; i++)
  {
    try
    {
      isdigit(StrToInt(edits[i]->Text));
    }
    catch(...)
    {
      MessageBox(Handle,"Incorrect input! Input value must be integer!","Warning!",MB_OK);

      return;
    }
    if (StrToInt(edits[i]->Text) > 127 || StrToInt(edits[i]->Text) < -127)
    {
      MessageBox(Handle,"Input must be above -127 and less than 127!","Warning!",MB_OK);
      edits[i]->Text = 0;
      return;
    }
  }
}
//---------------------------------------------------------------------------


