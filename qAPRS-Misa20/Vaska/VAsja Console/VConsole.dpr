program VConsole;

uses
  Forms,
  VClientConsole in 'VClientConsole.pas' {ClientForm};

{$R *.res}

begin
  Application.Initialize;
  Application.Title := 'VConsole';
  Application.CreateForm(TClientForm, ClientForm);
  Application.Run;
end.
