object MainModemForm: TMainModemForm
  Left = 431
  Top = 177
  Width = 798
  Height = 202
  Caption = ' Vasja Modem'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDefault
  OnActivate = FormActivate
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 161
    Top = 0
    Width = 4
    Height = 135
    Color = clBtnFace
    ParentColor = False
    OnCanResize = Splitter1CanResize
    OnMoved = Splitter1Moved
  end
  object PanelSpectr: TPanel
    Left = 0
    Top = 0
    Width = 161
    Height = 135
    Align = alLeft
    BevelInner = bvRaised
    BevelOuter = bvLowered
    Constraints.MinWidth = 32
    TabOrder = 1
    OnResize = PanelSpectrResize
    object pbss: TPaintBox
      Left = 2
      Top = 2
      Width = 129
      Height = 131
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clLime
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      PopupMenu = SpctrPopupMenu
      OnPaint = pbssPaint
    end
    object RulerPaintBox: TPaintBox
      Left = 131
      Top = 2
      Width = 28
      Height = 131
      Align = alRight
      Color = clBtnFace
      Font.Charset = RUSSIAN_CHARSET
      Font.Color = clBlack
      Font.Height = -11
      Font.Name = 'MS Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      PopupMenu = SpctrPopupMenu
      OnPaint = RulerPaintBoxPaint
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 135
    Width = 790
    Height = 21
    Panels = <
      item
        Width = 220
      end
      item
        Width = 220
      end
      item
        Width = 50
      end>
  end
  object MainPanel: TPanel
    Left = 165
    Top = 0
    Width = 625
    Height = 135
    Align = alClient
    BevelOuter = bvNone
    Color = clBlack
    Constraints.MinWidth = 625
    TabOrder = 2
    object PanelDebug: TPanel
      Left = 0
      Top = 65
      Width = 625
      Height = 70
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      object ControlPanel: TPanel
        Left = 288
        Top = 0
        Width = 337
        Height = 70
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object sbServer: TSpeedButton
          Left = 0
          Top = 32
          Width = 64
          Height = 26
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'Server'
          OnClick = sbServerClick
        end
        object BtnTestTone: TSpeedButton
          Left = 65
          Top = 32
          Width = 72
          Height = 26
          AllowAllUp = True
          GroupIndex = 1
          Caption = #1058#1077#1089#1090' '#1057#1080#1075#1085#1072#1083
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnClick = BtnTestToneClick
        end
        object SbClearTx: TSpeedButton
          Left = 138
          Top = 32
          Width = 64
          Height = 26
          BiDiMode = bdLeftToRight
          Caption = #1054#1095#1080#1089#1090#1080#1090#1100
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
        object btnPTT: TSpeedButton
          Left = 203
          Top = 32
          Width = 68
          Height = 26
          Caption = #1055#1056#1044'/'#1057#1073#1088#1086#1089
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnMouseDown = btnPTTMouseDown
          OnMouseUp = btnPTTMouseUp
        end
        object BtnTX: TSpeedButton
          Left = 272
          Top = 32
          Width = 65
          Height = 26
          Caption = #1055#1056#1044
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          OnClick = BtnTXClick
        end
        object ProgressBar: TProgressBar
          Left = 0
          Top = 5
          Width = 337
          Height = 9
          TabOrder = 0
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 229
        Height = 70
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 1
        object Panel1: TPanel
          Left = 0
          Top = 2
          Width = 223
          Height = 66
          BevelOuter = bvLowered
          Color = clBlack
          TabOrder = 0
          object imgFrs: TImage
            Left = 41
            Top = 6
            Width = 11
            Height = 11
            AutoSize = True
            Picture.Data = {
              07544269746D6170CE000000424DCE0000000000000076000000280000000B00
              00000B0000000100040000000000580000000000000000000000100000000000
              0000000000000000800000800000008080008000000080008000808000008080
              8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
              FF00DDD0DDDDDDD00000DDD00DDDDDD00000DDD000DDDDD00000DDD000ADDDD0
              0000DDD00AAADDD00000DDD0AAAAADD00000DDD00AAADDD00000DDD000ADDDD0
              0000DDD000DDDDD00000DDD00DDDDDD00000DDD0DDDDDDD00000}
            Transparent = True
            Visible = False
          end
          object imgSync: TImage
            Left = 42
            Top = 6
            Width = 11
            Height = 11
            AutoSize = True
            Picture.Data = {
              07544269746D6170CE000000424DCE0000000000000076000000280000000B00
              00000B0000000100040000000000580000000000000000000000100000000000
              0000000000000000800000800000008080008000000080008000808000008080
              8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
              FF00DDDADDDDDDD00000DDDAADDDDDD00000DDDAAADDDDD00000DDDAAAADDDD0
              0000DDDAAAAADDD00000DDDAAAAAADD00000DDDAAAAADDD00000DDDAAAADDDD0
              0000DDDAAADDDDD00000DDDAADDDDDD00000DDDADDDDDDD00000}
            Transparent = True
            Visible = False
          end
          object LabelRX: TLabel
            Left = 12
            Top = 3
            Width = 21
            Height = 15
            AutoSize = False
            Caption = 'RX'
            Font.Charset = ANSI_CHARSET
            Font.Color = clGray
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object LabelTX: TLabel
            Left = 12
            Top = 3
            Width = 20
            Height = 15
            AutoSize = False
            Caption = 'TX'
            Font.Charset = ANSI_CHARSET
            Font.Color = clRed
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            Visible = False
          end
          object VectorPaintBox: TPaintBox
            Left = 158
            Top = 1
            Width = 64
            Height = 64
            Align = alRight
            Color = clBlack
            ParentColor = False
            OnPaint = VectorPaintBoxPaint
          end
          object LabelSBVol: TLabel
            Left = 84
            Top = 48
            Width = 6
            Height = 13
            Caption = '0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clLime
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LabelPTTControl: TLabel
            Left = 12
            Top = 48
            Width = 6
            Height = 13
            Caption = '0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clLime
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LabelProtoName: TLabel
            Left = 12
            Top = 27
            Width = 75
            Height = 13
            Caption = 'Not Selected'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clLime
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
          end
          object LedDisplay: TLedDisplay
            Left = 58
            Top = 5
            Width = 100
            Height = 14
            Orientation = loHorizontal
            Ctl3D = False
            Spacing = 0
            Max = 32767
            Position = 0
            BlockACount = 18
            BlockBCount = 5
            BlockCCount = 2
            ActiveColor1 = clLime
            ActiveColor2 = clYellow
            ActiveColor3 = clRed
            DeadColor1 = clGreen
            DeadColor2 = clOlive
            DeadColor3 = clMaroon
          end
        end
      end
      object Panel3: TPanel
        Left = 229
        Top = 0
        Width = 59
        Height = 70
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 2
        object ImageSnd: TImage
          Left = 35
          Top = 5
          Width = 19
          Height = 19
          AutoSize = True
          Picture.Data = {
            07544269746D61703A020000424D3A02000000000000BE000000280000001300
            00001300000001000800000000007C010000120B0000120B0000220000002200
            000000000000FFFFFF0021202100636163003130310084828400525152004241
            42005A595A00009E000000860000006100000041000031AE3100319631003171
            310063BE630063A66300315131006BAE6B0094B694009C9E9C0084868400BDBE
            BD00A5A6A5000061210000613900639E9C0042BEBD00429E9C0000BEBD000041
            420042616300FFFFFF0001010101010101010101010101010101010101000101
            0101010101010101010101010101010101000101010101010101010101010101
            0101010101000101010101010101010101010101010101010100010101010101
            01010100000000000001010101000101010101010101011C1E1E1E1E1F010101
            01000107070707070707071B1D1D1D1D2007020101000107170A0C120B120B11
            0A190A19190A0C0101000107170A170613180C110A06020F070C0C0401000107
            170A0C0C0B0C0B14110D0D110D0E0C0301000107171A170613180C110A0A0C0C
            0B110C0301000107171A0C0C0B0C0B14140A18160C14120301000107170A1706
            13180C11110A17180C14120801000107171A110A0A110A111009100A0A110C01
            0100010715181818181818181818181818180601010001010101010101010101
            0101010101010101010001010101010101010101010101010101010101000101
            0101010101010101010101010101010101000101010101010101010101010101
            010101010100}
          Transparent = True
          OnMouseDown = ImageSndMouseDown
        end
        object tbMain: TTrackBar
          Left = 0
          Top = 0
          Width = 30
          Height = 67
          Max = 255
          Orientation = trVertical
          Frequency = 64
          Position = 127
          TabOrder = 0
          ThumbLength = 15
          TickMarks = tmBoth
          OnChange = tbMainChange
        end
      end
    end
    object MemosPanel: TPanel
      Left = 0
      Top = 0
      Width = 625
      Height = 65
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object Splitter2: TSplitter
        Left = 0
        Top = 30
        Width = 625
        Height = 5
        Cursor = crVSplit
        Align = alBottom
        Color = clBtnFace
        ParentColor = False
        ResizeStyle = rsUpdate
        OnCanResize = Splitter2CanResize
      end
      object MemoSMSFrom: TRichEdit
        Left = 0
        Top = 0
        Width = 625
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
        TabOrder = 0
      end
      object MemoSMS: TMemo
        Left = 0
        Top = 35
        Width = 625
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
        PopupMenu = SMSPopupMenu
        ScrollBars = ssVertical
        TabOrder = 1
      end
    end
  end
  object Mixer: TSHSMixer
    OnControlChange = MixerControlChange
    Left = 56
    Top = 32
  end
  object MainMenu: TMainMenu
    Left = 24
    Top = 33
    object mnuOptions: TMenuItem
      Caption = #1054#1087#1094#1080#1080
      object mnuSettings: TMenuItem
        Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080'...'
        GroupIndex = 1
        ShortCut = 115
        OnClick = mnuSettingsClick
      end
      object N2: TMenuItem
        Caption = '-'
        GroupIndex = 1
      end
      object mnuWaterfall: TMenuItem
        AutoCheck = True
        Caption = #1057#1087#1077#1082#1090#1088
        GroupIndex = 1
        ShortCut = 16471
        OnClick = mnuWaterfallClick
      end
    end
    object mnuProtocol: TMenuItem
      Caption = #1055#1088#1086#1090#1086#1082#1086#1083
      object mnuVasja10: TMenuItem
        AutoCheck = True
        Caption = '3.0'
        GroupIndex = 3
        RadioItem = True
        OnClick = mnuVasja10Click
      end
      object N10: TMenuItem
        Caption = '-'
        GroupIndex = 3
      end
      object mnuVasja11: TMenuItem
        AutoCheck = True
        Caption = '3.1'
        GroupIndex = 3
        RadioItem = True
        OnClick = mnuVasja10Click
      end
      object N9: TMenuItem
        Caption = '-'
        GroupIndex = 3
      end
      object mnuMorse: TMenuItem
        AutoCheck = True
        Caption = #1050#1086#1076' '#1052#1086#1088#1079#1077
        GroupIndex = 3
        RadioItem = True
        OnClick = mnuVasja10Click
      end
    end
    object mnuLanguage: TMenuItem
      Caption = 'Language'
      object mnuRussian: TMenuItem
        Caption = 'Russian'
        RadioItem = True
        OnClick = mnuRussianClick
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object mnuEnglish: TMenuItem
        Caption = 'English'
        RadioItem = True
        OnClick = mnuEnglishClick
      end
    end
  end
  object SMSPopupMenu: TPopupMenu
    Left = 24
    Top = 64
    object pmnuTransferF9: TMenuItem
      Caption = 'Transfer'
      ShortCut = 120
      Visible = False
      OnClick = BtnTXClick
    end
    object pmnuTransfer: TMenuItem
      Caption = #1055#1077#1088#1077#1076#1072#1095#1072
      ShortCut = 16397
      OnClick = BtnTXClick
    end
    object N13: TMenuItem
      Caption = '-'
    end
    object pmnuClear: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      ShortCut = 27
      OnClick = pmnuClearClick
    end
    object N14: TMenuItem
      Caption = '-'
    end
    object pmnuPaste: TMenuItem
      Caption = #1042#1089#1090#1072#1074#1080#1090#1100
      ShortCut = 16470
      OnClick = pmnuPasteClick
    end
  end
  object SpctrPopupMenu: TPopupMenu
    Left = 56
    Top = 64
    object pmnu12kHz: TMenuItem
      AutoCheck = True
      Caption = '12 kHz'
      GroupIndex = 1
      RadioItem = True
      OnClick = pmnu12kHzClick
    end
    object pmnu06kHz: TMenuItem
      AutoCheck = True
      Caption = '6 kHz'
      GroupIndex = 1
      RadioItem = True
      OnClick = pmnu06kHzClick
    end
    object pmnu03kHz: TMenuItem
      AutoCheck = True
      Caption = '3 kHz'
      GroupIndex = 1
      RadioItem = True
      OnClick = pmnu03kHzClick
    end
    object N8: TMenuItem
      Caption = '-'
      GroupIndex = 2
    end
    object pmnuMarker: TMenuItem
      AutoCheck = True
      Caption = 'Marker'
      GroupIndex = 2
    end
  end
end
