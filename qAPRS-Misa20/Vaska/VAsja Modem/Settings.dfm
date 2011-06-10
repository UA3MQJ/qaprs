object SettingsForm: TSettingsForm
  Left = 635
  Top = 356
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
  ClientHeight = 179
  ClientWidth = 442
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl: TPageControl
    Left = 0
    Top = 0
    Width = 442
    Height = 179
    ActivePage = TabSheet3
    Align = alClient
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = #1047#1074#1091#1082#1086#1074#1099#1077' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
      ImageIndex = 1
      object Label2: TLabel
        Left = 8
        Top = 6
        Width = 170
        Height = 13
        Caption = #1042#1099#1073#1086#1088' '#1047#1074#1091#1082#1086#1074#1086#1081' '#1050#1072#1088#1090#1099' '#1076#1083#1103' '#1055#1056#1052':'
      end
      object Label3: TLabel
        Left = 8
        Top = 48
        Width = 170
        Height = 13
        Caption = #1042#1099#1073#1086#1088' '#1047#1074#1091#1082#1086#1074#1086#1081' '#1050#1072#1088#1090#1099' '#1076#1083#1103' '#1055#1056#1044':'
      end
      object SbDefSound: TSpeedButton
        Left = 360
        Top = 96
        Width = 73
        Height = 22
        Caption = #1057#1073#1088#1086#1089
        Layout = blGlyphRight
        OnClick = SbDefSoundClick
      end
      object Label6: TLabel
        Left = 240
        Top = 6
        Width = 142
        Height = 13
        Caption = #1063#1091#1074#1089#1090#1074#1080#1090#1077#1083#1100#1085#1086#1089#1090#1100' '#1057#1087#1077#1082#1090#1088#1072':'
      end
      object LabelvRX: TLabel
        Left = 368
        Top = 34
        Width = 12
        Height = 13
        Caption = '70'
      end
      object sbClose1: TSpeedButton
        Left = 360
        Top = 124
        Width = 73
        Height = 22
        Caption = 'Ok'
        Layout = blGlyphRight
        OnClick = sbClose1Click
      end
      object CbWaveInDevice: TComboBox
        Left = 8
        Top = 20
        Width = 217
        Height = 21
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        TabOrder = 0
        OnChange = CbWaveInDeviceChange
      end
      object CbWaveOutDevice: TComboBox
        Left = 8
        Top = 64
        Width = 217
        Height = 21
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        TabOrder = 1
        OnChange = CbWaveOutDeviceChange
      end
      object cbVox: TCheckBox
        Left = 8
        Top = 123
        Width = 193
        Height = 17
        Caption = 'vox'
        TabOrder = 2
      end
      object tbRXPerception: TTrackBar
        Left = 234
        Top = 22
        Width = 127
        Height = 30
        Max = 100
        Min = 20
        Frequency = 5
        Position = 50
        TabOrder = 3
        ThumbLength = 15
        TickMarks = tmBoth
        OnChange = tbRXPerceptionChange
      end
      object CbVector: TCheckBox
        Left = 8
        Top = 99
        Width = 169
        Height = 17
        Caption = 'phase'
        Checked = True
        State = cbChecked
        TabOrder = 4
        OnClick = CbVectorClick
      end
    end
    object TabSheet3: TTabSheet
      Caption = #1059#1087#1088#1072#1074#1083#1077#1085#1080#1077' '#1087#1077#1088#1077#1076#1072#1095#1077#1081
      ImageIndex = 2
      object LabelPort: TLabel
        Left = 8
        Top = 6
        Width = 28
        Height = 13
        Caption = #1055#1086#1088#1090':'
      end
      object LabelBaudRate: TLabel
        Left = 96
        Top = 6
        Width = 51
        Height = 13
        Caption = #1057#1082#1086#1088#1086#1089#1090#1100':'
      end
      object LabelPTTDn: TLabel
        Left = 8
        Top = 67
        Width = 69
        Height = 13
        Caption = #1055#1056#1044' '#1053#1072#1078#1072#1090#1100':'
      end
      object LabelPTTUp: TLabel
        Left = 8
        Top = 102
        Width = 82
        Height = 13
        Caption = #1055#1056#1044' '#1054#1090#1087#1091#1089#1090#1080#1090#1100':'
      end
      object LabelSend: TLabel
        Left = 216
        Top = 6
        Width = 82
        Height = 13
        Caption = #1055#1086#1089#1083#1072#1090#1100' '#1074' COM:'
      end
      object sbClose2: TSpeedButton
        Left = 360
        Top = 124
        Width = 73
        Height = 22
        Caption = 'Ok'
        Layout = blGlyphRight
        OnClick = sbClose1Click
      end
      object cbPTTControl: TComboBox
        Left = 8
        Top = 24
        Width = 73
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        ItemHeight = 16
        ParentFont = False
        Sorted = True
        TabOrder = 0
        OnChange = cbPTTControlChange
      end
      object edPTTDn: TEdit
        Left = 96
        Top = 63
        Width = 241
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        MaxLength = 255
        ParentFont = False
        TabOrder = 4
      end
      object CbBrPTT: TComboBox
        Left = 96
        Top = 24
        Width = 81
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        ItemHeight = 16
        ItemIndex = 6
        ParentFont = False
        TabOrder = 1
        Text = '9600'
        Items.Strings = (
          '110'
          '300'
          '600'
          '1200'
          '2400'
          '4800'
          '9600'
          '14400'
          '19200'
          '38400'
          '56000'
          '57600'
          '115200'
          '128000'
          '256000')
      end
      object edPTTUp: TEdit
        Left = 96
        Top = 97
        Width = 241
        Height = 24
        Font.Charset = RUSSIAN_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        MaxLength = 255
        ParentFont = False
        TabOrder = 5
      end
      object CbRTS: TCheckBox
        Left = 216
        Top = 26
        Width = 41
        Height = 17
        Caption = 'RTS'
        TabOrder = 2
        OnClick = CbRTSClick
      end
      object CbStr: TCheckBox
        Left = 272
        Top = 26
        Width = 65
        Height = 17
        Caption = #1057#1090#1088#1086#1082#1091
        TabOrder = 3
        OnClick = CbRTSClick
      end
      object RbHex: TRadioButton
        Left = 296
        Top = 128
        Width = 49
        Height = 17
        Caption = 'HEX'
        Checked = True
        TabOrder = 6
        TabStop = True
      end
      object RbASCII: TRadioButton
        Left = 248
        Top = 128
        Width = 49
        Height = 17
        Caption = 'ASCII'
        TabOrder = 7
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'TCP/IP'
      ImageIndex = 2
      object LabelTCPPort: TLabel
        Left = 16
        Top = 24
        Width = 56
        Height = 13
        Caption = 'Server Port:'
      end
      object sbClose3: TSpeedButton
        Left = 360
        Top = 124
        Width = 73
        Height = 22
        Caption = 'Ok'
        Layout = blGlyphRight
        OnClick = sbClose3Click
      end
      object SbDefTCP: TSpeedButton
        Left = 360
        Top = 96
        Width = 73
        Height = 22
        Caption = #1057#1073#1088#1086#1089
        Layout = blGlyphRight
        OnClick = SbDefTCPClick
      end
      object PortEdit: TSpinEdit
        Left = 104
        Top = 20
        Width = 65
        Height = 22
        MaxLength = 5
        MaxValue = 65535
        MinValue = 0
        TabOrder = 0
        Value = 5000
      end
    end
  end
  object BComPTT: TBComPort
    BaudRate = br38400
    ByteSize = bs8
    InBufSize = 512
    OutBufSize = 512
    Parity = paNone
    Port = 'COM1'
    SyncMethod = smThreadSync
    StopBits = sb2
    Timeouts.ReadInterval = -1
    Timeouts.ReadTotalMultiplier = 500
    Timeouts.ReadTotalConstant = 500
    Timeouts.WriteTotalMultiplier = 500
    Timeouts.WriteTotalConstant = 1000
    Left = 408
    Top = 24
  end
end
