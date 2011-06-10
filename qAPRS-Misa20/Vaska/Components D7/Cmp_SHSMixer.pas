unit Cmp_SHSMixer;

interface

uses
    Windows, Messages, Classes, Forms, MMSystem;

type
    TMixerControlType = (Volume, Treble, Bass, Peak);
    //TMixerChange = procedure (Sender:TObject;MixerH:HMixer;ID:Integer) of object;
    TMixerChange = procedure (Sender:TObject) of object;

type
    TSHSMixer = class(TComponent)
    private
           fMixerHandle       : HMIXER;              // Handle for the open mixer
           fMixerCaps         : TMixerCaps;          // Mixer capabilities structure
           fAudioLine         : TMixerLine;          // Master mixer line
           XWndHandle         : HWnd;

    fControl : array [Low (TMixerControlType)..High (TMixerControlType)] of TMixerControl;
    fControlSupported : array [Low (TMixerControlType)..High (TMixerControlType)] of boolean;

    FOnControlChange:TMixerChange;
      
  protected
         procedure MixerCallBack (var Msg:TMessage);

  public
        procedure    OpenMixer(Device: Byte);
        procedure    MixerSetControl (control : TMixerControlType; Data: Integer);
        function     MixerGetControl (control : TMixerControlType) : Integer;
        procedure    CloseMixer;
        function     GetTmberSupport : Boolean;
        //constructor  Create (AOwner:TComponent); override;
        //destructor   Destroy; override;

  published
  property OnControlChange:TMixerChange read FOnControlChange write FOnControlChange;


end;

procedure Register;

implementation

var IsTember: Boolean = False;

{|Def: TSHSMIXER |}


function TSHSMixer.GetTmberSupport : Boolean;
begin
     GetTmberSupport:= IsTember;
end;

procedure TSHSMixer.MixerCallBack (var Msg:TMessage);
begin
     if (Msg.Msg = MM_MIXM_CONTROL_CHANGE) and
     Assigned (OnControlChange) then
     //OnControlChange (Self, Msg.wParam, Msg.lParam)
     OnControlChange (Self)
     else
     Msg.Result := DefWindowProc (XWndHandle, Msg.Msg, Msg.WParam, Msg.LParam);
end;

procedure TSHSMixer.OpenMixer(Device: Byte);
var
   i : Integer;

function GetMixerControlByType (const line : TMixerLine; tp : Integer; var control : TMixerControl) : boolean;
var
   mixerLineControls : TMixerLineControls;
   err : Integer;
   begin
        mixerLineControls.cbStruct := sizeof (mixerLineControls);
        mixerLineControls.dwLineID := line.dwLineID;
        mixerLineControls.dwControlType := tp;
        mixerLineControls.cControls := line.cControls;
        mixerLineControls.cbmxctrl := sizeof (control);
        mixerLineControls.pamxctrl := @control;
        err := mixerGetLineControls (fMixerHandle, @mixerLineControls, MIXER_GETLINECONTROLSF_ONEBYTYPE);
        result := err = MMSYSERR_NOERROR;
   end;

begin  {TBOpen}

     XWndHandle := AllocateHWnd(MixerCallBack);
     i:= mixerOpen (@fMixerHandle, Device, XWndHandle, 0,  CALLBACK_WINDOW OR MIXER_OBJECTF_MIXER);
     if i <> 0 then
     begin
          Application.messagebox( 'Can`t open mixer.','Warning', mb_OK);
          //MessageDlg ('Can`t open mixer.', mtWarning, [mbOK], 0);
          EXIT
     end;
     mixerGetDevCaps (fMixerHandle, @fMixerCaps, sizeof (fMixerCaps));

     for i := 0 to fMixerCaps.cDestinations - 1 do
     begin
          fAudioLine.cbStruct := sizeof (fAudioLine);
          fAudioLine.dwDestination := i;
          fAudioLine.dwSource := 0;
          mixerGetLineInfo (fMixerHandle, @fAudioLine, MIXER_GETLINEINFOF_DESTINATION);
          break;
     end;

     fControlSupported[Volume]:= GetMixerControlByType (fAudioLine, MIXERCONTROL_CONTROLTYPE_VOLUME, fControl [Volume]);
     fControlSupported[Treble]:= GetMixerControlByType (fAudioLine, MIXERCONTROL_CONTROLTYPE_TREBLE, fControl [Treble]);
     fControlSupported[Bass]:= GetMixerControlByType (fAudioLine, MIXERCONTROL_CONTROLTYPE_BASS, fControl [Bass]);
     fControlSupported[Peak]:= GetMixerControlByType (fAudioLine, MIXERCONTROL_CONTROLTYPE_PEAKMETER, fControl [Peak]);

     if fControlSupported[Treble] and fControlSupported[Bass] then
     IsTember:= True else IsTember:= False;
end;


procedure TSHSMixer.MixerSetControl (control : TMixerControlType; Data: Integer);
var
   details : TMixerControlDetails;
begin
     details.cbStruct := sizeof (details);
     details.dwControlID := fControl [control].dwControlID;
     details.cChannels := 1;
     details.cMultipleItems := fControl [control].cMultipleItems;
     details.cbDetails := sizeof (integer);
     details.paDetails := @data;
     if fControlSupported[control] then
     mixerSetControlDetails (fMixerHandle, @details, MIXER_SETCONTROLDETAILSF_VALUE);
end;


function TSHSMixer.MixerGetControl (control : TMixerControlType) : Integer;
var
   details : TMixerControlDetails;
   data    : array [0..15] of Integer;
begin
    details.cbStruct := sizeof (details);
    details.dwControlID := fControl [control].dwControlID;
    details.cChannels := 1;
    details.cMultipleItems := fControl [control].cMultipleItems;
    details.cbDetails := sizeof (integer);
    details.paDetails := @data;
    mixerGetControlDetails (fMixerHandle, @details, MIXER_GETCONTROLDETAILSF_VALUE);
    if fControlSupported[control] then
    Result:= data[0] else Result:= 0;
end;


procedure TSHSMixer.CloseMixer;
begin
     mixerClose(fMixerHandle);
     DeAllocateHwnd(XWndHandle);
end;

procedure Register;
begin
     RegisterComponents('Media', [TSHSMixer]);
end;

end.
