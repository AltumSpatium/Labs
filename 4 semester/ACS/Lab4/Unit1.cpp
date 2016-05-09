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
AnsiString randDouble()
{
  int intpart = rand() % 10 + 1;
  int fractpart = rand() % 10 + 1;

  AnsiString result = IntToStr(intpart) + '.' + IntToStr(fractpart);
  return result;
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
  edits[30] = Edit31;  edits[31] = Edit32;  edits[32] = Edit33;
  edits[33] = Edit34;  edits[34] = Edit35;  edits[35] = Edit36;
  edits[36] = Edit37;  edits[37] = Edit38;  edits[38] = Edit39;
  edits[39] = Edit40;

  for (int i = 0; i < 32; i++)
  {
    edits[i]->Text = IntToStr(i) + "." + IntToStr(i);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
  for (int i = 0; i < 40; i++)
  {
    edits[i]->Clear();
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{
  for (int i = 0; i < 32; i++)
  {
    edits[i]->Text = randDouble();
  }
  for (int i = 32; i < 40; i++)
  {
    edits[i]->Clear();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  bool issse2=false;
  
  asm
  {
    mov eax,1
    mov ecx,0
    cpuid
  };

  int edx=_EDX;

  if(edx & (1 << 1))
    issse2=true;

  if (issse2)
    MessageBox(Handle,"SSE2 is available!","SSE2 Check",MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  for (int i = 0; i < 32; i++)
  {
    try
    {
      isdigit(StrToFloat(edits[i]->Text));
    }
    catch(...)
    {
      MessageBox(Handle,"Incorrect input! Input value must be integer!","Warning!",MB_OK);

      return;
    }
    if ((StrToFloat(edits[i]->Text) > 2147483647.0 || StrToFloat(edits[i]->Text) < -2147483648.0) && i < 24)
    {
      MessageBox(Handle,"Input must be above -2 147 483 648.0 and less than 2 147 483 647.0 in arrays A, B, C!","Warning!",MB_OK);
      edits[i]->Text = "0.0";
      return;
    }
    if ((StrToFloat(edits[i]->Text) > 9223372036854775807.0 || StrToFloat(edits[i]->Text) < -9223372036854775808.0) && i >= 24)
    {
      MessageBox(Handle,"Input must be above -9 223 372 036 854 775 808.0 and less than 9 223 372 036 854 775 807.0 in array D!","Warning!",MB_OK);
      edits[i]->Text = "0.0";
      return;
    }
  }

  float A[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  float B[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  float C[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  double D[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  double R1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  double R2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  double F[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 8; i++)
  {
    A[i] = StrToFloat(edits[i]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    B[i] = StrToFloat(edits[i+8]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    C[i] = StrToFloat(edits[i+16]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    D[i] = StrToFloat(edits[i+24]->Text);
  }

  // Multiplication of B & C: B[i]*C[i]
  asm
  {
    // Loading arrays addresses into cpu registers
    lea eax, A
    lea ebx, B
    lea ecx, C
    lea edx, D
    lea edi, F

    /* -- FIRST PARTS -- */

    // Loading 1 of 4 parts of array B into SSE register
    movups xmm0, [ebx]

    // Converting float to double in this register
    cvtps2pd xmm0, xmm0

    // Loading 1 of 4 parts of array C into SSE register
    movups xmm1, [ecx]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Multiplying first of 4 parts of B and C
    mulpd xmm0, xmm1

    // Reseting register xmm1 and loading 1/4 of array A in it
    xorps xmm1, xmm1
    movups xmm1, [eax]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Adding 1/4 of array A to 1/4 of B*C
    addpd xmm0, xmm1

    // Reseting register xmm1 and loading 1/4 of array D in it
    xorps xmm1, xmm1
    movups xmm1, [edx]

    // Adding 1/4 of array D to 1/4 of A + B*C
    addpd xmm0, xmm1

    // Saving 1/4 of result into memory
    movups [edi], xmm0

    // Reseting registers
    xorps xmm0, xmm0
    xorps xmm1, xmm1

    /* -- SECOND PARTS -- */

    // Loading 2 of 4 parts of array B into SSE register
    movups xmm0, [ebx+8]

    // Converting float to double in this register
    cvtps2pd xmm0, xmm0

    // Loading 2 of 4 parts of array C into SSE register
    movups xmm1, [ecx+8]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Multiplying second of 4 parts of B and C
    mulpd xmm0, xmm1

    // Reseting register xmm1 and loading 2/4 of array A in it
    xorps xmm1, xmm1
    movups xmm1, [eax+8]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Adding 2/4 of array A to 2/4 of B*C
    addpd xmm0, xmm1

    // Reseting register xmm1 and loading 2/4 of array D in it
    xorps xmm1, xmm1
    movups xmm1, [edx+16]

    // Adding 2/4 of array D to 2/4 of A + B*C
    addpd xmm0, xmm1

    // Saving 2/4 of result into memory
    movups [edi+16], xmm0

    // Reseting registers
    xorps xmm0, xmm0
    xorps xmm1, xmm1

    /* -- THIRD PARTS -- */

    // Loading 3 of 4 parts of array B into SSE register
    movups xmm0, [ebx+16]

    // Converting float to double in this register
    cvtps2pd xmm0, xmm0

    // Loading 3 of 4 parts of array C into SSE register
    movups xmm1, [ecx+16]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Multiplying third of 4 parts of B and C
    mulpd xmm0, xmm1

    // Reseting register xmm1 and loading 3/4 of array A in it
    xorps xmm1, xmm1
    movups xmm1, [eax+16]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Adding 3/4 of array A to 3/4 of B*C
    addpd xmm0, xmm1

    // Reseting register xmm1 and loading 3/4 of array D in it
    xorps xmm1, xmm1
    movups xmm1, [edx+32]

    // Adding 3/4 of array D to 3/4 of A + B*C
    addpd xmm0, xmm1

    // Saving 3/4 of result into memory
    movups [edi+32], xmm0

    // Reseting registers
    xorps xmm0, xmm0
    xorps xmm1, xmm1

    /* -- FOURTH PARTS -- */

    // Loading 4 of 4 parts of array B into SSE register
    movups xmm0, [ebx+24]

    // Converting float to double in this register
    cvtps2pd xmm0, xmm0

    // Loading 4 of 4 parts of array C into SSE register
    movups xmm1, [ecx+24]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Multiplying fourth of 4 parts of B and C
    mulpd xmm0, xmm1

    // Reseting register xmm1 and loading 4/4 of array A in it
    xorps xmm1, xmm1
    movups xmm1, [eax+24]

    // Converting float to double in this register
    cvtps2pd xmm1, xmm1

    // Adding 4/4 of array A to 4/4 of B*C
    addpd xmm0, xmm1

    // Reseting register xmm1 and loading 4/4 of array D in it
    xorps xmm1, xmm1
    movups xmm1, [edx+48]

    // Adding 4/4 of array D to 4/4 of A + B*C
    addpd xmm0, xmm1

    // Saving 4/4 of result into memory
    movups [edi+48], xmm0

    // Reseting registers
    xorps xmm0, xmm0
    xorps xmm1, xmm1
  };

  for (int i = 0; i < 8; i++)
  {
    edits[i+32]->Text = FloatToStrF(F[i], ffFixed, 8, 5);
  }
}
//---------------------------------------------------------------------------

