function GetAJAXObject()
{
  var xmlHttp;
  try { // Firefox, Opera 8.0+, Safari
    xmlHttp=new XMLHttpRequest();
  } catch (e) { // Internet Explorer
    try {
      xmlHttp=new ActiveXObject("Msxml2.XMLHTTP");
    } catch (e) {
      try{
	xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      } catch (e) {
	alert("Your browser does not support AJAX!");
      }
    }
  }
  
  return xmlHttp;
}

function ajax_get_contents(url)
{
  var res=FALSE;
  var myxmlHttp=GetAJAXObject();
  
  myxmlHttp.onreadystatechange=function(){
    if (myxmlHttp.readyState==4) res=myxmlHttp.responseText;
  }
  
  myxmlHttp.open(url);
  myxmlHttp.send(null);
  
  delete myxmlHttp;

  return res;
}

function showajaxload(divid)
{
  var el=document.getElementById(divtag);
  var waithtml="<div style='width:100%;height:100%'><div style='vertical-align:middle;width:100%;'><center><img src='/img/uploading.gif'/></center></div></div>";
  if (el!=FALSE && el.innerHTML!=waithtml) el.innerHTML=waithtml; // Only show the wait if it is not already there....!
}

function fill_div_from_url(divid, url)
{
  showajaxload(divid);
  var res=ajax_get_contents(url);

  if (res){
    var el=document.getElementById(divtag);
    el.innerHTML=res;
    return 1;
  } else return 0;
}

function emptydiv(divid, confirm, tatocheck)
{
  if (confirm==1){
    var ta=document.getElementById(tatocheck);
    if (ta.value.length>2) if (!confirm("You will lose your data, continue to close?")) return;
  }

  var ta=document.getElementById(divid);
  ta.innerHTML='';
}

function include(filename){
  var head = document.getElementsByTagName('head')[0];
  
  script = document.createElement('script');
  script.src = filename;
  script.type = 'text/javascript';
  
  head.appendChild(script);
}

function createCookie(name,value,days) {
  if (days) {
    var date = new Date();
    date.setTime(date.getTime()+(days*24*60*60*1000));
    var expires = "; expires="+date.toGMTString();
  }
  else var expires = "";
  document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
  var nameEQ = name + "=";
  var ca = document.cookie.split(';');
  for(var i=0;i < ca.length;i++) {
    var c = ca[i];
    while (c.charAt(0)==' ') c = c.substring(1,c.length);
    if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
  }
  return null;
}

function eraseCookie(name) {
  createCookie(name,"",-1);
}