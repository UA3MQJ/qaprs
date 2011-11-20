unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TForm1 = class(TForm)
    Button5: TButton;
    Button1: TButton;
    Button2: TButton;
    Button3: TButton;
    Edit1: TEdit;
    Button4: TButton;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Edit3: TEdit;
    Button6: TButton;
    Button7: TButton;
    Button8: TButton;
    Memo1: TMemo;
    Memo0: TMemo;
    Button9: TButton;
    Button10: TButton;
    Memo2: TMemo;
    Button11: TButton;
    Button12: TButton;
    procedure Button5Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure Button7Click(Sender: TObject);
    procedure Button8Click(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button10Click(Sender: TObject);
    procedure Button11Click(Sender: TObject);
    procedure Button12Click(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

      znum:array[0..6]of double=(
      1.298893750e-02,
        -5.874967082e-02,
        0.1246558127,
        -0.1571493913,
        0.1246558127,
        -5.874967082e-02,
        1.298893750e-02);

zden:array[0..5]of double = (
         0.6981033315,
        -4.238297152,
        10.92365980,
        -15.29980859,
        12.28821814,
        -5.371234764) ;



implementation

{$R *.dfm}


procedure TForm1.Button5Click(Sender: TObject);
var a,b,c,d, out_sig:smallint;
    sig, sumden, sumnum, outnum:Real	;
    f:file of smallint;
    i,j:longint;
    delay:array[0..8]of Real	;

begin
  //randomize;

  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

  for j:=0 to 8 do delay[j]:=0;

  for i:=0 to 1000000 do begin

    sig:=(random(2000)-1000)/1000;

    sumden:=0;
    sumnum:=0;

    j:=0;
    while (j<>6) do begin
      delay[j] := delay[j+1];
      sumden:=sumden+delay[0]*zden[j];
      sumnum:=sumnum+delay[0]*znum[j];
      j:=j+1;
    end;
    delay[8] := sig-sumden;

    outnum:= delay[6]*znum[6] + sumnum;

    out_sig := round(32000*outnum);

    write(f, out_sig);
  end;

  closefile(f);

  close;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
a,b,c,d, out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
    f:file of smallint;
    i,j:longint;
    delay:array[0..24]of double;


begin
  //randomize;

  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

  //значения входного сигнала хранятся в delay[n]
  //где данные добавляются в самый конец. соответственно delay[0] это самый задержаный сигнал
  //       Z0      z1       z2       z3        z4  -задержки
  //             idx=3    idx=2     idx=1    idx=0
  //DEN = [1, -3.53251, 4.83144, -3.01768, .726389] <- поэтому справа на лево
  //NUM = [9.90307e-02, -.332205, .473980, -.332205, 9.90307e-02]  однохуйственно, ибо симметрично,
  //                                                               но думаю, что тоже


//ФВЧ
//DEN = [1, -3.28649, 4.23906, -2.52473, .592847]
//NUM = [.751139, -2.91026, 4.32034, -2.91026, .751139]

  for i:=0 to 1000000 do begin

    sig:=(random(64000)-32000)/32000;

    sumden:=0;
    sumnum:=0;

    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];

    //ФНЧ
    //sumden:=delay[0]*0.726389 + delay[1]*-3.01768 + delay[2]*4.83144 + delay[3]*-3.53251;
    //sumnum:=delay[0]*9.90307e-02 + delay[1]*-0.332205 + delay[2]*0.473980 + delay[3]*-0.332205 ;
    //delay[4] := sig-sumden;
    //
    //outnum:= delay[4]*9.90307e-02 + sumnum;

    //ФВЧ
//NUM = [3.89861407729653774e-02, -.124190079354458811, .174791496677258623, -.124190079354458811, 3.89861407729653774e-02]
//DEN = [1, -3.58077318575538916, 4.92938255280170896, -3.08244817335160102, .738222425819552981]


    sumden:=delay[0]*0.738222425819552981 + delay[1]*-3.08244817335160102 + delay[2]*4.92938255280170896 + delay[3]*-3.58077318575538916;
    sumnum:=delay[0]*3.89861407729653774e-02 + delay[1]*-0.124190079354458811 + delay[2]*0.174791496677258623 + delay[3]*-0.124190079354458811 ;
    delay[4] := sig-sumden;

    outnum:= delay[4]*3.89861407729653774e-02 + sumnum;

    out_sig := round(10000*outnum);

    write(f, out_sig);
  end;

  closefile(f);

  close;
end;

procedure TForm1.Button2Click(Sender: TObject);
var
a,b,c,d, out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
    f:file of smallint;
    i,j:longint;
    delay:array[0..7]of extended;


begin
  //randomize;

  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

  //значения входного сигнала хранятся в delay[n]
  //где данные добавляются в самый конец. соответственно delay[0] это самый задержаный сигнал
  //       Z0      z1       z2       z3        z4  -задержки
  //             idx=3    idx=2     idx=1    idx=0
  //DEN = [1, -3.53251, 4.83144, -3.01768, .726389] <- поэтому справа на лево
  //NUM = [9.90307e-02, -.332205, .473980, -.332205, 9.90307e-02]  однохуйственно, ибо симметрично,
  //                                                               но думаю, что тоже


//ФВЧ
//DEN = [1, -3.28649, 4.23906, -2.52473, .592847]
//NUM = [.751139, -2.91026, 4.32034, -2.91026, .751139]

    delay[0] := 0;
    delay[1] := 0;
    delay[2] := 0;
    delay[3] := 0;
    delay[4] := 0;
    delay[5] := 0;
    delay[6] := 0;
    delay[7] := 0;


  for i:=0 to 1000000 do begin

    sig:=(random(64000)-32000)/32000;

    sumden:=0;
    sumnum:=0;

    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];
    delay[4] := delay[5];
    delay[5] := delay[6];
    delay[6] := delay[7];

    //ФНЧ

    sumden:=delay[0]*-0.740834848235642186 + delay[1]*5.24773809122260909 + delay[2]*-16.0930640241974311 + delay[3]*27.6976882085760435 + delay[4]*-28.8977384069896076 + delay[5]*18.2806580059529614 + delay[6]*-6.49442243695636760 ;
    sumnum:=delay[0]*6.49970524589488569e-04 + delay[1]*-2.65954661442456424e-03 + delay[2]*4.23299672923018368e-03 + delay[3]*-2.21112595311232270e-03 + delay[4]*-2.21112595311232270e-03 + delay[5]*4.23299672923018368e-03 + delay[6]*-2.65954661442456424e-03 ;
    delay[7] := sig-sumden;

    outnum:= delay[7]*6.49970524589488569e-04 + sumnum;

    out_sig := round(10000*outnum);

    write(f, out_sig);
  end;

  closefile(f);

  close;
end;


procedure TForm1.Button3Click(Sender: TObject);
var
a,b,c,d,sig, out_sig:smallint;
    sumden, sumnum, outnum:extended;
    f:file of smallint;

    i,j:longint;
    delay:array[0..10]of int64;
    t1,t2,t3,t4,t5,t6:int64	;
    sumden2, sumnum2, outnum2:int64;
    zn1,zn2,zn3,zn4,zn5,zn6,zn7,zn8,zn9,zn10:int64;
    zd1,zd2,zd3,zd4,zd5,zd6,zd7,zd8,zd9,zd10:int64;


begin
  //randomize;

  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

    delay[0]  := 0;
    delay[1]  := 0;
    delay[2]  := 0;
    delay[3]  := 0;
    delay[4]  := 0;
    delay[5]  := 0;
    delay[6]  := 0;
    delay[7]  := 0;
    delay[8]  := 0;
    delay[9]  := 0;
    delay[10] := 0;

  //значения входного сигнала хранятся в delay[n]
  //где данные добавляются в самый конец. соответственно delay[0] это самый задержаный сигнал
  //       Z0      z1       z2       z3        z4  -задержки
  //             idx=3    idx=2     idx=1    idx=0
  //DEN = [1, -3.53251, 4.83144, -3.01768, .726389] <- поэтому справа на лево
  //NUM = [9.90307e-02, -.332205, .473980, -.332205, 9.90307e-02]  однохуйственно, ибо симметрично,
  //                                                               но думаю, что тоже

//NUM = [.10195580, -.40782320, .61173480, -.40782320, .10195580]
//DEN = [1, -3.9999811, 5.9999433, -3.9999433, .99998110]

//целая часть при 16 битах - 0..65535 ок
// 16 бит на целое, 16 на дробное
// старшие 16 бит - целая часть
// младшие 16 бит - это дробная часть * 65536


    //Elliptic LowPass Filter 8 Order 48000 Hz Sample Rate 60 dB 3000 Hz
    //DEN = [1, -7.9335207, 27.546185, -54.672609, 67.843542, -53.898546, 26.771642, -7.6012301, .94453795]
    //NUM = [1.1762793e-03, -9.3186765e-03, 3.2387962e-02, -6.4504221e-02, 8.0517311e-02, -6.4504221e-02, 3.2387962e-02, -9.3186765e-03, 1.1762793e-03]
    //
    //znum
    //    1,1762793e-03   =  0,0011762793 = hi16 =0 lo16=    77
    //    -9,3186765e-03  = -0,0093186765 = hi16 =0 lo16=  -611
    //    3,2387962e-02   =  0,032387962  = hi16 =0 lo16=  2123
    //    -6,4504221e-02  = -0,064504221  = hi16 =0 lo16= -4227
    //    8,0517311e-02   =  0,080517311  = hi16 =0 lo16=  5277
    //    -6,4504221e-02  = -0,064504221  = hi16 =0 lo16= -4227
    //    3,2387962e-02   =  0,032387962  = hi16 =0 lo16=  2123
    //    -9,3186765e-03  = -0,0093186765 = hi16 =0 lo16=  -611
    //    1,1762793e-03   =  0,0011762793 = hi16 =0 lo16=    77
    //
    //zden
    //     0,94453795,    = hi16 =   0   lo16= 61901
    //    -7,6012301,     = hi16 =  -7   lo16= 39402
    //    26,771642,      = hi16 =  26   lo16= 50570
    //   -53,898546,      = hi16 = -53   lo16= 58887
    //    67,843542,      = hi16 =  67   lo16= 55282
    //   -54,672609,      = hi16 = -54   lo16= 44080
    //    27,546185,      = hi16 =  27   lo16= 35795
    //    -7,9335207      = hi16 =  -7   lo16= 61179

    //t2 := 0*65536 + round(0.99 * 65536); // получается 0.5
    zn1:=   77;
    zn2:= -611;
    zn3:= 2123;
    zn4:=-4227;
    zn5:= 5277;
    zn6:=-4227;
    zn7:= 2123;
    zn8:= -611;
    zn9:=   77;

    zd1:=  (0*65536  + round(0.94453795 * 65536)); //
    zd2:= -(7*65536  + round(0.6012301 * 65536)); //
    zd3:=  (26*65536 + round(0.771642 * 65536)); //
    zd4:= -(53*65536 + round(0.898546 * 65536)); //
    zd5:=  (67*65536 + round(0.843542 * 65536)); //
    zd6:= -(54*65536 + round(0.672609 * 65536)); //
    zd7:=  (27*65536 + round(0.546185 * 65536)); //
    zd8:= -(7*65536  + round(0.9335207 * 65536)); // 

  for i:=0 to 1000000 do begin

   { sig:=(random(64000)-32000)/32000;

    sumden:=0;
    sumnum:=0;

    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];


    sumden:=delay[0]*0.738222425819552981 + delay[1]*-3.08244817335160102 + delay[2]*4.92938255280170896 + delay[3]*-3.58077318575538916;
    sumnum:=delay[0]*3.89861407729653774e-02 + delay[1]*-0.124190079354458811 + delay[2]*0.174791496677258623 + delay[3]*-0.124190079354458811 ;
    delay[4] := sig-sumden;

    outnum:= delay[4]*3.89861407729653774e-02 + sumnum;

    out_sig := round(10000*outnum);

    write(f, out_sig);         }

    {
    //берем исходный сигнал и несколько раз умножаем на 0.99 для проверки

    //пусть исходный сигнал будет в 16 младших битах
    sig:=(random(64000)-32000); // изначальный сигнал gjxnb 100%

    t1 := sig * 65536; //исходный сигнал 16 бит, а считаем в 32-х, поэтому сдвинем целую часть в старшие разряды
    t2 := 0*65536 + round(0.99 * 65536); // получается 0.5

    t3 := (t1 * t2) div 65536; //32bit X 32bit = 64bit
    //           цел  дроб
    // 0000 0000 0001.0000   1.0
    // Х                     x
    //           цел  дроб
    // 0000 000  0001.0000   1.0
    // =
    // 0000 0001 0000.0000
    // сдвигаем на 16 бит вправо
    // 0000 0000 0001.0000 результат

    t3 := (t3 * t2) div 65536;
    t3 := (t3 * t2) div 65536;
    t3 := (t3 * t2) div 65536;

    t4 := ((t3 div 65536) ); //сдвинули на 32 бита, то есть округлили до целых
    }

    //пусть исходный сигнал будет в 16 младших битах
    sig:=(random(64000)-32000) div 2; // изначальный сигнал 100% 16 бит

    t1 := sig * 65536; //исходный сигнал 16 бит, а считаем в 32-х, поэтому сдвинем целую часть в старшие разряды
                       //тем самым переведя в дробный формат

    out_sig := t4; //взяли только целую часть


    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];
    delay[4] := delay[5];
    delay[5] := delay[6];
    delay[6] := delay[7];
    delay[7] := delay[8];

    //sumden:=delay[0]*0.738222425819552981 + delay[1]*-3.08244817335160102 + delay[2]*4.92938255280170896 + delay[3]*-3.58077318575538916;
    //sumnum:=delay[0]*3.89861407729653774e-02 + delay[1]*-0.124190079354458811 + delay[2]*0.174791496677258623 + delay[3]*-0.124190079354458811 ;
    //delay[4] := sig-sumden;
    //outnum:= delay[4]*3.89861407729653774e-02 + sumnum;
    sumden2 := (zd1*delay[0] div 65536) + (zd2*delay[1] div 65536) + (zd3*delay[2] div 65536) + (zd4*delay[3] div 65536) + (zd5*delay[4] div 65536) + (zd6*delay[5] div 65536) + (zd7*delay[6] div 65536) + (zd8*delay[7] div 65536);
    sumnum2 := (zn1*delay[0] div 65536) + (zn2*delay[1] div 65536) + (zn3*delay[2] div 65536) + (zn4*delay[3] div 65536) + (zn5*delay[4] div 65536) + (zn6*delay[5] div 65536) + (zn7*delay[6] div 65536) + (zn8*delay[7] div 65536);

    //t1 нормальная дробь
    // результат частного - тоже нормальная дробь.
    delay[8] := t1 - (sumden2);

    //delay[8] нормальная дробь
    //умножим на zn1 нормальную дробь получаем сдвинутое число
    //складываем так же со здвинутой суммой.
    outnum2:= ((delay[8]*zn9 div 65536) + sumnum2) div 65536; //получаем результат в виде нормальной дроби


    out_sig := ( t1 div 65536); //преобразуем результат к целому


    write(f, out_sig);

  end;

  closefile(f);

  close;
end;


procedure TForm1.Button4Click(Sender: TObject);
begin
  edit1.Text:=floattostr(strtofloat(edit1.Text));
  edit2.Text:=inttostr( round( strtofloat(edit1.Text)*65536 ) );
  edit3.Text:=floattostr( 100-((strtofloat(edit2.Text)/65536)/strtofloat(edit1.Text))*100  );
end;

procedure TForm1.Button6Click(Sender: TObject);
var a1,a2,a3,a4,a5,a6:int64;
begin
  a1:= 0*65536 + round(0.9999 * 65536);
  a2:= -1;
  a3:=a1*a2;
  a3:=a3*a2;
  a3:=a3*a2;
  a3:=a3*a2;
  a3:=a3*a2;
  a3:=a3*a2;
end;

procedure TForm1.Button7Click(Sender: TObject);
var
a,b,c,d,sig, out_sig:smallint;
    sumden, sumnum, outnum:extended;
    f:file of smallint;

    i,j:longint;
    delay:array[0..10]of int64;
    t1,t2,t3,t4,t5,t6:int64	;
    sumden2, sumnum2, outnum2:int64;
    zn1,zn2,zn3,zn4,zn5,zn6,zn7,zn8,zn9,zn10:int64;
    zd1,zd2,zd3,zd4,zd5,zd6,zd7,zd8,zd9,zd10:int64;


begin
  //randomize;

  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

    delay[0]  := 0;
    delay[1]  := 0;
    delay[2]  := 0;
    delay[3]  := 0;
    delay[4]  := 0;
    delay[5]  := 0;
    delay[6]  := 0;
    delay[7]  := 0;
    delay[8]  := 0;
    delay[9]  := 0;
    delay[10] := 0;

  //значения входного сигнала хранятся в delay[n]
  //где данные добавляются в самый конец. соответственно delay[0] это самый задержаный сигнал
  //       Z0      z1       z2       z3        z4  -задержки
  //             idx=3    idx=2     idx=1    idx=0
  //DEN = [1, -3.53251, 4.83144, -3.01768, .726389] <- поэтому справа на лево
  //NUM = [9.90307e-02, -.332205, .473980, -.332205, 9.90307e-02]  однохуйственно, ибо симметрично,
  //                                                               но думаю, что тоже

//NUM = [.10195580, -.40782320, .61173480, -.40782320, .10195580]
//DEN = [1, -3.9999811, 5.9999433, -3.9999433, .99998110]

//целая часть при 16 битах - 0..65535 ок
// 16 бит на целое, 16 на дробное
// старшие 16 бит - целая часть
// младшие 16 бит - это дробная часть * 65536


    //NUM = [0,099531468, -0.39593910, 0.59282377, -0.39593910, 0,099531468]
    //DEN = [1, -3.9384485, 5.8219806, -3.8283930, 0.94486931]

    //NUM = [0.69626329, -1.3878694, 0.69626329]
    //DEN = [1, -1.9671299, 0.97178709]
    //
    //znum
    //    0,099531468  =  hi16 = 0 lo16=  6523
    //   -0,39593910   =  hi16 = 0 lo16=  25948
    //    0,59282377   =  hi16 = 0 lo16=  38851
    //   -0,39593910   =  hi16 = 0 lo16=  25948
    //    0,099531468  =  hi16 = 0 lo16=  6523
    //
    //zden
    //    0.94486931     = hi16 =   0  lo16= 61923
    //   -3.8283930      = hi16 =  -3  lo16= 54290
    //    5.8219806      = hi16 =   5  lo16= 53869
    //   -3.9384485      = hi16 =  -3  lo16= 61502

    //t2 := 0*65536 + round(0.99 * 65536); // получается 0.5
   { zn1:=  0,099531468;
    zn2:= -0,39593910 ;
    zn3:=  0,59282377;
    zn4:= -0,39593910;
    zn5:=  0,099531468;


    zd1:=  0.94486931; //
    zd2:= -(3*65536  + 54290); //
    zd3:=  (5*65536  + 53869); //
    zd4:= -(3*65536  + 61502); //
           }
  for i:=0 to 1000000 do begin

   { sig:=(random(64000)-32000)/32000;

    sumden:=0;
    sumnum:=0;

    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];


    sumden:=delay[0]*0.738222425819552981 + delay[1]*-3.08244817335160102 + delay[2]*4.92938255280170896 + delay[3]*-3.58077318575538916;
    sumnum:=delay[0]*3.89861407729653774e-02 + delay[1]*-0.124190079354458811 + delay[2]*0.174791496677258623 + delay[3]*-0.124190079354458811 ;
    delay[4] := sig-sumden;

    outnum:= delay[4]*3.89861407729653774e-02 + sumnum;

    out_sig := round(10000*outnum);

    write(f, out_sig);         }

    {
    //берем исходный сигнал и несколько раз умножаем на 0.99 для проверки

    //пусть исходный сигнал будет в 16 младших битах
    sig:=(random(64000)-32000); // изначальный сигнал gjxnb 100%

    t1 := sig * 65536; //исходный сигнал 16 бит, а считаем в 32-х, поэтому сдвинем целую часть в старшие разряды
    t2 := 0*65536 + round(0.99 * 65536); // получается 0.5

    t3 := (t1 * t2) div 65536; //32bit X 32bit = 64bit
    //           цел  дроб
    // 0000 0000 0001.0000   1.0
    // Х                     x
    //           цел  дроб
    // 0000 000  0001.0000   1.0
    // =
    // 0000 0001 0000.0000
    // сдвигаем на 16 бит вправо
    // 0000 0000 0001.0000 результат

    t3 := (t3 * t2) div 65536;
    t3 := (t3 * t2) div 65536;
    t3 := (t3 * t2) div 65536;

    t4 := ((t3 div 65536) ); //сдвинули на 32 бита, то есть округлили до целых
    }

    //пусть исходный сигнал будет в 16 младших битах
    sig:=(random(64000)-32000) div 8; // изначальный сигнал 100% 16 бит

    t1 := sig * 65536; //исходный сигнал 16 бит, а считаем в 32-х, поэтому сдвинем целую часть в старшие разряды
                       //тем самым переведя в дробный формат

    out_sig := t4; //взяли только целую часть


    delay[0] := delay[1];
    delay[1] := delay[2];
    delay[2] := delay[3];
    delay[3] := delay[4];

    sumden2:=0;
    sumnum2:=0;

    //sumden:=delay[0]*0.738222425819552981 + delay[1]*-3.08244817335160102 + delay[2]*4.92938255280170896 + delay[3]*-3.58077318575538916;
    //sumnum:=delay[0]*3.89861407729653774e-02 + delay[1]*-0.124190079354458811 + delay[2]*0.174791496677258623 + delay[3]*-0.124190079354458811 ;
    //delay[4] := sig-sumden;
    //outnum:= delay[4]*3.89861407729653774e-02 + sumnum;
    sumden2 := sumden2 + (zd1*delay[0] );
    sumden2 := sumden2 + (zd2*delay[1] );
    sumden2 := sumden2 + (zd3*delay[2] );
    sumden2 := sumden2 + (zd4*delay[3] );

    sumnum2 := sumnum2 + (zn1*delay[0] );
    sumnum2 := sumnum2 + (zn2*delay[1] );
    sumnum2 := sumnum2 + (zn3*delay[2] );
    sumnum2 := sumnum2 + (zn4*delay[3] );

    //t1 нормальная дробь
    // результат частного - тоже нормальная дробь.
    delay[4] := t1 - (sumden2 div 65536);


    sumnum2:= sumnum2 + ((delay[4]*zn5));

    outnum2 := sumnum2 div 65536;


    out_sig := ( outnum2 div 65536); //преобразуем результат к целому


    write(f, out_sig);

  end;

  closefile(f);

  close;
end;


procedure TForm1.Button8Click(Sender: TObject);
const length_of_arrays=20;
var tsnum, tsden, tts1:string;
     inznum:array[0..length_of_arrays] of Extended; //временные, т.к. перевернуть массив надо
     inzden:array[0..length_of_arrays] of Extended;
     znum_fl:array[0..length_of_arrays] of Extended;
     zden_fl:array[0..length_of_arrays] of Extended;
    delay_fl:array[0..length_of_arrays] of Extended;
    filter_order:integer; //порядок фильтра напр 4,
    //тогда количества znum и zden и ячеек в delay будет на 1 больше

    i,j:integer;
    f:file of smallint; out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
     mulidx:integer;

  function normalNum(s:string):string;
  var ts:string;
  begin
    ts:=s;
    //мудрить не буду
    if ts[1]='.' then ts:='0,'+copy(ts, 2, length(ts)-1); // '.123'
    if ((ts[1]='-') and (ts[2]='.')) then ts:='-0,'+copy(ts, 3, length(ts)-2); // '-.123'
    if ts[2]='.' then ts[2]:=',';
    if ts[3]='.' then ts[3]:=',';
    if ts[4]='.' then ts[4]:=',';
    if ts[5]='.' then ts[5]:=',';
    if ts[6]='.' then ts[6]:=',';
    result:=ts;
  end;
begin
  tsnum:=memo0.Lines.Strings[0];
  tsden:=memo0.Lines.Strings[1];

  tsnum:=copy( tsnum, 8, length(tsnum)-8); //обрезаем лишние данные
  tsden:=copy( tsden, 8, length(tsden)-8);

  filter_order:=0;

  //разбор znum
  memo1.Lines.Add( 'parse of znum' );
  while (pos(',', tsnum)>0) do begin

    tts1:=normalNum( copy(tsnum,1,pos(',', tsnum)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inznum[filter_order]:=strtofloat(tts1);
    delete(tsnum,1,pos(',', tsnum)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsnum );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inznum[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //разбор znum
  filter_order := 0;
  memo1.Lines.Add( 'parse of zden' );
  while (pos(',', tsden)>0) do begin

    tts1:=normalNum( copy(tsden,1,pos(',', tsden)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inzden[filter_order]:=strtofloat(tts1);
    delete(tsden,1,pos(',', tsden)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsden );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inzden[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //отчистка и инициализация
  for i:=0 to length_of_arrays do begin
    znum_fl[i]:=0;
    zden_fl[i]:=0;
    delay_fl[i]:=0;
  end;

  sumden:=0; sumnum:=0; outnum:=0;

  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of znum_fl' );
  for i:=0 to filter_order-1 do begin
    znum_fl[i]:=inznum[filter_order-i-1] ;
    memo1.Lines.Add( 'znum_fl['+inttostr(i)+']='+ floattostr(znum_fl[i]) );
  end;
  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of zden_fl' );
  for i:=1 to filter_order-1 do begin
    zden_fl[i-1]:=inzden[filter_order-i] ;
    memo1.Lines.Add( 'zden_fl['+inttostr(i-1)+']='+ floattostr(zden_fl[i-1]) );
  end;

  filter_order := filter_order - 1; //порядок фильтра на один меньше записей в строке
  memo1.Lines.Add( 'filter_order=' + inttostr(filter_order) );




  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

  for i:=0 to 1500000 do begin

    sumden:=0; sumnum:=0; outnum:=0;

    //генерим сигнал от -1 до 1
    sig:=(random(64000)-32000)/32000;

    //продвинем массив delay
    for j:=0 to filter_order-1 do begin
      delay_fl[j]:=delay_fl[j+1];
      sumden := sumden + (delay_fl[j]*zden_fl[j]);
      sumnum := sumnum + (delay_fl[j]*znum_fl[j]);
    end;

    delay_fl[filter_order] := sig-sumden;
    sumnum := sumnum + delay_fl[filter_order]*znum_fl[filter_order];

   { if mulidx=0 then sumnum:=sumnum*0;//(0);
    if mulidx=1 then sumnum:=sumnum*0.3826834323650897717284599840304;//(22.5);
    if mulidx=2 then sumnum:=sumnum*0.70710678118654752440084436210485;//(45);
    if mulidx=3 then sumnum:=sumnum*0.92387953251128675612818318939679;//(67.5);
    if mulidx=4 then sumnum:=sumnum*1;//(90);
    if mulidx=5 then sumnum:=sumnum*0.92387953251128675612818318939679;//(112.5);
    if mulidx=6 then sumnum:=sumnum*0.70710678118654752440084436210485;//(135);
    if mulidx=7 then sumnum:=sumnum*0.3826834323650897717284599840304;//(157.5);
    if mulidx=8 then sumnum:=sumnum*0;//(180);
    if mulidx=9 then sumnum:=sumnum*0.3826834323650897717284599840304;//(202.5);
    if mulidx=10 then sumnum:=sumnum*0.70710678118654752440084436210485;//(225);
    if mulidx=11 then sumnum:=sumnum*0.92387953251128675612818318939679;//(247.5);
    if mulidx=12 then sumnum:=sumnum*1;//(270);
    if mulidx=13 then sumnum:=sumnum*0.92387953251128675612818318939679;//(292.5);
    if mulidx=14 then sumnum:=sumnum*0.70710678118654752440084436210485;//(315);
    if mulidx=15 then sumnum:=sumnum*0.3826834323650897717284599840304;//(337.5);

       }

    out_sig := round(32000*sumnum);

    write(f, out_sig);

    mulidx:=mulidx+1;

    if mulidx=16 then mulidx:=0;

  end;

  closefile(f);
  //close;
end;

procedure TForm1.FormShow(Sender: TObject);
begin
//  Button8Click(sender);
end;

procedure TForm1.Button10Click(Sender: TObject);
const length_of_arrays=20;
var tsnum, tsden, tts1:string;
     inznum:array[0..length_of_arrays] of Extended; //временные, т.к. перевернуть массив надо
     inzden:array[0..length_of_arrays] of Extended;
     znum_fl:array[0..length_of_arrays] of Extended;
     zden_fl:array[0..length_of_arrays] of Extended;
    delay_fl:array[0..length_of_arrays] of Extended;
    filter_order:integer; //порядок фильтра напр 4,
    //тогда количества znum и zden и ячеек в delay будет на 1 больше

    i,j:integer;
    f:file of smallint; out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
    mulidx:integer;

  function normalNum(s:string):string;
  var ts:string;
  begin
    ts:=s;
    //мудрить не буду
    if ts[1]='.' then ts:='0,'+copy(ts, 2, length(ts)-1); // '.123'
    if ((ts[1]='-') and (ts[2]='.')) then ts:='-0,'+copy(ts, 3, length(ts)-2); // '-.123'
    if ts[2]='.' then ts[2]:=',';
    if ts[3]='.' then ts[3]:=',';
    if ts[4]='.' then ts[4]:=',';
    if ts[5]='.' then ts[5]:=',';
    if ts[6]='.' then ts[6]:=',';
    result:=ts;
  end;
begin
  tsnum:=memo2.Lines.Strings[0];
  //tsden:=memo0.Lines.Strings[1];

  tsnum:=copy( tsnum, 8, length(tsnum)-8); //обрезаем лишние данные
  //tsden:=copy( tsden, 8, length(tsden)-8);

  filter_order:=0;

  //разбор znum
  memo1.Lines.Add( 'parse of znum' );
  while (pos(',', tsnum)>0) do begin

    tts1:=normalNum( copy(tsnum,1,pos(',', tsnum)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inznum[filter_order]:=strtofloat(tts1);
    delete(tsnum,1,pos(',', tsnum)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsnum );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inznum[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //отчистка и инициализация
  for i:=0 to length_of_arrays do begin
    znum_fl[i]:=0;
    delay_fl[i]:=0;
  end;

  sumden:=0; sumnum:=0; outnum:=0;

  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of znum_fl' );
  for i:=0 to filter_order-1 do begin
    znum_fl[i]:=inznum[filter_order-i-1] ;
    memo1.Lines.Add( 'znum_fl['+inttostr(i)+']='+ floattostr(znum_fl[i]) );
  end;

  filter_order := filter_order - 1; //порядок фильтра на один меньше записей в строке
  memo1.Lines.Add( 'filter_order=' + inttostr(filter_order) );



  assignfile(f, 'd:\outellip.dat');
  rewrite(f);

  mulidx:=0;
  for i:=0 to 500000 do begin

    sumden:=0; sumnum:=0; outnum:=0;

    //генерим сигнал от -1 до 1
    sig:=(random(64000)-32000)/32000;

    //продвинем массив delay
    for j:=0 to filter_order-1 do begin
      delay_fl[j]:=delay_fl[j+1];
      sumnum := sumnum + (delay_fl[j]*znum_fl[j]);
    end;

    delay_fl[filter_order] := sig;
    sumnum := sumnum + delay_fl[filter_order]*znum_fl[filter_order];

   { if mulidx=0 then sumnum:=sumnum;
    if mulidx=1 then sumnum:=0;
    if mulidx=2 then sumnum:=-sumnum;
    if mulidx=3 then sumnum:=0;  }

    out_sig := round(32000*sumnum);


    write(f, out_sig);
    mulidx:=mulidx+1;
    if mulidx=4 then mulidx:=0;



  end;

  closefile(f);
  //close;
end;



procedure TForm1.Button11Click(Sender: TObject);
const length_of_arrays=20;
var tsnum, tsden, tts1:string;
     inznum:array[0..length_of_arrays] of Extended; //временные, т.к. перевернуть массив надо
     inzden:array[0..length_of_arrays] of Extended;
     znum_fl:array[0..length_of_arrays] of Extended;
     zden_fl:array[0..length_of_arrays] of Extended;
    delay_fl:array[0..length_of_arrays] of Extended;
    filter_order:integer; //порядок фильтра напр 4,
    //тогда количества znum и zden и ячеек в delay будет на 1 больше

    i,j:integer;
    f,fi:file of smallint; in_sig, out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
     mulidx:integer;

    time1, time2:tdatetime;

  function normalNum(s:string):string;
  var ts:string;
  begin
    ts:=s;
    //мудрить не буду
    if ts[1]='.' then ts:='0,'+copy(ts, 2, length(ts)-1); // '.123'
    if ((ts[1]='-') and (ts[2]='.')) then ts:='-0,'+copy(ts, 3, length(ts)-2); // '-.123'
    if ts[2]='.' then ts[2]:=',';
    if ts[3]='.' then ts[3]:=',';
    if ts[4]='.' then ts[4]:=',';
    if ts[5]='.' then ts[5]:=',';
    if ts[6]='.' then ts[6]:=',';
    result:=ts;
  end;
begin
  tsnum:=memo0.Lines.Strings[0];
  tsden:=memo0.Lines.Strings[1];

  tsnum:=copy( tsnum, 8, length(tsnum)-8); //обрезаем лишние данные
  tsden:=copy( tsden, 8, length(tsden)-8);

  filter_order:=0;

  //разбор znum
  memo1.Lines.Add( 'parse of znum' );
  while (pos(',', tsnum)>0) do begin

    tts1:=normalNum( copy(tsnum,1,pos(',', tsnum)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inznum[filter_order]:=strtofloat(tts1);
    delete(tsnum,1,pos(',', tsnum)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsnum );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inznum[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //разбор znum
  filter_order := 0;
  memo1.Lines.Add( 'parse of zden' );
  while (pos(',', tsden)>0) do begin

    tts1:=normalNum( copy(tsden,1,pos(',', tsden)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inzden[filter_order]:=strtofloat(tts1);
    delete(tsden,1,pos(',', tsden)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsden );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inzden[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //отчистка и инициализация
  for i:=0 to length_of_arrays do begin
    znum_fl[i]:=0;
    zden_fl[i]:=0;
    delay_fl[i]:=0;
  end;

  sumden:=0; sumnum:=0; outnum:=0;

  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of znum_fl' );
  for i:=0 to filter_order-1 do begin
    znum_fl[i]:=inznum[filter_order-i-1] ;
    memo1.Lines.Add( 'znum_fl['+inttostr(i)+']='+ floattostr(znum_fl[i]) );
  end;
  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of zden_fl' );
  for i:=1 to filter_order-1 do begin
    zden_fl[i-1]:=inzden[filter_order-i] ;
    memo1.Lines.Add( 'zden_fl['+inttostr(i-1)+']='+ floattostr(zden_fl[i-1]) );
  end;

  filter_order := filter_order - 1; //порядок фильтра на один меньше записей в строке
  memo1.Lines.Add( 'filter_order=' + inttostr(filter_order) );




  assignfile(f, 'd:\outellip.dat');
  assignfile(fi, 'd:\titiyo_-_come_along2.wav');
  rewrite(f);
  reset(fi);

  mulidx:=0;


  time1 := now();


  for i:=0 to 500000 do begin

    sumden:=0; sumnum:=0; outnum:=0;

    //генерим сигнал от -1 до 1
    //sig:=(random(64000)-32000)/32768;
    read(fi, in_sig);
    sig:=(in_sig)/33000;

    if mulidx=0  then sig:=sig*0.0        ;
    if mulidx=1  then sig:=sig*0.195090322;
    if mulidx=2  then sig:=sig*0.382683432;
    if mulidx=3  then sig:=sig*0.555570233;
    if mulidx=4  then sig:=sig*0.707106781;
    if mulidx=5  then sig:=sig*0.831469612;
    if mulidx=6  then sig:=sig*0.923879533 ;
    if mulidx=7  then sig:=sig*0.98078528 ;
    if mulidx=8  then sig:=sig*1.          ;
    if mulidx=9  then sig:=sig*0.98078528  ;
    if mulidx=10 then sig:=sig*0.923879533 ;
    if mulidx=11 then sig:=sig*0.831469612 ;
    if mulidx=12 then sig:=sig*0.707106781 ;
    if mulidx=13 then sig:=sig*0.555570233 ;
    if mulidx=14 then sig:=sig*0.382683432 ;
    if mulidx=15 then sig:=sig*0.195090322 ;
    if mulidx=16 then sig:=sig*0 ;
    if mulidx=17 then sig:=sig*-0.195090322;
    if mulidx=18 then sig:=sig*-0.382683432;
    if mulidx=19 then sig:=sig*-0.555570233;
    if mulidx=20 then sig:=sig*-0.707106781;
    if mulidx=21  then sig:=sig*-0.831469612;
    if mulidx=22  then sig:=sig*-0.923879533;
    if mulidx=23  then sig:=sig*-0.98078528 ;
    if mulidx=24  then sig:=sig*-1          ;
    if mulidx=25  then sig:=sig*-0.98078528 ;
    if mulidx=26  then sig:=sig*-0.923879533;
    if mulidx=27  then sig:=sig*-0.831469612;
    if mulidx=28  then sig:=sig*-0.707106781;
    if mulidx=29  then sig:=sig*-0.555570233;
    if mulidx=30  then sig:=sig*-0.382683432;
    if mulidx=31  then sig:=sig*-0.195090322;

    //продвинем массив delay
    for j:=0 to filter_order-1 do begin
      delay_fl[j]:=delay_fl[j+1];
      sumden := sumden + (delay_fl[j]*zden_fl[j]);
      sumnum := sumnum + (delay_fl[j]*znum_fl[j]);
    end;

    delay_fl[filter_order] := sig-sumden;
    sumnum := sumnum + delay_fl[filter_order]*znum_fl[filter_order];

    { //48000 / 16 перенос на 3000 Гц
    if mulidx=0 then sumnum:=sumnum*0;//(0);
    if mulidx=1 then sumnum:=sumnum*0.3826834323650897717284599840304;//(22.5);
    if mulidx=2 then sumnum:=sumnum*0.70710678118654752440084436210485;//(45);
    if mulidx=3 then sumnum:=sumnum*0.92387953251128675612818318939679;//(67.5);
    if mulidx=4 then sumnum:=sumnum*1;//(90);
    if mulidx=5 then sumnum:=sumnum*0.92387953251128675612818318939679;//(112.5);
    if mulidx=6 then sumnum:=sumnum*0.70710678118654752440084436210485;//(135);
    if mulidx=7 then sumnum:=sumnum*0.3826834323650897717284599840304;//(157.5);
    if mulidx=8 then sumnum:=sumnum*0;//(180);
    if mulidx=9 then sumnum:=sumnum*-0.3826834323650897717284599840304;//(202.5);
    if mulidx=10 then sumnum:=sumnum*-0.70710678118654752440084436210485;//(225);
    if mulidx=11 then sumnum:=sumnum*-0.92387953251128675612818318939679;//(247.5);
    if mulidx=12 then sumnum:=sumnum*-1;//(270);
    if mulidx=13 then sumnum:=sumnum*-0.92387953251128675612818318939679;//(292.5);
    if mulidx=14 then sumnum:=sumnum*-0.70710678118654752440084436210485;//(315);
    if mulidx=15 then sumnum:=sumnum*-0.3826834323650897717284599840304;//(337.5);
     }


   { //48000 / 32 переренос на 1500 Гц
    if mulidx=0  then sumnum:=sumnum*0.0        ;
    if mulidx=1  then sumnum:=sumnum*0.195090322;
    if mulidx=2  then sumnum:=sumnum*0.382683432;
    if mulidx=3  then sumnum:=sumnum*0.555570233;
    if mulidx=4  then sumnum:=sumnum*0.707106781;
    if mulidx=5  then sumnum:=sumnum*0.831469612;
    if mulidx=6  then sumnum:=sumnum*0.923879533 ;
    if mulidx=7  then sumnum:=sumnum*0.98078528 ;
    if mulidx=8  then sumnum:=sumnum*1.          ;
    if mulidx=9  then sumnum:=sumnum*0.98078528  ;
    if mulidx=10 then sumnum:=sumnum*0.923879533 ;
    if mulidx=11 then sumnum:=sumnum*0.831469612 ;
    if mulidx=12 then sumnum:=sumnum*0.707106781 ;
    if mulidx=13 then sumnum:=sumnum*0.555570233 ;
    if mulidx=14 then sumnum:=sumnum*0.382683432 ;
    if mulidx=15 then sumnum:=sumnum*0.195090322 ;
    if mulidx=16 then sumnum:=sumnum*0 ;
    if mulidx=17 then sumnum:=sumnum*-0.195090322;
    if mulidx=18 then sumnum:=sumnum*-0.382683432;
    if mulidx=19 then sumnum:=sumnum*-0.555570233;
    if mulidx=20 then sumnum:=sumnum*-0.707106781;
    if mulidx=21  then sumnum:=sumnum*-0.831469612;
    if mulidx=22  then sumnum:=sumnum*-0.923879533;
    if mulidx=23  then sumnum:=sumnum*-0.98078528 ;
    if mulidx=24  then sumnum:=sumnum*-1          ;
    if mulidx=25  then sumnum:=sumnum*-0.98078528 ;
    if mulidx=26  then sumnum:=sumnum*-0.923879533;
    if mulidx=27  then sumnum:=sumnum*-0.831469612;
    if mulidx=28  then sumnum:=sumnum*-0.707106781;
    if mulidx=29  then sumnum:=sumnum*-0.555570233;
    if mulidx=30  then sumnum:=sumnum*-0.382683432;
    if mulidx=31  then sumnum:=sumnum*-0.195090322;     }


    out_sig := round(32000*sumnum);

    write(f, out_sig);

    mulidx:=mulidx+1;

    if mulidx=32 then mulidx:=0;

  end;

  time2 := now();

  memo1.Lines.Add( datetimetostr(time2-time1) );

  closefile(f);
  closefile(fi);
  //close;
end;


procedure TForm1.Button12Click(Sender: TObject);
const length_of_arrays=20;
var tsnum, tsden, tts1:string;
     inznum:array[0..length_of_arrays] of Extended; //временные, т.к. перевернуть массив надо
     inzden:array[0..length_of_arrays] of Extended;
     znum_fl:array[0..length_of_arrays] of Extended;
     zden_fl:array[0..length_of_arrays] of Extended;
    delay_fl:array[0..length_of_arrays] of Extended;
    filter_order:integer; //порядок фильтра напр 4,
    //тогда количества znum и zden и ячеек в delay будет на 1 больше

    i,j:integer;
    f,fi:file of smallint; in_sig, out_sig:smallint;
    sig, sumden, sumnum, outnum:extended;
     mulidx:integer;

  function normalNum(s:string):string;
  var ts:string;
  begin
    ts:=s;
    //мудрить не буду
    if ts[1]='.' then ts:='0,'+copy(ts, 2, length(ts)-1); // '.123'
    if ((ts[1]='-') and (ts[2]='.')) then ts:='-0,'+copy(ts, 3, length(ts)-2); // '-.123'
    if ts[2]='.' then ts[2]:=',';
    if ts[3]='.' then ts[3]:=',';
    if ts[4]='.' then ts[4]:=',';
    if ts[5]='.' then ts[5]:=',';
    if ts[6]='.' then ts[6]:=',';
    result:=ts;
  end;
begin
  tsnum:=memo0.Lines.Strings[0];
  tsden:=memo0.Lines.Strings[1];

  tsnum:=copy( tsnum, 8, length(tsnum)-8); //обрезаем лишние данные
  tsden:=copy( tsden, 8, length(tsden)-8);

  filter_order:=0;

  //разбор znum
  memo1.Lines.Add( 'parse of znum' );
  while (pos(',', tsnum)>0) do begin

    tts1:=normalNum( copy(tsnum,1,pos(',', tsnum)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inznum[filter_order]:=strtofloat(tts1);
    delete(tsnum,1,pos(',', tsnum)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsnum );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inznum[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //разбор znum
  filter_order := 0;
  memo1.Lines.Add( 'parse of zden' );
  while (pos(',', tsden)>0) do begin

    tts1:=normalNum( copy(tsden,1,pos(',', tsden)-1) );

    memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+ tts1 + ' = ' + floattostr(strtofloat(tts1)) );
    inzden[filter_order]:=strtofloat(tts1);
    delete(tsden,1,pos(',', tsden)+1);
    //tsnum:=
    filter_order := filter_order + 1;
  end;

  tts1:=normalNum( tsden );
  memo1.Lines.Add( 'znum['+inttostr(filter_order)+']='+tts1 + ' = ' + floattostr(strtofloat(tts1)) );
  inzden[filter_order]:=strtofloat(tts1);
  filter_order := filter_order + 1;

  //отчистка и инициализация
  for i:=0 to length_of_arrays do begin
    znum_fl[i]:=0;
    zden_fl[i]:=0;
    delay_fl[i]:=0;
  end;

  sumden:=0; sumnum:=0; outnum:=0;

  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of znum_fl' );
  for i:=0 to filter_order-1 do begin
    znum_fl[i]:=inznum[filter_order-i-1] ;
    memo1.Lines.Add( 'znum_fl['+inttostr(i)+']='+ floattostr(znum_fl[i]) );
  end;
  //переворачиваем данные в массивах
  memo1.Lines.Add( 'reverse of zden_fl' );
  for i:=1 to filter_order-1 do begin
    zden_fl[i-1]:=inzden[filter_order-i] ;
    memo1.Lines.Add( 'zden_fl['+inttostr(i-1)+']='+ floattostr(zden_fl[i-1]) );
  end;

  filter_order := filter_order - 1; //порядок фильтра на один меньше записей в строке
  memo1.Lines.Add( 'filter_order=' + inttostr(filter_order) );




  assignfile(f, 'd:\outellip.dat');
  assignfile(fi, 'E:\Qt\Projects\DSProcessor\Signals\IQ96 UT4ZA LSB 7,086001MHz 06.10.2006 19 09 56.wav');
  rewrite(f);
  reset(fi);

  mulidx:=0;

  while not (eof(fi)) do begin

    sumden:=0; sumnum:=0; outnum:=0;

    //генерим сигнал от -1 до 1
    //sig:=(random(64000)-32000)/32768;
    read(fi, in_sig);
    sig:=(in_sig)/33000;

    //продвинем массив delay
    for j:=0 to filter_order-1 do begin
      delay_fl[j]:=delay_fl[j+1];
      sumden := sumden + (delay_fl[j]*zden_fl[j]);
      sumnum := sumnum + (delay_fl[j]*znum_fl[j]);
    end;

    delay_fl[filter_order] := sig-sumden;
    sumnum := sumnum + delay_fl[filter_order]*znum_fl[filter_order];


    out_sig := round(32000*sumnum);

    write(f, out_sig);

  end;

  closefile(f);
  closefile(fi);
  //close;
end;


end.


