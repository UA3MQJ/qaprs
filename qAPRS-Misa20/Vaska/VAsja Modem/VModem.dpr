program VModem;

uses
  Forms,
  Windows,
  TXMain in 'TXMain.pas' {MainModemForm},
  Settings in 'Settings.pas' {SettingsForm};

{$R *.res}

//var W: Integer;
begin
     //W:= FindWindow(nil, ' Text Preset ');
     //if W = 0 then
     begin
          Application.Initialize;
          Application.Title := 'VModem';
  Application.CreateForm(TMainModemForm, MainModemForm);
  Application.CreateForm(TSettingsForm, SettingsForm);
  Application.Run;
     end;
     //ShowWindow(W, SW_RESTORE);
end.

