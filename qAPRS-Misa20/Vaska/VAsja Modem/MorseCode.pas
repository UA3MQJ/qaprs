unit MorseCode;

interface

uses PHDConst, SysUtils, Classes;

function EncodeMorse(Source: String): ByteArray;


implementation


function GetCode(ch:char):string;
begin
     result:='';
     if (ch='�') or (ch='a') then result:='01';
     if (ch='�') or (ch='b') then result:='1000';
     if (ch='�') or (ch='c') then result:='1010';
     if (ch='�') or (ch='d') then result:='100';
     if (ch='�') or (ch='e') then result:='0';
     if (ch='�') or (ch='f') then result:='00010';
     if (ch='�') or (ch='g') then result:='110';
     if (ch='�') or (ch='h') then result:='0000';
     if (ch='�') or (ch='i') then result:='00';
     if (ch='�') or (ch='j') then result:='0111';
     if (ch='�') or (ch='k') then result:='101';
     if (ch='�') or (ch='l') then result:='0100';
     if (ch='�') or (ch='m') then result:='11';
     if (ch='�') or (ch='n') then result:='10';
     if (ch='�') or (ch='o') then result:='111';
     if (ch='�') or (ch='p') then result:='0110';
     if (ch='�') or (ch='q') then result:='1101';
     if (ch='�') or (ch='r') then result:='010';
     if (ch='�') or (ch='s') then result:='000';
     if (ch='�') or (ch='t') then result:='1';
     if (ch='�') or (ch='u') then result:='001';
     if (ch='�') or (ch='v') then result:='0001';
     if (ch='�') or (ch='w') then result:='011';
     if (ch='�') or (ch='x') then result:='1001';
     if (ch='�') or (ch='y') then result:='1011';
     if (ch='�') or (ch='z') then result:='1100';
     if ch='�' then result:='1110';
     if ch='�' then result:='1111';
     if ch='�' then result:='00100';
     if ch='�' then result:='0011';
     if ch='�' then result:='0101';
     if ch='1' then result:='01111';
     if ch='2' then result:='00111';
     if ch='3' then result:='00011';
     if ch='4' then result:='00001';
     if ch='5' then result:='00000';
     if ch='6' then result:='10000';
     if ch='7' then result:='11000';
     if ch='8' then result:='11100';
     if ch='9' then result:='11110';
     if ch='0' then result:='11111';
     if ch='?' then result:='001100';
     if ch='!' then result:='110011';
     if ch='.' then result:='000000';
     if ch=',' then result:='010101';
     if ch=':' then result:='111000';
     if ch=';' then result:='10101';
     if ch='"' then result:='010010';
     if ch='/' then result:='10010';
     if ch='@' then result:='011010';
     if (ch='(') or (ch=')') then result:='101101';
end;


function EncodeMorse(Source: String): ByteArray;
var i,j,n          : Longint;
    s              : String;
    pb             : Byte;
    MSB            : TMemoryStream;
    Buffer         : ByteArray;
    Size           : Integer;
begin
     MSB:= TMemoryStream.Create;

     Size:= 2500;
     Source:= AnsiLowerCase(Source);

     for i:=1 to Length(Source) do
     begin
          if MSB.Size <= MaxDataSize*2 then
          begin
               s:=GetCode(Source[i]);
               if s='' then
               begin //sleep(l*7);
                     pb:=0;
                     for j:= 1 to Size*7 do MSB.Write(pb, 1);
               end;
               for n:=1 to length(s) do
               begin
                    begin //playCode
                          if StrToInt(s[n])=0 then
                          begin
                               pb:=1;
                               for j:= 1 to Size do MSB.Write(pb, 1);
                          end;
                          if StrToInt(s[n])=1 then
                          begin
                               pb:=1;
                               for j:= 1 to Size*3 do MSB.Write(pb, 1);
                          end;
                    end;
                    begin //sleep(l);
                          pb:=0;
                          for j:= 1 to Size do MSB.Write(pb, 1);
                    end;
               end;
               begin //sleep(l*3);
               pb:=0;
               for j:= 1 to Size*3 do MSB.Write(pb, 1);
               end;
          end;
     end;

     MSB.Seek(0,soFromBeginning);

     Size := MSB.Size - MSB.Position;
     SetLength(Buffer, Size);
     MSB.Read(Buffer[0], Size);

     Result:=  Buffer;
     MSB.Free;
end;

begin
end.
