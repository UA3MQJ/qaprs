unit FFTasm;

interface

uses Math, SysUtils, Windows;

const
     { Used by the Inverse 2 Point FFT to Scaledown the Samples. }
     ScaleFFT2 = 1 / 2;
     ScaleFFT4 = ScaleFFT2 / 2;
     ScaleFFT8 = ScaleFFT4 / 2;
     ScaleFFT16 = ScaleFFT8 / 2;
     ScaleFFT32 = ScaleFFT16 / 2;
     ScaleFFT64 = ScaleFFT32 / 2;
     ScaleFFT128 = ScaleFFT64 / 2;
     ScaleFFT256 = ScaleFFT128 / 2;
     ScaleFFT512 = ScaleFFT256 / 2;
     ScaleFFT1024 = ScaleFFT512 / 2;
     ScaleFFT2048 = ScaleFFT1024 / 2;
     ScaleFFT4096 = ScaleFFT2048 / 2;
     ScaleFFT8192 = ScaleFFT4096 / 2;

type  TComplex = record re, im : Single; end;
      TComplexArray = array[0..8191] of TComplex;
      PComplexArray = ^TComplexArray;

var
   { Used to get the Real Index of a Frequency in a 2 Point FFT. }
   RevBins2 : array[0..1] of WORD = (0,1);
   RevBins4 : array[0..3] of WORD;
   RevBins8 : array[0..7] of WORD;
   RevBins16 : array[0..15] of WORD;
   RevBins32 : array[0..31] of WORD;
   RevBins64 : array[0..63] of WORD;
   RevBins128 : array[0..127] of WORD;
   RevBins256 : array[0..255] of WORD;
   RevBins512 : array[0..511] of WORD;
   RevBins1024 : array[0..1023] of WORD;
   RevBins2048 : array[0..2047] of WORD;
   RevBins4096 : array[0..4095] of WORD;
   RevBins8192 : array[0..8191] of WORD;


  procedure dspDoFFT(Cplx : PComplexArray; FFTSize : integer; Inverse,Scale,ReOrder : Boolean; SwapBuffer : Pointer = nil);

implementation



var
  d32 : array[0..6] of TComplex;
  d64 : array[0..14] of TComplex;
  d128 : array[0..30] of TComplex;
  d256 : array[0..62] of TComplex;
  d512 : array[0..126] of TComplex;
  d1024 : array[0..126] of TComplex;
  d2048 : array[0..254] of TComplex;
  d4096 : array[0..510] of TComplex;
  d8192 : array[0..1022] of TComplex;

  fft2    : procedure (Cplx : PComplexArray);
  ifft2   : procedure (Cplx : PComplexArray);
  fft4    : procedure (Cplx : PComplexArray);
  ifft4   : procedure (Cplx : PComplexArray);
  fft8    : procedure (Cplx : PComplexArray);
  ifft8   : procedure (Cplx : PComplexArray);
  fft16   : procedure (Cplx : PComplexArray);
  ifft16  : procedure (Cplx : PComplexArray);
  fft32   : procedure (Cplx : PComplexArray);
  ifft32  : procedure (Cplx : PComplexArray);
  fft64   : procedure (Cplx : PComplexArray);
  ifft64  : procedure (Cplx : PComplexArray);
  fft128  : procedure (Cplx : PComplexArray);
  ifft128 : procedure (Cplx : PComplexArray);
  fft256  : procedure (Cplx : PComplexArray);
  ifft256 : procedure (Cplx : PComplexArray);
  fft512  : procedure (Cplx : PComplexArray);
  ifft512 : procedure (Cplx : PComplexArray);
  fft1024  : procedure (Cplx : PComplexArray);
  ifft1024  : procedure (Cplx : PComplexArray);
  fft2048  : procedure (Cplx : PComplexArray);
  ifft2048  : procedure (Cplx : PComplexArray);
  fft4096  : procedure (Cplx : PComplexArray);
  ifft4096  : procedure (Cplx : PComplexArray);
  fft8192  : procedure (Cplx : PComplexArray);
  ifft8192  : procedure (Cplx : PComplexArray);


  SqrtHalf : Single = 0.70710678118654;
  d16re : Single = 0.92387953251128;
  d16im : Single = 0.38268343236508;
  nd16re : Single = -0.92387953251128;
  nd16im : Single = -0.38268343236508;

  Params : PChar;
  EnableMMX : Boolean = False;
  EnableSSE : Boolean = False;


procedure InitRevBitTable;

  procedure CopyFromTo(pFrom, pTo : PWORDArray; x,z,m : PInteger);
  var
    i : integer;
  begin
    if x^ = 0 then x^ := 1 else x^ := 0;
    z^ := z^ shl 1 - x^;
    m^ := m^ shl 1;
    for i := 0 to (m^ shr 1 - 1) do
    begin
      pTo^[i]            := pFrom^[i] * 2;
      pTo^[i + m^ shr 1] := pFrom^[i] * 2;
    end;
    for i := z^ to (m^ - (m^ shr 1 - (z^ - 1))) do inc(pTo^[i]);
  end;

var
  z : integer;
  x : integer;
  m : integer;
begin
  x := 1;
  z := 1;
  m := 2;
  CopyFromTo(@RevBins2,@RevBins4,@x,@z,@m);
  CopyFromTo(@RevBins4,@RevBins8,@x,@z,@m);
  CopyFromTo(@RevBins8,@RevBins16,@x,@z,@m);
  CopyFromTo(@RevBins16,@RevBins32,@x,@z,@m);
  CopyFromTo(@RevBins32,@RevBins64,@x,@z,@m);
  CopyFromTo(@RevBins64,@RevBins128,@x,@z,@m);
  CopyFromTo(@RevBins128,@RevBins256,@x,@z,@m);
  CopyFromTo(@RevBins256,@RevBins512,@x,@z,@m);
  CopyFromTo(@RevBins512,@RevBins1024,@x,@z,@m);
  CopyFromTo(@RevBins1024,@RevBins2048,@x,@z,@m);
  CopyFromTo(@RevBins2048,@RevBins4096,@x,@z,@m);
  CopyFromTo(@RevBins4096,@RevBins8192,@x,@z,@m);
end;

procedure InitSinCosTable;

  procedure SinCosTable(n : Integer; s : PComplexArray);
  var
    tmp : Double;
    i : integer;
    m : integer;
    fs,fc : Extended;
  begin
    if n > 512 then m := n shr 3
               else m := n shr 2;
    tmp := 2 * PI / n;
    for i := 1 to m -1 do
    begin
      SinCos(tmp * i,fs,fc);
      s^[i-1].re := fc;
      s^[i-1].im := fs;
    end;
  end;

begin
  SinCosTable(32,@d32);
  SinCosTable(64,@d64);
  SinCosTable(128,@d128);
  SinCosTable(256,@d256);
  SinCosTable(512,@d512);
  SinCosTable(1024,@d1024);
  SinCosTable(2048,@d2048);
  SinCosTable(4096,@d4096);
  SinCosTable(8192,@d8192);
end;


procedure c2_FPU(a : PComplexArray);
asm
        fld       DWORD PTR [eax]
        fld       st(0)
        fld       DWORD PTR [eax+8]
        fadd      st(1), st
        fsubp     st(2), st
        fld       DWORD PTR [eax+12]
        fld       DWORD PTR [eax+4]
        fld       st(0)
        fadd      st, st(2)
        fxch      st(2)
        fsubp     st(1), st
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fstp      DWORD PTR [eax+4]
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fstp      DWORD PTR [eax+12]                           
end;

procedure c4_FPU(a : PComplexArray);
asm
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+16]
        fld       st(0)
        fld       DWORD PTR [eax+8]
        fadd      DWORD PTR [eax+24]
        fadd      st(1), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+20]
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+8]
        fsub      DWORD PTR [eax+24]
        fxch      st(3)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+20]
        fld       st(0)
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fsubp     st(1), st
        fld       DWORD PTR [eax+12]
        fsub      DWORD PTR [eax+28]
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       st(2)
        fadd      st, st(4)
        fstp      DWORD PTR [eax+20]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+16]
        fxch      st(3)
        fsubp     st(2), st
        faddp     st(2), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+24]
end;

procedure c8_FPU(a : PComplexArray);
asm
        sub       esp, 16
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+32]
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+32]
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+36]
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+36]
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+16]
        fsub      DWORD PTR [eax+48]
        fld       DWORD PTR [eax+16]
        fadd      DWORD PTR [eax+48]
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax+20]
        fld       DWORD PTR [eax+52]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+20]
        fld       st(3)
        fsub      st, st(1)
        fst       DWORD PTR [esp]
        fstp      DWORD PTR [eax+32]
        fld       st(2)
        fadd      st, st(2)
        fst       DWORD PTR [esp+4]
        fstp      DWORD PTR [eax+36]
        faddp     st(3), st
        fxch      st(2)
        fst       DWORD PTR [esp+8]
        fstp      DWORD PTR [eax+48]
        fsubrp    st(1), st
        fst       DWORD PTR [esp+12]
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [eax+40]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fld       DWORD PTR [eax+44]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+24]
        fld       st(0)
        fld       DWORD PTR [eax+56]
        fsub      st(1), st
        fld       DWORD PTR [eax+28]
        fld       st(0)
        fld       DWORD PTR [eax+60]
        fsub      st(1), st
        fxch      st(3)
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+28]
        fld       st(3)
        fsub      st, st(2)
        fxch      st(2)
        faddp     st(4), st
        fld       st(2)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(3), st
        fld       st(1)
        fsub      st, st(3)
        fmul      DWORD PTR SqrtHalf
        fxch      st(3)
        faddp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)
        fmul      DWORD PTR SqrtHalf
        fxch      st(4)
        faddp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+48]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+52]
        fsub      st, st(4)
        fstp      DWORD PTR [eax+60]
        fld       DWORD PTR [esp+8]
        faddp     st(1), st
        fstp      DWORD PTR [eax+48]
        fld       DWORD PTR [esp+12]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+32]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+36]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [esp]
        faddp     st(1), st
        fstp      DWORD PTR [eax+32]
        fld       DWORD PTR [esp+4]
        faddp     st(1), st
        fstp      DWORD PTR [eax+36]
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+16]
        fld       st(0)
        fld       DWORD PTR [eax+8]
        fadd      DWORD PTR [eax+24]
        fadd      st(1), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+20]
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+8]
        fsub      DWORD PTR [eax+24]
        fxch      st(3)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+20]
        fld       st(0)
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fsubp     st(1), st
        fld       DWORD PTR [eax+12]
        fsub      DWORD PTR [eax+28]
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       st(2)
        fadd      st, st(4)
        fstp      DWORD PTR [eax+20]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+16]
        fxch      st(3)
        fsubp     st(2), st
        faddp     st(2), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+24]
        add       esp, 16
end;

procedure c16_FPU(a : PComplexArray);
asm
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+64]
        fld       DWORD PTR [eax+64]
        fadd      DWORD PTR [eax]
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+68]
        fld       DWORD PTR [eax+68]
        fadd      DWORD PTR [eax+4]
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+32]
        fsub      DWORD PTR [eax+96]
        fld       DWORD PTR [eax+96]
        fadd      DWORD PTR [eax+32]
        fstp      DWORD PTR [eax+32]
        fld       DWORD PTR [eax+36]
        fsub      DWORD PTR [eax+100]
        fld       DWORD PTR [eax+100]
        fadd      DWORD PTR [eax+36]
        fstp      DWORD PTR [eax+36]
        fld       st(3)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+64]
        fld       st(2)
        fsub      st, st(2)
        fstp      DWORD PTR [eax+100]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+96]
        faddp     st(1), st
        fstp      DWORD PTR [eax+68]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [eax+72]
        fsub      st(1), st
        fld       DWORD PTR [eax+8]
        faddp     st(1), st
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fsub      DWORD PTR [eax+76]
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [eax+76]
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+44]
        fsub      DWORD PTR [eax+108]
        fld       DWORD PTR [eax+44]
        fadd      DWORD PTR [eax+108]
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+40]
        fsub      DWORD PTR [eax+104]
        fld       DWORD PTR [eax+40]
        fadd      DWORD PTR [eax+104]
        fstp      DWORD PTR [eax+40]
        fld       st(3)
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st
        fld       st(0)
        fadd      st, st(3)
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR d16re
        fmul      st, st(4)
        fld       DWORD PTR nd16im
        fmul      st, st(2)
        faddp     st(1), st
        fstp      DWORD PTR [eax+72]
        fld       DWORD PTR d16re
        fld       DWORD PTR nd16im
        fmul      st, st(3)
        fld       st(4)
        fmul      st, st(2)
        faddp     st(1), st
        fstp      DWORD PTR [eax+108]
        fld       DWORD PTR d16im
        fmul      st(5), st
        fxch      st(1)
        fmul      st(2), st
        fxch      st(2)
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax+76]
        fmulp     st(1), st
        fxch      st(2)
        fmulp     st(1), st
        faddp     st(1), st
        fstp      DWORD PTR [eax+104]
        fld       DWORD PTR [eax+16]
        fsub      DWORD PTR [eax+80]
        fld       DWORD PTR [eax+16]
        fadd      DWORD PTR [eax+80]
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax+20]
        fsub      DWORD PTR [eax+84]
        fld       DWORD PTR [eax+20]
        fadd      DWORD PTR [eax+84]
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [eax+52]
        fsub      DWORD PTR [eax+116]
        fld       DWORD PTR [eax+52]
        fadd      DWORD PTR [eax+116]
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+48]
        fsub      DWORD PTR [eax+112]
        fld       DWORD PTR [eax+48]
        fadd      DWORD PTR [eax+112]
        fstp      DWORD PTR [eax+48]
        fld       st(0)
        fadd      st, st(3)
        fxch      st(1)
        fsubp     st(3), st
        fld       st(3)
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st
        fld       st(1)
        fadd      st, st(3)
        fmul      DWORD PTR SqrtHalf
        fstp      DWORD PTR [eax+112]
        fld       DWORD PTR SqrtHalf
        fxch      st(2)
        fsubp     st(3), st
        fxch      st(1)
        fmul      st(2), st
        fxch      st(2)
        fstp      DWORD PTR [eax+116]
        fld       st(2)
        fsub      st, st(1)
        fmulp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+80]
        faddp     st(1), st
        fmul      DWORD PTR SqrtHalf
        fstp      DWORD PTR [eax+84]
        fld       DWORD PTR [eax+24]
        fld       DWORD PTR [eax+88]
        fsub      st(1), st
        fld       DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [eax+28]
        fsub      DWORD PTR [eax+92]
        fld       DWORD PTR [eax+28]
        fadd      DWORD PTR [eax+92]
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [eax+60]
        fsub      DWORD PTR [eax+124]
        fld       DWORD PTR [eax+60]
        fadd      DWORD PTR [eax+124]
        fstp      DWORD PTR [eax+60]
        fld       DWORD PTR [eax+56]
        fsub      DWORD PTR [eax+120]
        fld       DWORD PTR [eax+56]
        fadd      DWORD PTR [eax+120]
        fstp      DWORD PTR [eax+56]
        fld       st(3)
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st
        fld       st(0)
        fadd      st, st(3)
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR d16im
        fmul      st, st(4)
        fld       DWORD PTR nd16re
        fmul      st, st(2)
        faddp     st(1), st
        fstp      DWORD PTR [eax+88]
        fld       DWORD PTR d16im
        fld       DWORD PTR nd16re
        fmul      st, st(3)
        fld       st(4)
        fmul      st, st(2)
        faddp     st(1), st
        fstp      DWORD PTR [eax+124]
        fld       DWORD PTR d16re
        fmul      st(5), st
        fxch      st(1)
        fmul      st(2), st
        fxch      st(2)
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax+92]
        fmulp     st(1), st
        fxch      st(2)
        fmulp     st(1), st
        faddp     st(1), st
        fstp      DWORD PTR [eax+120]
        sub       esp, 16
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+32]
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+32]
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+36]
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+36]
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+16]
        fsub      DWORD PTR [eax+48]
        fld       DWORD PTR [eax+16]
        fadd      DWORD PTR [eax+48]
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax+20]
        fld       DWORD PTR [eax+52]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+20]
        fld       st(3)
        fsub      st, st(1)
        fst       DWORD PTR [esp]
        fstp      DWORD PTR [eax+32]
        fld       st(2)
        fadd      st, st(2)
        fst       DWORD PTR [esp+4]
        fstp      DWORD PTR [eax+36]
        faddp     st(3), st
        fxch      st(2)
        fst       DWORD PTR [esp+8]
        fstp      DWORD PTR [eax+48]
        fsubrp    st(1), st
        fst       DWORD PTR [esp+12]
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [eax+40]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fld       DWORD PTR [eax+44]
        fld       st(1)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+24]
        fld       st(0)
        fld       DWORD PTR [eax+56]
        fsub      st(1), st
        fld       DWORD PTR [eax+28]
        fld       st(0)
        fld       DWORD PTR [eax+60]
        fsub      st(1), st
        fxch      st(3)
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+28]
        fld       st(3)
        fsub      st, st(2)
        fxch      st(2)
        faddp     st(4), st
        fld       st(2)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(3), st
        fld       st(1)
        fsub      st, st(3)
        fmul      DWORD PTR SqrtHalf
        fxch      st(3)
        faddp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)
        fmul      DWORD PTR SqrtHalf
        fxch      st(4)
        faddp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+48]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+52]
        fsub      st, st(4)
        fstp      DWORD PTR [eax+60]
        fld       DWORD PTR [esp+8]
        faddp     st(1), st
        fstp      DWORD PTR [eax+48]
        fld       DWORD PTR [esp+12]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+32]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+36]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [esp]
        faddp     st(1), st
        fstp      DWORD PTR [eax+32]
        fld       DWORD PTR [esp+4]
        faddp     st(1), st
        fstp      DWORD PTR [eax+36]
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+16]
        fld       st(0)
        fld       DWORD PTR [eax+8]
        fadd      DWORD PTR [eax+24]
        fadd      st(1), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+20]
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+8]
        fsub      DWORD PTR [eax+24]
        fxch      st(3)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+20]
        fld       st(0)
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fsubp     st(1), st
        fld       DWORD PTR [eax+12]
        fsub      DWORD PTR [eax+28]
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       st(2)
        fadd      st, st(4)
        fstp      DWORD PTR [eax+20]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+16]
        fxch      st(3)
        fsubp     st(2), st
        faddp     st(2), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+24]
        add       esp, 16
        fld       DWORD PTR [eax+64]
        fadd      DWORD PTR [eax+80]
        fld       st(0)
        fld       DWORD PTR [eax+72]
        fadd      DWORD PTR [eax+88]
        fadd      st(1), st
        fld       DWORD PTR [eax+64]
        fsub      DWORD PTR [eax+80]
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [eax+68]
        fsub      DWORD PTR [eax+84]
        fxch      st(2)
        fstp      DWORD PTR [eax+64]
        fld       DWORD PTR [eax+72]
        fsub      DWORD PTR [eax+88]
        fxch      st(3)
        fstp      DWORD PTR [eax+72]
        fld       DWORD PTR [eax+68]
        fadd      DWORD PTR [eax+84]
        fld       st(0)
        fld       DWORD PTR [eax+76]
        fadd      DWORD PTR [eax+92]
        fadd      st(1), st
        fxch      st(1)
        fstp      DWORD PTR [eax+68]
        fsubp     st(1), st
        fld       DWORD PTR [eax+76]
        fsub      DWORD PTR [eax+92]
        fxch      st(1)
        fstp      DWORD PTR [eax+76]
        fld       st(2)
        fadd      st, st(4)
        fstp      DWORD PTR [eax+84]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+80]
        fxch      st(3)
        fsubp     st(2), st
        faddp     st(2), st
        fstp      DWORD PTR [eax+92]
        fstp      DWORD PTR [eax+88]
        fld       DWORD PTR [eax+96]
        fadd      DWORD PTR [eax+112]
        fld       st(0)
        fld       DWORD PTR [eax+104]
        fadd      DWORD PTR [eax+120]
        fadd      st(1), st
        fld       DWORD PTR [eax+96]
        fsub      DWORD PTR [eax+112]
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [eax+100]
        fsub      DWORD PTR [eax+116]
        fxch      st(2)
        fstp      DWORD PTR [eax+96]
        fld       DWORD PTR [eax+104]
        fsub      DWORD PTR [eax+120]
        fxch      st(3)
        fstp      DWORD PTR [eax+104]
        fld       DWORD PTR [eax+100]
        fadd      DWORD PTR [eax+116]
        fld       st(0)
        fld       DWORD PTR [eax+108]
        fadd      DWORD PTR [eax+124]
        fadd      st(1), st
        fxch      st(1)
        fstp      DWORD PTR [eax+100]
        fsubp     st(1), st
        fld       DWORD PTR [eax+108]
        fsub      DWORD PTR [eax+124]
        fxch      st(1)
        fstp      DWORD PTR [eax+108]
        fld       st(2)
        fadd      st, st(4)
        fstp      DWORD PTR [eax+116]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+112]
        fxch      st(3)
        fsubp     st(2), st
        faddp     st(2), st
        fstp      DWORD PTR [eax+124]
        fstp      DWORD PTR [eax+120]
end;

procedure cpass_FPU(a,w : PComplexArray; n : integer);
asm
        push      edi
        push      esi
        push      ebp
        push      ebx
        sub       esp, 12
        lea       ebx, DWORD PTR [ecx+ecx]
        mov       DWORD PTR [esp+8], eax
        add       ebx, ebx
        add       ebx, ebx
        add       ebx, ebx
        add       ebx, ebx
        add       ebx, eax
        lea       edi, DWORD PTR [ecx-1]
        add       ecx, ecx
        add       ecx, ecx
        add       ecx, ecx
        add       ecx, ecx
        lea       ebp, DWORD PTR [eax+ecx]
        lea       esi, DWORD PTR [ebx+ecx]
        
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [ebx]
        fld       DWORD PTR [ebx]
        fadd      DWORD PTR [eax]
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [ebx+4]
        fld       DWORD PTR [ebx+4]
        fadd      DWORD PTR [eax+4]
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [ebp]
        fsub      DWORD PTR [esi]
        fld       DWORD PTR [esi]
        fadd      DWORD PTR [ebp]
        fstp      DWORD PTR [ebp]
        fld       DWORD PTR [ebp+4]
        fsub      DWORD PTR [esi+4]
        fld       DWORD PTR [esi+4]
        fadd      DWORD PTR [ebp+4]
        fstp      DWORD PTR [ebp+4]
        fld       st(3)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(4), st
        fstp      DWORD PTR [ebx]
        fld       st(1)
        fsub      st, st(1)
        fstp      DWORD PTR [esi+4]
        faddp     st(1), st
        fxch      st(1)
        fstp      DWORD PTR [esi]
        fstp      DWORD PTR [ebx+4]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [ebx+8]
        fsub      st(1), st
        fld       st(1)
        fld       DWORD PTR [eax+8]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fsub      DWORD PTR [ebx+12]
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [ebx+12]
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [ebp+12]
        fsub      DWORD PTR [esi+12]
        fsub      st(2), st
        fld       DWORD PTR [ebp+12]
        fadd      DWORD PTR [esi+12]
        fxch      st(4)
        faddp     st(1), st
        fxch      st(3)
        fstp      DWORD PTR [ebp+12]
        fld       DWORD PTR [ebp+8]
        fsub      DWORD PTR [esi+8]
        fld       st(0)
        fadd      st, st(2)
        fld       DWORD PTR [ebp+8]
        fadd      DWORD PTR [esi+8]
        fxch      st(2)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [ebp+8]
        fld       DWORD PTR [edx]
        fmul      st, st(3)
        fld       DWORD PTR [edx+4]
        fmul      st, st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [ebx+8]
        fld       DWORD PTR [edx+4]
        fmul      st, st(4)
        fld       DWORD PTR [edx]
        fmul      st, st(3)
        fsubrp    st(1), st
        fstp      DWORD PTR [esi+12]
        fxch      st(2)
        fmul      DWORD PTR [edx+4]
        fxch      st(2)
        fmul      DWORD PTR [edx]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [ebx+12]
        fxch      st(1)
        fmul      DWORD PTR [edx]
        fxch      st(1)
        fmul      DWORD PTR [edx+4]
        faddp     st(1), st
        fstp      DWORD PTR [esi+8]
        mov       DWORD PTR [esp+4], ecx
        jmp       @@B92
@@B93:
        mov       ebp, DWORD PTR [esp]
        mov       ebx, ecx
        mov       eax, DWORD PTR [esp+4]
        mov       DWORD PTR [esp+8], ebp
        lea       ebp, DWORD PTR [ebp+eax]
        lea       esi, DWORD PTR [ecx+eax]
@@B92:
        mov       eax, DWORD PTR [esp+8]
        lea       ecx, DWORD PTR [eax+16]
        mov       DWORD PTR [esp], ecx
        lea       ecx, DWORD PTR [ebx+16]
        fld       DWORD PTR [eax+16]
        fld       DWORD PTR [ebx+16]
        fsub      st(1), st
        fld       st(1)
        fld       DWORD PTR [eax+16]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax+20]
        fsub      DWORD PTR [ebx+20]
        fld       DWORD PTR [eax+20]
        fadd      DWORD PTR [ebx+20]
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [ebp+20]
        fsub      DWORD PTR [esi+20]
        fsub      st(2), st
        fld       DWORD PTR [ebp+20]
        fadd      DWORD PTR [esi+20]
        fxch      st(4)
        faddp     st(1), st
        fxch      st(3)
        fstp      DWORD PTR [ebp+20]
        fld       DWORD PTR [ebp+16]
        fsub      DWORD PTR [esi+16]
        fld       DWORD PTR [ebp+16]
        fadd      DWORD PTR [esi+16]
        fstp      DWORD PTR [ebp+16]
        fld       st(0)
        fadd      st, st(2)
        fxch      st(1)
        fsubp     st(2), st
        fld       DWORD PTR [edx+8]
        fmul      st, st(3)
        fld       DWORD PTR [edx+12]
        fmul      st, st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [ebx+16]
        fld       DWORD PTR [edx+12]
        fmul      st, st(4)
        fld       DWORD PTR [edx+8]
        fmul      st, st(3)
        fsubrp    st(1), st
        fstp      DWORD PTR [esi+20]
        fxch      st(2)
        fmul      DWORD PTR [edx+12]
        fxch      st(2)
        fmul      DWORD PTR [edx+8]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [ebx+20]
        fxch      st(1)
        fmul      DWORD PTR [edx+8]
        fxch      st(1)
        fmul      DWORD PTR [edx+12]
        faddp     st(1), st
        fstp      DWORD PTR [esi+16]
        add       edx, 16
        dec       edi
        fld       DWORD PTR [eax+24]
        fld       DWORD PTR [ebx+24]
        fsub      st(1), st
        fld       DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [eax+28]
        fsub      DWORD PTR [ebx+28]
        fld       DWORD PTR [eax+28]
        fadd      DWORD PTR [ebx+28]
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [ebp+28]
        fsub      DWORD PTR [esi+28]
        fld       DWORD PTR [ebp+28]
        fadd      DWORD PTR [esi+28]
        fstp      DWORD PTR [ebp+28]
        fld       DWORD PTR [ebp+24]
        fsub      DWORD PTR [esi+24]
        fld       st(0)
        fld       DWORD PTR [ebp+24]
        fadd      DWORD PTR [esi+24]
        fxch      st(1)
        fadd      st, st(4)
        fxch      st(1)
        fstp      DWORD PTR [ebp+24]
        fld       st(4)
        fsub      st, st(3)
        fxch      st(5)
        faddp     st(3), st
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [edx]
        fmul      st, st(4)
        fld       DWORD PTR [edx+4]
        fmul      st, st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [ebx+24]
        fld       DWORD PTR [edx+4]
        fmul      st, st(2)
        fld       DWORD PTR [edx]
        fmul      st, st(4)
        fsubrp    st(1), st
        fstp      DWORD PTR [esi+28]
        fxch      st(3)
        fmul      DWORD PTR [edx+4]
        fxch      st(3)
        fmul      DWORD PTR [edx]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [ebx+28]
        fxch      st(1)
        fmul      DWORD PTR [edx]
        fxch      st(1)
        fmul      DWORD PTR [edx+4]
        faddp     st(1), st
        fstp      DWORD PTR [esi+24]
        jne       @@B93
        add       esp, 12
        pop       ebx
        pop       ebp
        pop       esi
        pop       edi
end;

procedure cpassbig_FPU(a,w : PComplexArray; n : integer);
asm
        push      edi
        push      esi
        push      ebp
        push      ebx
        sub       esp, 32
        lea       esi, DWORD PTR [ecx+ecx]
        add       esi, esi
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+esi*8]
        fld       DWORD PTR [eax+esi*8]
        fadd      DWORD PTR [eax]
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+esi*8+4]
        fld       DWORD PTR [eax+esi*8+4]
        fadd      DWORD PTR [eax+4]
        fstp      DWORD PTR [eax+4]
        lea       ebp, DWORD PTR [ecx+ecx]
        lea       ebx, DWORD PTR [ecx+ecx]
        add       ebx, ebx
        sub       ebx, ecx
        add       ebx, ebx
        fld       DWORD PTR [eax+ebp*8]
        fsub      DWORD PTR [eax+ebx*8]
        fld       DWORD PTR [eax+ebx*8]
        fadd      DWORD PTR [eax+ebp*8]
        fstp      DWORD PTR [eax+ebp*8]
        fld       DWORD PTR [eax+ebp*8+4]                       
        fsub      DWORD PTR [eax+ebx*8+4]
        fld       DWORD PTR [eax+ebx*8+4]
        fadd      DWORD PTR [eax+ebp*8+4]
        fstp      DWORD PTR [eax+ebp*8+4]                       
        fld       st(3)
        fsub      st, st(1)
        fstp      DWORD PTR [eax+esi*8]
        fld       st(2)                                         
        fsub      st, st(2)
        fstp      DWORD PTR [eax+ebx*8+4]
        faddp     st(3), st
        fxch      st(2)                                         
        fstp      DWORD PTR [eax+ebx*8]
        faddp     st(1), st
        fstp      DWORD PTR [eax+esi*8+4]
        dec       ecx                                           
        mov       edi, ecx
        mov       DWORD PTR [esp+12], edx
        mov       DWORD PTR [esp+16], eax
        mov       DWORD PTR [esp+8], ecx
@@B122:
        lea       ecx, DWORD PTR [eax+8]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [eax+esi*8+8]
        fsub      st(1), st
        fld       DWORD PTR [eax+8]
        faddp     st(1), st
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]                            
        fsub      DWORD PTR [eax+esi*8+12]
        fld       DWORD PTR [eax+12]                            
        fadd      DWORD PTR [eax+esi*8+12]
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+ebp*8+12]
        fsub      DWORD PTR [eax+ebx*8+12]                      
        fld       DWORD PTR [eax+ebp*8+12]
        fadd      DWORD PTR [eax+ebx*8+12]                      
        fstp      DWORD PTR [eax+ebp*8+12]
        fld       DWORD PTR [eax+ebp*8+8]
        fsub      DWORD PTR [eax+ebx*8+8]                       
        fld       DWORD PTR [eax+ebp*8+8]
        fadd      DWORD PTR [eax+ebx*8+8]
        fstp      DWORD PTR [eax+ebp*8+8]
        fld       st(3)                                         
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st                                     
        fld       st(0)
        fadd      st, st(3)                                     
        fxch      st(1)
        fsubp     st(3), st
        fld       DWORD PTR [edx]
        fmul      st, st(4)                                     
        fld       DWORD PTR [edx+4]
        fmul      st, st(2)                                     
        fsubp     st(1), st                                     
        fstp      DWORD PTR [eax+esi*8+8]
        fld       DWORD PTR [edx+4]
        fmul      st, st(2)                                     
        fld       DWORD PTR [edx]
        fmul      st, st(4)                                     
        fsubrp    st(1), st
        fstp      DWORD PTR [eax+ebx*8+12]
        fld       DWORD PTR [edx+4]
        fmulp     st(4), st
        fld       DWORD PTR [edx]
        fmulp     st(1), st
        faddp     st(3), st                                     
        fxch      st(2)
        fstp      DWORD PTR [eax+esi*8+12]
        fld       DWORD PTR [edx]
        fmulp     st(2), st                                     
        fld       DWORD PTR [edx+4]
        fmulp     st(1), st                                     
        faddp     st(1), st
        fstp      DWORD PTR [eax+ebx*8+8]
        mov       eax, ecx
        add       edx, 8
        dec       edi
        jne       @@B122
@@B123:
        mov       DWORD PTR [esp], ecx
        mov       ecx, DWORD PTR [esp+8]
        mov       DWORD PTR [esp+8], ecx
        mov       ecx, DWORD PTR [esp]
        lea       edi, DWORD PTR [ecx+8]
        mov       DWORD PTR [esp+4], edi
        fld       DWORD PTR [ecx+8]
        fsub      DWORD PTR [eax+esi*8+8]
        fld       DWORD PTR [ecx+8]
        fadd      DWORD PTR [eax+esi*8+8]                       
        fstp      DWORD PTR [ecx+8]
        fld       DWORD PTR [ecx+12]                            
        fsub      DWORD PTR [eax+esi*8+12]
        fld       DWORD PTR [ecx+12]
        fadd      DWORD PTR [eax+esi*8+12]
        fstp      DWORD PTR [ecx+12]                            
        fld       DWORD PTR [eax+ebp*8+12]
        fsub      DWORD PTR [eax+ebx*8+12]                      
        fld       DWORD PTR [eax+ebp*8+12]
        fadd      DWORD PTR [eax+ebx*8+12]                      
        fstp      DWORD PTR [eax+ebp*8+12]
        fld       DWORD PTR [eax+ebp*8+8]
        fsub      DWORD PTR [eax+ebx*8+8]                       
        fld       DWORD PTR [eax+ebp*8+8]
        fadd      DWORD PTR [eax+ebx*8+8]
        fstp      DWORD PTR [eax+ebp*8+8]                       
        fld       st(0)                                         
        fadd      st, st(3)
        fxch      st(1)
        fsubp     st(3), st                                     
        fld       st(3)
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st
        fld       st(1)
        fadd      st, st(3)                                     
        fmul      DWORD PTR SqrtHalf
        fstp      DWORD PTR [eax+ebx*8+8]
        fld       DWORD PTR SqrtHalf
        fxch      st(2)
        fsubp     st(3), st
        fxch      st(1)
        fmul      st(2), st
        fxch      st(2)
        fstp      DWORD PTR [eax+ebx*8+12]
        fld       st(2)
        fsub      st, st(1)                                     
        fmulp     st(2), st
        fxch      st(1)                                         
        fstp      DWORD PTR [eax+esi*8+8]
        faddp     st(1), st
        fmul      DWORD PTR SqrtHalf
        fstp      DWORD PTR [eax+esi*8+12]
        add       edx, -8
        mov       ecx, DWORD PTR [esp+8]
        mov       DWORD PTR [esp+20], ecx
        mov       DWORD PTR [esp+24], edx
        mov       DWORD PTR [esp+28], edi
        mov       eax, edi                                      
@@B124:
        lea       edi, DWORD PTR [eax+8]
        fld       DWORD PTR [eax+8]
        fld       DWORD PTR [eax+esi*8+8]                       
        fsub      st(1), st
        fld       DWORD PTR [eax+8]
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]                            
        fsub      DWORD PTR [eax+esi*8+12]
        fld       DWORD PTR [eax+12]
        fadd      DWORD PTR [eax+esi*8+12]
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+ebp*8+12]
        fsub      DWORD PTR [eax+ebx*8+12]                      
        fld       DWORD PTR [eax+ebp*8+12]
        fadd      DWORD PTR [eax+ebx*8+12]
        fstp      DWORD PTR [eax+ebp*8+12]
        fld       DWORD PTR [eax+ebp*8+8]                       
        fsub      DWORD PTR [eax+ebx*8+8]
        fld       DWORD PTR [eax+ebp*8+8]
        fadd      DWORD PTR [eax+ebx*8+8]
        fstp      DWORD PTR [eax+ebp*8+8]
        fld       st(3)
        fsub      st, st(2)
        fxch      st(4)
        faddp     st(2), st
        fld       st(0)
        fadd      st, st(3)                                     
        fxch      st(1)
        fsubp     st(3), st                                     
        fld       DWORD PTR [edx+4]
        fmul      st, st(4)
        fld       DWORD PTR [edx]
        fmul      st, st(2)                                     
        fsubp     st(1), st
        fstp      DWORD PTR [eax+esi*8+8]
        fld       DWORD PTR [edx]
        fmul      st, st(2)
        fld       DWORD PTR [edx+4]
        fmul      st, st(4)                                     
        fsubrp    st(1), st
        fstp      DWORD PTR [eax+ebx*8+12]
        fld       DWORD PTR [edx]
        fmulp     st(4), st
        fld       DWORD PTR [edx+4]
        fmulp     st(1), st
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+esi*8+12]
        fld       DWORD PTR [edx+4]
        fmulp     st(2), st
        fld       DWORD PTR [edx]
        fmulp     st(1), st
        faddp     st(1), st
        fstp      DWORD PTR [eax+ebx*8+8]
        mov       eax, edi
        add       edx, -8
        dec       ecx
        jne       @@B124
@@B125:
        add       esp, 32
        pop       ebx
        pop       ebp
        pop       esi
        pop       edi
end;

procedure u2_FPU(a : PComplexArray);
begin
  c2_FPU(a);
end;

procedure u4_FPU(a : PComplexArray);
asm
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+8]
        fld       st(0)
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+24]
        fadd      DWORD PTR [eax+16]
        fadd      st(2), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+8]
        fxch      st(1)
        fsubp     st(4), st
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       st(0)
        fld       DWORD PTR [eax+20]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+12]
        fxch      st(1)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       st(2)
        fld       DWORD PTR [eax+20]
        fsub      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+24]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(5), st
        fxch      st(5)
        fstp      DWORD PTR [eax+16]
        fstp      DWORD PTR [eax+8]
        fxch      st(2)
        fstp      DWORD PTR [eax+24]
        fld       st(1)
        fadd      st, st(3)
        fstp      DWORD PTR [eax+12]
        fxch      st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+20]                            
end;

procedure u8_FPU(a : PComplexArray);
asm
        sub       esp, 8
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+8]
        fld       st(0)
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+24]
        fadd      DWORD PTR [eax+16]
        fadd      st(2), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+8]
        fxch      st(1)
        fsubp     st(4), st
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       st(0)
        fld       DWORD PTR [eax+20]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+12]
        fxch      st(1)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       st(2)
        fld       DWORD PTR [eax+20]
        fsub      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+24]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(5), st
        fxch      st(5)
        fstp      DWORD PTR [eax+16]
        fstp      DWORD PTR [eax+8]
        fxch      st(2)
        fstp      DWORD PTR [eax+24]
        fld       st(1)
        fadd      st, st(3)
        fstp      DWORD PTR [eax+12]
        fxch      st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [eax+32]
        fadd      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+32]
        fsub      DWORD PTR [eax+40]
        fst       DWORD PTR [eax+40]
        fld       DWORD PTR [eax+36]
        fadd      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+36]
        fsub      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+48]
        fadd      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+48]
        fsub      DWORD PTR [eax+56]
        fst       DWORD PTR [esp]
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+52]
        fadd      DWORD PTR [eax+60]
        fld       DWORD PTR [eax+52]
        fsub      DWORD PTR [eax+60]
        fstp      DWORD PTR [esp+4]
        fld       st(3)
        fsub      st, st(1)
        fld       st(2)
        fsub      st, st(7)
        fxch      st(3)
        faddp     st(7), st
        fxch      st(1)
        faddp     st(4), st
        fld       DWORD PTR [eax+16]
        fld       st(0)
        fsub      st, st(2)
        fstp      DWORD PTR [eax+48]
        faddp     st(1), st
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax]
        fld       st(0)
        fsub      st, st(6)
        fstp      DWORD PTR [eax+32]
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fld       st(0)
        fsub      st, st(3)
        fstp      DWORD PTR [eax+36]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+20]
        fld       st(0)
        fsub      st, st(4)
        fstp      DWORD PTR [eax+52]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+20]
        fld       st(0)
        fadd      st, st(2)
        fmul      DWORD PTR SqrtHalf
        fxch      st(1)
        fsubp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [esp]
        fld       st(0)
        fld       DWORD PTR [esp+4]
        fsub      st(1), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fxch      st(2)
        faddp     st(1), st
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)
        fxch      st(2)
        faddp     st(4), st
        fld       st(2)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(3), st
        fld       DWORD PTR [eax+28]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+60]
        fld       DWORD PTR [eax+28]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [eax+24]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [eax+8]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+8]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+12]
        faddp     st(1), st
        fstp      DWORD PTR [eax+12]
        add       esp, 8
end;

procedure u16_FPU(a : PComplexArray);
asm
        sub       esp, 8
        fld       DWORD PTR [eax]
        fadd      DWORD PTR [eax+8]
        fld       st(0)
        fld       DWORD PTR [eax+4]
        fadd      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+24]
        fadd      DWORD PTR [eax+16]
        fadd      st(2), st
        fld       DWORD PTR [eax]
        fsub      DWORD PTR [eax+8]
        fxch      st(1)
        fsubp     st(4), st
        fxch      st(2)
        fstp      DWORD PTR [eax]
        fld       st(0)
        fld       DWORD PTR [eax+20]
        fadd      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+4]
        fsub      DWORD PTR [eax+12]
        fxch      st(1)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       st(2)
        fld       DWORD PTR [eax+20]
        fsub      DWORD PTR [eax+28]
        fadd      st(1), st
        fld       DWORD PTR [eax+24]
        fsub      DWORD PTR [eax+16]
        fxch      st(1)
        fsubp     st(5), st
        fxch      st(5)
        fstp      DWORD PTR [eax+16]
        fstp      DWORD PTR [eax+8]
        fxch      st(2)
        fstp      DWORD PTR [eax+24]
        fld       st(1)
        fadd      st, st(3)
        fstp      DWORD PTR [eax+12]
        fxch      st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [eax+28]
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [eax+32]
        fadd      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+32]
        fsub      DWORD PTR [eax+40]
        fst       DWORD PTR [eax+40]
        fld       DWORD PTR [eax+36]
        fadd      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+36]
        fsub      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+48]
        fadd      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+48]
        fsub      DWORD PTR [eax+56]
        fst       DWORD PTR [esp]
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+52]
        fadd      DWORD PTR [eax+60]
        fld       DWORD PTR [eax+52]
        fsub      DWORD PTR [eax+60]
        fstp      DWORD PTR [esp+4]
        fld       st(3)
        fsub      st, st(1)
        fld       st(2)
        fsub      st, st(7)
        fxch      st(3)
        faddp     st(7), st
        fxch      st(1)
        faddp     st(4), st
        fld       DWORD PTR [eax+16]
        fld       st(0)
        fsub      st, st(2)
        fstp      DWORD PTR [eax+48]
        faddp     st(1), st
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax]
        fld       st(0)
        fsub      st, st(6)
        fstp      DWORD PTR [eax+32]
        faddp     st(5), st
        fxch      st(4)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        fld       st(0)
        fsub      st, st(3)
        fstp      DWORD PTR [eax+36]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+20]
        fld       st(0)
        fsub      st, st(4)
        fstp      DWORD PTR [eax+52]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+20]
        fld       st(0)
        fadd      st, st(2)
        fmul      DWORD PTR SqrtHalf
        fxch      st(1)
        fsubp     st(2), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [esp]
        fld       st(0)
        fld       DWORD PTR [esp+4]
        fsub      st(1), st
        fxch      st(1)
        fmul      DWORD PTR SqrtHalf
        fxch      st(2)
        faddp     st(1), st
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)
        fxch      st(2)
        faddp     st(4), st
        fld       st(2)
        fsub      st, st(1)
        fxch      st(1)
        faddp     st(3), st
        fld       DWORD PTR [eax+28]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+60]
        fld       DWORD PTR [eax+28]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [eax+24]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+24]
        faddp     st(1), st
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [eax+8]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+8]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+12]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+12]
        faddp     st(1), st
        fstp      DWORD PTR [eax+12]
        add       esp, 8
        fld       DWORD PTR [eax+64]
        fadd      DWORD PTR [eax+72]
        fld       st(0)
        fld       DWORD PTR [eax+68]
        fadd      DWORD PTR [eax+76]
        fld       DWORD PTR [eax+88]
        fadd      DWORD PTR [eax+80]
        fadd      st(2), st
        fld       DWORD PTR [eax+64]
        fsub      DWORD PTR [eax+72]
        fxch      st(1)
        fsubp     st(4), st
        fxch      st(2)
        fstp      DWORD PTR [eax+64]
        fld       st(0)
        fld       DWORD PTR [eax+84]
        fadd      DWORD PTR [eax+92]
        fadd      st(1), st
        fld       DWORD PTR [eax+68]
        fsub      DWORD PTR [eax+76]
        fxch      st(1)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [eax+68]
        fld       st(2)
        fld       DWORD PTR [eax+84]
        fsub      DWORD PTR [eax+92]
        fadd      st(1), st
        fld       DWORD PTR [eax+88]
        fsub      DWORD PTR [eax+80]
        fxch      st(1)
        fsubp     st(5), st
        fxch      st(5)
        fstp      DWORD PTR [eax+80]
        fstp      DWORD PTR [eax+72]
        fxch      st(2)
        fstp      DWORD PTR [eax+88]
        fld       st(1)
        fadd      st, st(3)
        fstp      DWORD PTR [eax+76]
        fxch      st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [eax+92]
        fstp      DWORD PTR [eax+84]
        fld       DWORD PTR [eax+96]
        fadd      DWORD PTR [eax+104]
        fld       st(0)
        fld       DWORD PTR [eax+100]
        fadd      DWORD PTR [eax+108]
        fld       DWORD PTR [eax+120]
        fadd      DWORD PTR [eax+112]
        fadd      st(2), st
        fld       DWORD PTR [eax+96]
        fsub      DWORD PTR [eax+104]
        fxch      st(1)
        fsubp     st(4), st
        fxch      st(2)
        fstp      DWORD PTR [eax+96]
        fld       st(0)
        fld       DWORD PTR [eax+116]
        fadd      DWORD PTR [eax+124]
        fadd      st(1), st
        fld       DWORD PTR [eax+100]
        fsub      DWORD PTR [eax+108]
        fxch      st(1)
        fsubp     st(3), st
        fxch      st(1)
        fstp      DWORD PTR [eax+100]
        fld       st(2)
        fld       DWORD PTR [eax+116]
        fsub      DWORD PTR [eax+124]
        fadd      st(1), st
        fld       DWORD PTR [eax+120]
        fsub      DWORD PTR [eax+112]
        fxch      st(1)
        fsubp     st(5), st
        fxch      st(5)
        fstp      DWORD PTR [eax+112]
        fstp      DWORD PTR [eax+104]
        fxch      st(2)
        fstp      DWORD PTR [eax+120]
        fld       st(1)
        fadd      st, st(3)
        fstp      DWORD PTR [eax+108]
        fxch      st(2)
        fsubp     st(1), st
        fstp      DWORD PTR [eax+124]
        fstp      DWORD PTR [eax+116]
        fld       DWORD PTR [eax+64]
        fadd      DWORD PTR [eax+96]
        fld       DWORD PTR [eax+68]
        fadd      DWORD PTR [eax+100]
        fld       DWORD PTR [eax+68]
        fsub      DWORD PTR [eax+100]
        fld       DWORD PTR [eax+96]
        fsub      DWORD PTR [eax+64]
        fld       DWORD PTR [eax]
        fsub      st, st(4)
        fstp      DWORD PTR [eax+64]
        fld       DWORD PTR [eax+4]
        fsub      st, st(3)
        fstp      DWORD PTR [eax+68]
        fld       DWORD PTR [eax+32]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+96]
        fld       DWORD PTR [eax+36]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+100]
        fld       DWORD PTR [eax]
        faddp     st(4), st
        fxch      st(3)
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+32]
        faddp     st(1), st
        fstp      DWORD PTR [eax+32]
        fld       DWORD PTR [eax+36]
        faddp     st(1), st
        fstp      DWORD PTR [eax+36]
        fld       DWORD PTR [eax+80]
        fadd      DWORD PTR [eax+84]
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+84]
        fsub      DWORD PTR [eax+80]
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+112]
        fsub      DWORD PTR [eax+116]
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+116]
        fadd      DWORD PTR [eax+112]
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)
        fld       st(3)
        fsub      st, st(2)
        fxch      st(3)
        faddp     st(5), st
        fxch      st(1)
        faddp     st(3), st
        fld       DWORD PTR [eax+52]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+116]
        fld       DWORD PTR [eax+52]
        faddp     st(1), st
        fstp      DWORD PTR [eax+52]
        fld       DWORD PTR [eax+48]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+112]
        fld       DWORD PTR [eax+48]
        faddp     st(1), st
        fstp      DWORD PTR [eax+48]
        fld       DWORD PTR [eax+16]
        fsub      st, st(2)
        fstp      DWORD PTR [eax+80]
        fld       DWORD PTR [eax+16]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+16]
        fld       DWORD PTR [eax+20]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+84]
        fld       DWORD PTR [eax+20]
        faddp     st(1), st
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [eax+72]
        fmul      DWORD PTR d16re
        fld       DWORD PTR [eax+76]
        fmul      DWORD PTR d16im
        fld       DWORD PTR [eax+104]
        fmul      DWORD PTR d16re
        fld       DWORD PTR [eax+76]
        fmul      DWORD PTR d16re
        fld       DWORD PTR [eax+72]
        fmul      DWORD PTR d16im
        fld       DWORD PTR [eax+108]
        fmul      DWORD PTR d16re
        fxch      st(4)
        faddp     st(5), st
        fld       DWORD PTR [eax+108]
        fmul      DWORD PTR nd16im
        faddp     st(3), st
        fsubp     st(1), st
        fld       DWORD PTR [eax+104]
        fmul      DWORD PTR d16im
        faddp     st(3), st
        fld       st(3)
        fadd      st, st(2)
        fxch      st(4)
        fsubp     st(2), st
        fld       st(0)
        fsub      st, st(3)
        fxch      st(3)
        faddp     st(1), st
        fld       DWORD PTR [eax+12]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+76]
        fld       DWORD PTR [eax+40]
        fsub      st, st(3)
        fstp      DWORD PTR [eax+104]
        fld       DWORD PTR [eax+8]
        fsub      st, st(4)
        fstp      DWORD PTR [eax+72]
        fld       DWORD PTR [eax+40]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+40]
        fld       DWORD PTR [eax+12]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+8]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+8]
        fld       DWORD PTR [eax+44]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+108]
        fld       DWORD PTR [eax+44]
        faddp     st(1), st
        fstp      DWORD PTR [eax+44]
        fld       DWORD PTR [eax+88]
        fmul      DWORD PTR d16im
        fld       DWORD PTR [eax+92]
        fmul      DWORD PTR d16re
        fld       DWORD PTR [eax+120]
        fmul      DWORD PTR d16im
        fld       DWORD PTR [eax+92]
        fmul      DWORD PTR d16im
        fld       DWORD PTR [eax+88]
        fmul      DWORD PTR d16re
        fld       DWORD PTR [eax+124]
        fmul      DWORD PTR d16im
        fxch      st(4)
        faddp     st(5), st
        fld       DWORD PTR [eax+124]
        fmul      DWORD PTR nd16re
        faddp     st(3), st
        fsubp     st(1), st
        fld       DWORD PTR [eax+120]
        fmul      DWORD PTR d16re
        faddp     st(3), st
        fld       st(3)
        fadd      st, st(2)
        fxch      st(4)
        fsubp     st(2), st
        fld       st(0)
        fsub      st, st(3)
        fxch      st(3)
        faddp     st(1), st
        fld       DWORD PTR [eax+28]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+92]
        fld       DWORD PTR [eax+56]
        fsub      st, st(3)
        fstp      DWORD PTR [eax+120]
        fld       DWORD PTR [eax+24]
        fsub      st, st(4)
        fstp      DWORD PTR [eax+88]
        fld       DWORD PTR [eax+56]
        faddp     st(3), st
        fxch      st(2)
        fstp      DWORD PTR [eax+56]
        fld       DWORD PTR [eax+28]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [eax+24]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [eax+60]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+124]
        fld       DWORD PTR [eax+60]
        faddp     st(1), st
        fstp      DWORD PTR [eax+60]
end;

procedure upass_FPU(a,w : PComplexArray; n : integer);
asm
        push      edi
        push      esi
        push      ebp
        push      ebx
        sub       esp, 12
        lea       edi, DWORD PTR [ecx+ecx]
        add       edi, edi
        add       edi, edi
        add       edi, edi
        add       edi, edi
        add       edi, eax
        lea       esi, DWORD PTR [ecx-1]
        mov       DWORD PTR [esp+8], edi
        add       ecx, ecx
        add       ecx, ecx
        add       ecx, ecx
        add       ecx, ecx                                      
        lea       ebx, DWORD PTR [edi+ecx]
        fld       DWORD PTR [edi]
        fadd      DWORD PTR [ebx]                               
        lea       ebp, DWORD PTR [eax+ecx]
        fld       DWORD PTR [edi+4]                             
        fadd      DWORD PTR [ebx+4]                             
        fld       DWORD PTR [edi+4]                             
        fsub      DWORD PTR [ebx+4]
        fld       DWORD PTR [ebx]                               
        fsub      DWORD PTR [edi]
        fld       DWORD PTR [eax]                               
        fsub      st, st(4)
        fstp      DWORD PTR [edi]
        fld       DWORD PTR [eax+4]                             
        fsub      st, st(3)                                     
        fstp      DWORD PTR [edi+4]
        fld       DWORD PTR [ebp]
        fsub      st, st(2)                                     
        fstp      DWORD PTR [ebx]
        fld       DWORD PTR [ebp+4]
        fsub      st, st(1)
        fstp      DWORD PTR [ebx+4]                             
        fld       DWORD PTR [eax]
        faddp     st(4), st                                     
        fxch      st(3)                                         
        fstp      DWORD PTR [eax]
        fld       DWORD PTR [eax+4]                             
        faddp     st(2), st
        fxch      st(1)                                         
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [ebp]                               
        faddp     st(1), st                                     
        fstp      DWORD PTR [ebp]
        fld       DWORD PTR [ebp+4]
        faddp     st(1), st
        fstp      DWORD PTR [ebp+4]
        fld       DWORD PTR [edi+8]                             
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [edi+12]
        fmul      DWORD PTR [edx+4]
        faddp     st(1), st
        fld       DWORD PTR [ebx+8]                             
        fmul      DWORD PTR [edx]                               
        fld       DWORD PTR [edi+12]
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [edi+8]                             
        fmul      DWORD PTR [edx+4]
        fsubp     st(1), st                                     
        fld       DWORD PTR [ebx+12]                            
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [edx+4]
        fld       DWORD PTR [ebx+12]                            
        fmul      st, st(1)                                     
        fsubp     st(4), st
        fld       st(3)                                         
        fld       DWORD PTR [ebx+8]                             
        fmulp     st(2), st                                     
        fxch      st(1)
        faddp     st(2), st
        fadd      st, st(4)                                     
        fxch      st(4)
        fsubp     st(3), st
        fld       st(1)                                         
        fsub      st, st(1)                                     
        fxch      st(2)
        faddp     st(1), st
        fld       DWORD PTR [eax+12]                            
        fsub      st, st(1)                                     
        fstp      DWORD PTR [edi+12]
        fld       DWORD PTR [ebp+8]                             
        fsub      st, st(2)
        fstp      DWORD PTR [ebx+8]
        fld       DWORD PTR [eax+8]
        fsub      st, st(4)
        fstp      DWORD PTR [edi+8]
        fld       DWORD PTR [ebp+8]                             
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [ebp+8]                             
        fld       DWORD PTR [eax+12]                            
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+12]
        fld       DWORD PTR [eax+8]                             
        faddp     st(2), st                                     
        fxch      st(1)
        fstp      DWORD PTR [eax+8]                             
        fld       DWORD PTR [ebp+12]                            
        fsub      st, st(1)
        fstp      DWORD PTR [ebx+12]
        fld       DWORD PTR [ebp+12]
        faddp     st(1), st                                     
        fstp      DWORD PTR [ebp+12]
        mov       DWORD PTR [esp+4], ecx                        
        jmp       @@B42
@@B43:
        mov       eax, edi
        mov       ebx, DWORD PTR [esp]
        mov       ecx, DWORD PTR [esp+4]
        mov       DWORD PTR [esp+8], ebx                        
        lea       ebx, DWORD PTR [ebx+ecx]                      
        lea       ebp, DWORD PTR [edi+ecx]                      
@@B42:
        mov       ecx, DWORD PTR [esp+8]
        lea       edi, DWORD PTR [ecx+16]                       
        mov       DWORD PTR [esp], edi                          
        lea       edi, DWORD PTR [eax+16]
        fld       DWORD PTR [ecx+16]                            
        fmul      DWORD PTR [edx+8]                             
        fld       DWORD PTR [ecx+20]                            
        fmul      DWORD PTR [edx+12]                            
        faddp     st(1), st
        fld       DWORD PTR [ebx+16]                            
        fmul      DWORD PTR [edx+8]
        fld       DWORD PTR [ecx+20]
        fmul      DWORD PTR [edx+8]
        fld       DWORD PTR [ecx+16]
        fmul      DWORD PTR [edx+12]
        fsubp     st(1), st                                     
        fld       DWORD PTR [ebx+20]
        fmul      DWORD PTR [edx+8]                             
        fld       DWORD PTR [edx+12]
        add       edx, 16
        dec       esi
        fld       DWORD PTR [ebx+20]                            
        fmul      st, st(1)
        fsubp     st(4), st                                     
        fld       DWORD PTR [ebx+16]                            
        fmulp     st(1), st                                     
        faddp     st(1), st                                     
        fld       st(2)
        fadd      st, st(4)                                     
        fxch      st(4)                                         
        fsubp     st(3), st                                     
        fld       st(1)
        fsub      st, st(1)
        fxch      st(2)                                         
        faddp     st(1), st
        fld       DWORD PTR [eax+20]
        fsub      st, st(1)                                     
        fstp      DWORD PTR [ecx+20]
        fld       DWORD PTR [ebp+16]                            
        fsub      st, st(2)                                     
        fstp      DWORD PTR [ebx+16]
        fld       DWORD PTR [eax+16]
        fsub      st, st(4)                                     
        fstp      DWORD PTR [ecx+16]
        fld       DWORD PTR [ebp+16]
        faddp     st(2), st                                     
        fxch      st(1)
        fstp      DWORD PTR [ebp+16]
        fld       DWORD PTR [eax+20]                            
        faddp     st(1), st
        fstp      DWORD PTR [eax+20]
        fld       DWORD PTR [eax+16]
        faddp     st(2), st                                     
        fxch      st(1)                                         
        fstp      DWORD PTR [eax+16]                            
        fld       DWORD PTR [ebp+20]                            
        fsub      st, st(1)
        fstp      DWORD PTR [ebx+20]                            
        fld       DWORD PTR [ebp+20]
        faddp     st(1), st                                     
        fstp      DWORD PTR [ebp+20]
        fld       DWORD PTR [ecx+24]                            
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [ecx+28]
        fmul      DWORD PTR [edx+4]
        faddp     st(1), st
        fld       DWORD PTR [ebx+24]                            
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [ecx+28]                            
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [ecx+24]
        fmul      DWORD PTR [edx+4]                             
        fsubp     st(1), st
        fld       DWORD PTR [ebx+28]                            
        fmul      DWORD PTR [edx]                               
        fld       DWORD PTR [edx+4]
        fld       DWORD PTR [ebx+28]                            
        fmul      st, st(1)
        fsubp     st(4), st
        fld       DWORD PTR [ebx+24]
        fmulp     st(1), st
        faddp     st(1), st                                     
        fld       st(2)
        fadd      st, st(4)                                     
        fxch      st(4)
        fsubp     st(3), st                                     
        fld       st(1)                                         
        fsub      st, st(1)                                     
        fxch      st(2)
        faddp     st(1), st
        fld       DWORD PTR [eax+28]                            
        fsub      st, st(1)
        fstp      DWORD PTR [ecx+28]                            
        fld       DWORD PTR [ebp+24]
        fsub      st, st(2)
        fstp      DWORD PTR [ebx+24]                            
        fld       DWORD PTR [eax+24]
        fsub      st, st(4)
        fstp      DWORD PTR [ecx+24]
        fld       DWORD PTR [ebp+24]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [ebp+24]
        fld       DWORD PTR [eax+28]
        faddp     st(1), st
        fstp      DWORD PTR [eax+28]
        fld       DWORD PTR [eax+24]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+24]
        fld       DWORD PTR [ebp+28]
        fsub      st, st(1)
        fstp      DWORD PTR [ebx+28]
        fld       DWORD PTR [ebp+28]
        faddp     st(1), st
        fstp      DWORD PTR [ebp+28]
        jne       @@B43
        add       esp, 12
        pop       ebx
        pop       ebp
        pop       esi
        pop       edi
end;

procedure upassbig_FPU(a,w : PComplexArray; n : integer);
asm
        push      edi
        push      esi
        push      ebp
        push      ebx
        sub       esp, 24
        lea       esi, DWORD PTR [ecx+ecx]
        add       esi, esi
        fld       DWORD PTR [eax+esi*8]
        lea       ebp, DWORD PTR [ecx+ecx]
        add       ebp, ebp
        sub       ebp, ecx
        add       ebp, ebp
        fadd      DWORD PTR [eax+ebp*8]
        fld       DWORD PTR [eax+esi*8+4]
        fadd      DWORD PTR [eax+ebp*8+4]
        fld       DWORD PTR [eax+esi*8+4]
        fsub      DWORD PTR [eax+ebp*8+4]
        fld       DWORD PTR [eax+ebp*8]
        fsub      DWORD PTR [eax+esi*8]                         
        fld       DWORD PTR [eax]                               
        fsub      st, st(4)
        fstp      DWORD PTR [eax+esi*8]
        fld       DWORD PTR [eax+4]                             
        fsub      st, st(3)                                     
        fstp      DWORD PTR [eax+esi*8+4]                       
        lea       ebx, DWORD PTR [ecx+ecx]                      
        fld       DWORD PTR [eax+ebx*8]                         
        fsub      st, st(2)                                     
        fstp      DWORD PTR [eax+ebp*8]
        fld       DWORD PTR [eax+ebx*8+4]
        fsub      st, st(1)                                     
        fstp      DWORD PTR [eax+ebp*8+4]                       
        fld       DWORD PTR [eax]
        faddp     st(4), st                                     
        fxch      st(3)
        fstp      DWORD PTR [eax]                               
        fld       DWORD PTR [eax+4]
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [eax+4]
        fld       DWORD PTR [eax+ebx*8]                         
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+ebx*8]                         
        fld       DWORD PTR [eax+ebx*8+4]                       
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+ebx*8+4]
        dec       ecx                                           
        mov       edi, ecx                                      
        mov       DWORD PTR [esp+4], eax                        
        mov       DWORD PTR [esp+8], edx                        
        mov       DWORD PTR [esp], ecx                          
        mov       ecx, eax                                      
@@B12:
        fld       DWORD PTR [ecx+esi*8+8]
        fmul      DWORD PTR [edx]                               
        fld       DWORD PTR [ecx+esi*8+12]
        fmul      DWORD PTR [edx+4]                             
        fld       DWORD PTR [ecx+ebp*8+8]                       
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [ecx+esi*8+12]                      
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [ecx+esi*8+8]
        fmul      DWORD PTR [edx+4]                             
        fld       DWORD PTR [ecx+ebp*8+12]                      
        fmul      DWORD PTR [edx]                               
        fxch      st(4)                                         
        faddp     st(5), st                                     
        fld       DWORD PTR [edx+4]                             
        fld       DWORD PTR [ecx+ebp*8+12]                      
        fmul      st, st(1)
        fsubp     st(4), st                                     
        fxch      st(1)                                         
        fsubp     st(2), st
        fld       DWORD PTR [ecx+ebp*8+8]                       
        fmulp     st(1), st
        faddp     st(3), st                                     
        fld       st(1)
        fadd      st, st(4)
        fxch      st(4)                                         
        fsubp     st(2), st                                     
        fld       st(0)                                         
        fsub      st, st(3)                                     
        fxch      st(1)
        faddp     st(3), st                                     
        lea       eax, DWORD PTR [ecx+8]                        
        fld       DWORD PTR [ecx+12]
        fsub      st, st(3)                                     
        fstp      DWORD PTR [ecx+esi*8+12]                      
        fld       DWORD PTR [ecx+ebx*8+8]
        fsub      st, st(1)                                     
        fstp      DWORD PTR [ecx+ebp*8+8]                       
        fld       DWORD PTR [ecx+8]
        fsub      st, st(4)                                     
        fstp      DWORD PTR [ecx+esi*8+8]
        fld       DWORD PTR [ecx+ebx*8+8]                       
        faddp     st(1), st                                     
        fstp      DWORD PTR [ecx+ebx*8+8]
        fld       DWORD PTR [ecx+12]                            
        faddp     st(2), st                                     
        fxch      st(1)                                         
        fstp      DWORD PTR [ecx+12]
        fld       DWORD PTR [ecx+8]                             
        faddp     st(2), st                                     
        fxch      st(1)                                         
        fstp      DWORD PTR [ecx+8]
        fld       DWORD PTR [ecx+ebx*8+12]                      
        fsub      st, st(1)                                     
        fstp      DWORD PTR [ecx+ebp*8+12]                      
        fld       DWORD PTR [ecx+ebx*8+12]                      
        faddp     st(1), st
        fstp      DWORD PTR [ecx+ebx*8+12]
        add       edx, 8                                        
        mov       ecx, eax
        dec       edi                                           
        jne       @@B12
@@B13:
        mov       ecx, DWORD PTR [esp]
        fld       DWORD PTR [eax+esi*8+8]
        fadd      DWORD PTR [eax+esi*8+12]                      
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+esi*8+12]
        fsub      DWORD PTR [eax+esi*8+8]
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+ebp*8+8]                       
        fsub      DWORD PTR [eax+ebp*8+12]
        fmul      DWORD PTR SqrtHalf
        fld       DWORD PTR [eax+ebp*8+12]
        fadd      DWORD PTR [eax+ebp*8+8]                       
        fmul      DWORD PTR SqrtHalf
        fld       st(1)
        fsub      st, st(4)                                     
        fld       st(3)
        fsub      st, st(2)                                     
        fxch      st(3)
        faddp     st(5), st
        fxch      st(1)                                         
        faddp     st(3), st                                     
        fld       DWORD PTR [eax+ebx*8+12]
        fsub      st, st(1)                                     
        fstp      DWORD PTR [eax+ebp*8+12]
        fld       DWORD PTR [eax+ebx*8+12]                      
        faddp     st(1), st
        fstp      DWORD PTR [eax+ebx*8+12]                      
        fld       DWORD PTR [eax+ebx*8+8]                       
        fsub      st, st(1)
        fstp      DWORD PTR [eax+ebp*8+8]                       
        fld       DWORD PTR [eax+ebx*8+8]                       
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+ebx*8+8]
        lea       edi, DWORD PTR [eax+8]                        
        fld       DWORD PTR [eax+8]
        fsub      st, st(2)                                     
        fstp      DWORD PTR [eax+esi*8+8]                       
        fld       DWORD PTR [eax+8]
        faddp     st(2), st
        fxch      st(1)                                         
        fstp      DWORD PTR [eax+8]                             
        fld       DWORD PTR [eax+12]
        fsub      st, st(1)
        fstp      DWORD PTR [eax+esi*8+12]                      
        fld       DWORD PTR [eax+12]                            
        faddp     st(1), st                                     
        fstp      DWORD PTR [eax+12]                            
        add       edx, -8                                       
        mov       DWORD PTR [esp+12], ecx                       
        mov       DWORD PTR [esp+16], edi
        mov       DWORD PTR [esp+20], edx                       
@@B14:
        fld       DWORD PTR [edi+esi*8+8]                       
        fmul      DWORD PTR [edx+4]
        fld       DWORD PTR [edi+esi*8+12]
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [edi+ebp*8+8]
        fmul      DWORD PTR [edx+4]                             
        fld       DWORD PTR [edi+esi*8+12]                      
        fmul      DWORD PTR [edx+4]
        fld       DWORD PTR [edi+esi*8+8]                       
        fmul      DWORD PTR [edx]
        fld       DWORD PTR [edi+ebp*8+12]                      
        fmul      DWORD PTR [edx+4]                             
        fxch      st(4)                                         
        faddp     st(5), st
        fld       DWORD PTR [edx]                               
        fld       DWORD PTR [edi+ebp*8+12]                      
        fmul      st, st(1)                                     
        fsubp     st(4), st                                     
        fxch      st(1)
        fsubp     st(2), st                                     
        fld       DWORD PTR [edi+ebp*8+8]
        fmulp     st(1), st
        faddp     st(3), st                                     
        fld       st(1)
        fadd      st, st(4)
        fxch      st(4)                                         
        fsubp     st(2), st                                     
        fld       st(0)                                         
        fsub      st, st(3)
        fxch      st(1)                                         
        faddp     st(3), st                                     
        lea       eax, DWORD PTR [edi+8]                        
        fld       DWORD PTR [edi+12]
        fsub      st, st(3)                                     
        fstp      DWORD PTR [edi+esi*8+12]                      
        fld       DWORD PTR [edi+ebx*8+8]                       
        fsub      st, st(1)                                     
        fstp      DWORD PTR [edi+ebp*8+8]
        fld       DWORD PTR [edi+8]
        fsub      st, st(4)
        fstp      DWORD PTR [edi+esi*8+8]                       
        fld       DWORD PTR [edi+ebx*8+8]                       
        faddp     st(1), st
        fstp      DWORD PTR [edi+ebx*8+8]                       
        fld       DWORD PTR [edi+12]                            
        faddp     st(2), st
        fxch      st(1)
        fstp      DWORD PTR [edi+12]
        fld       DWORD PTR [edi+8]                             
        faddp     st(2), st                                     
        fxch      st(1)                                         
        fstp      DWORD PTR [edi+8]                             
        fld       DWORD PTR [edi+ebx*8+12]
        fsub      st, st(1)                                     
        fstp      DWORD PTR [edi+ebp*8+12]
        fld       DWORD PTR [edi+ebx*8+12]                      
        faddp     st(1), st
        fstp      DWORD PTR [edi+ebx*8+12]                      
        add       edx, -8
        mov       edi, eax                                      
        dec       ecx
        jne       @@B14
@@B15:
        add       esp, 24
        pop       ebx
        pop       ebp
        pop       esi
        pop       edi
end;

procedure c32_FPU(a : PComplexArray);
begin
  cpass_FPU(a,@d32,4);
  c8_FPU(@a^[16]);
  c8_FPU(@a^[24]);
  c16_FPU(a);
end;

procedure c64_FPU(a : PComplexArray);
begin
  cpass_FPU(a,@d64,8);
  c16_FPU(@a^[32]);
  c16_FPU(@a^[48]);
  c32_FPU(a);
end;

procedure c128_FPU(a : PComplexArray);
begin
  cpass_FPU(a,@d128,16);
  c32_FPU(@a^[64]);
  c32_FPU(@a^[96]);
  c64_FPU(a);
end;

procedure c256_FPU(a : PComplexArray);
begin
  cpass_FPU(a,@d256,32);
  c64_FPU(@a^[128]);
  c64_FPU(@a^[192]);
  c128_FPU(a);
end;

procedure c512_FPU(a : PComplexArray);
begin
  cpass_FPU(a,@d512,64);
  c128_FPU(@a^[384]);
  c128_FPU(@a^[256]);
  c256_FPU(a);
end;
procedure c1024_FPU(a : PComplexArray);
begin
  cpassbig_FPU(a,@d1024,128);
  c256_FPU(@a^[768]);
  c256_FPU(@a^[512]);
  c512_FPU(a);
end;

procedure c2048_FPU(a : PComplexArray);
begin
  cpassbig_FPU(a,@d2048,256);
  c512_FPU(@a^[1536]);
  c512_FPU(@a^[1024]);
  c1024_FPU(a);
end;

procedure c4096_FPU(a : PComplexArray);
begin
  cpassbig_FPU(a,@d4096,512);
  c1024_FPU(@a^[3072]);
  c1024_FPU(@a^[2048]);
  c2048_FPU(a);
end;

procedure c8192_FPU(a : PComplexArray);
begin
  cpassbig_FPU(a,@d8192,1024);
  c2048_FPU(@a^[6144]);
  c2048_FPU(@a^[4096]);
  c4096_FPU(a);
end;

procedure u32_FPU(a : PComplexArray);
begin
  u16_FPU(a);
  u8_FPU(@a^[16]);
  u8_FPU(@a^[24]);
  upass_FPU(a,@d32,4);
end;

procedure u64_FPU(a : PComplexArray);
begin
  u32_FPU(a);
  u16_FPU(@a^[32]);
  u16_FPU(@a^[48]);
  upass_FPU(a,@d64,8);
end;

procedure u128_FPU(a : PComplexArray);
begin
  u64_FPU(a);
  u32_FPU(@a^[64]);
  u32_FPU(@a^[96]);
  upass_FPU(a,@d128,16);
end;

procedure u256_FPU(a : PComplexArray);
begin
  u128_FPU(a);
  u64_FPU(@a^[128]);
  u64_FPU(@a^[192]);
  upass_FPU(a,@d256,32);
end;

procedure u512_FPU(a : PComplexArray);
begin
  u256_FPU(a);
  u128_FPU(@a^[256]);
  u128_FPU(@a^[384]);
  upass_FPU(a,@d512,64);
end;

procedure u1024_FPU(a : PComplexArray);
begin
  u512_FPU(a);
  u256_FPU(@a^[512]);
  u256_FPU(@a^[768]);
  upassbig_FPU(a,@d1024,128);
end;

procedure u2048_FPU(a : PComplexArray);
begin
  u1024_FPU(a);
  u512_FPU(@a^[1024]);
  u512_FPU(@a^[1536]);
  upassbig_FPU(a,@d2048,256);
end;

procedure u4096_FPU(a : PComplexArray);
begin
  u2048_FPU(a);
  u1024_FPU(@a^[2048]);
  u1024_FPU(@a^[3072]);
  upassbig_FPU(a,@d4096,512);
end;

procedure u8192_FPU(a : PComplexArray);
begin
  u4096_FPU(a);
  u2048_FPU(@a^[4096]);
  u2048_FPU(@a^[6144]);
  upassbig_FPU(a,@d8192,1024);
end;

procedure fft2__FPU(a : PComplexArray);
begin
  c2_FPU(a);
end;

procedure ifft2__FPU(a : PComplexArray);
begin
  u2_FPU(a);
end;

procedure fft4__FPU(a : PComplexArray);
begin
  c4_FPU(a);
end;

procedure ifft4__FPU(a : PComplexArray);
begin
  u4_FPU(a);
end;

procedure fft8__FPU(a : PComplexArray);
begin
  c8_FPU(a);
end;

procedure ifft8__FPU(a : PComplexArray);
begin
  u8_FPU(a);
end;

procedure fft16__FPU(a : PComplexArray);
begin
  c16_FPU(a);
end;

procedure ifft16__FPU(a : PComplexArray);
begin
  u16_FPU(a);
end;

procedure fft32__FPU(a : PComplexArray);
begin
  c32_FPU(a);
end;

procedure ifft32__FPU(a : PComplexArray);
begin
  u32_FPU(a);
end;

procedure fft64__FPU(a : PComplexArray);
begin
  c64_FPU(a);
end;

procedure ifft64__FPU(a : PComplexArray);
begin
  u64_FPU(a);
end;

procedure fft128__FPU(a : PComplexArray);
begin
  c128_FPU(a);
end;

procedure ifft128__FPU(a : PComplexArray);
begin
  u128_FPU(a);
end;

procedure fft256__FPU(a : PComplexArray);
begin
  c256_FPU(a);
end;

procedure ifft256__FPU(a : PComplexArray);
begin
  u256_FPU(a);
end;

procedure fft512__FPU(a : PComplexArray);
begin
  c512_FPU(a);
end;

procedure ifft512__FPU(a : PComplexArray);
begin
  u512_FPU(a);
end;

procedure fft1024__FPU(a : PComplexArray);
begin
  c1024_FPU(a);
end;

procedure ifft1024__FPU(a : PComplexArray);
begin
  u1024_FPU(a);
end;

procedure fft2048__FPU(a : PComplexArray);
begin
  c2048_FPU(a);
end;

procedure ifft2048__FPU(a : PComplexArray);
begin
  u2048_FPU(a);
end;

procedure fft4096__FPU(a : PComplexArray);
begin
  c4096_FPU(a);
end;

procedure ifft4096__FPU(a : PComplexArray);
begin
  u4096_FPU(a);
end;

procedure fft8192__FPU(a : PComplexArray);
begin
  c8192_FPU(a);
end;

procedure ifft8192__FPU(a : PComplexArray);
begin
  u8192_FPU(a);
end;


procedure scalec(a : PComplexArray; n : integer; u : Single);
asm
        push      ebx
        shl       edx, 3
        xor       ebx, ebx
        cmp       EnableSSE, 0
        je        @@3DNOW
@@CSSE:
        cmp       edx, 16
        jl        @@3DNOW
        movss     xmm7, u
        shufps    xmm7, xmm7, $0
@@Loop2:
        movaps    xmm0, [eax+ebx]
        mulps     xmm0, xmm7
        movaps    [eax+ebx], xmm0
        add       ebx, 16
        cmp       ebx, edx
        jl        @@Loop2
        jmp       @@End

@@3DNOW:
        {
        cmp       has3DNow, 0
        je        @@CFPU
        femms
        movd      mm7, u
        punpckldq mm7, mm7
@@Loop1:
        movq      mm0, [eax+ebx]
        pfmul     mm0, mm7
        movq      [eax+ebx], mm0
        add       ebx, 8
        cmp       ebx, edx
        jl        @@Loop1
        femms
        jmp       @@End   }

@@CFPU:
        fld       [eax+ebx]
        fmul      u
        fstp      [eax+ebx]
        add       ebx, 4
        cmp       ebx, edx
        jl        @@CFPU
@@End:
        pop       ebx
end;




procedure ReOrderFFT(a : PComplexArray; Size : integer; RevBin : PWORDArray; Inverse : Boolean; SwapBuffer : Pointer);
var
  tmp : PComplexArray;
  i : integer;
  FreeBuffer : Boolean;
begin
  if Assigned(SwapBuffer) then
  begin
    FreeBuffer := False;
    tmp := SwapBuffer;
  end else
  begin
    FreeBuffer := True;
    tmp := AllocMem(Size shl 3);
  end;
  Move(a^,tmp^,Size shl 3);
  if not Inverse then for i := 0 to Size -1 do a[i] := tmp[RevBin[i]]
                 else for i := 0 to Size -1 do a[RevBin[i]] := tmp[i];
  if FreeBuffer then FreeMem(tmp);
end;

procedure dspDoFFT(Cplx : PComplexArray; FFTSize : integer; Inverse,Scale,ReOrder : Boolean; SwapBuffer : Pointer = nil);
begin
  if not Inverse then
  begin
    case FFTSize of
      2:
      begin
        fft2(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins2,Inverse,SwapBuffer);
      end;
      4:
      begin
        fft4(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins4,Inverse,SwapBuffer);
      end;
      8:
      begin
        fft8(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins8,Inverse,SwapBuffer);
      end;
      16:
      begin
        fft16(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins16,Inverse,SwapBuffer);
      end;
      32:
      begin
        fft32(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins32,Inverse,SwapBuffer);
      end;
      64:
      begin
        fft64(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins64,Inverse,SwapBuffer);
      end;
      128:
      begin
        fft128(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins128,Inverse,SwapBuffer);
      end;
      256:
      begin
        fft256(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins256,Inverse,SwapBuffer);
      end;
      512:
      begin
        fft512(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins512,Inverse,SwapBuffer);
      end;
      1024:
      begin
        fft1024(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins1024,Inverse,SwapBuffer);
      end;
      2048:
      begin
        fft2048(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins2048,Inverse,SwapBuffer);
      end;
      4096:
      begin
        fft4096(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins4096,Inverse,SwapBuffer);
      end;
      8192:
      begin
        fft8192(Cplx);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins8192,Inverse,SwapBuffer);
      end;
    end;
  end else
  begin
    case FFTSize of
      2:
      begin
        if Scale   then scalec(Cplx,2,ScaleFFT2);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins2,Inverse,SwapBuffer);
        ifft2(Cplx);
      end;
      4:
      begin
        if Scale   then scalec(Cplx,4,ScaleFFT4);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins4,Inverse,SwapBuffer);
        ifft4(Cplx);
      end;
      8:
      begin
        if Scale   then scalec(Cplx,8,ScaleFFT8);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins8,Inverse,SwapBuffer);
        ifft8(Cplx);
      end;
      16:
      begin
        if Scale   then scalec(Cplx,16,ScaleFFT16);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins16,Inverse,SwapBuffer);
        ifft16(Cplx);
      end;
      32:
      begin
        if Scale   then scalec(Cplx,32,ScaleFFT32);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins32,Inverse,SwapBuffer);
        ifft32(Cplx);
      end;
      64:
      begin
        if Scale   then scalec(Cplx,64,ScaleFFT64);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins64,Inverse,SwapBuffer);
        ifft64(Cplx);
      end;
      128:
      begin
        if Scale   then scalec(Cplx,128,ScaleFFT128);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins128,Inverse,SwapBuffer);
        ifft128(Cplx);
      end;
      256:
      begin
        if Scale   then scalec(Cplx,256,ScaleFFT256);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins256,Inverse,SwapBuffer);
        ifft256(Cplx);
      end;
      512:
      begin
        if Scale   then scalec(Cplx,512,ScaleFFT512);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins512,Inverse,SwapBuffer);
        ifft512(Cplx);
      end;
      1024:
      begin
        if Scale   then scalec(Cplx,1024,ScaleFFT1024);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins1024,Inverse,SwapBuffer);
        ifft1024(Cplx);
      end;
      2048:
      begin
        if Scale   then scalec(Cplx,2048,ScaleFFT2048);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins2048,Inverse,SwapBuffer);
        ifft2048(Cplx);
      end;
      4096:
      begin
        if Scale   then scalec(Cplx,4096,ScaleFFT4096);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins4096,Inverse,SwapBuffer);
        ifft4096(Cplx);
      end;
      8192:
      begin
        if Scale   then scalec(Cplx,8192,ScaleFFT8192);
        if ReOrder then ReOrderFFT(Cplx,FFTSize,@RevBins8192,Inverse,SwapBuffer);
        ifft8192(Cplx);
      end;
    end;
  end;
end;



procedure InitializeDCFFT;
begin
  InitRevBitTable;
  InitSinCosTable;
    @fft2  := @fft2__FPU;
    @ifft2 := @ifft2__FPU;
    @fft4  := @fft4__FPU;
    @ifft4 := @ifft4__FPU;
    @fft8 := @fft8__FPU;
    @ifft8 := @ifft8__FPU;
    @fft16 := @fft16__FPU;
    @ifft16 := @ifft16__FPU;
    @fft32 := @fft32__FPU;
    @ifft32 := @ifft32__FPU;
    @fft64 := @fft64__FPU;
    @ifft64 := @ifft64__FPU;
    @fft128 := @fft128__FPU;
    @ifft128 := @ifft128__FPU;
    @fft256 := @fft256__FPU;
    @ifft256 := @ifft256__FPU;
    @fft512 := @fft512__FPU;
    @ifft512 := @ifft512__FPU;
    @fft1024 := @fft1024__FPU;
    @ifft1024 := @ifft1024__FPU;
    @fft2048 := @fft2048__FPU;
    @ifft2048 := @ifft2048__FPU;
    @fft4096 := @fft4096__FPU;
    @ifft4096 := @ifft4096__FPU;
    @fft8192 := @fft8192__FPU;
    @ifft8192 := @ifft8192__FPU;
end;

procedure CheckMMMXSSE; {Called Once by Unit Initialisation}
asm
  push    ebx
  pushfd
  pop     eax
  mov     edx, eax
  xor     edx, $200000
  push    eax
  popfd
  pushfd
  pop     eax
  cmp     eax, edx
  jz      @Exit {No CPUID Support}
  mov     eax, 0 {Check for Get Fetures Support}
  cpuid {Get highest supported cpuid fuction into eax}
  jz      @Exit {No support for getting CPU features}
  mov     eax, 1
  cpuid {Get feature bits into edx}
  test    edx, (1 shl 23) {Test for MMX Support}
  setnz   EnableMMX
  test    edx, (1 shl 25) {Test for SSE Support}
  setnz   EnableSSE
@Exit:
  pop     ebx
end; {CheckMMMXSSE}



initialization
  //has3DNow:= False;
  CheckMMMXSSE;
  EnableSSE:= False;
  InitializeDCFFT;


end.

