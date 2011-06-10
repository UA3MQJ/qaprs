unit Settings;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, Buttons, ComCtrls, mmsystem, mmErrMsg, PHDConst,
  Spin, BCPort, Server_Socket;

var TerminalString : String;
    CanUseTerminal : Boolean;
    RLSDState      : Boolean;
    RXBtnDown      : Boolean;

type
  TSettingsForm = class(TForm)
    PageControl: TPageControl;
    TabSheet2: TTabSheet;
    CbWaveInDevice: TComboBox;
    CbWaveOutDevice: TComboBox;
    TabSheet3: TTabSheet;
    Label2: TLabel;
    Label3: TLabel;
    cbPTTControl: TComboBox;
    edPTTDn: TEdit;
    CbBrPTT: TComboBox;
    LabelPort: TLabel;
    LabelBaudRate: TLabel;
    edPTTUp: TEdit;
    LabelPTTDn: TLabel;
    LabelPTTUp: TLabel;
    LabelSend: TLabel;
    CbRTS: TCheckBox;
    CbStr: TCheckBox;
    SbDefSound: TSpeedButton;
    RbHex: TRadioButton;
    RbASCII: TRadioButton;
    cbVox: TCheckBox;
    Label6: TLabel;
    tbRXPerception: TTrackBar;
    LabelvRX: TLabel;
    CbVector: TCheckBox;
    sbClose1: TSpeedButton;
    sbClose2: TSpeedButton;
    BComPTT: TBComPort;
    TabSheet1: TTabSheet;
    PortEdit: TSpinEdit;
    LabelTCPPort: TLabel;
    sbClose3: TSpeedButton;
    SbDefTCP: TSpeedButton;
    procedure FormHide(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure CbWaveInDeviceChange(Sender: TObject);
    procedure CbWaveOutDeviceChange(Sender: TObject);
    procedure cbPTTControlChange(Sender: TObject);
    procedure SbDefSoundClick(Sender: TObject);
    procedure CbRTSClick(Sender: TObject);
    procedure CbVectorClick(Sender: TObject);
    procedure tbRXPerceptionChange(Sender: TObject);
    procedure BComTerminalTxEmpty(Sender: TObject);
    procedure BComTerminalRLSDChange(Sender: TObject; State: Boolean);
    procedure sbClose1Click(Sender: TObject);
    procedure SbDefTCPClick(Sender: TObject);
    procedure sbClose3Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
  private
    { Private declarations }
    tp: Longint;
    lt: Longint;
    FormStarted: Boolean;
    procedure SetEnabledComponents;
  public
    { Public declarations }
    ServerSocket   : TServerSocket;
    procedure ServerSocketAccept(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketClientConnect(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketClientDisconnect(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketClientError(Sender: TObject; Socket: TCustomWinSocket; ErrorEvent: TErrorEvent; var ErrorCode: Integer);
    procedure ServerSocketClientRead(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketClientWrite(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketGetSocket(Sender: TObject; Socket: Integer; var ClientSocket: TServerClientWinSocket);
    procedure ServerSocketGetThread(Sender: TObject; ClientSocket: TServerClientWinSocket; var SocketThread: TServerClientThread);
    procedure ServerSocketListen(Sender: TObject; Socket: TCustomWinSocket);
    procedure ServerSocketThreadEnd(Sender: TObject; Thread: TServerClientThread);
    procedure ServerSocketThreadStart(Sender: TObject; Thread: TServerClientThread);
    procedure PTTUP;
    procedure PTTDOWN;
  end;

var
  SettingsForm: TSettingsForm;

implementation

{$R *.dfm}
uses TXMain, PSKCodec;


function ASCIIFilter(InStr: String): String;
var S1, S2: String;
    i: Longint;
begin
     S1:= AnsiUpperCase(InStr);
     for i:= 1 to length(S1) do
     begin
          if (Char(S1[i]) in ['0'..'9']) or (Char(S1[i]) in ['A'..'Z']) then
          S2:= S2+ S1[i];
     end;
     ASCIIFilter:= S2;
end;

function DecodeHexSMS(var BinStr: String): String;
var i,j,k: Longint;
    sd, ts: String;
begin
     j:=0; sd:= ''; ts:= '';
     for i:= 1 to length(BinStr) div 2 do
     begin
          sd:= Copy(BinStr, j+1, 2);
          k:= HexToInt64(sd);
          ts:= ts + Char(k);
          Inc(j,2);
     end;
     Result:= ts;
end;

function HEXFilter(InStr: String): String;
var S1, S2: String;
    i: Longint;
begin
     S1:= AnsiUpperCase(InStr);
     for i:= 1 to length(S1) do
     begin
          if (Char(S1[i]) in ['0'..'9']) or (Char(S1[i]) in ['A'..'F']) then
          S2:= S2+ S1[i];
     end;
     HEXFilter:= DecodeHexSMS(S2);
end;

procedure TSettingsForm.FormHide(Sender: TObject);
begin
     MainModemForm.mnuSettings.Enabled:= True;
     tp:= Top;
     lt:= Left;
end;

procedure TSettingsForm.FormCreate(Sender: TObject);
begin
     FormStarted:= False;
     tp:= Top;
     lt:= Left;
     EnumComPorts(CbPTTControl.Items);

     ServerSocket:= TServerSocket.Create(self);
     ServerSocket.OnAccept:= ServerSocketAccept;
     ServerSocket.OnClientConnect:= ServerSocketClientConnect;
     ServerSocket.OnClientDisconnect:= ServerSocketClientDisconnect;
     ServerSocket.OnClientError:= ServerSocketClientError;
     ServerSocket.OnClientRead:= ServerSocketClientRead;
     ServerSocket.OnClientWrite:= ServerSocketClientWrite;
     ServerSocket.OnGetSocket:= ServerSocketGetSocket;
     ServerSocket.OnGetThread:= ServerSocketGetThread;
     ServerSocket.OnListen:= ServerSocketListen;
     ServerSocket.OnThreadEnd:= ServerSocketThreadEnd;
     ServerSocket.OnThreadStart:= ServerSocketThreadStart;     
end;

procedure TSettingsForm.FormShow(Sender: TObject);
begin
     Top:= tp;
     Left:= lt;
     tbRXPerception.Position:= RXPerception;
     LabelvRX.Caption:= FormatFloat('00',RXPerception) + '%';
end;

procedure TSettingsForm.CbWaveInDeviceChange(Sender: TObject);
var S: String;
begin
     with MainModemForm do
     begin
          StartStopWaveIn(False);
          S:=  Copy(CbWaveInDevice.Text, 3, Length(CbWaveInDevice.Text)-2);
          StatusBar.Panels[0].Text:= ' In' + S;
          if S = ': Unsupported Device' then Exit else
          begin
               InDeviceId:= CbWaveInDevice.ItemIndex;
               StartStopWaveIn(True);
          end;
     end;
end;

procedure TSettingsForm.CbWaveOutDeviceChange(Sender: TObject);
var S: String;
begin
     with MainModemForm do
     begin
          BtnTX.Enabled:= False;
          S:=  Copy(CbWaveOutDevice.Text, 3, Length(CbWaveOutDevice.Text)-2);
          StatusBar.Panels[1].Text:= ' Out' + S;
          if MixerOpened then
          begin
               Mixer.CloseMixer;
               MixerOpened:= False;
          end;
          if S = ': Unsupported Device' then Exit else
          begin
               OutDeviceId:= CbWaveOutDevice.ItemIndex;
               Mixer.OpenMixer(OutDeviceId);
               MixerControlChange(Self);
               MixerOpened:= True;
               BtnTX.Enabled:= True;
          end;
     end;
end;

procedure TSettingsForm.cbPTTControlChange(Sender: TObject);
var S: String;
begin
     MainModemForm.WriteToPort(False);
     MainModemForm.isPTT:= False;

     MainModemForm.PTTControlId:= CbPTTControl.ItemIndex;
     S:= CbPTTControl.Text;

     if BComPTT.Connected then BComPTT.Close;

     if (S = 'COM1') or (S = 'COM2') or (S = 'COM3') or (S = 'COM4')  or (S = 'COM5') or (S = 'COM6') or (S = 'COM7') or (S = 'COM8')
     or (S = 'COM9') or (S = 'COM10') or (S = 'COM11') or (S = 'COM12')  or (S = 'COM13') or (S = 'COM14') or (S = 'COM15') then
     begin
          SetEnabledComponents;
          MainModemForm.PTTControl:= 0;
          BComPTT.Port := S;
          BComPTT.BaudRate := TBaudRate(CbBrPTT.ItemIndex);
          if BComPTT.Connected then BComPTT.Close;
          Application.ProcessMessages;
          Sleep(100);
          if CbBrPTT.ItemIndex > 0 then BComPTT.Open;
          if CbRTS.Checked and CbStr.Checked then BComPTT.SetRTS(True);
     end else SetEnabledComponents;

     if (S = 'LPT') then MainModemForm.PTTControl:= 1;
     if (S = 'Off') then MainModemForm.PTTControl:= -1;

     if S <> 'Off' then MainModemForm.LabelPTTControl.Font.Color:= clLime
     else MainModemForm.LabelPTTControl.Font.Color:= clGreen;
     MainModemForm.LabelPTTControl.Caption:= 'Port : ' + S;
end;

procedure TSettingsForm.SetEnabledComponents;
var S: String;
begin
     CbRTS.Enabled:= False;
     CbStr.Enabled:= False;
     edPTTDn.Enabled:= False;
     edPTTUp.Enabled:= False;
     LabelSend.Enabled:= False;
     LabelPTTDn.Enabled:= False;
     LabelPTTUp.Enabled:= False;
     RbASCII.Enabled:= False;
     RbHex.Enabled:= False;
     CbBrPTT.Enabled:= True;
     LabelBaudRate.Enabled:= True;

     S:= CbPTTControl.Text;
     if (S = 'COM1') or (S = 'COM2') or (S = 'COM3') or (S = 'COM4')  or (S = 'COM5') or (S = 'COM6') or (S = 'COM7') or (S = 'COM8')
     or (S = 'COM9') or (S = 'COM10') or (S = 'COM11') or (S = 'COM12')  or (S = 'COM13') or (S = 'COM14') or (S = 'COM15') then
     begin
          CbRTS.Enabled:= True;
          CbStr.Enabled:= True;
          LabelSend.Enabled:= True;
          LabelBaudRate.Enabled:= False;
          CbBrPTT.Enabled:= False;
          if CbStr.Checked then
          begin
               edPTTDn.Enabled:= True;
               edPTTUp.Enabled:= True;
               LabelPTTDn.Enabled:= True;
               LabelPTTUp.Enabled:= True;
               RbASCII.Enabled:= True;
               RbHex.Enabled:= True;
          end;
     end;
end;

procedure TSettingsForm.SbDefSoundClick(Sender: TObject);
begin
     CbWaveInDevice.ItemIndex:= 0;
     CbWaveInDeviceChange(Self);
     CbWaveOutDevice.ItemIndex:= 0;
     CbWaveOutDeviceChange(Self);
     cbVox.Checked:= False;
     tbRXPerception.Position:= 70;
     RXPerception:= 70;
     CbVector.Checked:= True;     
end;

procedure TSettingsForm.PTTDOWN;
begin
     if BComPTT.Connected then
     begin
          if CbRTS.Checked and not CbStr.Checked then BComPTT.SetRTS(True);
          if CbStr.Checked then
          begin
               if RbASCII.Checked then BComPTT.WriteStr(ASCIIFilter(edPTTDn.Text));
               if RbHEX.Checked then BComPTT.WriteStr(HEXFilter(edPTTDn.Text));
          end;
     end;
end;

procedure TSettingsForm.PTTUP;
begin
     if BComPTT.Connected then
     begin
          if CbRTS.Checked and not CbStr.Checked then BComPTT.SetRTS(False);
          if CbStr.Checked then
          begin
               if RbASCII.Checked then BComPTT.WriteStr(ASCIIFilter(edPTTUp.Text));
               if RbHEX.Checked then BComPTT.WriteStr(HEXFilter(edPTTUp.Text));
          end;
     end;
end;

procedure TSettingsForm.CbRTSClick(Sender: TObject);
begin
     if CbStr.Checked and SettingsForm.Visible then
     begin
          case
              MessageDlg('ВНИМАНИЕ! Неверная комманда может повредить ваш трансивер.' +#13 +#10 + 'Вы хотите продолжить?'
              +#13+#10+#13+#10 + 'WARNING! Improper command can damage your transceiver.' +#13 +#10 + 'Do you want to continue?',
              mtWarning, [mbYes, mbNo, mbCancel], 0)
          of
              idNo:    CbStr.Checked:= False;
              idCancel: CbStr.Checked:= False;
          end;
     end;

     SetEnabledComponents;

     if BComPTT.Connected then
     begin
          BComPTT.SetRTS(True);
          if CbRTS.Checked and CbStr.Checked then BComPTT.SetRTS(False);
     end;
end;

procedure TSettingsForm.CbVectorClick(Sender: TObject);
begin
     CanDrawVector:= CbVector.Checked;
     MainModemForm.VectorPaintBox.Repaint;
end;

procedure TSettingsForm.tbRXPerceptionChange(Sender: TObject);
begin
     RXPerception:= tbRXPerception.Position;
     LabelvRX.Caption:= FormatFloat('00',RXPerception) + '%';
end;

procedure TSettingsForm.BComTerminalTxEmpty(Sender: TObject);
begin
     with MainModemForm do
     begin
          ProgressBar.Position:= 0;
          BtnTX.Enabled:= True;
          DisableTx:= False;
     end;
end;

procedure TSettingsForm.BComTerminalRLSDChange(Sender: TObject; State: Boolean);
begin
     RLSDState:= State;
end;

procedure TSettingsForm.sbClose1Click(Sender: TObject);
begin
     Close;
end;

procedure TSettingsForm.SbDefTCPClick(Sender: TObject);
begin
     with MainModemForm do
     begin
          ReceivedData:= '';
          if ServerSocket.Active then ServerSocket.Active := False;
          Sleep(100);
          PortEdit.Value:= 5000;
          Application.ProcessMessages;
          ServerSocket.Port := SettingsForm.PortEdit.Value;
          if sbServer.Down then ServerSocket.Open;
     end;
     FIniFile.WriteInteger('RadioModem', 'ServerPort', PortEdit.Value);
end;

procedure TSettingsForm.sbClose3Click(Sender: TObject);
begin
     with MainModemForm do
     begin
          ReceivedData:= '';
          if ServerSocket.Active then ServerSocket.Active := False;
          Sleep(100);
          Application.ProcessMessages;
          ServerSocket.Port := SettingsForm.PortEdit.Value;
          if sbServer.Down then ServerSocket.Open;
     end;
     FIniFile.WriteInteger('RadioModem', 'ServerPort', PortEdit.Value);
     Close;
end;

procedure TSettingsForm.FormDestroy(Sender: TObject);
begin
     if ServerSocket.Active then ServerSocket.Close;
end;

//****************************** Server Socket**********************************
procedure TSettingsForm.ServerSocketAccept(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' Accept';
end;

procedure TSettingsForm.ServerSocketClientConnect(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ClientConnect';
end;

procedure TSettingsForm.ServerSocketClientDisconnect(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ClientDisconnect';
end;

procedure TSettingsForm.ServerSocketClientError(Sender: TObject; Socket: TCustomWinSocket; ErrorEvent: TErrorEvent; var ErrorCode: Integer);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ClientError';
end;

procedure TSettingsForm.ServerSocketClientRead(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ClientRead';
     if MainModemForm.sbServer.Down then
     begin
          MainModemForm.MemoSMSFrom.Lines.Add( 'Data received from: ' + Socket.RemoteAddress + ' - ' + LocalDateNow);
          ReceivedData:= Socket.ReceiveText;
          SendMessage(MainModemForm.Handle, WM_USER+1, 0, 0);
     end;
end;

procedure TSettingsForm.ServerSocketClientWrite(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ClientWrite';
end;

procedure TSettingsForm.ServerSocketGetSocket(Sender: TObject; Socket: Integer; var ClientSocket: TServerClientWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' GetSocket';
end;

procedure TSettingsForm.ServerSocketGetThread(Sender: TObject; ClientSocket: TServerClientWinSocket; var SocketThread: TServerClientThread);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' GetThread';
end;

procedure TSettingsForm.ServerSocketListen(Sender: TObject; Socket: TCustomWinSocket);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' Listen';
end;

procedure TSettingsForm.ServerSocketThreadEnd(Sender: TObject; Thread: TServerClientThread);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ThreadEnd';
end;

procedure TSettingsForm.ServerSocketThreadStart(Sender: TObject; Thread: TServerClientThread);
begin
     MainModemForm.StatusBar.Panels[2].Text:= ' ThreadStart';
end;
//****************************** ...Server Socket*******************************

end.



