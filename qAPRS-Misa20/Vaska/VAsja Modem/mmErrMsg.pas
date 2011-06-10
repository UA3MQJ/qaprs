unit
    mmErrMsg;

interface

function translate_mm_error (error_number: word): pChar;

implementation

uses
    mmSystem;

function translate_mm_error (error_number: word): pChar;
begin
  case error_number of
         mmsyserr_NoError: translate_mm_error := 'no error';
           mmsyserr_Error: translate_mm_error := 'unspecified error';
     mmsyserr_BadDeviceID: translate_mm_error := 'device ID out of range';
      mmsyserr_NotEnabled: translate_mm_error := 'driver failed enable';
       mmsyserr_Allocated: translate_mm_error := 'device already allocated';
     mmsyserr_InvalHandle: translate_mm_error := 'device handle is invalid';
        mmsyserr_NoDriver: translate_mm_error := 'no device driver present';
           mmsyserr_NoMem: translate_mm_error := 'memory allocation error';
    mmsyserr_NotSupported: translate_mm_error := 'function isn''t supported';
       mmsyserr_BadErrNum: translate_mm_error := 'error value out of range';
       mmsyserr_InvalFlag: translate_mm_error := 'invalid flag passed';
      mmsyserr_InvalParam: translate_mm_error := 'invalid parameter passed';
         waverr_BadFormat: translate_mm_error := 'unsupported wave format';
      waverr_StillPlaying: translate_mm_error := 'still something playing';
        waverr_Unprepared: translate_mm_error := 'header not prepared';
              waverr_Sync: translate_mm_error := 'device is synchronous';
       midierr_Unprepared: translate_mm_error := 'header not prepared';
     midierr_StillPlaying: translate_mm_error := 'still something playing';
            midierr_NoMap: translate_mm_error := 'no current map';
         midierr_NotReady: translate_mm_error := 'hardware is still busy';
         midierr_NoDevice: translate_mm_error := 'port no longer connected';
     midierr_InvalidSetup: translate_mm_error := 'invalid setup';
{        midierr_LastError: translate_mm_error := 'last error in range'; } 
{           timerr_NoError: translate_mm_error := 'no error'; }
           timerr_NoCanDo: translate_mm_error := 'request not completed';
            timerr_Struct: translate_mm_error := 'time struct size';
{           joyerr_NoError: translate_mm_error := 'no error'; }
             joyerr_Parms: translate_mm_error := 'bad parameters';
           joyerr_NoCanDo: translate_mm_error := 'request not completed';
         joyerr_Unplugged: translate_mm_error := 'joystick is unplugged';
     mmioerr_FileNotFound: translate_mm_error := 'file not found';
      mmioerr_OutOfMemory: translate_mm_error := 'out of memory';
       mmioerr_CannotOpen: translate_mm_error := 'cannot open';
      mmioerr_CannotClose: translate_mm_error := 'cannot close';
       mmioerr_CannotRead: translate_mm_error := 'cannot read';
      mmioerr_CannotWrite: translate_mm_error := 'cannot write';
       mmioerr_CannotSeek: translate_mm_error := 'cannot seek';
     mmioerr_CannotExpand: translate_mm_error := 'cannot expand file';
    mmioerr_ChunkNotFound: translate_mm_error := 'chunk not found';
       mmioerr_Unbuffered: translate_mm_error := 'file is unbuffered';
  else
      translate_mm_error := 'invalid parameter passed to error translation routine!';
  end;
end;

begin
end.