unit CircularDomain;

interface

uses  SysUtils;

type

  THCicleBuf = class
  private
         Len: integer;
         Pos: integer;
         Buf: array of Double;
  public
        constructor Create(l: integer); virtual;
        destructor Destroy; Override;
        procedure SetLen(l: integer);
        procedure Add(n: Double);
        procedure Put(n: integer; s: Double);
        function Get(n: integer): Double;
  end;

implementation

constructor THCicleBuf.Create(l: integer);
begin
     Len := l;
     SetLength(Buf, l);
     Pos := 0;
end;

destructor THCicleBuf.Destroy;
begin
end;

procedure THCicleBuf.SetLen(l: integer);
begin
     Len := l;
     SetLength(Buf, l);
     Pos := 0;
end;

procedure THCicleBuf.Add(n: Double);
begin
     Inc(Pos);
     if Pos = Len then Pos := 0;
     Buf[Pos] := n;
end;

procedure THCicleBuf.Put(n: integer; s: Double);
begin
     if (n > Len) or (n < -Len) then
     begin
          raise ERangeError.CreateFmt(
          '%d is not within the valid range of %d..%d',
          [n, -Len, Len]);
          Exit;
     end;
     Inc(n, Pos);
     if n >= Len then Dec(n, Len);
     if n < 0 then Inc(n, Len);
     Buf[n] := s;
end;

function THCicleBuf.Get(n: integer): Double;
begin
     if (n > Len) or (n < -Len) then
     begin
          raise ERangeError.CreateFmt(
          '%d is not within the valid range of %d..%d',
          [n, -Len, Len]);
          Exit;
     end;
     Inc(n, Pos);
     if n >= Len then Dec(n, Len);
     if n < 0 then Inc(n, Len);
     Get := Buf[n];
end;

end.
