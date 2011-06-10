object ClientForm: TClientForm
  Left = 204
  Top = 111
  Width = 533
  Height = 145
  Caption = ' VAsja Console'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 65
    Width = 525
    Height = 53
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object sbConnect: TSpeedButton
      Left = 8
      Top = 6
      Width = 81
      Height = 21
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Connect'
      OnClick = sbConnectClick
    end
    object LabelTCPPort: TLabel
      Left = 240
      Top = 11
      Width = 22
      Height = 13
      Caption = 'Port:'
    end
    object Label1: TLabel
      Left = 96
      Top = 11
      Width = 25
      Height = 13
      Caption = 'Host:'
    end
    object EditIP: TEdit
      Left = 128
      Top = 6
      Width = 97
      Height = 21
      TabOrder = 0
      Text = '127.0.0.1'
    end
    object PortEdit: TSpinEdit
      Left = 272
      Top = 6
      Width = 65
      Height = 22
      MaxLength = 5
      MaxValue = 65535
      MinValue = 0
      TabOrder = 1
      Value = 5000
    end
    object StatusBar: TStatusBar
      Left = 0
      Top = 32
      Width = 525
      Height = 21
      Panels = <
        item
          Width = 225
        end
        item
          Width = 50
        end>
    end
    object Panel2: TPanel
      Left = 348
      Top = 0
      Width = 177
      Height = 32
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 3
      object SbClearTx: TSpeedButton
        Left = 0
        Top = 6
        Width = 81
        Height = 21
        BiDiMode = bdLeftToRight
        Caption = 'Clear Memo'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Layout = blGlyphRight
        ParentFont = False
        ParentBiDiMode = False
        OnClick = SbClearTxClick
      end
      object sbSend: TSpeedButton
        Left = 88
        Top = 6
        Width = 81
        Height = 21
        Caption = 'Send'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        OnClick = sbSendClick
      end
    end
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 525
    Height = 65
    Align = alClient
    BevelOuter = bvNone
    Color = clBlack
    Constraints.MinWidth = 525
    TabOrder = 1
    object MemosPanel: TPanel
      Left = 0
      Top = 0
      Width = 525
      Height = 65
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object Splitter2: TSplitter
        Left = 0
        Top = 30
        Width = 525
        Height = 5
        Cursor = crVSplit
        Align = alBottom
        Color = clBtnFace
        ParentColor = False
        ResizeStyle = rsUpdate
      end
      object MemoSMSFrom: TRichEdit
        Left = 0
        Top = 0
        Width = 525
        Height = 30
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        Ctl3D = True
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        HideSelection = False
        HideScrollBars = False
        Constraints.MinHeight = 30
        ParentCtl3D = False
        ParentFont = False
        PlainText = True
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object MemoSMS: TMemo
        Left = 0
        Top = 35
        Width = 525
        Height = 30
        Align = alBottom
        Constraints.MinHeight = 30
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        MaxLength = 4000
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
  end
  object ReconnectTimer: TTimer
    Enabled = False
    Interval = 10000
    OnTimer = ReconnectTimerTimer
    Left = 16
    Top = 8
  end
end
