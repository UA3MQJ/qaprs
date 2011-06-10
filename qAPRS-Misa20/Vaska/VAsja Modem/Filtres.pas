unit Filtres;


interface

uses SysUtils, Math, CircularDomain;



type
 TIirLPF = class //БИХ-фильтр ФНЧ
 private
  A,B,C,D,K: Double;
  P: Byte;
  Zz: array of array of Double;
 public
{Расчет коэффициентов фильтра}
    constructor Create(Fsr,Fdis,Hhm:Double; Por: Byte; Kp: Double); virtual;
    destructor Destroy; Override;
    function Exec(sample: Double): Double; //фильтровать след. выборку
 end;

{ФНЧ 1-го порядка }
 TLPFPor1 = class
 private
  A,B: Double;
  Zint: Double;
 public
{Расчет коэффициентов фильтра}
  constructor Create(Fsr,Fdis: Double); virtual;
  function Exec(sample: Double):Double;
 end;



implementation


///////////////////// TIirLPF ///////////////////////////
constructor TIirLPF.Create(Fsr,Fdis,Hhm:Double; Por: Byte; Kp: Double);
var T,A0,B0:Double;
begin
     SetLength(Zz,0,0);
     SetLength(Zz,2,Por);
     P:=Por;
     K:=Kp;
     T:=Pi*Fsr/Fdis;
     A0:=(Sin(T)/Cos(T))/Sqrt(Hhm*Hhm-0.5);
     B0:=Hhm*Hhm*A0*A0;
     A:=(1-A0+B0)/(1+A0+B0);
     B:=2*(1-B0)/(1+A0+B0);
     C:=B0/(1+A0+B0);
     D:=2;

  end;

  destructor TIirLPF.Destroy;
  begin

  end;

function TIirLPF.Exec(sample: Double): Double; //фильтровать след. выборку
var U,Yy:Double;
    i:Integer;
begin
     for i:=0 to P-1 do
     begin
         U:=sample*C+Zz[0,i]*B-Zz[1,i]*A;
         Yy:=U+Zz[0,i]*D+Zz[1,i];
         sample:=K*Yy;
         Zz[1,i]:=Zz[0,i];
         Zz[0,i]:=U;
      end;
      Exec := sample;
end;

///////////////////////////// TLPFPor1 ///////////////////////////////
constructor TLPFPor1.Create(Fsr,Fdis: Double);
begin
   B:=2*Pi*Fsr/Fdis;
   A:=1-B;
   Zint:=0.0;
end;

function TLPFPor1.Exec(sample: Double):Double;
var Y: Double;
begin
   Y:=sample*B+Zint*A;
   Zint := Y;
   Exec := Y;
end;

end.
