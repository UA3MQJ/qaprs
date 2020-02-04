// Javascript code for comments
function pleaselogin()
{
  alert("Please login to post and reply on comments");
}

function showbox(divtag, thingid, commentid, type, onuser)
{
  var tag="ta"+divtag;
  var box="";
  if (type=='commentflag') box+="<b>Please identify the reason(s) for flagging this comment:</b>";
  if (type=='thingflag') box+="<b>Please identify the reason(s) for flagging this content:</b>";
  if (type=='replyto') box+="<b>Write your reply here:</b>";
  if (type=='wallmessage') box+="<b>Write your message here:</b>";
  if (type=='bugreport') box+="<b>Please describe the bug in detail:</b>";
  if (type=='featurerequest') box+="<b>Please describe your feature in detail:</b>";
  box+="<form action='' method='POST'>\n";
  box+="<textarea id='"+tag+"' cols=70 rows=10 name='response'></textarea>\n";
  box+="<br/><input type='submit' value='Post Response'/>\n";
  box+="<input type='hidden' name='type' value='"+type+"'/>";

  if (thingid>0) box+="<input type='hidden' name='thingid' value='"+thingid+"'/>";

  if (typeof(onuser)!='underfined') box+="<input type='hidden' name='onuser' value='"+onuser+"'/>";

  box+="<input type='hidden' name='commentid' value='"+commentid+"'/>";
  box+="<input type='button' value='Cancel' onclick='emptydiv(\""+divtag+"\",1,\""+tag+"\")'/>\n";
  box+="</form>";

  var el=document.getElementById(divtag);
  if (el.innerHTML.length>3) {
    emptydiv(divtag,1,tag);
    return;
  } else el.innerHTML=box;
}

function replyto(divtag, commentid, thingid)
{
  thingid = typeof(thingid) != 'undefined' ? thingid : -10;
  showbox(divtag, thingid, commentid,"replyto");
}

function commentflag(divtag, commentid, thingid)
{
  thingid = typeof(thingid) != 'undefined' ? thingid : -10;
  showbox(divtag, thingid, commentid,"commentflag");
}
