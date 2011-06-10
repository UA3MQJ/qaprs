unit TXMain;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, ComCtrls, Buttons, mmsystem, math,  IniFiles, Menus,
  LPTIO, Cmp_LedDisplay, Cmp_SHSMixer,  PSKCodec, mmErrMsg,
  Hilbert, FIRFilter, Filtres, DateUtils, FFTasm,
  Generators, PHDConst, MorseCode, Resample;

const
     Buffers            = 4;
     Channels           = 1;
     SamplesPerSec      = 24000;
     BufferLengthInt    = 1024;
     BufferLengthByte   = BufferLengthInt * 2;

type
    PBuffer = ^TBuffer;
    TBuffer = array of SmallInt;

type
  TMainModemForm = class(TForm)
    Mixer: TSHSMixer;
    StatusBar: TStatusBar;
    MainMenu: TMainMenu;
    mnuOptions: TMenuItem;
    N2: TMenuItem;
    mnuProtocol: TMenuItem;
    mnuSettings: TMenuItem;
    mnuVasja10: TMenuItem;
    mnuMorse: TMenuItem;
    N9: TMenuItem;
    mnuWaterfall: TMenuItem;
    PanelSpectr: TPanel;
    MainPanel: TPanel;
    PanelDebug: TPanel;
    MemosPanel: TPanel;
    MemoSMSFrom: TRichEdit;
    N10: TMenuItem;
    mnuVasja11: TMenuItem;
    SMSPopupMenu: TPopupMenu;
    pmnuTransfer: TMenuItem;
    pmnuTransferF9: TMenuItem;
    pbss: TPaintBox;
    Splitter1: TSplitter;
    RulerPaintBox: TPaintBox;
    Splitter2: TSplitter;
    SpctrPopupMenu: TPopupMenu;
    pmnu06kHz: TMenuItem;
    pmnu12kHz: TMenuItem;
    MemoSMS: TMemo;
    ControlPanel: TPanel;
    btnPTT: TSpeedButton;
    BtnTX: TSpeedButton;
    Panel2: TPanel;
    BtnTestTone: TSpeedButton;
    pmnuClear: TMenuItem;
    Panel1: TPanel;
    imgFrs: TImage;
    imgSync: TImage;
    LabelRX: TLabel;
    LabelTX: TLabel;
    VectorPaintBox: TPaintBox;
    LabelSBVol: TLabel;
    LabelPTTControl: TLabel;
    LabelProtoName: TLabel;
    LedDisplay: TLedDisplay;
    ProgressBar: TProgressBar;
    Panel3: TPanel;
    ImageSnd: TImage;
    tbMain: TTrackBar;
    N8: TMenuItem;
    pmnuMarker: TMenuItem;
    mnuLanguage: TMenuItem;
    mnuRussian: TMenuItem;
    mnuEnglish: TMenuItem;
    N11: TMenuItem;
    pmnuPaste: TMenuItem;
    N13: TMenuItem;
    N14: TMenuItem;
    pmnu03kHz: TMenuItem;
    sbServer: TSpeedButton;
    SbClearTx: TSpeedButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure FormShow(Sender: TObject);
    procedure tbMainChange(Sender: TObject);
    procedure MixerControlChange(Sender: TObject);
    procedure btnPTTMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure btnPTTMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure BtnTXClick(Sender: TObject);
    procedure SbClearTxClick(Sender: TObject);
    procedure mnuVasja10Click(Sender: TObject);
    procedure mnuSettingsClick(Sender: TObject);
    procedure pbssPaint(Sender: TObject);
    procedure mnuWaterfallClick(Sender: TObject);
    procedure RulerPaintBoxPaint(Sender: TObject);
    procedure pmnu06kHzClick(Sender: TObject);
    procedure pmnu12kHzClick(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure PanelSpectrResize(Sender: TObject);
    procedure Splitter2CanResize(Sender: TObject; var NewSize: Integer;
      var Accept: Boolean);
    procedure VectorPaintBoxPaint(Sender: TObject);
    procedure Splitter1Moved(Sender: TObject);
    procedure Splitter1CanResize(Sender: TObject; var NewSize: Integer;
      var Accept: Boolean);
    procedure BtnTestToneClick(Sender: TObject);
    procedure pmnuClearClick(Sender: TObject);
    procedure ImageSndMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure mnuRussianClick(Sender: TObject);
    procedure mnuEnglishClick(Sender: TObject);
    procedure pmnuPasteClick(Sender: TObject);
    procedure pmnu03kHzClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure sbServerClick(Sender: TObject);

    private
         { Private declarations }
         LPT               :  TLptPortConnection;
         LPTAvailability   :  Boolean;
         CurrentPort       :  Byte;
         WaveInHeader      :  array [0..Buffers-1] of TWaveHdr;
         WaveInBuffers     :  array [0..Buffers-1] of array [0..BufferLengthInt] of SmallInt;
         WaveOutHeader     :  array[0..Buffers-1] of TWaveHdr;
         WaveOutBuffers    :  array [0..Buffers-1] of array [0..BufferLengthInt] of SmallInt;
         tmpwoa1           :  array [0..BufferLengthInt] of Double;
         tmpwoa2           :  array [0..BufferLengthInt] of Double;
         tmpwia1           :  array [0..BufferLengthInt] of Double;
         tmpwia2           :  array [0..BufferLengthInt] of Double;
         oComplexArray     : TComplexArray;
         iComplexArray     : TComplexArray;
         MixerGet          :  Boolean;
         CloseOutCnt       :  Longint;
         Err               :  Longint;
         aData             :  array [0.. BufferLengthInt] of SmallInt;
         pData             :  array [0.. BufferLengthInt] of SmallInt;
         SyncData          :  array [0.. BufferLengthInt] of SmallInt;
         sDataCounter      :  Longint;
         CloseWInCommand   :  Boolean;
         WavInCloseCount   :  Longint;
         WavInOpenCount    :  Longint;
         WavOutOpened      :  Boolean;
         BPFMorse          :  TFIRFilter;
         BPFOut            :  TFIRFilter;
         GenSinusS         :  TGenSin;
         WOutBuffer        :  ByteArray;
         WOutBufPos        :  Int64;
         WOutBufCount      :  Int64;
         MaxWaveSize       :  Int64;
         PhaseOld          :  Byte;
         FadeIn            :  Double;
         FadeOut           :  Double;
         Spectra           :  TBitmap;
         bImage            :  TBitmap;
         lImage            :  TBitmap;
         OldVc             :  Longint;
         VcBool            :  Boolean;
         LeftScrollPoint   :  Longint;
         OldVol            :  Longint;
         Invertor1         :  Longint;
         Invertor2         :  Longint;
         hpf,lpf           :  Longint;
         MarkTrig          :  Byte;

         procedure SetLanguage;
         function Pin1 : boolean;
         function Pin3 : boolean;
         function Pin7 : boolean;
         function Pin16 : boolean;
         procedure StartWavOut;
         procedure CreateFilters;
         procedure DestroyFilters;

  public
        { Public declarations }
        WaveFormat        :  TWaveFormatEx;
        WaveIn,WaveIn2    :  HWaveIn;
        WavInOpened       :  Boolean;
        WaveOut           :  HWaveOut;
        ProtcolNum        :  Byte;
        InDeviceId        :  Longint;
        OutDeviceId       :  Longint;
        MixerOpened       :  Boolean;
        WtfRange          :  Longint;
        PTTControlId      :  Byte;
        TerminalPort      :  Byte;
        PTTControl        :  Longint;
        BaseFrq           :  Longint;
        isPTT             :  Boolean;
        DisableTx         :  Boolean;
        CloseWOutCommand  :  Boolean;
        DecodeNow         :  Boolean;
        WTCDelay          :  Longint;

        procedure WriteToPort(Mode: Boolean);
        procedure StartStopWaveIn(Start: Boolean);
        procedure ResetReciver;
        procedure DecodeHexData(HexData: String);
        procedure SetProtcolLanguage;
        procedure EnableTXButtons;
        procedure SetProtcolNum;

  protected
           procedure WndProc(var Message: TMessage); override;
  end;

var
  MainModemForm: TMainModemForm;

  
implementation

uses Settings;

{$R *.dfm}

procedure TMainModemForm.WndProc(var Message: TMessage);
var  FreeHeader  :  pWaveHdr;
     i,j,k,Vc    :  Longint;
     fByte       :  Byte;
     s           :  Double;
     tmps        :  String;
     r, g, b     :  Byte;
     fftm        :  Double;
     iFFTOut     :  array[0..BufferLengthInt] of Double;

BEGIN


IF MESSAGE.MSG = WM_USER+1 then
Begin
     BtnTX.Click;
End;

//================================================================================================= WAV IN

IF MESSAGE.MSG = MM_WIM_OPEN then  //OPEN WAV IN
Begin
     Inc(WavInOpenCount);
     if WavInOpenCount =  1 then 
     begin
          dF:= 0.0;
          ResetReciver;
          WavInOpened:= True;
          WavInOpenCount:= 0;
          LabelRX.Font.Color:= clLime;
     end;
     
     for i:=0 to BufferLengthInt-1 do
     begin
          tmpwia1[i]:= 0.0;
          tmpwia2[i]:= 0.0;
          SyncData[i]:= 0;
          iFFTOut[i]:= 0.0;
     end;
End;


IF MESSAGE.MSG = MM_WIM_DATA then //DATA WAV IN
Begin
     FreeHeader := pWaveHdr(Message.lParam);

     if not CloseWInCommand then 
     begin
          CopyMemory(@aData, FreeHeader.lpData, FreeHeader.dwBytesRecorded);

          if (SynFound) or (CanDrawVector) then vBitmap.Canvas.FillRect(vBitmap.Canvas.ClipRect);

          Vc:=0;

          for i:=0 to BufferLengthInt-1 do
          begin
               if Vc < abs(aData[i]) then  Vc:= abs(aData[i]);
               tmpwia1[i]:= tmpwia2[i];
               tmpwia2[i]:= aData[i];
               iComplexArray[i].re:= tmpwia1[i]*WindowingArr[i];
               iComplexArray[i+BufferLengthInt].re:= tmpwia2[i]*WindowingArr[i+BufferLengthInt];
               iComplexArray[i].im:= 0.0;
               iComplexArray[i+BufferLengthInt].im:= 0.0;
          end; 

          dspDoFFT(@iComplexArray, BufferLengthInt * 2, false,false,true); 

          for i := 0 to BufferLengthInt -1 do
          iFFTOut[i]:= (  abs(iComplexArray[i].re) + abs(iComplexArray[i].im)  );

          for i:= BufferLengthInt * 2 - hpf to BufferLengthInt * 2 -1 do
          begin
               iComplexArray[i].re:=0;
               iComplexArray[i].im:=0;
          end;

          for i:= lpf to BufferLengthInt * 2-lpf do
          begin
               iComplexArray[i].re:=0;
               iComplexArray[i].im:=0;
          end;

          for i:= 0 to hpf do
          begin
               iComplexArray[i].re:=0;
               iComplexArray[i].im:=0;
          end;

          dspDoFFT(@iComplexArray,BufferLengthInt * 2,true,true,true);

          for i := 0 to BufferLengthInt -1 do
          begin
               S:= iComplexArray[i + BufferLengthInt div 2].re /WindowingArr[i + BufferLengthInt div 2];
               DecodeVector(S, ProtcolNum);
          end;

          if (SynFound) or (CanDrawVector) then VectorPaintBoxPaint(nil);

          if VcBool then
          begin
               if Vc > OldVc then
               LedDisplay.Position:= Vc
               else LedDisplay.Position:= OldVc;
          end else OldVc:= Vc;
          VcBool:= not VcBool;


          if (PanelSpectr.Visible) and (not DisableTx) then
          begin
               BitBlt(Spectra.canvas.Handle,0,0,Spectra.Width,Spectra.Height,Spectra.canvas.Handle, 1,0,SRCCOPY);
               bImage.Height:= BufferLengthInt div 10;
               k:=0;
               if WtfRange = 0 then begin k:= BufferLengthInt div 4;  bImage.Height:= k end; //0..3000
               if WtfRange = 1 then begin k:= BufferLengthInt div 2;  bImage.Height:= k end; //0..6000
               if WtfRange = 2 then begin k:= BufferLengthInt-1;      bImage.Height:= k end; //0..12000

               for i:= 0 to k do
               begin
                    fftm:= (Log10(iFFTOut[i]/2000 + 1)*120) * RXPerception;
                    if fftm > 32767 then  fftm:=32767;
                    DoubleToRGB(fftm, r, g, b);
                    bImage.Canvas.Pixels[0,(i-bImage.Height)*-1]:=RGB(r, g, b);
               end;

               Strecth(bImage, lImage, ResampleFilters[2].Filter, ResampleFilters[2].Width); //2=Triangle
               if pmnuMarker.Checked then
               begin
                    Inc(MarkTrig);
                    if MarkTrig > 3 then i:= 255 else i:= 0;
                    if BaseFrq = 2400 then
                    begin
                         if WtfRange = 0 then lImage.Canvas.Pixels[0, Round(lImage.Height / 4.97)]:=RGB(i, i, i);
                         if WtfRange = 1 then lImage.Canvas.Pixels[0, Round(lImage.Height / 1.666)]:=RGB(i, i, i);
                         if WtfRange = 2 then lImage.Canvas.Pixels[0, Round(lImage.Height / 1.25)]:=RGB(i, i, i);
                    end;
                    if BaseFrq = 1500 then
                    begin
                         if WtfRange = 0 then lImage.Canvas.Pixels[0, lImage.Height div 2+ 2]:=RGB(i,i,i);
                         if WtfRange = 1 then lImage.Canvas.Pixels[0, lImage.Height -(lImage.Height div 4)]:=RGB(i, i, i);
                         if WtfRange = 2 then lImage.Canvas.Pixels[0, lImage.Height -(lImage.Height div 8)]:=RGB(i, i, i);
                    end;
                    if MarkTrig > 6 then MarkTrig:= 0;
               end;
               Spectra.Canvas.Draw(Spectra.Width-1,0, lImage);
               pbssPaint(nil);
          end;

          if not SynFound then
          begin
               tmps:= StrTran(HexSynString, '012345', '$', True);

               if Length(HexSynString) > 12 then HexSynString:= '';
               i:= pos('$',tmps);
               if i <> 0 then
               begin
                    ResetReciver;
                    SynFound:= True;
                    BadData:= 0;
                    HexSynString:= ''; 
                    HexDataString:= '';
               end;
          end;

          if SynFound and not EndFound then
          begin
               imgSync.Visible:= True;
               tmps:= StrTran(HexSynString, '###', '$', True);
               i:= pos('$',tmps);
               if i<>0 then EndFound:= True;
               if Length(HexSynString) > 3 then HexSynString:= '';

               if BadData > 20 then ResetReciver;  // Более 20 ошибок
          end;
          if EndFound then DecodeHexData(HexDataString);
     end;

     if MaxDataCounter > (MaxDataSize + 144000) then  ResetReciver;
     if BadData > 50 then ResetReciver; 

     if WaveIn <> 0 then
     begin
          Err:= waveInAddBuffer(WaveIn, FreeHeader,sizeof(TWaveHdr));
          if Err<>0 then MessageDlg (translate_mm_error (Err), mtWarning, [mbOK], 0);
     end;
End;

IF MESSAGE.MSG = MM_WIM_CLOSE then  //CLOSE WAV IN
Begin
     Inc(WavInCloseCount);
     if WavInCloseCount =  2 then
     begin
          WavInOpened:= False;
          WavInCloseCount:= 0;
          CloseWInCommand:= False;
          LabelRX.Font.Color:= clGray;
          LedDisplay.Position:= 0;
          ResetReciver;
     end;
End;

//================================================================================================ WAV OUT

IF MESSAGE.MSG = MM_WOM_OPEN then //OPEN WAV OUT
Begin
     BtnTX.Enabled:= False;
     for i:=0 to BufferLengthInt-1 do
     begin
          tmpwoa1[i]:= 0.0;
          tmpwoa2[i]:= 0.0;
     end;

     PhaseOld   := 0;
     FadeIn     := 50;
     FadeOut    := 1.0;
     Invertor1  := 0;
     Invertor2  := 0;

     MaxWaveSize:= Length(WOutBuffer);
     WOutBufCount:= MaxWaveSize;
     WOutBufPos:= 0;

     if ProtcolNum <> 64 then GenSinusS  :=  TGenSin.Create(-180, BaseFrq, 32767,  SamplesPerSec);

     if ProtcolNum = 64  then GenSinusS  :=  TGenSin.Create(-180, 1000, 32767*190,  SamplesPerSec); //Morse

     if ProtcolNum = 255 then WOutBufCount:= 1;

     for i:=0 to Buffers-1 do
     begin
          for j := 0 to BufferLengthInt -1 do WaveOutBuffers[i][j]:= 0;

          with WaveOutHeader[i] do
          begin
               lpdata:= @WaveOutBuffers[i];
               dwBufferlength:= BufferLengthByte;
               dwBytesrecorded:= 0;
               dwFlags:= 0;
               dwLoops:= 0;
               lpNext:=  nil;
               reserved:=  0;
          end;

          if WaveOutPrepareHeader (WaveOut, @WaveOutHeader[i], SizeOf (TWaveHdr))<>0 then
          application.messagebox('WaveOutPrepareHeader', 'Error', mb_OK);
     end;

     for i:=0 to Buffers-1 do
     begin
          if WaveOutWrite (WaveOut, @WaveOutHeader[i], SizeOf (TWaveHdr))<>0 then
          application.messagebox('WaveOutWrite', 'Error', mb_OK);
     end;
End;


IF MESSAGE.MSG = MM_WOM_DONE then //WAV OUT
Begin
     FreeHeader := pWaveHdr (Message.lParam);
     if CloseWOutCommand then
     begin
          if WaveOut<>0 then WaveOutReset (WaveOut);
          if WaveOut<>0 then WaveOutClose (WaveOut);
          WaveOut:= 0;
          EXIT;
     end;
          s:= 0.0;
          if  (WavOutOpened) and (not CloseWOutCommand) and (WOutBufCount > 0) then
          begin
               for i := 0 to BufferLengthInt -1 do
               begin
                    fByte:= 0;
                    if (ProtcolNum <> 255) then
                    begin
                         if (WOutBufPos < MaxWaveSize) then fByte:= WOutBuffer[WOutBufPos];
                         Inc(WOutBufPos);
                    end;

                    if (ProtcolNum = 0) or (ProtcolNum = 1)then
                    begin
                         s:= GenSinusS.Exec;

                         PhaseOld:= fByte;

                         if fByte = 0 then s:= -s;

                         if WOutBufPos < 1024 then
                         begin
                              FadeIn:= FadeIn - ( 0.0038175 * FadeIn );
                              s:= s / FadeIn;
                         end;

                         if WOutBufCount < 1024 then
                         begin
                              FadeOut:= FadeOut + ( 0.0038175 * FadeOut );
                              s:= s / FadeOut;
                         end;
                    end;

                    if (ProtcolNum = 64) then
                    begin
                         if (fByte = 1) and (WOutBufPos >= 0) then  s:= GenSinusS.Exec;
                         s:= BPFMorse.Exec(s);
                    end;

                    if (ProtcolNum = 255) then
                    begin
                         s:= GenSinusS.Exec;
                         if not mnuMorse.Checked then
                         begin
                              Inc(Invertor1);
                              Inc(Invertor2);
                              if Invertor2 >= 150000 then Invertor2:= 0;

                              if Invertor2 < 100000 then
                              begin
                                   Inc(Invertor2);
                                   if BaseFrq = 1500 then
                                   begin
                                        if Invertor1 <= 40 then s:= -s;
                                        if Invertor1 >= 80 then Invertor1:= 0;
                                   end;
                                   if BaseFrq = 2400 then
                                   begin
                                        if Invertor1 <= 20 then s:= -s;
                                        if Invertor1 >= 40 then Invertor1:= 0;
                                   end;
                              end;
                         end;
                    end else Dec(WOutBufCount);

                    if ProtcolNum = 64 then  pData[i]:= Round(S) else
                    begin
                         tmpwoa1[i]:= tmpwoa2[i];
                         tmpwoa2[i]:= s;

                         oComplexArray[i].re:= tmpwoa1[i] * WindowingArr[i];
                         oComplexArray[i+BufferLengthInt].re:= tmpwoa2[i] * WindowingArr[i+BufferLengthInt];
                         oComplexArray[i].im:= 0.0;
                         oComplexArray[i+BufferLengthInt].im:= 0.0;
                    end;
               end;

               ProgressBar.Position:= WOutBufPos;

               if ProtcolNum <> 64 then
               begin
                    dspDoFFT(@oComplexArray,BufferLengthInt * 2,false,false,true);

                    for i:= BufferLengthInt * 2 - hpf to BufferLengthInt * 2 -1 do
                    begin
                         oComplexArray[i].re:=0;
                         oComplexArray[i].im:=0;
                    end;

                    for i:= lpf to BufferLengthInt * 2-lpf do
                    begin
                         oComplexArray[i].re:=0;
                         oComplexArray[i].im:=0;
                    end;

                    for i:= 0 to hpf do
                    begin
                         oComplexArray[i].re:=0;
                         oComplexArray[i].im:=0;
                    end;

                    dspDoFFT(@oComplexArray,BufferLengthInt * 2,true,true,true);
                    for i := 0 to BufferLengthInt -1 do
                    begin
                         if BaseFrq = 1500 then
                         S:= BPFOut.Exec(oComplexArray[i + BufferLengthInt div 2].re / WindowingArr[i + BufferLengthInt div 2])*2.59;
                         if BaseFrq = 2400 then
                         S:= BPFOut.Exec(oComplexArray[i + BufferLengthInt div 2].re / WindowingArr[i + BufferLengthInt div 2])*2.15;
                         pData[i]:= Round(S);
                    end;
               end;

               CopyMemory(FreeHeader.lpData, @pData,  BufferLengthByte);
               if WaveOutWrite (WaveOut, FreeHeader, SizeOf (TWaveHdr))<>0 then
               application.messagebox('WaveOutWrite.', 'Error', mb_OK);
          end else
          begin
               Inc(CloseOutCnt);

               if CloseOutCnt < 5 then
               begin
                    for i := 0 to BufferLengthInt -1 do pData[i]:=0;
                    CopyMemory(FreeHeader.lpData, @pData,  BufferLengthByte);
                    if WaveOutWrite (WaveOut, FreeHeader, SizeOf (TWaveHdr))<>0 then
                    application.messagebox('WaveOutWrite.', 'Error', mb_OK);
               end;

               if CloseOutCnt >= 5 then CloseWOutCommand:= True;
          end;
End;


IF MESSAGE.MSG = MM_WOM_CLOSE then //WAV OUT
Begin
     WOutBufCount:= 0;
     CloseOutCnt:= 0;
     GenSinusS.Free;

     CloseWOutCommand:= False;
     begin
          WriteToPort(False);
          isPTT:= False;
     end;

     ProgressBar.Position:= 0;
     WavOutOpened:= False;
     BtnTX.Enabled:= True;
     DisableTx:= False;
End;


     INHERITED WndProc(MESSAGE);
END;

function TMainModemForm.Pin1 : boolean ;
var d : boolean;
begin
     d := true;
     d := d xor (STROBE  = (STROBE  and Lpt.ReadPort( CurrentPort,2))) ;
     Pin1 := d;
end;

function TMainModemForm.Pin3 : boolean ;
var d : boolean;
begin
     d := true;
     d := d and (2 = (2 and Lpt.ReadPort( CurrentPort ,0))) ;
     Pin3 := d;
end;

function TMainModemForm.Pin7 : boolean ;
var d : boolean;
begin
     d := true;
     d := d and (32 = (32 and Lpt.ReadPort( CurrentPort,0))) ;
     Pin7 := d;
end;

function TMainModemForm.Pin16 : boolean ;
var d : boolean;
begin
     d := true;
     d := d and (INIT  = (INIT  and Lpt.ReadPort( CurrentPort,2))) ;
     Pin16 := d;
end;


procedure TMainModemForm.FormCreate(Sender: TObject);
begin
     Constraints.MaxHeight:= Screen.Height;
     Constraints.MaxWidth:= Screen.Width;
     PanelSpectr.Constraints.MaxWidth:= Screen.Width-MainPanel.Constraints.MinWidth-4;

     SetTimer(Handle, 10001, 100, nil);

     FIniFile := TIniFile.Create(ExtractFilePath(Application.ExeName)+'VModem.ini');

     Language:= FIniFile.ReadInteger('RadioModem', 'Language', 1);

     if Language = 0 then mnuRussian.Checked:= True;
     if Language = 0 then mnuEnglish.Checked:= False;
     if Language = 1 then mnuEnglish.Checked:= True;
     if Language = 1 then mnuRussian.Checked:= False;

     ProtcolNum:= FIniFile.ReadInteger('RadioModem', 'Protcol', 0);
     BaseFrq:= 1500;
     if ProtcolNum = 0 then BaseFrq:= 1500;
     if ProtcolNum = 1 then BaseFrq:= 2400;
     if ProtcolNum = 64 then BaseFrq:= 1000;

     CreateFilters;
     ResetReciver;
     
     sDataCounter       := 0;
     WavInOpened        := False;
     CloseWInCommand    := False;
     WavInOpenCount     := 0;
     WavInCloseCount    := 0;
     CloseOutCnt        := 0;
     WavOutOpened       := False;
     CloseWOutCommand   := False;
     isPTT              := False;
     WTCDelay           := 0;

     InitWindowing(BufferLengthInt*2);

     with WaveFormat do
     begin
          wFormatTag      := WAVE_FORMAT_PCM;
          wBitsPerSample  := 16;
          nChannels       := Channels;
          nSamplesPerSec  := SamplesPerSec;
          nBlockAlign     := (nChannels * wBitsPerSample) div 8;
          nAvgBytesPerSec := (nChannels * nSamplesPerSec * wBitsPerSample) div 8;
          cbSize          := 0;
     end;

     Spectra                   := TBitmap.create;
     Spectra.Canvas.Font.Color := clLime;
     Spectra.PixelFormat       := pf16bit;
     Spectra.width             := Screen.Width - MainPanel.Constraints.MinWidth - RulerPaintBox.Width;
     Spectra.height            := Screen.Height;
     Spectra.Canvas.Brush.Color:= clBlack;
     Spectra.Canvas.FillRect(Rect(0, 0, Spectra.Width, Spectra.Height));
     bImage                  := TBitmap.create;
     bImage.PixelFormat      := pf16bit;
     bImage.Width            := 1;
     bImage.Height           := BufferLengthInt div 2;
     lImage                  := TBitmap.create;
     lImage.PixelFormat      := pf16bit;
     lImage.Width            := 1;
     lImage.Height           := Spectra.height;
     vBitmap                 := TBitmap.create;
     vBitmap.PixelFormat     := pf16bit;
     vBitmap.width           := 64;
     vBitmap.height          := 64;
     vBitmap.Canvas.Brush.Color := clBlack;
     vBitmap.Canvas.FillRect(vBitmap.Canvas.ClipRect);

     WaveIn:=  0;
     WaveIn2:= 0;
     WaveOut:= 0;

     PanelSpectr.Width:= FIniFile.ReadInteger('RadioModem', 'SpectrWidth', 204);
     if PanelSpectr.Width < RulerPaintBox.Width then PanelSpectr.Width:= RulerPaintBox.Width + 4;

     mnuWaterfall.Checked:= FIniFile.ReadBool('RadioModem', 'Waterfall', False);
     Splitter1.Visible:= mnuWaterfall.Checked;
     PanelSpectr.Visible:= mnuWaterfall.Checked;

     Left:= FIniFile.ReadInteger('RadioModem', 'Left', 0);
     Top:= FIniFile.ReadInteger('RadioModem', 'Top', 0);
     Width:= FIniFile.ReadInteger('RadioModem', 'Width', 0);
     Height:= FIniFile.ReadInteger('RadioModem', 'Height', 0);

     if (Top > Screen.Height - Height div 2) or (Top < 0) or (Left > Screen.Width - Width)  or (Left < -250) then Position:= poScreenCenter;
end;


procedure TMainModemForm.SetLanguage;
begin
     if Language = 0 then
     begin
          mnuOptions.Caption:= 'Опции';
          mnuProtocol.Caption:= 'Протокол';
          mnuSettings.Caption:= 'Настройки...';
          mnuWaterfall.Caption:= 'Спектр...';
          mnuVasja10.Caption:= 'Шумный канал';
          mnuVasja11.Caption:= 'Чистый канал';
          mnuMorse.Caption:= 'Код Морзе';
          SbClearTx.Caption:= 'Очистить';
          BtnTestTone.Caption:= 'Тест сигнал';
          btnPTT.Caption:= 'ПРД/Сброс';
          BtnTX.Caption:= 'ПРД';
          SettingsForm.TabSheet2.Caption:= 'Звуковые устройства';
          SettingsForm.Caption:= ' Настройки';
          SettingsForm.Label2.Caption:= 'Выбор звуковой карты для приема:';
          SettingsForm.Label3.Caption:= 'Выбор звуковой карты для передачи:';
          SettingsForm.Label6.Caption:= 'Чувствительность спектра:';
          SettingsForm.cbVox.Caption:= 'Голосовая активация передачи';
          SettingsForm.CbVector.Caption:= 'Постоянный контроль фазы';
          SettingsForm.SbDefSound.Caption:= 'Сброс';
          SettingsForm.SbDefTCP.Caption:= 'Сброс';
          SettingsForm.LabelTCPPort.Caption:= 'Порт сервера:';
          SettingsForm.TabSheet3.Caption:= 'Управление передачей';
          SettingsForm.LabelPort.Caption:= 'Порт:';
          SettingsForm.LabelBaudRate.Caption:= 'Скорость:';
          SettingsForm.LabelSend.Caption:= 'Послать в COM:';
          SettingsForm.LabelPTTDn.Caption:= 'ПРД Нажать:';
          SettingsForm.LabelPTTUp.Caption:= 'ПРД Отпустить:';
          SettingsForm.CbStr.Caption:= 'Строку';
          pmnuTransfer.Caption:= 'Передача';
          pmnuClear.Caption:= 'Очистить';
          pmnuPaste.Caption:= 'Вставить';
          pmnuMarker.Caption:= 'Маркер';
          sbServer.Caption:= 'Сервер';
     end;

     if Language = 1 then
     begin
          mnuOptions.Caption:= 'Options';
          mnuProtocol.Caption:= 'Protocol';
          mnuSettings.Caption:= 'Settings...';
          mnuWaterfall.Caption:= 'Waterfall';
          mnuVasja10.Caption:= 'Noisy Channel';
          mnuVasja11.Caption:= 'Clean Channel';
          mnuMorse.Caption:= 'Morse Code';
          SbClearTx.Caption:= 'Clear';
          BtnTestTone.Caption:= 'Test Tone';
          btnPTT.Caption:= 'PTT/Reset';
          BtnTX.Caption:= 'Send';
          SettingsForm.TabSheet2.Caption:= 'Sound Device';
          SettingsForm.Caption:= ' Settings';
          SettingsForm.Label2.Caption:= 'Wave In Device:';
          SettingsForm.Label3.Caption:= 'Wave Out Device:';
          SettingsForm.Label6.Caption:= 'Waterfall Sensitivity:';
          SettingsForm.cbVox.Caption:= 'VOX';
          SettingsForm.CbVector.Caption:= 'Always Show Vector';
          SettingsForm.SbDefSound.Caption:= 'Default';
          SettingsForm.SbDefTCP.Caption:= 'Default';
          SettingsForm.LabelTCPPort.Caption:= 'Server Port:';
          SettingsForm.TabSheet3.Caption:= 'PTT Control';
          SettingsForm.LabelPort.Caption:= 'Port:';
          SettingsForm.LabelBaudRate.Caption:= 'Baud Rate:';
          SettingsForm.LabelSend.Caption:= 'Write to COM:';
          SettingsForm.LabelPTTDn.Caption:= 'PTT Down Str:';
          SettingsForm.LabelPTTUp.Caption:= 'PTT Up Str:';
          SettingsForm.CbStr.Caption:= 'String';
          pmnuTransfer.Caption:= 'Transfer';
          pmnuClear.Caption:= 'Clear';
          pmnuPaste.Caption:= 'Paste';
          pmnuMarker.Caption:= 'Marker';
          sbServer.Caption:= 'Server';
     end;
     SetProtcolLanguage;
     MixerControlChange(nil);
end;


procedure TMainModemForm.FormShow(Sender: TObject);
var i,j           : Longint;
    FWaveInCaps   : TWaveInCaps;
    FWaveOutCaps  : TWaveOutCaps;
begin
     WtfRange:= FIniFile.ReadInteger('RadioModem', 'WtfRange', 1);
     if WtfRange = 0 then pmnu03kHz.Checked:= True;
     if WtfRange = 1 then pmnu06kHz.Checked:= True;
     if WtfRange = 2 then pmnu12kHz.Checked:= True;

     RXPerception:= FIniFile.ReadInteger('RadioModem', 'RXPerception', 70);

     Lpt := TLptPortConnection.Create;
     if  Lpt.Ready then
     if Lpt.IsPortPresent(LPT1) or Lpt.IsPortPresent(LPT2) or Lpt.IsPortPresent(LPT3) then
     begin
          if Lpt.IsPortPresent(LPT1) then CurrentPort:= LPT1;
          if Lpt.IsPortPresent(LPT2) then CurrentPort:= LPT2;
          if Lpt.IsPortPresent(LPT3) then CurrentPort:= LPT3;
          SettingsForm.CbPTTControl.Items.Add('LPT');
          LPTAvailability:= True;
     end else LPTAvailability:= False;

     SettingsForm.CbPTTControl.Items.Add('Off');

     PTTControlId:= FIniFile.ReadInteger('RadioModem', 'PTTControl', 0);
     TerminalPort:= FIniFile.ReadInteger('RadioModem', 'TerminalPort', 0);

     if PTTControlId < SettingsForm.CbPTTControl.Items.Count then
     SettingsForm.CbPTTControl.ItemIndex:= PTTControlId;

     pmnuMarker.Checked:= FIniFile.ReadBool('RadioModem', 'Marker', True);
     SettingsForm.CbVector.Checked:= FIniFile.ReadBool('RadioModem', 'ShowVector', True);
     SettingsForm.CbBrPTT.ItemIndex:= FIniFile.ReadInteger('RadioModem', 'BrPTT', 6);
     SettingsForm.CbRTS.Checked:= FIniFile.ReadBool('RadioModem', 'CbRTS', False);
     SettingsForm.CbStr.Checked:= FIniFile.ReadBool('RadioModem', 'CbStr', False);
     SettingsForm.edPTTDn.Text:= FIniFile.ReadString('RadioModem', 'PTTDn','00 00 00 00 08');
     SettingsForm.edPTTUp.Text:= FIniFile.ReadString('RadioModem', 'PTTUp','00 00 00 00 88');
     SettingsForm.RbASCII.Checked:= FIniFile.ReadBool('RadioModem', 'ASCII', False);
     SettingsForm.RbHEX.Checked:= FIniFile.ReadBool('RadioModem', 'HEX', True);
     SettingsForm.cbVox.Checked:= FIniFile.ReadBool('RadioModem', 'VOX', False);
     SettingsForm.PortEdit.Value:= FIniFile.ReadInteger('RadioModem', 'ServerPort', 5000);
     OldVol:= FIniFile.ReadInteger('RadioModem', 'Volume', 128);

     SettingsForm.CbVectorClick(nil);

     InDeviceId:=  FIniFile.ReadInteger('RadioModem', 'WInDevice', 0);
     j:= WaveInGetNumDevs;
     if InDeviceId > j - 1 then InDeviceId:= 0;

     for i:= 0 to j - 1 do
     begin
          if (waveInGetDevCaps(i, @FWaveInCaps, SizeOf(TWaveInCaps)) = 0 ) and
          (waveInOpen(nil, i, @WaveFormat, 0, 0, WAVE_FORMAT_QUERY) = 0) then
          begin
               SettingsForm.CbWaveInDevice.Items.Add(IntToStr(i) + ' : ' + FWaveInCaps.szPname);
          end else SettingsForm.CbWaveInDevice.Items.Add(IntToStr(i) + ' : ' + 'Unsupported Device');
     end;
     SettingsForm.CbWaveInDevice.ItemIndex:= InDeviceId;
     StatusBar.Panels[0].Text:= ' In' +  Copy(SettingsForm.CbWaveInDevice.Text, 3, Length(SettingsForm.CbWaveInDevice.Text)-2);

     BtnTX.Enabled:= False;

     OutDeviceId:= FIniFile.ReadInteger('RadioModem', 'WOutDevice', 0);
     j:= WaveOutGetNumDevs;
     if OutDeviceId > j - 1 then OutDeviceId:= 0;
     for i:= 0 to j - 1 do
     begin
          if (waveOutGetDevCaps(i, @FWaveOutCaps, SizeOf(TWaveOutCaps)) = 0 ) and
          (waveOutOpen(nil, i, @WaveFormat, 0, 0, WAVE_FORMAT_QUERY) = 0) then
          begin
               SettingsForm.CbWaveOutDevice.Items.Add(IntToStr(i) + ' : ' + FWaveOutCaps.szPname);
               BtnTX.Enabled:= True;
          end else SettingsForm.CbWaveOutDevice.Items.Add(IntToStr(i) + ' : ' + 'Unsupported Device');
     end;
     SettingsForm.CbWaveOutDevice.ItemIndex:= OutDeviceId;
     SettingsForm.CbWaveOutDeviceChange(nil);
     StatusBar.Panels[1].Text:= ' Out' +  Copy(SettingsForm.CbWaveOutDevice.Text, 3, Length(SettingsForm.CbWaveOutDevice.Text)-2);

     tbMainChange(nil);

     if Height > Screen.Height-30 then Height:= Screen.Height-30;
     MemoSMS.SetFocus;

     MemoSMS.Height:= FIniFile.ReadInteger('RadioModem', 'SMSHeight', 30);

     LabelRX.Font.Color:= clLime;
     CanDrawVector:= SettingsForm.CbVector.Checked;
     ResetReciver;

     SettingsForm.cbPTTControlChange(nil);
     WriteToPort(false);

     mnuVasja10Click(Nil);
     
end;


procedure TMainModemForm.FormDestroy(Sender: TObject);
begin
     if WaveIn <> 0  then WaveInReset(WaveIn);
     if WaveIn <> 0  then WaveInClose(WaveIn);
     if WaveIn2 <> 0 then WaveInReset(WaveIn2);
     if WaveIn2 <> 0 then WaveInClose(WaveIn2);
     Mixer.CloseMixer;
     FIniFile.Free;
     DestroyFilters;
     Spectra.Free;
     bImage.Free;
     lImage.Free;
     vBitmap.Free;
end;


procedure TMainModemForm.CreateFilters;
var i,j: Integer;
    Wp1, Wp2: Double;
begin
     j:= 1000;
     i:= 1;
     Wp1 := (j - i/2)/SamplesPerSec;
     Wp2 := (j + i/2)/SamplesPerSec;
     BPFMorse := TFIRFilter.Create(2, Wp1, Wp2, 1, 1.0, 200);

     fHilbert:= THilbert.Create;

     j:= BaseFrq;
     i:= BaseFrq div 4;
     Wp1 := (j - i/2)/SamplesPerSec;
     Wp2 := (j + i/2)/SamplesPerSec;
     BPFIn0 := TFIRFilter.Create(2, Wp1, Wp2, 1, 1.0, 40);

     j:= BaseFrq;
     i:= BaseFrq div 2;
     Wp1 := (j - i/2)/SamplesPerSec;
     Wp2 := (j + i/2)/SamplesPerSec;
     BPFIn1 := TFIRFilter.Create(2, Wp1, Wp2, 40, 1.0, 20);

     j:= BaseFrq;
     i:= BaseFrq div 2;
     Wp1 := (j - i/2)/SamplesPerSec;
     Wp2 := (j + i/2)/SamplesPerSec;
     BPFOut := TFIRFilter.Create(2, Wp1, Wp2, 10, 1.0, 20);
     
     GenSinCos := TGenSinCosUpr.Create(BaseFrq, SamplesPerSec);

     LPF1_1:= TLPFpor1.Create(BaseFrq / 2, SamplesPerSec);
     LPF1 := TIirLPF.Create(BaseFrq / 2, SamplesPerSec, 0.8, 2, 1);
     LPF2 := TIirLPF.Create(BaseFrq / 2, SamplesPerSec, 0.8, 2, 1);
end;


procedure TMainModemForm.DestroyFilters;
begin
     BPFMorse.Destroy;
     fHilbert.Free;
     BPFIn0.Free;
     BPFIn1.Free;
     BPFOut.Free;
     GenSinCos.Free;
     LPF1_1.Destroy;
     LPF2.Destroy;
     LPF1.Destroy;
end;


procedure TMainModemForm.DecodeHexData(HexData: String);
var i                     : Longint;
    tmps1                 : String;
    tmps2                 : String;
    Data                  : String;
begin
     DecodeNow  := True;

     if (CanDrawVector) then
     begin
          vBitmap.Canvas.FillRect(vBitmap.Canvas.ClipRect);
          VectorPaintBoxPaint(nil);
     end;

     SynFound   := False;
     EndFound   := False;

     TRY
        tmps1:= StrTran(HexData, '45B0F#', '$', True);
        i:= pos('$',tmps1)+6;
        tmps2:= Copy(HexData, i, Length(HexData)-i);
        tmps1:= StrTran(tmps2, '#E0F#', '$', True);
        i:= pos('$',tmps1);
        tmps2:= Copy(tmps1, 1, i-1);
        Data:= DecodeHexSMS(tmps2);

        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if sbServer.Down then
        begin
             {Посылаем ВСЕМ клиентам}
             for i := 0 to SettingsForm.ServerSocket.Socket.ActiveConnections - 1 do
             begin
                  SettingsForm.ServerSocket.Socket.Connections[i].SendText(Data);
                  MemoSMS.Lines.Add( 'Data sending to: ' + SettingsForm.ServerSocket.Socket.Connections[i].RemoteAddress  + ' - ' + LocalDateNow);
             end;
        end else MemoSMSFrom.Lines.Add(Data);
        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        if WavOutOpened then
        begin
             CloseWOutCommand:= True;
             repeat Application.ProcessMessages until not WavOutOpened;
        end;
     FINALLY
            ResetReciver;
            BtnTX.Enabled:= True;
     END;
end;


procedure TMainModemForm.ResetReciver;
begin
     dF:= 0.0;
     PSKCounter:= 0;
     BadData:= 0;
     SynFound:= False;
     EndFound:= False;
     imgFrs.Visible:= False;
     imgSync.Visible:= False;
     MaxDataCounter:= 0;
     HexSynString:= '';
     HexDataString:= '';
     VectorPaintBox.Repaint;
end;


procedure TMainModemForm.StartWavOut;
begin
     WavOutOpened:= True;
     if WaveOutOpen (@WaveOut, OutDeviceId, @WaveFormat, Handle, 0, CALLBACK_WINDOW)<>0 then
     begin
          application.messagebox('WaveOutOpen.', 'Error', mb_OK);
          WriteToPort(False);
     end;
end;


procedure TMainModemForm.WriteToPort(Mode: Boolean);
begin
     if PTTControl = 0 then
     begin
          if Mode then SettingsForm.PTTDOWN;
          if not Mode then SettingsForm.PTTUP;
          if Mode then LabelTX.Visible:= True else LabelTX.Visible:= False;
     end;

     if LPTAvailability and Lpt.Ready and (PTTControl = 1) then 
     begin
          {Pin 1/16}
          if Mode then  if  Pin1 then Lpt.WritePort (CurrentPort,2,(1 xor Lpt.ReadPort( (CurrentPort) ,2) ));//pin1
          if not Mode then  if not Pin1 then Lpt.WritePort (CurrentPort,2,(1 xor Lpt.ReadPort( (CurrentPort) ,2) ));//pin1
          if Mode then  if not Pin16 then Lpt.WritePort (CurrentPort,2,(4 xor Lpt.ReadPort( (CurrentPort) ,2) ));//pin16
          if not Mode then  if Pin16 then Lpt.WritePort (CurrentPort,2,(4 xor Lpt.ReadPort( (CurrentPort) ,2) ));//pin16
          {Pin 3/7}
          if Mode then  if not Pin3 then Lpt.WritePort (CurrentPort,0,(2 xor Lpt.ReadPort( CurrentPort ,0) )); //pin3
          if not Mode then  if Pin3 then Lpt.WritePort (CurrentPort,0,(2 xor Lpt.ReadPort( CurrentPort ,0) )); //pin3
          if Mode then LabelTX.Visible:= True else LabelTX.Visible:= False;

          if Pin7 then Lpt.WritePort (CurrentPort,0,(32 xor Lpt.ReadPort( CurrentPort ,0) )); //pin7 (gnd initialization)
     end;
end;


procedure TMainModemForm.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
     if DecodeNow then Sleep(250);

     if WavInOpened then
     begin
          CloseWInCommand:= True;
          if WaveIn <> 0 then WaveInReset(WaveIn);
          if WaveIn <> 0 then WaveInClose(WaveIn);
          WaveIn:= 0;
          if WaveIn2 <> 0 then WaveInReset(WaveIn2);
          if WaveIn2 <> 0 then WaveInClose(WaveIn2);
          WaveIn2:= 0;
          repeat Application.ProcessMessages until not WavInOpened;
     end;

     if WavOutOpened then
     begin
          CloseWOutCommand:= True;
          repeat Application.ProcessMessages; until not WavOutOpened;
     end;

     CanClose:=  (not WavOutOpened) and (not WavInOpened);
     if CanClose then
     begin

          if WindowState <> wsMaximized then
          begin
               FIniFile.WriteInteger('RadioModem', 'Left', Left);
               FIniFile.WriteInteger('RadioModem', 'Top', Top);
               FIniFile.WriteInteger('RadioModem', 'Width', Width);
               FIniFile.WriteInteger('RadioModem', 'Height', Height);
               FIniFile.WriteInteger('RadioModem', 'SMSHeight', MemoSMS.Height);
          end;

          SetProtcolNum;

          FIniFile.WriteInteger('RadioModem', 'WInDevice', InDeviceId);
          FIniFile.WriteInteger('RadioModem', 'WOutDevice', OutDeviceId);
          FIniFile.WriteInteger('RadioModem', 'WtfRange', WtfRange);
          FIniFile.WriteInteger('RadioModem', 'Protcol', ProtcolNum);
          FIniFile.WriteInteger('RadioModem', 'PTTControl', PTTControlId);
          FIniFile.WriteInteger('RadioModem', 'TerminalPort', TerminalPort);
          FIniFile.WriteBool('RadioModem', 'Waterfall', mnuWaterfall.Checked);
          FIniFile.WriteInteger('RadioModem', 'RXPerception', RXPerception);
          FIniFile.WriteInteger('RadioModem', 'BrPTT', SettingsForm.CbBrPTT.ItemIndex);
          FIniFile.WriteBool('RadioModem', 'CbRTS', SettingsForm.CbRTS.Checked);
          FIniFile.WriteBool('RadioModem', 'CbStr', SettingsForm.CbStr.Checked);
          FIniFile.WriteString('RadioModem', 'PTTDn',SettingsForm.edPTTDn.Text);
          FIniFile.WriteString('RadioModem', 'PTTUp',SettingsForm.edPTTUp.Text);
          FIniFile.WriteBool('RadioModem', 'ShowVector', SettingsForm.CbVector.Checked);
          FIniFile.WriteInteger('RadioModem', 'SpectrWidth', PanelSpectr.Width);
          FIniFile.WriteBool('RadioModem', 'ASCII', SettingsForm.RbASCII.Checked);
          FIniFile.WriteBool('RadioModem', 'HEX', SettingsForm.RbHEX.Checked);
          FIniFile.WriteBool('RadioModem', 'VOX', SettingsForm.cbVox.Checked);
          FIniFile.WriteInteger('RadioModem', 'Volume', tbMain.Position);
          FIniFile.WriteBool('RadioModem', 'Marker', pmnuMarker.Checked);
          FIniFile.WriteInteger('RadioModem', 'Language', Language);

          Application.ProcessMessages;
          Lpt.Destroy;
     end;
     WriteToPort(False);
end;


procedure TMainModemForm.StartStopWaveIn(Start: Boolean);
var  i: Longint;
begin
     if (not WavInOpened)  and (Start) then
     begin
          Err:= waveInOpen(@WaveIn,InDeviceId,@WaveFormat,handle,0,CALLBACK_WINDOW);
          if Err<>0 then
          begin
               MessageDlg ('Can`t open audio device !' + #13+#10 + translate_mm_error (Err),
               mtWarning, [mbOK], 0);
               EXIT;
          end;          
          Err:= waveInOpen(@WaveIn2,InDeviceId,@WaveFormat,handle,0,CALLBACK_WINDOW);
          if Err<>0 then
          begin
               MessageDlg ('Can`t open audio device !' + #13+#10 + translate_mm_error (Err),
               mtWarning, [mbOK], 0);
               EXIT;
          end;

          WavInOpenCount     := 0;
          WavInCloseCount    := 0;

          for i:=0 to Buffers-1 do
          begin
               with WaveInHeader[i] do
               begin
                    lpdata:= @WaveInBuffers[i];
                    dwBufferlength:= BufferLengthByte;
                    dwBytesrecorded:= 0;
                    dwFlags:= 0;
                    dwLoops:= 0;
                    lpNext:=  nil;
                    reserved:=  0;
               end;

               Err:= waveInPrepareHeader(WaveIn,@WaveInHeader[i],sizeof(TWavehdr));
               if Err<>0 then MessageDlg (translate_mm_error (Err), mtWarning, [mbOK], 0);
               Err:= waveInAddBuffer(WaveIn, @WaveInHeader[i],sizeof(TWaveHdr));
               if Err<>0 then MessageDlg (translate_mm_error (Err), mtWarning, [mbOK], 0);
          end;

          Err:= WaveInStart(WaveIn);
          if Err<>0 then MessageDlg (translate_mm_error (Err), mtWarning, [mbOK], 0);
     end;

     if not Start then
     begin
          CloseWInCommand:= True;
          if WaveIn <> 0 then WaveInReset(WaveIn);
          if WaveIn <> 0 then WaveInClose(WaveIn);
          WaveIn:= 0;
          if WaveIn2 <> 0 then WaveInReset(WaveIn2);
          if WaveIn2 <> 0 then WaveInClose(WaveIn2);
          WaveIn2:= 0;
          repeat Application.ProcessMessages until not WavInOpened;
     end;
     ResetReciver;
end;


procedure TMainModemForm.tbMainChange(Sender: TObject);
var b: Byte;
begin
     if MixerGet then Exit;
     Mixer.MixerSetControl(Volume, 65535 - tbMain.Position* 257);
     B:= abs(tbMain.Position-255);
     if B > 14 then LabelSBVol.Font.Color:= clLime
     else LabelSBVol.Font.Color:= clGreen;
     if Language = 0 then LabelSBVol.Caption:= 'Уровень : ' + FormatFloat('000', B);
     if Language = 1 then LabelSBVol.Caption:= 'Volume : ' + FormatFloat('000', B);
end;


procedure TMainModemForm.MixerControlChange(Sender: TObject);
var b: Byte;
begin
     MixerGet:= True;
     tbMain.Position:= (65535 - Mixer.MixerGetControl(Volume)) div 257;
     B:= abs(tbMain.Position-255);
     if B > 14 then LabelSBVol.Font.Color:= clLime
     else LabelSBVol.Font.Color:= clGreen;
     if Language = 0 then LabelSBVol.Caption:= 'Уровень : ' + FormatFloat('000', B);
     if Language = 1 then LabelSBVol.Caption:= 'Volume : ' + FormatFloat('000', B);
     MixerGet:= False;
end;


procedure TMainModemForm.btnPTTMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
     CanUseTerminal:= False;
     CloseWOutCommand:= True;
     SbClearTx.Enabled:= True;
     WriteToPort(True);
     isPTT:= True;
     BtnTestTone.Down:= False;
end;


procedure TMainModemForm.btnPTTMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
     EnableTXButtons;
     SetProtcolNum;
     ResetReciver;
     BtnTX.Enabled:= True;
end;


procedure TMainModemForm.BtnTXClick(Sender: TObject);
var i,n        : Longint;
begin
     if DisableTx then EXIT;
     DisableTx:= True;

     BtnTX.Enabled:= False;

     if WavOutOpened then
     begin
          BtnTX.Enabled:= False;
          CloseWOutCommand:= True;
          repeat Application.ProcessMessages until not WavOutOpened;
     end;

     if ProtcolNum = 255 then
     begin
          ProgressBar.Position:= 0;
          begin
               WriteToPort(True);
               isPTT:= True;
          end;
          StartWavOut;
          EXIT;
     end;

     //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
     if sbServer.Down then
     begin
          if (ProtcolNum = 0) or (ProtcolNum = 1) then WOutBuffer:= EncodeData(ReceivedData, ProtcolNum);
          if ProtcolNum = 64 then WOutBuffer:= EncodeMorse(ReceivedData);
          ReceivedData:= '';
     end else
     begin
          if (ProtcolNum = 0) or (ProtcolNum = 1) then WOutBuffer:= EncodeData(MemoSMS.Text, ProtcolNum);
          if ProtcolNum = 64 then WOutBuffer:= EncodeMorse(MemoSMS.Text); //Morse Code
     end;
     //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

     i:= MaxDataSize - 8192;
     if ProtcolNum = 64 then i:= i*2;
     n:= Length(WOutBuffer);

     if n > i then
     begin
          if Language = 0 then MessageDlg ('Сообщение слишком большое.', mtWarning, [mbOK], 0);
          if Language = 1 then MessageDlg ('Message Too Much.', mtWarning, [mbOK], 0);
          EnableTXButtons;
          EXIT;
     end;

     ProgressBar.Position:= 0;
     ProgressBar.Max:= Length(WOutBuffer);

     begin
          WriteToPort(True);
          isPTT:= True;
          n:= GetTickCount;
          repeat
                Application.ProcessMessages;
                Sleep(1);
                i:= GetTickCount;
          until (i - n > 250);
     end;

     CloseWOutCommand:= False;
     StartWavOut;
end;


procedure TMainModemForm.SbClearTxClick(Sender: TObject);
begin
     MemoSMS.ReadOnly:= False;
     MemoSMS.Clear;
     MemoSMS.SetFocus;
     MemoSMSFrom.Clear;
end;

procedure TMainModemForm.mnuWaterfallClick(Sender: TObject);
begin
     Splitter1.Visible:= mnuWaterfall.Checked;
     PanelSpectr.Visible:= mnuWaterfall.Checked;
     StatusBar.Align:= alBottom;
end;


procedure TMainModemForm.SetProtcolLanguage;
begin
     if Language = 0 then
     begin
          if ProtcolNum = 0 then LabelProtoName.Caption:= 'Шумный канал';
          if ProtcolNum = 1 then LabelProtoName.Caption:= 'Чистый канал';
          if ProtcolNum = 64 then LabelProtoName.Caption:= 'Код Морзе';
     end;

     if Language = 1 then
     begin
          if ProtcolNum = 0 then LabelProtoName.Caption:= 'Noisy Channel';
          if ProtcolNum = 1 then LabelProtoName.Caption:= 'Clean Channel';
          if ProtcolNum = 64 then LabelProtoName.Caption:= 'Morse Code';
     end;
end;

procedure TMainModemForm.SetProtcolNum;
begin
     if mnuVasja10.Checked then ProtcolNum:= 0;
     if mnuVasja11.Checked then ProtcolNum:= 1;
     if mnuMorse.Checked then ProtcolNum:= 64;

     if ProtcolNum = 0   then mnuVasja10.Checked:= True;
     if ProtcolNum = 1   then mnuVasja11.Checked:= True;
     if ProtcolNum = 64  then mnuMorse.Checked:= True;

     SetProtcolLanguage;
end;

procedure TMainModemForm.EnableTXButtons;
begin
     CloseWOutCommand:= True;

     if isPTT then
     begin
          WriteToPort(False);
          isPTT:= False;
     end;

     if WavOutOpened then
     repeat
           Application.ProcessMessages
     until not WavOutOpened;

     SbClearTx.Enabled:= True;
     ProgressBar.Position:= 0;
     DisableTx:= False;
end;

procedure TMainModemForm.mnuVasja10Click(Sender: TObject);
begin
     ResetReciver;
     EnableTXButtons;
     SetProtcolNum;

     StartStopWaveIn(False);
     BaseFrq:= 1500;
     if ProtcolNum = 1 then BaseFrq:= 2400;
     if ProtcolNum = 64 then BaseFrq:= 1000;
     BtnTestTone.Down:= False;

     DestroyFilters;
     Application.ProcessMessages;
     Sleep(2);
     CreateFilters;
     Application.ProcessMessages;
     Sleep(2);
     StartStopWaveIn(True);
     Application.ProcessMessages;
     Sleep(2);

     hpf:= 84; lpf:= 172;
     if ProtcolNum = 1 then hpf:= 110;
     if ProtcolNum = 1 then lpf:= 300;

     ResetReciver;
     BtnTestTone.Enabled:= ProtcolNum <> 64;
end;

procedure TMainModemForm.mnuSettingsClick(Sender: TObject);
begin
     SettingsForm.Show;
end;

procedure TMainModemForm.pbssPaint(Sender: TObject);
begin
     BitBlt(pbss.canvas.Handle, pbss.Width-Spectra.Width,0,Spectra.Width, Spectra.Height, Spectra.canvas.Handle, 0,0,SRCCOPY);
end;

procedure TMainModemForm.RulerPaintBoxPaint(Sender: TObject);
var i: integer;
    k,j: double;
begin
     if RulerPaintBox.Width > 2 then
     begin
          if  WtfRange = 0 then
          begin
               j:= 11;  k:= RulerPaintBox.Height/12 -6;
               for i:=1 to 12 do
               begin
                    if RulerPaintBox.Height > 250 then RulerPaintBox.Canvas.TextOut(0, Round(k-(RulerPaintBox.Height)/24),'-');
                    if i<12 then RulerPaintBox.Canvas.TextOut(0, Round(k),'- ' + FormatFloat('0.00',j/4));
                    k:=k + ((RulerPaintBox.Height)/11.96);
                    j:= j - 1.0;
               end;
          end;

          if  WtfRange = 1 then
          begin
               j:= 11;  k:= RulerPaintBox.Height/12 -6;
               for i:=1 to 12 do
               begin
                    if RulerPaintBox.Height > 250 then RulerPaintBox.Canvas.TextOut(0, Round(k-(RulerPaintBox.Height)/24),'-');
                    if i<12 then RulerPaintBox.Canvas.TextOut(0, Round(k),'- ' + FormatFloat('0.0',j/2));
                    k:=k + ((RulerPaintBox.Height)/11.98);
                    j:= j - 1.0;
               end;
          end;
          if  WtfRange = 2 then
          begin
               j:= 22;  k:= RulerPaintBox.Height/12 -6;
               for i:=1 to 12 do
               begin
                    if RulerPaintBox.Height > 250 then RulerPaintBox.Canvas.TextOut(0, Round(k-(RulerPaintBox.Height)/24),'-');
                    if i<12 then RulerPaintBox.Canvas.TextOut(0, Round(k),'- ' + FormatFloat('0.0',j/2));
                    k:=k + ((RulerPaintBox.Height)/11.99);
                    j:= j - 2.0;
               end;
          end;
     end;
end;

procedure TMainModemForm.pmnu03kHzClick(Sender: TObject);
begin
     WtfRange:= 0;
     pmnu03kHz.Checked:= True;
     RulerPaintBox.Repaint
end;

procedure TMainModemForm.pmnu06kHzClick(Sender: TObject);
begin
     WtfRange:= 1;
     pmnu06kHz.Checked:= True;
     RulerPaintBox.Repaint
end;

procedure TMainModemForm.pmnu12kHzClick(Sender: TObject);
begin
     WtfRange:= 2;
     pmnu12kHz.Checked:= True;
     RulerPaintBox.Repaint
end;

procedure TMainModemForm.FormResize(Sender: TObject);
begin
     if MemoSMSFrom.Height <= 30 then
     begin
          MemoSMSFrom.Align:= alTop;
          Splitter2.Align:= alTop;
          MemoSMS.Align:= alClient;
     end;
     if MemoSMS.Height <= 30 then
     begin
          MemoSMS.Align:= alBottom;
          Splitter2.Align:= alBottom;
          MemoSMSFrom.Align:= alClient;
     end;
end;

procedure TMainModemForm.PanelSpectrResize(Sender: TObject);
begin
     Spectra.Height:= pbss.Height;
     lImage.Height:= pbss.Height;
end;

procedure TMainModemForm.Splitter2CanResize(Sender: TObject;
  var NewSize: Integer; var Accept: Boolean);
begin
     Accept:= (MemoSMSFrom.Height <> 30) or (MemoSMS.Height <> 30);
end;

procedure TMainModemForm.VectorPaintBoxPaint(Sender: TObject);
begin
     if (SynFound) or (CanDrawVector) then
     BitBlt(VectorPaintBox.canvas.Handle, 0, 0, vBitmap.Width, vBitmap.Height, vBitmap.canvas.Handle, 0, 0, SRCCOPY);
end;

procedure TMainModemForm.Splitter1Moved(Sender: TObject);
begin
     if (LeftScrollPoint-PanelSpectr.Width > 0) and (WindowState <> wsMaximized) then
     Width:= Width - (LeftScrollPoint-PanelSpectr.Width);
end;

procedure TMainModemForm.Splitter1CanResize(Sender: TObject;
  var NewSize: Integer; var Accept: Boolean);
begin
     LeftScrollPoint:= PanelSpectr.Width;
end;

procedure TMainModemForm.BtnTestToneClick(Sender: TObject);
begin
     if BtnTestTone.Down then
     begin
          if ProtcolNum = 0 then LabelProtoName.Caption:= 'Test Tone 1500 Hz';
          if ProtcolNum = 1 then LabelProtoName.Caption:= 'Test Tone 2400 Hz';
          if ProtcolNum = 64 then LabelProtoName.Caption:= 'Test Tone 1000 Hz';
          ProtcolNum:= 255;
          if not WavOutOpened then CloseWOutCommand:= False;
          BtnTXClick(nil);
     end else
     begin
          EnableTXButtons;
          SetProtcolNum;
     end;
     ProgressBar.Position:= 0;
end;

procedure TMainModemForm.pmnuClearClick(Sender: TObject);
begin
     SbClearTxClick(nil);
end;

procedure TMainModemForm.ImageSndMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var W: HWnd;
begin
     W:= FindWindow(nil,'Recording Control');
     if Button= MBRight then
     begin
          if W = 0 then WinExec('SNDVOL32.EXE /r', SW_SHOWNORMAL)
          else ShowWindow(W, SW_RESTORE);
     end;
     if Button= MBLeft then
     begin
          if W = 0 then WinExec('SNDVOL32.EXE', SW_SHOWNORMAL)
          else ShowWindow(W, SW_RESTORE);
     end;
end;

procedure TMainModemForm.mnuRussianClick(Sender: TObject);
begin
     mnuRussian.Checked:= True;
     mnuEnglish.Checked:= False;
     Language:= 0;
     SetLanguage;
end;

procedure TMainModemForm.mnuEnglishClick(Sender: TObject);
begin
     mnuEnglish.Checked:= True;
     mnuRussian.Checked:= False;
     Language:= 1;
     SetLanguage;
end;

procedure TMainModemForm.pmnuPasteClick(Sender: TObject);
begin
     MemoSMS.PasteFromClipboard;
end;

procedure TMainModemForm.FormActivate(Sender: TObject);
begin
     SetLanguage;
     StartStopWaveIn(True);
end;

procedure TMainModemForm.sbServerClick(Sender: TObject);
begin
     if SettingsForm.ServerSocket.Active then SettingsForm.ServerSocket.Close;
     StatusBar.Panels[2].Text:= '';
     Application.ProcessMessages;
     if sbServer.Down then
     begin
          SettingsForm.ServerSocket.Port := SettingsForm.PortEdit.Value;
          SettingsForm.ServerSocket.Open;
     end;
     ReceivedData:= '';
end;

end.
