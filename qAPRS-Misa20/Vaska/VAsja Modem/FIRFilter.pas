unit FIRFilter;

interface

uses CircularDomain;

type
  TFIRFilter = class
  private
    Fpor: Longint;
    arA: array of Double;
    cbBuf: THCicleBuf;
  public
    constructor Create(Ftype: Longint; Wp1, Wp2, Wper, Kp: Double; por: Longint);
    destructor Destroy; Override;
    function Exec(sample: Double): Double;
  end;

implementation


  constructor TFIRFilter.Create(Ftype: Longint; Wp1, Wp2, Wper, Kp: Double;
                                por: Longint);
  var
    i, m: Longint;
    Wz1, Wz2, v: Double;
  begin
    Fpor := por;
    m := (Fpor-1) div 2;
    SetLength(arA, Fpor);
    case Ftype of
    0:
        begin
          Wz1 := Wp1 + Wp1*Wper/100;
          arA[m] := Wp1 + (Wz1 - Wp1)/2;
          for i := 1 to m do
          begin
            v := 2*pi*i;
            arA[i + m] := ((cos(v*Wp1) - cos(v*Wz1))/(Wz1 - Wp1))/(v*v);
            arA[m - i] := ((cos(v*Wp1) - cos(v*Wz1))/(Wz1 - Wp1))/(v*v);
          end
        end;
    1:
        begin
          Wz1 := Wp1 - (0.5-Wp1)*Wper/100;
          arA[m] := 0.5 - Wp1 + (Wp1 - Wz1)/2;
          for i := 1 to m do
          begin
            v := 2*pi*i;
            arA[i + m] := ((cos(v*Wp1) - cos(v*Wz1))/(Wp1 - Wz1))/(v*v);
            arA[m - i] := ((cos(v*Wp1) - cos(v*Wz1))/(Wp1 - Wz1))/(v*v);
          end
        end;
    2:
        begin
          Wz1 := Wp1 - (Wp2-Wp1)*Wper/100;
          Wz2 := Wp2 + (Wp2-Wp1)*Wper/100;
          arA[m] := (Wp2 - Wp1) + ((Wp1 - Wp2) + (Wz2 - Wz1))/2;
          for i := 1 to m do
          begin
            v := 2*pi*i;
            arA[i + m] := ((cos(v*Wp1) - cos(v*Wz1))/(Wp1 - Wz1)+
                                        (cos(v*Wp2) - cos(v*Wz2))/(Wz2 - Wp2))/(v*v);
            arA[m - i] := ((cos(v*Wp1) - cos(v*Wz1))/(Wp1 - Wz1)+
                                        (cos(v*Wp2) - cos(v*Wz2))/(Wz2 - Wp2))/(v*v);
          end
        end;
    3:
        begin
          Wz1 := Wp1 + (Wp2-Wp1)*Wper/100;
          Wz2 := Wp2 - (Wp2-Wp1)*Wper/100;
          arA[m] := 0.5 + (Wp1 - Wp2) + ((Wp2 - Wp1) + (Wz1 - Wz2))/2;
          for i := 1 to m do
          begin
            v := 2*pi*i;
            arA[i + m] := ((cos(v*Wp1) - cos(v*Wz1))/(Wz1 - Wp1)+
                                        (cos(v*Wp2) - cos(v*Wz2))/(Wp2 - Wz2))/(v*v);
            arA[m - i] := ((cos(v*Wp1) - cos(v*Wz1))/(Wz1 - Wp1)+
                                        (cos(v*Wp2) - cos(v*Wz2))/(Wp2 - Wz2))/(v*v);
          end
        end;
    end;
    cbBuf:= THCicleBuf.Create(Fpor);

  end;


  destructor TFIRFilter.Destroy;
  begin
       SetLength(arA, 0);
       cbBuf.Destroy;
  end;


  function TFIRFilter.Exec(sample: Double): Double;
  var
    i: integer;
    s: Double;
  begin
       cbBuf.Add(sample);

    s := 0;
    for i := 0 to Fpor - 1 do
      s := s + arA[i]*cbBuf.Get(i);
    Exec := s;
  end;
      


end.
 