$LoginVisible=0;
function HideLogin()
{
  e=document.getElementById("loginform");
  
  $i=0;
  e.style.visibility="hidden";
}

function ShowHideLogin()
{
  e=document.getElementById("loginform");

  if ($LoginVisible==0){
    e.style.visibility="visible";
    $i=1;
  } else {
    $LoginVisible=0;
    e.style.visibility="hidden";
  }
}

function WriteLoginForm()
{
  LoginForm=document.getElementById("loginform");

  LoginForm.innerHTML="<form action='' method='post'><a href='javascript:HideLogin();'><img src='/img/close.png' style='float:right;' alt='Click here to hide the login form'/></a><table><tr><td>E-mail</td><td><input class='smallfont' type='text' name='email'/></td><td colspan='2'><input type='checkbox' name='remember'/> Keep logged in</td></tr><tr><td>Password</td><td><input class='smallfont' type='password' name='password'/></td><td colspan='2'><input type='submit' class='smallfont' name='submit' value='Login'/></td></tr></table></form>";
}