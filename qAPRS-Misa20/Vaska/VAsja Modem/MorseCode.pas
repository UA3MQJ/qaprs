unit MorseCode;

interface

uses PHDConst, SysUtils, Classes;

function EncodeMorse(Source: String): ByteArray;


implementation


function GetCode(ch:char):string;
begin
     result:='';
     if (ch='à') or (ch='a') then result:='01';
     if (ch='á') or (ch='b') then result:='1000';
     if (ch='ö') or (ch='c') then result:='1010';
     if (ch='ä') or (ch='d') then result:='100';
     if (ch='å') or (ch='e') then result:='0';
     if (ch='ô') or (ch='f') then result:='00010';
     if (ch='ã') or (ch='g') then result:='110';
     if (ch='õ') or (ch='h') then result:='0000';
     if (ch='è') or (ch='i') then result:='00';
     if (ch='é') or (ch='j') then result:='0111';
     if (ch='ê') or (ch='k') then result:='101';
     if (ch='ë') or (ch='l') then result:='0100';
     if (ch='ì') or (ch='m') then result:='11';
     if (ch='í') or (ch='n') then result:='10';
     if (ch='î') or (ch='o') then result:='111';
     if (ch='ï') or (ch='p') then result:='0110';
     if (ch='ù') or (ch='q') then result:='1101';
     if (ch='ð') or (ch='r') then result:='010';
     if (ch='ñ') or (ch='s') then result:='000';
     if (ch='ò') or (ch='t') then result:='1';
     if (ch='ó') or (ch='u') then result:='001';
     if (ch='æ') or (ch='v') then result:='0001';
     if (ch='â') or (ch='w') then result:='011';
     if (ch='ü') or (ch='x') then result:='1001';
     if (ch='û') or (ch='y') then result:='1011';
     if (ch='ç') or (ch='z') then result:='1100';
     if ch='÷' then result:='1110';
     if ch='ø' then result:='1111';
     if ch='ý' then result:='00100';
     if ch='þ' then result:='0011';
     if ch='ÿ' then result:='0101';
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
