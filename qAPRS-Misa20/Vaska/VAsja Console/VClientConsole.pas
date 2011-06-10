unit VClientConsole;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Client_Socket, StdCtrls, ComCtrls, ExtCtrls, Spin, Buttons, IniFiles;

type
  TClientForm = class(TForm)
    Panel1: TPanel;
    sbConnect: TSpeedButton;
    LabelTCPPort: TLabel;
    EditIP: TEdit;
    PortEdit: TSpinEdit;
    StatusBar: TStatusBar;
    MainPanel: TPanel;
    MemosPanel: TPanel;
    Splitter2: TSplitter;
    MemoSMSFrom: TRichEdit;
    MemoSMS: TMemo;
    Panel2: TPanel;
    SbClearTx: TSpeedButton;
    sbSend: TSpeedButton;
    Label1: TLabel;
    ReconnectTimer: TTimer;
    procedure FormCreate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure FormDestroy(Sender: TObject);
    procedure sbConnectClick(Sender: TObject);
    procedure sbSendClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure SbClearTxClick(Sender: TObject);
    procedure ReconnectTimerTimer(Sender: TObject);
  private
    { Private declarations }
    FIniFile          :  TIniFile;
    ClientSocket      : TClientSocket;

    procedure ClientSocketConnect(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocketConnecting(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocketDisconnect(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocketError(Sender: TObject; Socket: TCustomWinSocket; ErrorEvent: TErrorEvent; var ErrorCode: Integer);
    procedure ClientSocketLookup(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocketRead(Sender: TObject; Socket: TCustomWinSocket);
    procedure ClientSocketWrite(Sender: TObject; Socket: TCustomWinSocket);

  public
    { Public declarations }
  end;

var
  ClientForm: TClientForm;

implementation

{$R *.dfm}

//******************************* Client Socket ********************************
procedure TClientForm.ClientSocketConnect(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Connect';
end;

procedure TClientForm.ClientSocketConnecting(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Connecting';
end;

procedure TClientForm.ClientSocketDisconnect(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Disconnect';
     if sbConnect.Down then ReconnectTimer.Enabled:= True;
end;

procedure TClientForm.ClientSocketError(Sender: TObject; Socket: TCustomWinSocket; ErrorEvent: TErrorEvent; var ErrorCode: Integer);
begin
     StatusBar.Panels[0].Text:= ' Error';
     if sbConnect.Down then ReconnectTimer.Enabled:= True;
end;

procedure TClientForm.ClientSocketLookup(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Lookup';
end;

procedure TClientForm.ClientSocketRead(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Read';
     MemoSMSFrom.Lines.Add(Socket.ReceiveText);
end;

procedure TClientForm.ClientSocketWrite(Sender: TObject; Socket: TCustomWinSocket);
begin
     StatusBar.Panels[0].Text:= ' Write';
end;
//******************************* ...Socket ********************************

procedure TClientForm.FormCreate(Sender: TObject);
begin
     FIniFile := TIniFile.Create(ExtractFilePath(Application.ExeName) + 'VConsole.ini');
     ClientSocket:= TClientSocket.Create(self);
     ClientSocket.OnConnect:= ClientSocketConnect;
     ClientSocket.OnConnecting:= ClientSocketConnecting;
     ClientSocket.OnDisconnect:= ClientSocketDisconnect;
     ClientSocket.OnError:= ClientSocketError;
     ClientSocket.OnLookup:= ClientSocketLookup;
     ClientSocket.OnRead:= ClientSocketRead;
     ClientSocket.OnWrite:= ClientSocketWrite;
     Left:= FIniFile.ReadInteger('Console', 'Left', 0);
     Top:= FIniFile.ReadInteger('Console', 'Top', 0);
     Width:= FIniFile.ReadInteger('Console', 'Width', 0);
     Height:= FIniFile.ReadInteger('Console', 'Height', 0);
     EditIP.Text:= FIniFile.ReadString('Console', 'HostIP', '127.0.0.1');
     PortEdit.Value:= FIniFile.ReadInteger('Console', 'Port', 5000);
     if (Top > Screen.Height - Height div 2) or (Top < 0) or (Left > Screen.Width - Width)  or (Left < -250) then Position:= poScreenCenter;
end;

procedure TClientForm.FormCloseQuery(Sender: TObject;
  var CanClose: Boolean);
begin
     if ClientSocket.Active then ClientSocket.Close;
     if WindowState <> wsMaximized then
     begin
          FIniFile.WriteInteger('Console', 'Left', Left);
          FIniFile.WriteInteger('Console', 'Top', Top);
          FIniFile.WriteInteger('Console', 'Width', Width);
          FIniFile.WriteInteger('Console', 'Height', Height);
          FIniFile.WriteInteger('Console', 'SMSHeight', MemoSMS.Height);
     end;
     FIniFile.WriteString('Console', 'HostIP', EditIP.Text);
     FIniFile.WriteInteger('Console', 'Port', PortEdit.Value);
end;

procedure TClientForm.FormDestroy(Sender: TObject);
begin
     ClientSocket.Destroy;
     FIniFile.Free;
end;

procedure TClientForm.sbConnectClick(Sender: TObject);
begin
     ReconnectTimer.Enabled:= False;
     StatusBar.Panels[1].Text:= '';
     if ClientSocket.Active then ClientSocket.Active:= False;
     Application.ProcessMessages;
     ClientSocket.Port:= PortEdit.Value;
     ClientSocket.Host:= EditIP.Text;
     sbSend.Enabled:= sbConnect.Down;
     EditIP.Enabled:= not sbConnect.Down;
     PortEdit.Enabled:= not sbConnect.Down;
     ClientSocket.Active:= sbConnect.Down;
end;

procedure TClientForm.sbSendClick(Sender: TObject);
begin
     StatusBar.Panels[0].Text:= ' Write';
     ClientSocket.Socket.SendText(MemoSMS.Text);
end;

procedure TClientForm.FormShow(Sender: TObject);
begin
     MemoSMS.Height:= FIniFile.ReadInteger('Console', 'SMSHeight', 30);
     MemoSMS.SetFocus;
end;

procedure TClientForm.SbClearTxClick(Sender: TObject);
begin
     MemoSMSFrom.Clear;
     MemoSMS.Clear;
end;

procedure TClientForm.ReconnectTimerTimer(Sender: TObject);
begin
     StatusBar.Panels[1].Text:= '';
     ClientSocket.Active:= sbConnect.Down;
end;

end.
