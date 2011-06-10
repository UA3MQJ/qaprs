unit Cmp_LedDisplay;

interface

uses
  Windows, Forms, Classes, Graphics, Controls, ComCtrls, extctrls;

type
  TLedDisplayOrientation = (loHorizontal, loVertical);
  TLedDisplay = class(TCustomControl)
  private
    fOrientation : TLedDisplayOrientation;
    fCtl3D : boolean;
    fSpacing : Integer;
    fMax : Integer;
    fPosition : Integer;
    {BlocksCount}
    bACount : Integer;
    bBCount : Integer;
    bCCount : Integer;

    fActiveColorA: TColor;
    fActiveColorB: TColor;
    fActiveColorC: TColor;
    fDeadColorA  : TColor;
    fDeadColorB  : TColor;
    fDeadColorC  : TColor;

    procedure SetOrientation (value : TLedDisplayOrientation);
    procedure SetCtl3D (value : boolean);
    procedure SetSpacing (value : Integer);
    procedure SetMax (value : Integer);
    procedure SetPosition (value : Integer);
    {BlocksCount}
    procedure SetBACount (value : Integer);
    procedure SetBBCount (value : Integer);
    procedure SetBCCount (value : Integer);
    {Color}
    procedure SetActiveColorA (value:TColor);
    procedure SetActiveColorB (value:TColor);
    procedure SetActiveColorC (value:TColor);
    procedure SetDeadColorA   (value:TColor);
    procedure SetDeadColorB   (value:TColor);
    procedure SetDeadColorC   (value:TColor);

  protected
    procedure Paint; override;
  public
    constructor Create (AOwner : TComponent); override;
  published
    procedure PaintLEDs;
    property Orientation : TLedDisplayOrientation read fOrientation write SetOrientation;
    property Ctl3D : boolean read fCtl3D write SetCtl3D;
    property Spacing : Integer read fSpacing write SetSpacing;
    property Max : Integer read fMax write SetMax;
    property Position : Integer read fPosition write SetPosition;
    {BlocksCount}
    property BlockACount : Integer read bACount write SetBACount;
    property BlockBCount : Integer read bBCount write SetBBCount;
    property BlockCCount : Integer read bCCount write SetBCCount;
    { Color properties }
    property ActiveColor1: TColor read fActiveColorA write SetActiveColorA;
    property ActiveColor2: TColor read fActiveColorB write SetActiveColorB;
    property ActiveColor3: TColor read fActiveColorC write SetActiveColorC;
    property DeadColor1  : TColor read fDeadColorA write SetDeadColorA;
    property DeadColor2  : TColor read fDeadColorB write SetDeadColorB;
    property DeadColor3  : TColor read fDeadColorC write SetDeadColorC;
    { Standard events }
    property OnClick;
    property OnDblClick;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
  end;

procedure Register;

implementation

{|Def: TLedDisplay |}

constructor TLedDisplay.Create (AOwner : TComponent);
begin
  inherited Create (AOwner);
  height := 200;
  width := 30;
  fMax := 10;
  fOrientation := loVertical;
  fCtl3D := True;
  fSpacing := 2;
  bACount:= 7;
  bBCount:= 3;
  bCCount:= 1;
  {Color}
  fActiveColorA:= clLime;
  fActiveColorB:= clYellow;
  fActiveColorC:= clRed;
  fDeadColorA  := clGreen;
  fDeadColorB  := clOlive;
  fDeadColorC  := clMaroon;
end;

procedure TLedDisplay.SetOrientation (value : TLedDisplayOrientation);
begin
  if value <> Orientation then
  begin
    fOrientation := value;
    Repaint
  end
end;

procedure TLedDisplay.SetCtl3D (value : boolean);
begin
  if value <> Ctl3D then
  begin
    fCtl3D := value;
    Repaint
  end
end;

procedure TLedDisplay.SetSpacing (value : Integer);
begin
  if value <> fSpacing then
  begin
    fSpacing := value;
    Repaint
  end
end;

procedure TLedDisplay.SetMax (value : Integer);
begin
  if value <> fMax then
  begin
   fMax := value;
   Repaint
  end
end;

procedure TLedDisplay.SetPosition (value : Integer);
begin
  if value <> fPosition then
  begin
   fPosition := value;
   PaintLEDs
  end
end;

procedure TLedDisplay.PaintLEDs;
var
  i, posLED, LedCount : Integer;
  rect : TRect;
begin
     LedCount:= bACount+ bBCount+ bCCount;
     if max = 0 then max:= 1;
     posLED := Position * LedCount div max;
     if posLED > LedCount then  posLED:= LedCount;

  with Canvas do
  begin

    if Orientation = loVertical then
    begin
      for i := 0 to LedCount-(bBCount+bCCount) do
      begin
           rect.Left := 2;
           rect.right := width-2;
           rect.Top := ((LedCount - i) * height div LedCount)+1;
           rect.Bottom := height div LedCount + rect.Top-spacing-2;
        if i<=posLED then
         Brush.Color := fActiveColorA
        else
         Brush.Color := fDeadColorA;
        FillRect (rect)
      end;

      for i := bACount+1 to LedCount-bCCount do
      begin
           rect.Left := 2;
           rect.right := width-2;
           rect.Top := ((LedCount - i) * height div LedCount)+1;
           rect.Bottom := height div LedCount + rect.Top-spacing-2;
        if i<=posLED then
         Brush.Color := fActiveColorB
        else
         Brush.Color := fDeadColorB;
        FillRect (rect)
      end;

      for i := LedCount-(bCCount-1) to LedCount do
      begin
           rect.Left := 2;
           rect.right := width-2;
           rect.Top := ((LedCount - i) * height div LedCount)+1;
           rect.Bottom := height div LedCount + rect.Top-spacing-2;
        if i<=posLED then
         Brush.Color := fActiveColorC
        else
         Brush.Color := fDeadColorC;
        FillRect (rect)
      end
    end
           else
    begin
      for i := 0 to LedCount-(bBCount+bCCount)-1 do
      begin
           rect.left := (i * width div LedCount)+1;
           rect.right := width div LedCount + rect.left - Spacing-2;
           rect.top := 2;
           rect.bottom := height - 2;
        if i<=posLED-1 then
         Brush.Color := fActiveColorA
        else
         Brush.Color := fDeadColorA;
        FillRect (rect)
      end;

      for i := bACount to LedCount-bCCount-1 do
      begin
           rect.left := (i * width div LedCount)+1;
           rect.right := width div LedCount + rect.left - Spacing-2;
           rect.top := 2;
           rect.bottom := height - 2;
        if i<=posLED-1 then
         Brush.Color := fActiveColorB
        else
         Brush.Color := fDeadColorB;
        FillRect (rect)
      end;

      for i := LedCount-bCCount to LedCount-1 do
      begin
           rect.left := (i * width div LedCount)+1;
           rect.right := width div LedCount + rect.left - Spacing-2;
           rect.top := 2;
           rect.bottom := height - 2;
        if i<=posLED-1 then
         Brush.Color := fActiveColorC
        else
         Brush.Color := fDeadColorC;
        FillRect (rect)
      end
    end {Else}
  end {Canvas}
end;


procedure TLedDisplay.Paint;
var
  i, LCount : Integer;
  rect : TRect;
begin
if Ctl3D then
  with Canvas do
  begin
       LCount:= bACount+ bBCount+ bCCount;
 if Orientation = loVertical then
      begin
      for i := 0 to LCount - 1 do
       begin
       rect.Left := 1;
       rect.right := width - 1;
       rect.Top := i * height div LCount;
       rect.Bottom := height div LCount + rect.Top - spacing;
       Frame3d (Canvas, rect, clBtnShadow, clBtnHighlight, 1);
       FillRect (rect)
       end
      end
     else
     begin
      for i := 0 to LCount - 1 do
       begin
        rect.left := (LCount - i - 1) * width div LCount;
        rect.right := width div LCount + rect.left - Spacing;
        rect.top := 1;
        rect.bottom := height - 1;
        Frame3d (Canvas, rect, clBtnShadow, clBtnHighlight, 1);
        FillRect (rect);
       end;
    end;
  end;
     PaintLEDs
end;


procedure TLedDisplay.SetBACount (value : Integer);
begin
  if value <> bACount then
  begin
    if value <= 0 then value := 1;
    bACount := value;
    rePaint
  end
end;

procedure TLedDisplay.SetBBCount (value : Integer);
begin
  if value <> bBCount then
  begin
    if value <= 0 then value := 1;
    bBCount := value;
    rePaint
  end
end;

procedure TLedDisplay.SetBCCount (value : Integer);
begin
  if value <> bCCount then
  begin
    if value <= 0 then value := 1;
    bCCount := value;
    rePaint
  end
end;

procedure TLedDisplay.SetActiveColorA(value:TColor);
begin
     fActiveColorA:= value;
     PaintLeds
end;

procedure TLedDisplay.SetActiveColorB(value:TColor);
begin
     fActiveColorB:= value;
     PaintLeds
end;

procedure TLedDisplay.SetActiveColorC(value:TColor);
begin
     fActiveColorC:= value;
     PaintLeds
end;

procedure TLedDisplay.SetDeadColorA(value:TColor);
begin
     fDeadColorA:= value;
     PaintLeds
end;

procedure TLedDisplay.SetDeadColorB(value:TColor);
begin
     fDeadColorB:= value;
     PaintLeds
end;

procedure TLedDisplay.SetDeadColorC(value:TColor);
begin
     fDeadColorC:= value;
     PaintLeds
end;

procedure Register;
begin
  RegisterComponents('Media', [TLedDisplay]);
end;

end.
