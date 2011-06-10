unit resample;

interface

uses
  Windows, SysUtils,  Classes, Graphics,   math;

type
  TRGBArray = ARRAY[0..32767] OF TRGBTriple;
  pRGBArray = ^TRGBArray;
  TFilterProc = function(Value: Single): Single;

  function SplineFilter(Value: Single): Single;
  function BellFilter(Value: Single): Single;
  function TriangleFilter(Value: Single): Single;
  function BoxFilter(Value: Single): Single;
  function HermiteFilter(Value: Single): Single;
  function Lanczos3Filter(Value: Single): Single;
  function MitchellFilter(Value: Single): Single;
  procedure Strecth(Src, Dst: TBitmap; filter: TFilterProc; fwidth: single);
  procedure ConvolveFilter(filternr,edgenr:integer;src:TBitmap);

const
  ResampleFilters: array[0..6] of record
    Name: string;
    Filter: TFilterProc;
    Width: Single;
  end = (
    (Name: 'Box';	Filter: BoxFilter;	Width: 0.5),
    (Name: 'Hermite';	Filter: HermiteFilter;	Width: 1.0),
    (Name: 'Triangle';	Filter: TriangleFilter;	Width: 1.0),
    (Name: 'B-Spline';	Filter: SplineFilter;	Width: 2.0),
    (Name: 'Bell';	Filter: BellFilter;	Width: 1.5),
    (Name: 'Mitchell';	Filter: MitchellFilter;	Width: 2.0),
    (Name: 'Lanczos3';	Filter: Lanczos3Filter;	Width: 3.0)
    );

implementation

function BoxFilter(Value: Single): Single;
begin
  if (Value > -0.5) and (Value <= 0.5) then
    Result := 1.0
  else
    Result := 0.0;
end;

function TriangleFilter(Value: Single): Single;
begin
  if (Value < 0.0) then
    Value := -Value;
  if (Value < 1.0) then
    Result := 1.0 - Value
  else
    Result := 0.0;
end;

function HermiteFilter(Value: Single): Single;
begin
  if (Value < 0.0) then
    Value := -Value;
  if (Value < 1.0) then
    Result := (2.0 * Value - 3.0) * Sqr(Value) + 1.0
  else
    Result := 0.0;
end;

function BellFilter(Value: Single): Single;
begin
  if (Value < 0.0) then
    Value := -Value;
  if (Value < 0.5) then
    Result := 0.75 - Sqr(Value)
  else if (Value < 1.5) then
  begin
    Value := Value - 1.5;
    Result := 0.5 * Sqr(Value);
  end else
    Result := 0.0;
end;

function SplineFilter(Value: Single): Single;
var
  tt			: single;
begin
  if (Value < 0.0) then
    Value := -Value;
  if (Value < 1.0) then
  begin
    tt := Sqr(Value);
    Result := 0.5*tt*Value - tt + 2.0 / 3.0;
  end else if (Value < 2.0) then
  begin
    Value := 2.0 - Value;
    Result := 1.0/6.0 * Sqr(Value) * Value;
  end else
    Result := 0.0;
end;

function Lanczos3Filter(Value: Single): Single;
  function SinC(Value: Single): Single;
  begin
    if (Value <> 0.0) then
    begin
      Value := Value * Pi;
      Result := sin(Value) / Value
    end else
      Result := 1.0;
  end;
begin
  if (Value < 0.0) then
    Value := -Value;
  if (Value < 3.0) then
    Result := SinC(Value) * SinC(Value / 3.0)
  else
    Result := 0.0;
end;

function MitchellFilter(Value: Single): Single;
const
  B		= (1.0 / 3.0);
  C		= (1.0 / 3.0);
var
  tt			: single;
begin
  if (Value < 0.0) then
    Value := -Value;
  tt := Sqr(Value);
  if (Value < 1.0) then
  begin
    Value := (((12.0 - 9.0 * B - 6.0 * C) * (Value * tt))
      + ((-18.0 + 12.0 * B + 6.0 * C) * tt)
      + (6.0 - 2 * B));
    Result := Value / 6.0;
  end else
  if (Value < 2.0) then
  begin
    Value := (((-1.0 * B - 6.0 * C) * (Value * tt))
      + ((6.0 * B + 30.0 * C) * tt)
      + ((-12.0 * B - 48.0 * C) * Value)
      + (8.0 * B + 24 * C));
    Result := Value / 6.0;
  end else
    Result := 0.0;
end;


type
  TContributor = record
    pixel: integer;
    weight: single;
  end;

  TContributorList = array[0..0] of TContributor;
  PContributorList = ^TContributorList;

  TCList = record
    n		: integer;
    p		: PContributorList;
  end;

  TCListList = array[0..0] of TCList;
  PCListList = ^TCListList;

  TRGB = packed record
    r, g, b	: single;
  end;

  TColorRGB = packed record
    r, g, b	: BYTE;
  end;
  PColorRGB = ^TColorRGB;

  TRGBList = packed array[0..0] of TColorRGB;
  PRGBList = ^TRGBList;

procedure Strecth(Src, Dst: TBitmap; filter: TFilterProc; fwidth: single);
var
  xscale, yscale	: single;
  i, j, k		: integer;
  center		: single;
  width, fscale, weight	: single;
  left, right		: integer;
  n			: integer;
  Work			: TBitmap;
  contrib		: PCListList;
  rgb			: TRGB;
  color			: TColorRGB;

  SourceLine		,
  DestLine		: PRGBList;
  SourcePixel		,
  DestPixel		: PColorRGB;
  Delta			,
  DestDelta		: integer;

  SrcWidth		,
  SrcHeight		,
  DstWidth		,
  DstHeight		: integer;

  function Color2RGB(Color: TColor): TColorRGB;
  begin
    Result.r := Color AND $000000FF;
    Result.g := (Color AND $0000FF00) SHR 8;
    Result.b := (Color AND $00FF0000) SHR 16;
  end;

  function RGB2Color(Color: TColorRGB): TColor;
  begin
    Result := Color.r OR (Color.g SHL 8) OR (Color.b SHL 16);
  end;

begin
  DstWidth := Dst.Width;
  DstHeight := Dst.Height;
  SrcWidth := Src.Width;
  SrcHeight := Src.Height;
  if (SrcWidth < 1) or (SrcHeight < 1) then
    raise Exception.Create('Source bitmap too small');

  Work := TBitmap.Create;
  try
    Work.Height := SrcHeight;
    Work.Width := DstWidth;
    if (SrcWidth = 1) then
      xscale:= DstWidth / SrcWidth
    else
      xscale:= (DstWidth - 1) / (SrcWidth - 1);
    if (SrcHeight = 1) then
      yscale:= DstHeight / SrcHeight
    else
      yscale:= (DstHeight - 1) / (SrcHeight - 1);

    Src.PixelFormat := pf24bit;
    Dst.PixelFormat := Src.PixelFormat;
    Work.PixelFormat := Src.PixelFormat;

    GetMem(contrib, DstWidth* sizeof(TCList));
    if (xscale < 1.0) then
    begin
      width := fwidth / xscale;
      fscale := 1.0 / xscale;
      for i := 0 to DstWidth-1 do
      begin
        contrib^[i].n := 0;
        GetMem(contrib^[i].p, trunc(width * 2.0 + 1) * sizeof(TContributor));
        center := i / xscale;
        left := floor(center - width);
        right := ceil(center + width);
        for j := left to right do
        begin
          weight := filter((center - j) / fscale) / fscale;
          if (weight = 0.0) then
            continue;
          if (j < 0) then
            n := -j
          else if (j >= SrcWidth) then
            n := SrcWidth - j + SrcWidth - 1
          else
            n := j;
          k := contrib^[i].n;
          contrib^[i].n := contrib^[i].n + 1;
          contrib^[i].p^[k].pixel := n;
          contrib^[i].p^[k].weight := weight;
        end;
      end;
    end else
    begin
      for i := 0 to DstWidth-1 do
      begin
        contrib^[i].n := 0;
        GetMem(contrib^[i].p, trunc(fwidth * 2.0 + 1) * sizeof(TContributor));
        center := i / xscale;
        left := floor(center - fwidth);
        right := ceil(center + fwidth);
        for j := left to right do
        begin
          weight := filter(center - j);
          if (weight = 0.0) then
            continue;
          if (j < 0) then
            n := -j
          else if (j >= SrcWidth) then
            n := SrcWidth - j + SrcWidth - 1
          else
            n := j;
          k := contrib^[i].n;
          contrib^[i].n := contrib^[i].n + 1;
          contrib^[i].p^[k].pixel := n;
          contrib^[i].p^[k].weight := weight;
        end;
      end;
    end;
    for k := 0 to SrcHeight-1 do
    begin
      SourceLine := Src.ScanLine[k];
      DestPixel := Work.ScanLine[k];
      for i := 0 to DstWidth-1 do
      begin
        rgb.r := 0.0;
        rgb.g := 0.0;
        rgb.b := 0.0;
        for j := 0 to contrib^[i].n-1 do
        begin
          color := SourceLine^[contrib^[i].p^[j].pixel];
          weight := contrib^[i].p^[j].weight;
          if (weight = 0.0) then
            continue;
          rgb.r := rgb.r + color.r * weight;
          rgb.g := rgb.g + color.g * weight;
          rgb.b := rgb.b + color.b * weight;
        end;
        if (rgb.r > 255.0) then
          color.r := 255
        else if (rgb.r < 0.0) then
          color.r := 0
        else
          color.r := round(rgb.r);
        if (rgb.g > 255.0) then
          color.g := 255
        else if (rgb.g < 0.0) then
          color.g := 0
        else
          color.g := round(rgb.g);
        if (rgb.b > 255.0) then
          color.b := 255
        else if (rgb.b < 0.0) then
          color.b := 0
        else
          color.b := round(rgb.b);
        DestPixel^ := color;
        inc(DestPixel);

      end;
    end;

    for i := 0 to DstWidth-1 do
      FreeMem(contrib^[i].p);
    FreeMem(contrib);
    GetMem(contrib, DstHeight* sizeof(TCList));
    if (yscale < 1.0) then
    begin
      width := fwidth / yscale;
      fscale := 1.0 / yscale;
      for i := 0 to DstHeight-1 do
      begin
        contrib^[i].n := 0;
        GetMem(contrib^[i].p, trunc(width * 2.0 + 1) * sizeof(TContributor));
        center := i / yscale;
        left := floor(center - width);
        right := ceil(center + width);
        for j := left to right do
        begin
          weight := filter((center - j) / fscale) / fscale;
          if (weight = 0.0) then
            continue;
          if (j < 0) then
            n := -j
          else if (j >= SrcHeight) then
            n := SrcHeight - j + SrcHeight - 1
          else
            n := j;
          k := contrib^[i].n;
          contrib^[i].n := contrib^[i].n + 1;
          contrib^[i].p^[k].pixel := n;
          contrib^[i].p^[k].weight := weight;
        end;
      end
    end else
    begin
      for i := 0 to DstHeight-1 do
      begin
        contrib^[i].n := 0;
        GetMem(contrib^[i].p, trunc(fwidth * 2.0 + 1) * sizeof(TContributor));
        center := i / yscale;
        left := floor(center - fwidth);
        right := ceil(center + fwidth);
        for j := left to right do
        begin
          weight := filter(center - j);
          if (weight = 0.0) then
            continue;
          if (j < 0) then
            n := -j
          else if (j >= SrcHeight) then
            n := SrcHeight - j + SrcHeight - 1
          else
            n := j;
          k := contrib^[i].n;
          contrib^[i].n := contrib^[i].n + 1;
          contrib^[i].p^[k].pixel := n;
          contrib^[i].p^[k].weight := weight;
        end;
      end;
    end;

    SourceLine := Work.ScanLine[0];
    Delta := integer(Work.ScanLine[1]) - integer(SourceLine);
    DestLine := Dst.ScanLine[0];
    DestDelta := integer(Dst.ScanLine[1]) - integer(DestLine);

    for k := 0 to DstWidth-1 do
    begin
      DestPixel := pointer(DestLine);
      for i := 0 to DstHeight-1 do
      begin
        rgb.r := 0;
        rgb.g := 0;
        rgb.b := 0;
        for j := 0 to contrib^[i].n-1 do
        begin
          color := PColorRGB(integer(SourceLine)+contrib^[i].p^[j].pixel*Delta)^;
          weight := contrib^[i].p^[j].weight;
          if (weight = 0.0) then
            continue;
          rgb.r := rgb.r + color.r * weight;
          rgb.g := rgb.g + color.g * weight;
          rgb.b := rgb.b + color.b * weight;
        end;
        if (rgb.r > 255.0) then
          color.r := 255
        else if (rgb.r < 0.0) then
          color.r := 0
        else
          color.r := round(rgb.r);
        if (rgb.g > 255.0) then
          color.g := 255
        else if (rgb.g < 0.0) then
          color.g := 0
        else
          color.g := round(rgb.g);
        if (rgb.b > 255.0) then
          color.b := 255
        else if (rgb.b < 0.0) then
          color.b := 0
        else
          color.b := round(rgb.b);

        DestPixel^ := color;
        inc(integer(DestPixel), DestDelta);

      end;
      Inc(SourceLine, 1);
      Inc(DestLine, 1);
    end;
    for i := 0 to DstHeight-1 do
      FreeMem(contrib^[i].p);
      FreeMem(contrib);
  finally
    Work.Free;
  end;
end;

procedure CopyMe(tobmp: TBitmap; frbmp : TGraphic);
begin
  tobmp.Width := frbmp.Width;
  tobmp.Height := frbmp.Height;
  tobmp.PixelFormat := pf24bit;
  tobmp.Canvas.Draw(0,0,frbmp);
end;

function Set255(Clr : integer) : integer;
asm
  MOV  EAX,Clr
  CMP  EAX,254
  JG   @SETHI
  CMP  EAX,1
  JL   @SETLO
  RET
@SETHI:
  MOV  EAX,255
  RET
@SETLO:
  MOV  EAX,0
end;           

procedure ConvolveM(ray: array of integer; z: word;
  aBmp: TBitmap);
var
  O, T, C, B : pRGBArray;
  x, y : integer;
  tBufr : TBitmap;
begin
  tBufr := TBitmap.Create;
  tBufr.Width:=aBmp.Width+2;
  tBufr.Height:=aBmp.Height+2;
  tBufr.PixelFormat := pf24bit;
  O := tBufr.ScanLine[0];
  T := aBmp.ScanLine[0];
  O[0] := T[0];
  O[tBufr.Width - 1] := T[aBmp.Width - 1];
  tBufr.Canvas.CopyRect(RECT(1,0,tBufr.Width - 1,1),aBmp.Canvas,
          RECT(0,aBmp.Height - 1,aBmp.Width,aBmp.Height-2));

  O := tBufr.ScanLine[tBufr.Height - 1];
  T := aBmp.ScanLine[aBmp.Height - 1];
  O[0] := T[0];
  O[tBufr.Width - 1] := T[aBmp.Width - 1];
  tBufr.Canvas.CopyRect(RECT(1,tBufr.Height-1,tBufr.Width - 1,tBufr.Height),
         aBmp.Canvas,RECT(0,0,aBmp.Width,1));
  tBufr.Canvas.CopyRect(RECT(tBufr.Width-1,1,tBufr.Width,tBufr.Height-1),
         aBmp.Canvas,RECT(0,0,1,aBmp.Height));
  tBufr.Canvas.CopyRect(RECT(0,1,1,tBufr.Height-1),
         aBmp.Canvas,RECT(aBmp.Width - 1,0,aBmp.Width,aBmp.Height));
  tBufr.Canvas.CopyRect(RECT(1,1,tBufr.Width - 1,tBufr.Height - 1),
    aBmp.Canvas,RECT(0,0,aBmp.Width,aBmp.Height));
  for x := 0 to aBmp.Height - 1 do begin
    O := aBmp.ScanLine[x];
    T := tBufr.ScanLine[x];
    C := tBufr.ScanLine[x+1];
    B := tBufr.ScanLine[x+2];  
    for y := 1 to (tBufr.Width - 2) do begin  
      O[y-1].rgbtRed := Set255(
          ((T[y-1].rgbtRed*ray[0]) +
          (T[y].rgbtRed*ray[1]) + (T[y+1].rgbtRed*ray[2]) +
          (C[y-1].rgbtRed*ray[3]) +
          (C[y].rgbtRed*ray[4]) + (C[y+1].rgbtRed*ray[5])+
          (B[y-1].rgbtRed*ray[6]) +
          (B[y].rgbtRed*ray[7]) + (B[y+1].rgbtRed*ray[8])) div z
          );
      O[y-1].rgbtBlue := Set255(
          ((T[y-1].rgbtBlue*ray[0]) +
          (T[y].rgbtBlue*ray[1]) + (T[y+1].rgbtBlue*ray[2]) +
          (C[y-1].rgbtBlue*ray[3]) +
          (C[y].rgbtBlue*ray[4]) + (C[y+1].rgbtBlue*ray[5])+
          (B[y-1].rgbtBlue*ray[6]) +
          (B[y].rgbtBlue*ray[7]) + (B[y+1].rgbtBlue*ray[8])) div z
          );
      O[y-1].rgbtGreen := Set255(
          ((T[y-1].rgbtGreen*ray[0]) +
          (T[y].rgbtGreen*ray[1]) + (T[y+1].rgbtGreen*ray[2]) +
          (C[y-1].rgbtGreen*ray[3]) +
          (C[y].rgbtGreen*ray[4]) + (C[y+1].rgbtGreen*ray[5])+
          (B[y-1].rgbtGreen*ray[6]) +
          (B[y].rgbtGreen*ray[7]) + (B[y+1].rgbtGreen*ray[8])) div z
          );
    end;
  end;
  tBufr.Free;
end;

procedure ConvolveE(ray: array of integer; z: word;
  aBmp: TBitmap);
var
  O, T, C, B : pRGBArray;
  x, y : integer;
  tBufr : TBitmap;
begin
  tBufr := TBitmap.Create;
  tBufr.Width:=aBmp.Width+2;
  tBufr.Height:=aBmp.Height+2;
  tBufr.PixelFormat := pf24bit;
  O := tBufr.ScanLine[0];
  T := aBmp.ScanLine[0];
  O[0] := T[0];
  O[tBufr.Width - 1] := T[aBmp.Width - 1];
  tBufr.Canvas.CopyRect(RECT(1,0,tBufr.Width - 1,1),aBmp.Canvas,
          RECT(0,0,aBmp.Width,1));

  O := tBufr.ScanLine[tBufr.Height - 1]; 
  T := aBmp.ScanLine[aBmp.Height - 1];
  O[0] := T[0];
  O[tBufr.Width - 1] := T[aBmp.Width - 1];
  tBufr.Canvas.CopyRect(RECT(1,tBufr.Height-1,tBufr.Width - 1,tBufr.Height),
         aBmp.Canvas,RECT(0,aBmp.Height-1,aBmp.Width,aBmp.Height));
  tBufr.Canvas.CopyRect(RECT(tBufr.Width-1,1,tBufr.Width,tBufr.Height-1),
         aBmp.Canvas,RECT(aBmp.Width-1,0,aBmp.Width,aBmp.Height));
  tBufr.Canvas.CopyRect(RECT(0,1,1,tBufr.Height-1),
         aBmp.Canvas,RECT(0,0,1,aBmp.Height));
  tBufr.Canvas.CopyRect(RECT(1,1,tBufr.Width - 1,tBufr.Height - 1),
    aBmp.Canvas,RECT(0,0,aBmp.Width,aBmp.Height));
  for x := 0 to aBmp.Height - 1 do begin
    O := aBmp.ScanLine[x];
    T := tBufr.ScanLine[x];
    C := tBufr.ScanLine[x+1];
    B := tBufr.ScanLine[x+2];
    for y := 1 to (tBufr.Width - 2) do begin
      O[y-1].rgbtRed := Set255(
          ((T[y-1].rgbtRed*ray[0]) +
          (T[y].rgbtRed*ray[1]) + (T[y+1].rgbtRed*ray[2]) +
          (C[y-1].rgbtRed*ray[3]) +
          (C[y].rgbtRed*ray[4]) + (C[y+1].rgbtRed*ray[5])+
          (B[y-1].rgbtRed*ray[6]) +
          (B[y].rgbtRed*ray[7]) + (B[y+1].rgbtRed*ray[8])) div z
          );
      O[y-1].rgbtBlue := Set255(
          ((T[y-1].rgbtBlue*ray[0]) +
          (T[y].rgbtBlue*ray[1]) + (T[y+1].rgbtBlue*ray[2]) +
          (C[y-1].rgbtBlue*ray[3]) +
          (C[y].rgbtBlue*ray[4]) + (C[y+1].rgbtBlue*ray[5])+
          (B[y-1].rgbtBlue*ray[6]) +
          (B[y].rgbtBlue*ray[7]) + (B[y+1].rgbtBlue*ray[8])) div z
          );
      O[y-1].rgbtGreen := Set255(
          ((T[y-1].rgbtGreen*ray[0]) +
          (T[y].rgbtGreen*ray[1]) + (T[y+1].rgbtGreen*ray[2]) +
          (C[y-1].rgbtGreen*ray[3]) +
          (C[y].rgbtGreen*ray[4]) + (C[y+1].rgbtGreen*ray[5])+
          (B[y-1].rgbtGreen*ray[6]) +
          (B[y].rgbtGreen*ray[7]) + (B[y+1].rgbtGreen*ray[8])) div z
          );
    end;
  end;
  tBufr.Free;
end;


procedure ConvolveI(ray: array of integer; z: word;
  aBmp: TBitmap);
var
  O, T, C, B : pRGBArray;
  x, y : integer;
  tBufr : TBitmap;
begin
  tBufr := TBitmap.Create;
  CopyMe(tBufr,aBmp);
  for x := 1 to aBmp.Height - 2 do begin
    O := aBmp.ScanLine[x];
    T := tBufr.ScanLine[x-1];
    C := tBufr.ScanLine[x];
    B := tBufr.ScanLine[x+1];
    for y := 1 to (tBufr.Width - 2) do begin 
      O[y].rgbtRed := Set255(
          ((T[y-1].rgbtRed*ray[0]) +
          (T[y].rgbtRed*ray[1]) + (T[y+1].rgbtRed*ray[2]) +
          (C[y-1].rgbtRed*ray[3]) +
          (C[y].rgbtRed*ray[4]) + (C[y+1].rgbtRed*ray[5])+
          (B[y-1].rgbtRed*ray[6]) +
          (B[y].rgbtRed*ray[7]) + (B[y+1].rgbtRed*ray[8])) div z
          );
      O[y].rgbtBlue := Set255(
          ((T[y-1].rgbtBlue*ray[0]) +
          (T[y].rgbtBlue*ray[1]) + (T[y+1].rgbtBlue*ray[2]) +
          (C[y-1].rgbtBlue*ray[3]) +
          (C[y].rgbtBlue*ray[4]) + (C[y+1].rgbtBlue*ray[5])+
          (B[y-1].rgbtBlue*ray[6]) +
          (B[y].rgbtBlue*ray[7]) + (B[y+1].rgbtBlue*ray[8])) div z
          );
      O[y].rgbtGreen := Set255(
          ((T[y-1].rgbtGreen*ray[0]) +
          (T[y].rgbtGreen*ray[1]) + (T[y+1].rgbtGreen*ray[2]) +
          (C[y-1].rgbtGreen*ray[3]) +
          (C[y].rgbtGreen*ray[4]) + (C[y+1].rgbtGreen*ray[5])+
          (B[y-1].rgbtGreen*ray[6]) +
          (B[y].rgbtGreen*ray[7]) + (B[y+1].rgbtGreen*ray[8])) div z
          );
    end;
  end;
  tBufr.Free;
end;


procedure ConvolveFilter(filternr,edgenr:integer;src:TBitmap);
var
  z : integer;
  ray : array [0..8] of integer;
  OrigBMP : TBitmap;
begin
  case filternr of
    0 : begin
      ray[0] := -1; ray[1] := -1; ray[2] := -1;
      ray[3] := -1; ray[4] :=  8; ray[5] := -1;
      ray[6] := -1; ray[7] := -1; ray[8] := -1;
      z := 1;
      end;
    1 : begin
      ray[0] := -1; ray[1] := -1; ray[2] := -1;
      ray[3] := -1; ray[4] :=  9; ray[5] := -1;
      ray[6] := -1; ray[7] := -1; ray[8] := -1;
      z := 1;
      end;
    2 : begin  
      ray[0] :=  1; ray[1] :=  1; ray[2] :=  1;
      ray[3] :=  1; ray[4] := -2; ray[5] :=  1;
      ray[6] := -1; ray[7] := -1; ray[8] := -1;
      z := 1;
      end;
    3 : begin
      ray[0] := -1; ray[1] := -1; ray[2] := -1;
      ray[3] := -1; ray[4] := 16; ray[5] := -1;
      ray[6] := -1; ray[7] := -1; ray[8] := -1;
      z := 8;
      end;
    4 : begin
      ray[0] :=  0; ray[1] := -1; ray[2] :=  0;
      ray[3] := -1; ray[4] :=  5; ray[5] := -1;
      ray[6] :=  0; ray[7] := -1; ray[8] :=  0;
      z := 1;
      end;
    5 : begin
      ray[0] :=  1; ray[1] :=  0; ray[2] :=  1;
      ray[3] :=  0; ray[4] :=  0; ray[5] :=  0;
      ray[6] :=  1; ray[7] :=  0; ray[8] := -2;
      z := 1;
      end;
    6 : begin
      ray[0] :=  2; ray[1] :=  2; ray[2] :=  2;
      ray[3] :=  2; ray[4] :=  0; ray[5] :=  2;
      ray[6] :=  2; ray[7] :=  2; ray[8] :=  2;
      z := 16;
      end;
    7 : begin
      ray[0] :=  3; ray[1] :=  3; ray[2] :=  3;
      ray[3] :=  3; ray[4] :=  8; ray[5] :=  3;
      ray[6] :=  3; ray[7] :=  3; ray[8] :=  3;
      z := 32;
      end;
    8 : begin
      ray[0] :=  0; ray[1] :=  1; ray[2] :=  0;
      ray[3] :=  1; ray[4] :=  2; ray[5] :=  1;
      ray[6] :=  0; ray[7] :=  1; ray[8] :=  0;
      z := 6;
      end;
    else exit;
  end;
  OrigBMP := TBitmap.Create;
  CopyMe(OrigBMP,src);
  case Edgenr of
    0 : ConvolveM(ray,z,OrigBMP);
    1 : ConvolveE(ray,z,OrigBMP);
    2 : ConvolveI(ray,z,OrigBMP);
  end;
  src.Assign(OrigBMP);
  OrigBMP.Free;
end;


end.
