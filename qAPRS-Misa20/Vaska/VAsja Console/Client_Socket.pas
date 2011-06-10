unit Client_Socket;

interface

uses SysUtils, Windows, Messages, Classes, WinSock, SyncObjs;

const
  CM_SOCKETMESSAGE = WM_USER + $0001;
  CM_DEFERFREE = WM_USER + $0002;
  CM_LOOKUPCOMPLETE = WM_USER + $0003;

type
  ESocketError = class(Exception);

  TCMSocketMessage = record
    Msg: Cardinal;
    Socket: TSocket;
    SelectEvent: Word;
    SelectError: Word;
    Result: Longint;
  end;

  TCMLookupComplete = record
    Msg: Cardinal;
    LookupHandle: THandle;
    AsyncBufLen: Word;
    AsyncError: Word;
    Result: Longint;
  end;

  TCustomWinSocket = class;
  TCustomSocket = class;



  TServerType = (stNonBlocking, stThreadBlocking);
  TClientType = (ctNonBlocking, ctBlocking);
  TAsyncStyle = (asRead, asWrite, asOOB, asAccept, asConnect, asClose);
  TAsyncStyles = set of TAsyncStyle;
  TSocketEvent = (seLookup, seConnecting, seConnect, seDisconnect, seListen,
    seAccept, seWrite, seRead);
  TLookupState = (lsIdle, lsLookupAddress, lsLookupService);
  TErrorEvent = (eeGeneral, eeSend, eeReceive, eeConnect, eeDisconnect, eeAccept, eeLookup);

  TSocketEventEvent = procedure (Sender: TObject; Socket: TCustomWinSocket;
    SocketEvent: TSocketEvent) of object;
  TSocketErrorEvent = procedure (Sender: TObject; Socket: TCustomWinSocket;
    ErrorEvent: TErrorEvent; var ErrorCode: Integer) of object;
  TSocketNotifyEvent = procedure (Sender: TObject; Socket: TCustomWinSocket) of object;

  TCustomWinSocket = class
  private
    FSocket: TSocket;
    FConnected: Boolean;
    FSendStream: TStream;
    FDropAfterSend: Boolean;
    FHandle: HWnd;
    FAddr: TSockAddrIn;
    FAsyncStyles: TASyncStyles;
    FLookupState: TLookupState;
    FLookupHandle: THandle;
    FOnSocketEvent: TSocketEventEvent;
    FOnErrorEvent: TSocketErrorEvent;
    FSocketLock: TCriticalSection;
    FGetHostData: Pointer;
    FData: Pointer;
    // Used during non-blocking host and service lookups
    FService: string;
    FPort: Word;
    FClient: Boolean;
    FQueueSize: Integer;
    function SendStreamPiece: Boolean;
    procedure WndProc(var Message: TMessage);
    procedure CMLookupComplete(var Message: TCMLookupComplete); message CM_LOOKUPCOMPLETE;
    procedure CMSocketMessage(var Message: TCMSocketMessage); message CM_SOCKETMESSAGE;
    procedure CMDeferFree(var Message); message CM_DEFERFREE;
    procedure DoSetAsyncStyles;
    function GetHandle: HWnd;
    function GetLocalHost: string;
    function GetLocalAddress: string;
    function GetLocalPort: Integer;
    function GetRemoteHost: string;
    function GetRemoteAddress: string;
    function GetRemotePort: Integer;
    function GetRemoteAddr: TSockAddrIn;
  protected
    procedure AsyncInitSocket(const Name, Address, Service: string; Port: Word;
      QueueSize: Integer; Client: Boolean);
    procedure DoOpen;
    procedure DoListen(QueueSize: Integer);
    function InitSocket(const Name, Address, Service: string; Port: Word;
      Client: Boolean): TSockAddrIn;
    procedure Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent); dynamic;
    procedure Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
      var ErrorCode: Integer); dynamic;
    procedure SetAsyncStyles(Value: TASyncStyles);
  public
    constructor Create(ASocket: TSocket);
    destructor Destroy; override;
    procedure Close;
    procedure DefaultHandler(var Message); override;
    procedure Lock;
    procedure Unlock;
    procedure Listen(const Name, Address, Service: string; Port: Word;
      QueueSize: Integer; Block: Boolean = True);
    procedure Open(const Name, Address, Service: string; Port: Word; Block: Boolean = True);
    procedure Accept(Socket: TSocket); virtual;
    procedure Connect(Socket: TSocket); virtual;
    procedure Disconnect(Socket: TSocket); virtual;
    procedure Read(Socket: TSocket); virtual;
    procedure Write(Socket: TSocket); virtual;
    function LookupName(const name: string): TInAddr;
    function LookupService(const service: string): Integer;

    function ReceiveLength: Integer;
    function ReceiveBuf(var Buf; Count: Integer): Integer;
    function ReceiveText: string;
    function SendBuf(var Buf; Count: Integer): Integer;
    function SendStream(AStream: TStream): Boolean;
    function SendStreamThenDrop(AStream: TStream): Boolean;
    function SendText(const S: string): Integer;

    property LocalHost: string read GetLocalHost;
    property LocalAddress: string read GetLocalAddress;
    property LocalPort: Integer read GetLocalPort;

    property RemoteHost: string read GetRemoteHost;
    property RemoteAddress: string read GetRemoteAddress;
    property RemotePort: Integer read GetRemotePort;
    property RemoteAddr: TSockAddrIn read GetRemoteAddr;

    property Connected: Boolean read FConnected;
    property Addr: TSockAddrIn read FAddr;
    property ASyncStyles: TAsyncStyles read FAsyncStyles write SetAsyncStyles;
    property Handle: HWnd read GetHandle;
    property SocketHandle: TSocket read FSocket;
    property LookupState: TLookupState read FLookupState;

    property OnSocketEvent: TSocketEventEvent read FOnSocketEvent write FOnSocketEvent;
    property OnErrorEvent: TSocketErrorEvent read FOnErrorEvent write FOnErrorEvent;

    property Data: Pointer read FData write FData;
  end;

  TClientWinSocket = class(TCustomWinSocket)
  private
    FClientType: TClientType;
  protected
    procedure SetClientType(Value: TClientType);
  public
    procedure Connect(Socket: TSocket); override;
    property ClientType: TClientType read FClientType write SetClientType;
  end;





  TAbstractSocket = class(TComponent)
  private
    FActive: Boolean;
    FPort: Integer;
    FAddress: string;
    FHost: string;
    FService: string;
    procedure DoEvent(Sender: TObject; Socket: TCustomWinSocket;
      SocketEvent: TSocketEvent);
    procedure DoError(Sender: TObject; Socket: TCustomWinSocket;
      ErrorEvent: TErrorEvent; var ErrorCode: Integer);
  protected
    procedure Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
      virtual; abstract;
    procedure Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
      var ErrorCode: Integer); virtual; abstract;
    procedure DoActivate(Value: Boolean); virtual; abstract;
    procedure InitSocket(Socket: TCustomWinSocket);
    procedure Loaded; override;
    procedure SetActive(Value: Boolean);
    procedure SetAddress(Value: string);
    procedure SetHost(Value: string);
    procedure SetPort(Value: Integer);
    procedure SetService(Value: string);
    property Active: Boolean read FActive write SetActive;
    property Address: string read FAddress write SetAddress;
    property Host: string read FHost write SetHost;
    property Port: Integer read FPort write SetPort;
    property Service: string read FService write SetService;
  public
    procedure Open;
    procedure Close;
  end;

  TCustomSocket = class(TAbstractSocket)
  private
    FOnLookup: TSocketNotifyEvent;
    FOnConnect: TSocketNotifyEvent;
    FOnConnecting: TSocketNotifyEvent;
    FOnDisconnect: TSocketNotifyEvent;
    FOnListen: TSocketNotifyEvent;
    FOnAccept: TSocketNotifyEvent;
    FOnRead: TSocketNotifyEvent;
    FOnWrite: TSocketNotifyEvent;
    FOnError: TSocketErrorEvent;
  protected
    procedure Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent); override;
    procedure Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
      var ErrorCode: Integer); override;
    property OnLookup: TSocketNotifyEvent read FOnLookup write FOnLookup;
    property OnConnecting: TSocketNotifyEvent read FOnConnecting write FOnConnecting;
    property OnConnect: TSocketNotifyEvent read FOnConnect write FOnConnect;
    property OnDisconnect: TSocketNotifyEvent read FOnDisconnect write FOnDisconnect;
    property OnListen: TSocketNotifyEvent read FOnListen write FOnListen;
    property OnAccept: TSocketNotifyEvent read FOnAccept write FOnAccept;
    property OnRead: TSocketNotifyEvent read FOnRead write FOnRead;
    property OnWrite: TSocketNotifyEvent read FOnWrite write FOnWrite;
    property OnError: TSocketErrorEvent read FOnError write FOnError;
  end;

  TWinSocketStream = class(TStream)
  private
    FSocket: TCustomWinSocket;
    FTimeout: Longint;
    FEvent: TSimpleEvent;
  public
    constructor Create(ASocket: TCustomWinSocket; TimeOut: Longint);
    destructor Destroy; override;
    function WaitForData(Timeout: Longint): Boolean;
    function Read(var Buffer; Count: Longint): Longint; override;
    function Write(const Buffer; Count: Longint): Longint; override;
    function Seek(Offset: Longint; Origin: Word): Longint; override;
    property TimeOut: Longint read FTimeout write FTimeout;
  end;

  TClientSocket = class(TCustomSocket)
  private
    FClientSocket: TClientWinSocket;
  protected
    procedure DoActivate(Value: Boolean); override;
    function GetClientType: TClientType;
    procedure SetClientType(Value: TClientType);
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    property Socket: TClientWinSocket read FClientSocket;
  published
    property Active;
    property Address;
    property ClientType: TClientType read GetClientType write SetClientType;
    property Host;
    property Port;
    property Service;
    property OnLookup;
    property OnConnecting;
    property OnConnect;
    property OnDisconnect;
    property OnRead;
    property OnWrite;
    property OnError;
  end;


  TSocketErrorProc = procedure (ErrorCode: Integer);

function SetErrorProc(ErrorProc: TSocketErrorProc): TSocketErrorProc;

implementation

uses RTLConsts, VClientConsole;

threadvar
  SocketErrorProc: TSocketErrorProc;

var
  WSAData: TWSAData;

function SetErrorProc(ErrorProc: TSocketErrorProc): TSocketErrorProc;
begin
  Result := SocketErrorProc;
  SocketErrorProc := ErrorProc;
end;

function CheckSocketResult(ResultCode: Integer; const Op: string): Integer;
begin
  if ResultCode <> 0 then
  begin
    Result := WSAGetLastError;
    if Result <> WSAEWOULDBLOCK then
      if Assigned(SocketErrorProc) then
        SocketErrorProc(Result)
      else raise ESocketError.CreateResFmt(@sWindowsSocketError,
        [SysErrorMessage(Result), Result, Op]);
  end else Result := 0;
end;

procedure Startup;
var
  ErrorCode: Integer;
begin
  ErrorCode := WSAStartup($0101, WSAData);
  if ErrorCode <> 0 then
    raise ESocketError.CreateResFmt(@sWindowsSocketError,
      [SysErrorMessage(ErrorCode), ErrorCode, 'WSAStartup']);
end;

procedure Cleanup;
var
  ErrorCode: Integer;
begin
  ErrorCode := WSACleanup;
  if ErrorCode <> 0 then
    raise ESocketError.CreateResFmt(@sWindowsSocketError,
      [SysErrorMessage(ErrorCode), ErrorCode, 'WSACleanup']);
end;

{ TCustomWinSocket }

constructor TCustomWinSocket.Create(ASocket: TSocket);
begin
  inherited Create;
  Startup;
  FSocketLock := TCriticalSection.Create;
  FASyncStyles := [asRead, asWrite, asConnect, asClose];
  FSocket := ASocket;
  FAddr.sin_family := PF_INET;
  FAddr.sin_addr.s_addr := INADDR_ANY;
  FAddr.sin_port := 0;
  FConnected := FSocket <> INVALID_SOCKET;
end;

destructor TCustomWinSocket.Destroy;
begin
  FOnSocketEvent := nil;  { disable events }
  if FConnected and (FSocket <> INVALID_SOCKET) then
    Disconnect(FSocket);
  if FHandle <> 0 then DeallocateHWnd(FHandle);
  FSocketLock.Free;
  Cleanup;
  FreeMem(FGetHostData);
  FGetHostData := nil;
  inherited Destroy;
end;

procedure TCustomWinSocket.Accept(Socket: TSocket);
begin
end;

procedure TCustomWinSocket.AsyncInitSocket(const Name, Address,
  Service: string; Port: Word; QueueSize: Integer; Client: Boolean);
var
  ErrorCode: Integer;
begin
  try
    case FLookupState of
      lsIdle:
        begin
          if not Client then
          begin
            FLookupState := lsLookupAddress;
            FAddr.sin_addr.S_addr := INADDR_ANY;
          end else if Name <> '' then
          begin
            if FGetHostData = nil then
              FGetHostData := AllocMem(MAXGETHOSTSTRUCT);
            FLookupHandle := WSAAsyncGetHostByName(Handle, CM_LOOKUPCOMPLETE,
              PChar(Name), FGetHostData, MAXGETHOSTSTRUCT);
            CheckSocketResult(Ord(FLookupHandle = 0), 'WSAASyncGetHostByName');
            FService := Service;
            FPort := Port;
            FQueueSize := QueueSize;
            FClient := Client;
            FLookupState := lsLookupAddress;
            Exit;
          end else if Address <> '' then
          begin
            FLookupState := lsLookupAddress;
            FAddr.sin_addr.S_addr := inet_addr(PChar(Address));
          end else
          begin
            ErrorCode := 1110;
            Error(Self, eeLookup, ErrorCode);
            Disconnect(FSocket);
            if ErrorCode <> 0 then
              raise ESocketError.CreateRes(@sNoAddress);
            Exit;
          end;
        end;
      lsLookupAddress:
        begin
          if Service <> '' then
          begin
            if FGetHostData = nil then
              FGetHostData := AllocMem(MAXGETHOSTSTRUCT);
            FLookupHandle := WSAASyncGetServByName(Handle, CM_LOOKUPCOMPLETE,
              PChar(Service), 'tcp' , FGetHostData, MAXGETHOSTSTRUCT);
            CheckSocketResult(Ord(FLookupHandle = 0), 'WSAASyncGetServByName');
            FLookupState := lsLookupService;
            Exit;
          end else
          begin
            FLookupState := lsLookupService;
            FAddr.sin_port := htons(Port);
          end;
        end;
      lsLookupService:
        begin
          FLookupState := lsIdle;
          if Client then
            DoOpen
          else DoListen(QueueSize);
        end;
    end;
    if FLookupState <> lsIdle then
      ASyncInitSocket(Name, Address, Service, Port, QueueSize, Client);
  except
    Disconnect(FSocket);
    raise;
  end;
end;

procedure TCustomWinSocket.Close;
begin
  Disconnect(FSocket);
end;

procedure TCustomWinSocket.Connect(Socket: TSocket);
begin
end;

procedure TCustomWinSocket.Lock;
begin
  FSocketLock.Enter;
end;

procedure TCustomWinSocket.Unlock;
begin
  FSocketLock.Leave;
end;

procedure TCustomWinSocket.CMSocketMessage(var Message: TCMSocketMessage);

  function CheckError: Boolean;
  var
    ErrorEvent: TErrorEvent;
    ErrorCode: Integer;
  begin
    if Message.SelectError <> 0 then
    begin
      Result := False;
      ErrorCode := Message.SelectError;
      case Message.SelectEvent of
        FD_CONNECT: ErrorEvent := eeConnect;
        FD_CLOSE: ErrorEvent := eeDisconnect;
        FD_READ: ErrorEvent := eeReceive;
        FD_WRITE: ErrorEvent := eeSend;
        FD_ACCEPT: ErrorEvent := eeAccept;
      else
        ErrorEvent := eeGeneral;
      end;
      Error(Self, ErrorEvent, ErrorCode);
      if ErrorCode <> 0 then
        //raise ESocketError.CreateResFmt(@sASyncSocketError, [ErrorCode]);
        ClientForm.StatusBar.Panels[1].Text:= ' ' + sASyncSocketError + IntToStr(ErrorCode);
    end else Result := True;
  end;

begin
  with Message do
    if CheckError then
      case SelectEvent of
        FD_CONNECT: Connect(Socket);
        FD_CLOSE: Disconnect(Socket);
        FD_READ: Read(Socket);
        FD_WRITE: Write(Socket);
        FD_ACCEPT: Accept(Socket);
      end;
end;

procedure TCustomWinSocket.CMDeferFree(var Message);
begin
  Free;
end;

procedure TCustomWinSocket.DoSetAsyncStyles;
var
  Msg: Integer;
  Wnd: HWnd;
  Blocking: Longint;
begin
  Msg := 0;
  Wnd := 0;
  if FAsyncStyles <> [] then
  begin
    Msg := CM_SOCKETMESSAGE;
    Wnd := Handle;
  end;
  WSAAsyncSelect(FSocket, Wnd, Msg, Longint(Byte(FAsyncStyles)));
  if FASyncStyles = [] then
  begin
    Blocking := 0;
    ioctlsocket(FSocket, FIONBIO, Blocking);
  end;
end;

procedure TCustomWinSocket.DoListen(QueueSize: Integer);
begin
  CheckSocketResult(bind(FSocket, FAddr, SizeOf(FAddr)), 'bind');
  DoSetASyncStyles;
  if QueueSize > SOMAXCONN then QueueSize := SOMAXCONN;
  Event(Self, seListen);
  CheckSocketResult(Winsock.listen(FSocket, QueueSize), 'listen');
  FLookupState := lsIdle;
  FConnected := True;
end;

procedure TCustomWinSocket.DoOpen;
begin
  DoSetASyncStyles;
  Event(Self, seConnecting);
  CheckSocketResult(WinSock.connect(FSocket, FAddr, SizeOf(FAddr)), 'connect');
  FLookupState := lsIdle;
  if not (asConnect in FAsyncStyles) then
  begin
    FConnected := FSocket <> INVALID_SOCKET;
    Event(Self, seConnect);
  end;
end;

function TCustomWinSocket.GetHandle: HWnd;
begin
  if FHandle = 0 then
    FHandle := AllocateHwnd(WndProc);
  Result := FHandle;
end;

function TCustomWinSocket.GetLocalAddress: string;
var
  SockAddrIn: TSockAddrIn;
  Size: Integer;
begin
  Lock;
  try
    Result := '';
    if FSocket = INVALID_SOCKET then Exit;
    Size := SizeOf(SockAddrIn);
    if getsockname(FSocket, SockAddrIn, Size) = 0 then
      Result := inet_ntoa(SockAddrIn.sin_addr);
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetLocalHost: string;
var
  LocalName: array[0..255] of Char;
begin
  Lock;
  try
    Result := '';
    if FSocket = INVALID_SOCKET then Exit;
    if gethostname(LocalName, SizeOf(LocalName)) = 0 then
      Result := LocalName;
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetLocalPort: Integer;
var
  SockAddrIn: TSockAddrIn;
  Size: Integer;
begin
  Lock;
  try
    Result := -1;
    if FSocket = INVALID_SOCKET then Exit;
    Size := SizeOf(SockAddrIn);
    if getsockname(FSocket, SockAddrIn, Size) = 0 then
      Result := ntohs(SockAddrIn.sin_port);
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetRemoteHost: string;
var
  SockAddrIn: TSockAddrIn;
  Size: Integer;
  HostEnt: PHostEnt;
begin
  Lock;
  try
    Result := '';
    if not FConnected then Exit;
    Size := SizeOf(SockAddrIn);
    CheckSocketResult(getpeername(FSocket, SockAddrIn, Size), 'getpeername');
    HostEnt := gethostbyaddr(@SockAddrIn.sin_addr.s_addr, 4, PF_INET);
    if HostEnt <> nil then Result := HostEnt.h_name;
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetRemoteAddress: string;
var
  SockAddrIn: TSockAddrIn;
  Size: Integer;
begin
  Lock;
  try
    Result := '';
    if not FConnected then Exit;
    Size := SizeOf(SockAddrIn);
    CheckSocketResult(getpeername(FSocket, SockAddrIn, Size), 'getpeername');
    Result := inet_ntoa(SockAddrIn.sin_addr);
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetRemotePort: Integer;
var
  SockAddrIn: TSockAddrIn;
  Size: Integer;
begin
  Lock;
  try
    Result := 0;
    if not FConnected then Exit;
    Size := SizeOf(SockAddrIn);
    CheckSocketResult(getpeername(FSocket, SockAddrIn, Size), 'getpeername');
    Result := ntohs(SockAddrIn.sin_port);
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.GetRemoteAddr: TSockAddrIn;
var
  Size: Integer;
begin
  Lock;
  try
    FillChar(Result, SizeOf(Result), 0);
    if not FConnected then Exit;
    Size := SizeOf(Result);
    if getpeername(FSocket, Result, Size) <> 0 then
      FillChar(Result, SizeOf(Result), 0);
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.LookupName(const Name: string): TInAddr;
var
  HostEnt: PHostEnt;
  InAddr: TInAddr;
begin
  HostEnt := gethostbyname(PChar(Name));
  FillChar(InAddr, SizeOf(InAddr), 0);
  if HostEnt <> nil then
  begin
    with InAddr, HostEnt^ do
    begin
      S_un_b.s_b1 := h_addr^[0];
      S_un_b.s_b2 := h_addr^[1];
      S_un_b.s_b3 := h_addr^[2];
      S_un_b.s_b4 := h_addr^[3];
    end;
  end;
  Result := InAddr;
end;

function TCustomWinSocket.LookupService(const Service: string): Integer;
var
  ServEnt: PServEnt;
begin
  ServEnt := getservbyname(PChar(Service), 'tcp');
  if ServEnt <> nil then
    Result := ntohs(ServEnt.s_port)
  else Result := 0;
end;

function TCustomWinSocket.InitSocket(const Name, Address, Service: string; Port: Word;
  Client: Boolean): TSockAddrIn;
begin
  Result.sin_family := PF_INET;
  if Name <> '' then
    Result.sin_addr := LookupName(name)
  else if Address <> '' then
    Result.sin_addr.s_addr := inet_addr(PChar(Address))
  else if not Client then
    Result.sin_addr.s_addr := INADDR_ANY
  else raise ESocketError.CreateRes(@sNoAddress);
  if Service <> '' then
    Result.sin_port := htons(LookupService(Service))
  else
    Result.sin_port := htons(Port);
end;

procedure TCustomWinSocket.Listen(const Name, Address, Service: string; Port: Word;
  QueueSize: Integer; Block: Boolean);
begin
  if FConnected then raise ESocketError.CreateRes(@sCannotListenOnOpen);
  FSocket := socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
  if FSocket = INVALID_SOCKET then raise ESocketError.CreateRes(@sCannotCreateSocket);
  try
    Event(Self, seLookUp);
    if Block then
    begin
      FAddr := InitSocket(Name, Address, Service, Port, False);
      DoListen(QueueSize);
    end else
      AsyncInitSocket(Name, Address, Service, Port, QueueSize, False);
  except
    Disconnect(FSocket);
    raise;
  end;
end;

procedure TCustomWinSocket.Open(const Name, Address, Service: string; Port: Word; Block: Boolean);
begin
  if FConnected then raise ESocketError.CreateRes(@sSocketAlreadyOpen);
  FSocket := socket(PF_INET, SOCK_STREAM, IPPROTO_IP);
  if FSocket = INVALID_SOCKET then raise ESocketError.CreateRes(@sCannotCreateSocket);
  try
    Event(Self, seLookUp);
    if Block then
    begin
      FAddr := InitSocket(Name, Address, Service, Port, True);
      DoOpen;
    end else
      AsyncInitSocket(Name, Address, Service, Port, 0, True);
  except
    Disconnect(FSocket);
    raise;
  end;
end;

procedure TCustomWinSocket.Disconnect(Socket: TSocket);
begin
  Lock;
  try
    if FLookupHandle <> 0 then
      CheckSocketResult(WSACancelASyncRequest(FLookupHandle), 'WSACancelASyncRequest');
    FLookupHandle := 0;
    if (Socket = INVALID_SOCKET) or (Socket <> FSocket) then exit;
    Event(Self, seDisconnect);
    CheckSocketResult(closesocket(FSocket), 'closesocket');
    FSocket := INVALID_SOCKET;
    FAddr.sin_family := PF_INET;
    FAddr.sin_addr.s_addr := INADDR_ANY;
    FAddr.sin_port := 0;
    FConnected := False;
    FreeAndNil(FSendStream);
  finally
    Unlock;
  end;
end;

procedure TCustomWinSocket.DefaultHandler(var Message);
begin
  with TMessage(Message) do
    if FHandle <> 0 then
      Result := CallWindowProc(@DefWindowProc, FHandle, Msg, wParam, lParam);
end;

procedure TCustomWinSocket.Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
begin
  if Assigned(FOnSocketEvent) then FOnSocketEvent(Self, Socket, SocketEvent);
end;

procedure TCustomWinSocket.Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
  var ErrorCode: Integer);
begin
  if Assigned(FOnErrorEvent) then FOnErrorEvent(Self, Socket, ErrorEvent, ErrorCode);
end;

function TCustomWinSocket.SendText(const s: string): Integer;
begin
  Result := SendBuf(Pointer(S)^, Length(S));
end;

function TCustomWinSocket.SendStreamPiece: Boolean;
var
  Buffer: array[0..4095] of Byte;
  StartPos: Integer;
  AmountInBuf: Integer;
  AmountSent: Integer;
  ErrorCode: Integer;

  procedure DropStream;
  begin
    if FDropAfterSend then Disconnect(FSocket);
    FDropAfterSend := False;
    FSendStream.Free;
    FSendStream := nil;
  end;

begin
  Lock;
  try
    Result := False;
    if FSendStream <> nil then
    begin
      if (FSocket = INVALID_SOCKET) or (not FConnected) then exit;
      while True do
      begin
        StartPos := FSendStream.Position;
        AmountInBuf := FSendStream.Read(Buffer, SizeOf(Buffer));
        if AmountInBuf > 0 then
        begin
          AmountSent := send(FSocket, Buffer, AmountInBuf, 0);
          if AmountSent = SOCKET_ERROR then
          begin
            ErrorCode := WSAGetLastError;
            if ErrorCode <> WSAEWOULDBLOCK then
            begin
              Error(Self, eeSend, ErrorCode);
              Disconnect(FSocket);
              DropStream;
              if FAsyncStyles <> [] then Abort;
              Break;
            end else
            begin
              FSendStream.Position := StartPos;
              Break;
            end;
          end else if AmountInBuf > AmountSent then
            FSendStream.Position := StartPos + AmountSent
          else if FSendStream.Position = FSendStream.Size then
          begin
            DropStream;
            Break;
          end;
        end else
        begin
          DropStream;
          Break;
        end;
      end;
      Result := True;
    end;
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.SendStream(AStream: TStream): Boolean;
begin
  Result := False;
  if FSendStream = nil then
  begin
    FSendStream := AStream;
    Result := SendStreamPiece;
  end;
end;

function TCustomWinSocket.SendStreamThenDrop(AStream: TStream): Boolean;
begin
  FDropAfterSend := True;
  Result := SendStream(AStream);
  if not Result then FDropAfterSend := False;
end;

function TCustomWinSocket.SendBuf(var Buf; Count: Integer): Integer;
var
  ErrorCode: Integer;
begin
  Lock;
  try
    Result := 0;
    if not FConnected then Exit;
    Result := send(FSocket, Buf, Count, 0);
    if Result = SOCKET_ERROR then
    begin
      ErrorCode := WSAGetLastError;
      if (ErrorCode <> WSAEWOULDBLOCK) then
      begin
        Error(Self, eeSend, ErrorCode);
        Disconnect(FSocket);
        if ErrorCode <> 0 then
          raise ESocketError.CreateResFmt(@sWindowsSocketError,
            [SysErrorMessage(ErrorCode), ErrorCode, 'send']);
      end;
    end;
  finally
    Unlock;
  end;
end;

procedure TCustomWinSocket.SetAsyncStyles(Value: TASyncStyles);
begin
  if Value <> FASyncStyles then
  begin
    FASyncStyles := Value;
    if FSocket <> INVALID_SOCKET then
      DoSetAsyncStyles;
  end;
end;

procedure TCustomWinSocket.Read(Socket: TSocket);
begin
  if (FSocket = INVALID_SOCKET) or (Socket <> FSocket) then Exit;
  Event(Self, seRead);
end;

function TCustomWinSocket.ReceiveBuf(var Buf; Count: Integer): Integer;
var
  ErrorCode: Integer;
begin
  Lock;
  try
    Result := 0;
    if (Count = -1) and FConnected then
      ioctlsocket(FSocket, FIONREAD, Longint(Result))
    else begin
      if not FConnected then Exit;
      Result := recv(FSocket, Buf, Count, 0);
      if Result = SOCKET_ERROR then
      begin
        ErrorCode := WSAGetLastError;
        if ErrorCode <> WSAEWOULDBLOCK then
        begin
          Error(Self, eeReceive, ErrorCode);
          Disconnect(FSocket);
          if ErrorCode <> 0 then
            raise ESocketError.CreateResFmt(@sWindowsSocketError,
              [SysErrorMessage(ErrorCode), ErrorCode, 'recv']);
        end;
      end;
    end;
  finally
    Unlock;
  end;
end;

function TCustomWinSocket.ReceiveLength: Integer;
begin
  Result := ReceiveBuf(Pointer(nil)^, -1);
end;

function TCustomWinSocket.ReceiveText: string;
begin
  SetLength(Result, ReceiveBuf(Pointer(nil)^, -1));
  SetLength(Result, ReceiveBuf(Pointer(Result)^, Length(Result)));
end;

procedure TCustomWinSocket.WndProc(var Message: TMessage);
begin
  try
    Dispatch(Message);
  except
    if Assigned(ApplicationHandleException) then
      ApplicationHandleException(Self);
  end;
end;

procedure TCustomWinSocket.Write(Socket: TSocket);
begin
  if (FSocket = INVALID_SOCKET) or (Socket <> FSocket) then Exit;
  if not SendStreamPiece then Event(Self, seWrite);
end;

procedure TCustomWinSocket.CMLookupComplete(var Message: TCMLookupComplete);
var
  ErrorCode: Integer;
begin
  if Message.LookupHandle = FLookupHandle then
  begin
    FLookupHandle := 0;
    if Message.AsyncError <> 0 then
    begin
      ErrorCode := Message.AsyncError;
      Error(Self, eeLookup, ErrorCode);
      Disconnect(FSocket);
      if ErrorCode <> 0 then
        raise ESocketError.CreateResFmt(@sWindowsSocketError,
          [SysErrorMessage(Message.AsyncError), Message.ASyncError, 'ASync Lookup']);
      Exit;
    end;
    if FLookupState = lsLookupAddress then
    begin
      FAddr.sin_addr.S_addr := Integer(Pointer(PHostEnt(FGetHostData).h_addr^)^);
      ASyncInitSocket('', '', FService, FPort, FQueueSize, FClient);
    end else if FLookupState = lsLookupService then
    begin
      FAddr.sin_port := PServEnt(FGetHostData).s_port;
      FPort := 0;
      FService := '';
      ASyncInitSocket('', '', '', 0, FQueueSize, FClient);
    end;
  end;
end;

{ TClientWinSocket }

procedure TClientWinSocket.Connect(Socket: TSocket);
begin
  FConnected := True;
  Event(Self, seConnect);
end;

procedure TClientWinSocket.SetClientType(Value: TClientType);
begin
  if Value <> FClientType then
    if not FConnected then
    begin
      FClientType := Value;
      if FClientType = ctBlocking then
        ASyncStyles := []
      else ASyncStyles := [asRead, asWrite, asConnect, asClose];
    end else raise ESocketError.CreateRes(@sCantChangeWhileActive);
end;


{ TAbstractSocket }

procedure TAbstractSocket.DoEvent(Sender: TObject; Socket: TCustomWinSocket;
  SocketEvent: TSocketEvent);
begin
  Event(Socket, SocketEvent);
end;

procedure TAbstractSocket.DoError(Sender: TObject; Socket: TCustomWinSocket;
  ErrorEvent: TErrorEvent; var ErrorCode: Integer);
begin
  Error(Socket, ErrorEvent, ErrorCode);
end;

procedure TAbstractSocket.SetActive(Value: Boolean);
begin
  if Value <> FActive then
  begin
    if (csDesigning in ComponentState) or (csLoading in ComponentState) then
      FActive := Value;
    if not (csLoading in ComponentState) then
      DoActivate(Value);
  end;
end;

procedure TAbstractSocket.InitSocket(Socket: TCustomWinSocket);
begin
  Socket.OnSocketEvent := DoEvent;
  Socket.OnErrorEvent := DoError;
end;

procedure TAbstractSocket.Loaded;
begin
  inherited Loaded;
  DoActivate(FActive);
end;

procedure TAbstractSocket.SetAddress(Value: string);
begin
  if CompareText(Value, FAddress) <> 0 then
  begin
    if not (csLoading in ComponentState) and FActive then
      raise ESocketError.CreateRes(@sCantChangeWhileActive);
    FAddress := Value;
  end;
end;

procedure TAbstractSocket.SetHost(Value: string);
begin
  if CompareText(Value, FHost) <> 0 then
  begin
    if not (csLoading in ComponentState) and FActive then
      raise ESocketError.CreateRes(@sCantChangeWhileActive);
    FHost := Value;
  end;
end;

procedure TAbstractSocket.SetPort(Value: Integer);
begin
  if FPort <> Value then
  begin
    if not (csLoading in ComponentState) and FActive then
      raise ESocketError.CreateRes(@sCantChangeWhileActive);
    FPort := Value;
  end;
end;

procedure TAbstractSocket.SetService(Value: string);
begin
  if CompareText(Value, FService) <> 0 then
  begin
    if not (csLoading in ComponentState) and FActive then
      raise ESocketError.CreateRes(@sCantChangeWhileActive);
    FService := Value;
  end;
end;

procedure TAbstractSocket.Open;
begin
  Active := True;
end;

procedure TAbstractSocket.Close;
begin
  Active := False;
end;

{ TCustomSocket }

procedure TCustomSocket.Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
begin
  case SocketEvent of
    seLookup: if Assigned(FOnLookup) then FOnLookup(Self, Socket);
    seConnecting: if Assigned(FOnConnecting) then FOnConnecting(Self, Socket);
    seConnect:
      begin
        FActive := True;
        if Assigned(FOnConnect) then FOnConnect(Self, Socket);
      end;
    seListen:
      begin
        FActive := True;
        if Assigned(FOnListen) then FOnListen(Self, Socket);
      end;
    seDisconnect:
      begin
        FActive := False;
        if Assigned(FOnDisconnect) then FOnDisconnect(Self, Socket);
      end;
    seAccept: if Assigned(FOnAccept) then FOnAccept(Self, Socket);
    seRead: if Assigned(FOnRead) then FOnRead(Self, Socket);
    seWrite: if Assigned(FOnWrite) then FOnWrite(Self, Socket);
  end;
end;

procedure TCustomSocket.Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
  var ErrorCode: Integer);
begin
  if Assigned(FOnError) then FOnError(Self, Socket, ErrorEvent, ErrorCode);
end;

{ TWinSocketStream }

constructor TWinSocketStream.Create(ASocket: TCustomWinSocket; TimeOut: Longint);
begin
  if ASocket.ASyncStyles <> [] then
    raise ESocketError.CreateRes(@sSocketMustBeBlocking);
  FSocket := ASocket;
  FTimeOut := TimeOut;
  FEvent := TSimpleEvent.Create;
  inherited Create;
end;

destructor TWinSocketStream.Destroy;
begin
  FEvent.Free;
  inherited Destroy;
end;

function TWinSocketStream.WaitForData(Timeout: Longint): Boolean;
var
  FDSet: TFDSet;
  TimeVal: TTimeVal;
begin
  TimeVal.tv_sec := Timeout div 1000;
  TimeVal.tv_usec := (Timeout mod 1000) * 1000;
  FD_ZERO(FDSet);
  FD_SET(FSocket.SocketHandle, FDSet);
  Result := select(0, @FDSet, nil, nil, @TimeVal) > 0;
end;

function TWinSocketStream.Read(var Buffer; Count: Longint): Longint;
var
  Overlapped: TOverlapped;
  ErrorCode: Integer;
begin
  FSocket.Lock;
  try
    FillChar(OVerlapped, SizeOf(Overlapped), 0);
    Overlapped.hEvent := FEvent.Handle;
    if not ReadFile(FSocket.SocketHandle, Buffer, Count, DWORD(Result),
      @Overlapped) and (GetLastError <> ERROR_IO_PENDING) then
    begin
      ErrorCode := GetLastError;
      raise ESocketError.CreateResFmt(@sSocketIOError, [sSocketRead, ErrorCode,
        SysErrorMessage(ErrorCode)]);
    end;
    if FEvent.WaitFor(FTimeOut) <> wrSignaled then
      Result := 0
    else
    begin
      GetOverlappedResult(FSocket.SocketHandle, Overlapped, DWORD(Result), False);
      FEvent.ResetEvent;
    end;
  finally
    FSocket.Unlock;
  end;
end;

function TWinSocketStream.Write(const Buffer; Count: Longint): Longint;
var
  Overlapped: TOverlapped;
  ErrorCode: Integer;
begin
  FSocket.Lock;
  try
    FillChar(OVerlapped, SizeOf(Overlapped), 0);
    Overlapped.hEvent := FEvent.Handle;
    if not WriteFile(FSocket.SocketHandle, Buffer, Count, DWORD(Result),
      @Overlapped) and (GetLastError <> ERROR_IO_PENDING) then
    begin
      ErrorCode := GetLastError;
      raise ESocketError.CreateResFmt(@sSocketIOError, [sSocketWrite, ErrorCode,
        SysErrorMessage(ErrorCode)]);
    end;
    if FEvent.WaitFor(FTimeOut) <> wrSignaled then
      Result := 0
    else GetOverlappedResult(FSocket.SocketHandle, Overlapped, DWORD(Result), False);
  finally
    FSocket.Unlock;
  end;
end;

function TWinSocketStream.Seek(Offset: Longint; Origin: Word): Longint;
begin
  Result := 0;
end;

{ TClientSocket }

constructor TClientSocket.Create(AOwner: TComponent);
begin
  inherited Create(AOwner);
  FClientSocket := TClientWinSocket.Create(INVALID_SOCKET);
  InitSocket(FClientSocket);
end;

destructor TClientSocket.Destroy;
begin
  FClientSocket.Free;
  inherited Destroy;
end;

procedure TClientSocket.DoActivate(Value: Boolean);
begin
  if (Value <> FClientSocket.Connected) and not (csDesigning in ComponentState) then
  begin
    if FClientSocket.Connected then
      FClientSocket.Disconnect(FClientSocket.FSocket)
    else FClientSocket.Open(FHost, FAddress, FService, FPort, ClientType = ctBlocking);
  end;
end;

function TClientSocket.GetClientType: TClientType;
begin
  Result := FClientSocket.ClientType;
end;

procedure TClientSocket.SetClientType(Value: TClientType);
begin
  FClientSocket.ClientType := Value;
end;



end.