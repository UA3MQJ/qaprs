unit Generators;

interface

uses PHDConst;

type
  TGenPila = class
  private
    LastZ, Aw, M, Rez: Double;
  public
    constructor Create(Fgen, rM, SampleRate: Double); virtual;
    destructor Destroy; Override;
    function Exec(dF: Double): Double;
  end;

  TGenSin = class
  private
    LastZ, Aw, Rez: Double;
    M: Double;
  public
    constructor Create(Phase: integer; Fgen, rM: Double; SampleRate: integer); virtual;
    destructor Destroy; Override;
    function Exec{(sample: Double)}: Double;
  end;

  TGenSinCos = class
  private
    LastZ, Aw, Rez: Double;
    M: Double;
  public
    constructor Create(Fgen, rM: Double; SampleRate: integer); virtual;
    destructor Destroy; Override;
    procedure Exec(var rCos, rSin: Double);
    procedure Reset(Fgen, rM: Double; SampleRate: integer);
  end;

  TGenSinCosUpr = class
  private
  Zp1: Double;
  Zp2: Double;
  LastZ, Aw, Rez: Double;

  public
    constructor Create(Fgen, SampleRate: Double); virtual;
    destructor Destroy; Override;
    procedure Exec(dF, Amp: Double; var rCos, rSin: Double); //генерировать след. выборку
    procedure Reset(Fgen, rM: Double; SampleRate: integer);
  end;

implementation

///////////////////// TGenPila ///////////////////////////
  constructor TGenPila.Create(Fgen, rM, SampleRate: Double);
  begin
    M := rM;
    LastZ := -M;
    Aw := Fgen*2*M/SampleRate;
    Rez := Aw;
  end;

  destructor TGenPila.Destroy;
  begin

  end;

  function TGenPila.Exec(dF: Double): Double;
  begin
    Rez := Aw + LastZ + dF;
    if Rez > M then Rez := {Rez + H} -M;
    Result := LastZ;
    LastZ := Rez;
  end;

///////////////////// TGenSin ///////////////////////////
  constructor TGenSin.Create(Phase: integer; Fgen, rM: Double; SampleRate: integer);
  begin
    M := rM;
    LastZ := Phase;
    Aw := Fgen*2*180/SampleRate;
    Rez := Aw;
  end;

  destructor TGenSin.Destroy;
  begin
  end;

  function TGenSin.Exec{(sample: Double)}: Double;
  begin
    Rez := Aw + LastZ;
    if Rez >= 180 then Rez := Rez - 360;
    Result := M*aSin[round(LastZ+180)];
    LastZ := Rez;
  end;

///////////////////// TGenSinCos ///////////////////////////
  constructor TGenSinCos.Create(Fgen, rM: Double; SampleRate: integer);
  begin
    M := rM;
    LastZ := -180;
    Aw := Fgen*2*180/SampleRate;
    Rez := Aw;
  end;

  procedure TGenSinCos.Reset(Fgen, rM: Double; SampleRate: integer);
  begin
    M := rM;
    LastZ := -180;
    Aw := Fgen*2*180/SampleRate;
    Rez := Aw;
  end;

  destructor TGenSinCos.Destroy;
  begin
  end;

  procedure TGenSinCos.Exec(var rCos, rSin: Double);
  begin
    Rez := Aw + LastZ;
    if Rez >= 180 then Rez := Rez - 360;
    rSin := M*aSin[round(LastZ+180)];
    rCos := M*aCos[round(LastZ+180)];
    LastZ := Rez;
  end;

///////////////////// TGenSinCosUpr ///////////////////////////
  constructor TGenSinCosUpr.Create(Fgen, SampleRate: Double);
  begin
     Aw:=2*Fgen/SampleRate;
  end;

  destructor TGenSinCosUpr.Destroy;
  begin
  end;

  procedure TGenSinCosUpr.Exec(dF, Amp: Double; var rCos, rSin: Double);
  var Xx1,Xx2,Xq1,Xq2,A: Double;
  begin
   A:=(Aw+dF);
   if A < 0 then A:=2+A;
   if Zp1+A<=1 then Zp1:=Zp1+A
               else Zp1:=Zp1+A-2;
   if Zp1+0.5<=1 then Zp2:=Zp1+0.5
                 else Zp2:=Zp1-1.5;
   Xx1:=Abs(Zp1)*2-1;
   Xq1:=Sqr(Xx1);
   rSin:=Amp*Xx1*2*(0.7725425-Xq1*(0.295085-0.0225425*Xq1));
   Xx2:=Abs(Zp2)*2-1;
   Xq2:=Sqr(Xx2);
   rCos:=Amp*Xx2*2*(0.7725425-Xq2*(0.295085-0.0225425*Xq2));
  end;

  procedure TGenSinCosUpr.Reset(Fgen, rM: Double; SampleRate: integer);
  begin
    LastZ := -180;
    Aw := Fgen*360/SampleRate;
    Rez := Aw;
  end;
      


end.
 