unit PSKCodec;

interface

uses Windows, SysUtils, Forms, classes, PHDConst,
     FIRFilter, Generators, Hilbert, Filtres, math, Dialogs, DateUtils;

var HexSynString   : String;
    HexDataString  : String;
    dF             : Double;
    SynFound       :  Boolean;
    EndFound       :  Boolean;
    MaxDataCounter : Int64;
    BPFIn0         : TFIRFilter;
    BPFIn1         : TFIRFilter;
    GenSinCos      : TGenSinCosUpr;
    fHilbert       : THilbert;
    LPF1_1         : TLPFpor1;
    LPF1, LPF2     : TIirLPF;

function  StrTran(sInput, SourceWord, TargetWord: String; Once: Boolean): String;
function  HexToInt64(const Value: String): Int64;
function  DecodeHexSMS(var BinStr: String): String;
function  String2Hex(SrcStr: String): String;
function  EncodeData(Data: String; Speed: Byte): ByteArray;
procedure DecodeVector(Data: Double; Speed: Byte);

implementation

uses TXMain, Graphics, Settings;

var  OldBt       : Boolean;

function StrTran(sInput, SourceWord, TargetWord: String; Once: Boolean): String;
var i,j   : smallint;
    Exist : boolean;
begin
     i:=1;
     Result:=sInput;
     repeat
           j:=Pos(SourceWord, Copy(Result,i,length(Result)));
           if j>0 then
           begin
                Exist:=true;
                j:=j+i-1;
                if j>0 then
                begin
                     Result:=copy(Result,1,j-1)+ TargetWord+copy(Result,j+Length(SourceWord),Length(Result));
                     if Once then Exist:=false;
                end;
           end
           else
           Exist:=false;
     until (not Exist);
end;

procedure MakeHexString(Bt: Boolean; Speed: Byte);
var C  : Char;
begin
     C:='n';
     Inc(PSKCounter);
     if Speed = 0 then if (PSKCounter > 900) then Inc(BadData);
     if Speed = 1 then if (PSKCounter > 200) then Inc(BadData);

     if bt <> OldBt then
     begin
          if Speed = 0 then
          begin
               if (PSKCounter < 56) then Inc(BadData);
               if (PSKCounter>= 56) and (PSKCounter<104) then C:= '0';
               if (PSKCounter>=104) and (PSKCounter<152) then C:= '1';
               if (PSKCounter>=152) and (PSKCounter<200) then C:= '2';
               if (PSKCounter>=200) and (PSKCounter<248) then C:= '3';
               if (PSKCounter>=248) and (PSKCounter<296) then C:= '4';
               if (PSKCounter>=296) and (PSKCounter<344) then C:= '5';
               if (PSKCounter>=344) and (PSKCounter<392) then C:= '6';
               if (PSKCounter>=392) and (PSKCounter<440) then C:= '7';
               if (PSKCounter>=440) and (PSKCounter<488) then C:= '8';
               if (PSKCounter>=488) and (PSKCounter<536) then C:= '9';
               if (PSKCounter>=536) and (PSKCounter<584) then C:= 'A';
               if (PSKCounter>=584) and (PSKCounter<632) then C:= 'B';
               if (PSKCounter>=632) and (PSKCounter<680) then C:= 'C';
               if (PSKCounter>=680) and (PSKCounter<728) then C:= 'D';
               if (PSKCounter>=728) and (PSKCounter<776) then C:= 'E';
               if (PSKCounter>=776) and (PSKCounter<824) then C:= 'F';
               if (PSKCounter>=824) and (PSKCounter<872) then C:= '#';
               if PSKCounter >= 56  then PSKCounter:= 0;
          end;
          if Speed = 1 then
          begin
               if (PSKCounter < 15) then Inc(BadData);
               if (PSKCounter>=15)  and (PSKCounter<25)  then C:= '0';
               if (PSKCounter>=25)  and (PSKCounter<35)  then C:= '1';
               if (PSKCounter>=35)  and (PSKCounter<45)  then C:= '2';
               if (PSKCounter>=45)  and (PSKCounter<55)  then C:= '3';
               if (PSKCounter>=55)  and (PSKCounter<65)  then C:= '4';
               if (PSKCounter>=65)  and (PSKCounter<75)  then C:= '5';
               if (PSKCounter>=75)  and (PSKCounter<85)  then C:= '6';
               if (PSKCounter>=85)  and (PSKCounter<95)  then C:= '7';
               if (PSKCounter>=95)  and (PSKCounter<105) then C:= '8';
               if (PSKCounter>=105) and (PSKCounter<115) then C:= '9';
               if (PSKCounter>=115) and (PSKCounter<125) then C:= 'A';
               if (PSKCounter>=125) and (PSKCounter<135) then C:= 'B';
               if (PSKCounter>=135) and (PSKCounter<145) then C:= 'C';
               if (PSKCounter>=145) and (PSKCounter<155) then C:= 'D';
               if (PSKCounter>=155) and (PSKCounter<165) then C:= 'E';
               if (PSKCounter>=165) and (PSKCounter<175) then C:= 'F';
               if (PSKCounter>=175) and (PSKCounter<185) then C:= '#';
               if PSKCounter >= 15  then PSKCounter:= 0;
          end;

          if C <> 'n' then
          begin
               HexSynString:= HexSynString + C;
               if SynFound and not EndFound then
               HexDataString:= HexDataString + C;
          end;
     end;
     OldBt:= bt;
     Inc(MaxDataCounter);
end;
 
procedure DecodeVector(Data: Double; Speed: Byte);
var
    Xc, Xs       : Double;
    Sin, Cos     : Double;
    bt           : Boolean;
    x, y, F, F1  : Double;
    Re, Im       : Double;
    r,g,b        : Byte;
    i,j          : Longint;
    vs           : Double;
begin
     if Speed = 0 then Data := BPFIn0.Exec(Data);
     if Speed = 1 then Data := BPFIn1.Exec(Data);

     fHilbert.Exec(Data, Xc, Xs);

     x := sqrt(sqr(Xs)) + sqrt(sqr(Xc));
     x := LPF1_1.Exec(x);
     if x < 0.01 then x:= 0.01;
     y := 100 / x;
     Xc := Xc*y;
     Xs := Xs*y;

     if Speed = 0 then GenSinCos.Exec(dF / 300, 100, Cos, Sin);
     if Speed = 1 then GenSinCos.Exec(dF / 600, 100, Cos, Sin);

     if Speed = 255 then GenSinCos.Exec(dF / 2500, 100, Cos, Sin);
     if Speed = 64 then GenSinCos.Exec(dF / 2500, 100, Cos, Sin);

     Re := Xc*Cos - Xs*-Sin;
     Im := Xc*-Sin + Xs*Cos;

     Re := LPF1.Exec(Re);
     Im := LPF2.Exec(Im);
     X := sqrt(sqr(Re + 100) + sqr(Im));
     Y := sqrt(sqr(Re - 100) + sqr(Im));

     if X > Y then
     begin
          bt:= True;
          F := 0;
     end else
     begin
          bt:= False;
          F := pi;
     end;

     F1 := ArcTan2(Im, Re);
     if F1 < 0 then F1 := F1 + 2*pi;
     dF := F1 - F;
     if dF > pi then dF := dF - 2*pi;

     if (SynFound) or (CanDrawVector) then
     begin
          vs:= (X + Y) + 8000;
          DoubleToRGB(  vs, r,g,b   );
          i:= round(Re/540)+ vBitmap.Width div 2;
          j:= vBitmap.Height div 2 - round(Im/540);
          vBitmap.Canvas.Pixels[i,j]:=RGB(r,g,b);
     end;

     MakeHexString(Bt, Speed);
end;


function HexToInt64(const Value: String): Int64;
var i: Longint;
begin
     Result := 0;
     i := 1;
     if Value = '' then Exit;
     while i <= Length(Value) do
     begin
          if Value[i] in ['0'..'9'] then Result := (Result shl 4) or (Ord(Value[i]) - Ord('0'))
          else if Value[i] in ['A'..'F'] then Result := (Result shl 4) or (Ord(Value[i]) - Ord('A') + 10)
          else
          break;
          Inc(i);
     end;
end;

function String2Hex(SrcStr: String): String;
var
   i    : Longint;
   sd   : String;
   B    : Byte;
begin
     if Length(SrcStr)<1 then
     begin
          Result:='';
          EXIT;
     end;
     sd:= '';
     for i:= 1 to length(SrcStr) do
     begin
          B:= Byte(SrcStr[i]);
          sd:= sd  + IntToHex(B,2);
     end;
     Result:= sd;
end;

function DecodeHexSMS(var BinStr: String): String;
var i,j,k: Longint;
    sd, ts: String;
begin
     j:=0; sd:= ''; ts:= '';
     for i:= 1 to length(BinStr) div 2 do
     begin
          sd:= Copy(BinStr, j+1, 2);
          k:= HexToInt64(sd);
          ts:= ts + Char(k);
          Inc(j,2);
     end;
     Result:= ts;
end;

function EncodeData(Data: String; Speed: Byte): ByteArray;
var i,j          : Longint;
    TXStr        : String;
    tmps         : String;
    sd           : String;
    pb           : Boolean;
    MS           : TMemoryStream;
    Buffer       : ByteArray;
    Size         : Integer;
begin
     tmps:= '';
     tmps:= String2Hex(Data);

     TXStr:= '012345012345012345012345012345012345012345012345012345012345012345012345B0F#' + tmps+ '#E0F##########';

     if SettingsForm.cbVox.Checked then
     begin
          if Speed = 0 then  //1500
          TXStr:= 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' + TXStr;
          if Speed = 1 then  //2400
          TXStr:= 'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF'+
          'FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF' + TXStr;
     end;

     MS:= TMemoryStream.Create;

     pb:= False;
     sd:= '';

     for i:= 1 to Length(TXStr) do
     begin
          if MS.Size <= MaxDataSize then
          begin
               sd:= Copy(TXStr, i, 1);
               if Speed = 0 then
               begin
                    if sd = '0' then  for j:= 1 to 80  do MS.Write(pb, 1);
                    if sd = '1' then  for j:= 1 to 128 do MS.Write(pb, 1);
                    if sd = '2' then  for j:= 1 to 176 do MS.Write(pb, 1);
                    if sd = '3' then  for j:= 1 to 224 do MS.Write(pb, 1);
                    if sd = '4' then  for j:= 1 to 272 do MS.Write(pb, 1);
                    if sd = '5' then  for j:= 1 to 320 do MS.Write(pb, 1);
                    if sd = '6' then  for j:= 1 to 368 do MS.Write(pb, 1);
                    if sd = '7' then  for j:= 1 to 416 do MS.Write(pb, 1);
                    if sd = '8' then  for j:= 1 to 464 do MS.Write(pb, 1);
                    if sd = '9' then  for j:= 1 to 512 do MS.Write(pb, 1);
                    if sd = 'A' then  for j:= 1 to 560 do MS.Write(pb, 1);
                    if sd = 'B' then  for j:= 1 to 608 do MS.Write(pb, 1);
                    if sd = 'C' then  for j:= 1 to 656 do MS.Write(pb, 1);
                    if sd = 'D' then  for j:= 1 to 704 do MS.Write(pb, 1);
                    if sd = 'E' then  for j:= 1 to 752 do MS.Write(pb, 1);
                    if sd = 'F' then  for j:= 1 to 800 do MS.Write(pb, 1);
                    if sd = '#' then  for j:= 1 to 848 do MS.Write(pb, 1);
               end;
               if Speed = 1 then
               begin
                    if sd = '0' then  for j:= 1 to 20  do MS.Write(pb, 1);
                    if sd = '1' then  for j:= 1 to 30  do MS.Write(pb, 1);
                    if sd = '2' then  for j:= 1 to 40  do MS.Write(pb, 1);
                    if sd = '3' then  for j:= 1 to 50  do MS.Write(pb, 1);
                    if sd = '4' then  for j:= 1 to 60  do MS.Write(pb, 1);
                    if sd = '5' then  for j:= 1 to 70  do MS.Write(pb, 1);
                    if sd = '6' then  for j:= 1 to 80  do MS.Write(pb, 1);
                    if sd = '7' then  for j:= 1 to 90  do MS.Write(pb, 1);
                    if sd = '8' then  for j:= 1 to 100 do MS.Write(pb, 1);
                    if sd = '9' then  for j:= 1 to 110 do MS.Write(pb, 1);
                    if sd = 'A' then  for j:= 1 to 120 do MS.Write(pb, 1);
                    if sd = 'B' then  for j:= 1 to 130 do MS.Write(pb, 1);
                    if sd = 'C' then  for j:= 1 to 140 do MS.Write(pb, 1);
                    if sd = 'D' then  for j:= 1 to 150 do MS.Write(pb, 1);
                    if sd = 'E' then  for j:= 1 to 160 do MS.Write(pb, 1);
                    if sd = 'F' then  for j:= 1 to 170 do MS.Write(pb, 1);
                    if sd = '#' then  for j:= 1 to 180 do MS.Write(pb, 1);
               end;
               pb:= not pb;
               Application.ProcessMessages;
          end;
     end;

     MS.Seek(0,soFromBeginning);

     Size := MS.Size - MS.Position;
     SetLength(Buffer, Size);
     MS.Read(Buffer[0], Size);

     Result:=  Buffer;
     MS.Free;
end;

initialization

begin
     HexSynString:= '';
     PSKCounter:= 0;
     MaxDataCounter:= 0;
end;


end.

