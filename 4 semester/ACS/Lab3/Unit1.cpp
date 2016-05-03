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
  edits[30] = Edit31;  edits[31] = Edit32;  edits[32] = Edit33;
  edits[33] = Edit34;  edits[34] = Edit35;  edits[35] = Edit36;
  edits[36] = Edit37;  edits[37] = Edit38;  edits[38] = Edit39;
  edits[39] = Edit40;

  for (int i = 0; i < 32; i++)
  {
    edits[i]->Text = 0;
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
    edits[i]->Text = StrToInt(rand() % 10 + 1);
  }
  for (int i = 32; i < 40; i++)
  {
    edits[i]->Clear();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  bool ismmx=false;
  
  asm
  {
    mov eax,1
    mov ecx,0
    cpuid
  };

  int edx=_EDX;

  if(edx & (1<<23))
    ismmx=true;

  if (ismmx)
    MessageBox(Handle,"MMX is available!","MMX Check",MB_OK);
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
    if ((StrToInt(edits[i]->Text) > 127 || StrToInt(edits[i]->Text) < -128) && i < 24)
    {
      MessageBox(Handle,"Input must be above -128 and less than 127 in arrays A, B, C!","Warning!",MB_OK);
      edits[i]->Text = 0;
      return;
    }
    if ((StrToInt(edits[i]->Text) > 32767 || StrToInt(edits[i]->Text) < -32768) && i >= 24)
    {
      MessageBox(Handle,"Input must be above -32768 and less than 32767 in array D!","Warning!",MB_OK);
      edits[i]->Text = 0;
      return;
    }
  }

  __int8 A[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int8 B[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int8 C[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int16 D[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int16 R1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int16 R2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  __int16 F[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  for (int i = 0; i < 8; i++)
  {
    A[i] = StrToInt(edits[i]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    B[i] = StrToInt(edits[i+8]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    C[i] = StrToInt(edits[i+16]->Text);
  }

  for (int i = 0; i < 8; i++)
  {
    D[i] = StrToInt(edits[i+24]->Text);
  }

  // Multiplication of B & C: B[i]*C[i]
  asm
  {
    pusha // Saving all registers to stack

    // Loading arrays B and C into registers eax and ebx
    lea eax, B
    lea ebx, C
    lea edx, R1

    // -- LOW PART --

    // Loading these arrays into MMX registers
    movq mm0, [eax]
    movq mm1, [ebx]

    // Reseting register mm5
    pxor mm5, mm5

    // Comparing array B to zero and unpacking it to mm0 & mm2
    pcmpgtb mm5, mm0
    punpcklbw mm0,mm5
    movq mm2, mm0

    // Comparing array C to zero and unpacking it to mm1 & mm3
    pcmpgtb mm5, mm1
    punpcklbw mm1, mm5
    movq mm3, mm1

    pmullw mm2, mm3 // Multiplying B&C's low bits
    pmulhw mm0, mm1 // Multiplying B&C's high bits

    // Copying high bits of multiplication
    movq mm3, mm2
    movq mm1, mm2
    movq mm2, mm0

    // Joining registers mm3-mm2 & mm1-mm0 and packing them into mm1 (dw -> w)
    punpckhwd mm3, mm2
    punpcklwd mm1, mm0
    packssdw mm1, mm3

    // Loading result in memory by the address of the array R1
    movq [edx], mm1

    // -- HIGH PART --

    // Loading these arrays into MMX registers
    movq mm0, [eax]
    movq mm1, [ebx]

    // Reseting register mm5
    pxor mm5, mm5

    // Comparing array B to zero and unpacking it to mm0 & mm2
    pcmpgtb mm5, mm0
    punpckhbw mm0,mm5
    movq mm2, mm0

    // Comparing array C to zero and unpacking it to mm1 & mm3
    pcmpgtb mm5, mm1
    punpckhbw mm1, mm5
    movq mm3, mm1

    pmullw mm2, mm3 // Multiplying B&C's low bits
    pmulhw mm0, mm1 // Multiplying B&C's high bits

    // Copying high bits of multiplication
    movq mm3, mm2
    movq mm1, mm2
    movq mm2, mm0

    // Joining registers mm3-mm2 & mm1-mm0 and packing them into mm1 (dw -> w)
    punpckhwd mm3, mm2
    punpcklwd mm1, mm0
    packssdw mm1, mm3

    // Loading result in memory by the address of the array R1
    movq [edx+8], mm1

    popa // Restoring all registers
    emms // Clearing MMX state
  };

  // Addition of A & B*C: A[i] + B[i]*C[i]
  asm
  {
    pusha // Saving all registers to stack

    // Loading arrays into cpu registers
    lea eax, A
    lea ebx, R1
    lea edx, R2

    // Loading array A and first part of result of multiplying into MMX registers
    movq mm0, [eax]
    movq mm1, [ebx]

    // Comparing array A to zero and unpacking data in mm0
    pcmpgtb mm5, mm0
    punpcklbw mm0, mm5

    // Adding mm1 to mm0
    paddsw mm0, mm1

    // Loading result in memory by the address of the array R2
    movq [edx], mm0

    // Loading array A and second part of result of multiplying into MMX registers
    movq mm2, [eax]
    movq mm3, [ebx+8]

    // Comparing array A to zero and unpacking data in mm2
    pxor mm5, mm5
    pcmpgtb mm5, mm2
    punpckhbw mm2, mm5

    // Adding mm3 to mm2
    paddsw mm2, mm3

    // Loading result in memory by the address of the array R2
    movq [edx+8], mm2

    popa // Loading all registers back
    emms // Clearing MMX state
  };

  // Addition of A + B*C & D: A[i] + B[i]*C[i] + D[i]
  asm
  {
    pusha // Saving all registers to stack

    // Loading arrays into cpu registers
    lea eax, D
    lea ebx, R2
    lea edx, F

    // Loading first parts of array D and result of multiplying into MMX registers
    movq mm0, [eax]
    movq mm1, [ebx]

    // Adding mm1 to mm0
    paddsw mm0, mm1

    // Loading result in memory by the address of the array F
    movq [edx], mm0

    // Loading second parts of array D and result of multiplying into MMX registers
    movq mm0, [eax+8]
    movq mm1, [ebx+8]

    // Adding mm1 to mm0
    paddsw mm0, mm1

    // Loading result in memory by the address of the array F
    movq [edx+8], mm0

    popa // Loading all registers back
    emms // Clearing MMX state
  }

  for (int i = 0; i < 8; i++)
  {
    edits[i+32]->Text = IntToStr(F[i]);
  }
}
//---------------------------------------------------------------------------

