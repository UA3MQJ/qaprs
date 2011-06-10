unit Hilbert;

interface

uses CircularDomain, PHDConst;

type
  TnHilbert = class
  private
    Por: integer;
    cbBuf: THCicleBuf;
  public
    constructor Create; virtual;
    destructor Destroy; Override;
    procedure Exec(sample: Double; var C, S: Double);
  end;

  THilbert = class
  private
    Por: integer;
    cbBuf: THCicleBuf;
  public
    constructor Create; virtual;
    destructor Destroy; Override;
    procedure Exec(sample: Double; var C, S: Double);
  end;

  TAGControl = class
  private
    Por: integer;
    cbBuf: THCicleBuf;
  public
    constructor Create; virtual;
    destructor Destroy; Override;
    procedure Exec(sample: Double; var Val: Double);
  end;

implementation


constructor TnHilbert.Create;
begin
    Por := HILBPFIR_LENGTH;
    cbBuf:= THCicleBuf.Create(Por);
end;

destructor TnHilbert.Destroy;
begin
       cbBuf.Destroy;
end;

procedure TnHilbert.Exec(sample: Double; var C, S: Double);
var i: integer;
begin
     cbBuf.Add(sample);
     C:=0.0;
     S:=0.0;
     for i := 0 to HILBPFIR_LENGTH-1 do
     begin
          S:=  S +    cbBuf.Get(i) *  QHilbertBPFirCoef[i];
          C := C +  -(cbBuf.Get(i) *  IHilbertBPFirCoef[i]);
     end;
end;


///////////////////// THilbert ///////////////////////////
  constructor THilbert.Create;
  begin
    Por := 35;
    cbBuf:= THCicleBuf.Create(Por);
  end;

  destructor THilbert.Destroy;
  begin
       cbBuf.Destroy;
  end;

  procedure THilbert.Exec(sample: Double; var C, S: Double);
  var i: integer;
  begin
    cbBuf.Add(sample);
    C := cbBuf.Get(-(Por div 2 ));
    S := 0;
    for i := 0 to Por-1 do
     S := S + cbBuf.Get(-i)*aHilCoef[i];
  end;


///////////////////// TAGControl ///////////////////////////
constructor TAGControl.Create;
begin
    Por := 60; // por 60 =  ~440 Hz Без искажений
    cbBuf:= THCicleBuf.Create(Por);
end;

procedure TAGControl.Exec(sample: Double; var Val: Double);
var i: integer;
begin
     cbBuf.Add(sample);
     Val:=0.0;
     for i := 0 to Por-1 do  if Val < abs(cbBuf.Get(i)) then  Val:= abs(cbBuf.Get(i));
end;

destructor TAGControl.Destroy;
begin
       cbBuf.Destroy;
end;


end.




