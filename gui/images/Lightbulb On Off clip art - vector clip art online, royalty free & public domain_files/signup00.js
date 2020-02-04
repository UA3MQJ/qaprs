function signup()
{
    var myxmlHttp=GetAJAXObject();
    
    myxmlHttp.onreadystatechange=function(){
	var el=document.getElementById("signup");
	if (el.style.visibility=="visible")
	    return;

	if (myxmlHttp.readyState==4){
	    el.innerHTML=myxmlHttp.responseText;
	    el.style.visibility="visible";
        }
    }
    myxmlHttp.open("GET","/signup.html?showform=yes",true);
    myxmlHttp.send(null);

    delete myxmlHttp;
}

function hidesignup()
{
    var el=document.getElementById("signup");
    el.innerHTML='';
    el.style.visibility="hidden";
}

function isValidEmail(str)
{
    return (str.indexOf(".") > 2) && (str.indexOf("@") > 0);
}

function ValidateSignup(form)
{
    // Check if the emails are empty
    if (typeof(form.email_signup.value)=="undefined"){
	alert("E-mail is empty");
	return;
    }

    if (typeof(form.email_signup2.value)=="undefined"){
	alert("Confirm E-mail is empty");
	return;
    }

    // Check that the emails are valid
    if (!isValidEmail(form.email_signup.value)){
	alert(form.email_signup.value+" is not a valid email");
	return;
    }

    if (!isValidEmail(form.email_signup2.value)){
	alert(form.email_signup.value2+" is not a valid email");
	return;
    }

    // Check that the emails are equal
    if (form.email_signup.value!=form.email_signup2.value){
	alert("Emails don't match");
	return;
    }

    // Check that the passwords are not empty
    if (typeof(form.password_signup.value)=="undefined"){
	alert("Password is empty");
	return;
    }

    if (typeof(form.password_signup2.value)=="undefined"){
	alert("Confirm Password is empty");
	return;
    }

    // Check that the passwords are equal
    if (form.password_signup.value!=form.password_signup2.value){
	alert("passwords don't match");
	return;
    }

    if (typeof(form.name.value)=="undefined"){
	alert("Name is empty");
	return;
    }

    var el=document.getElementById("signupform");
    el.innerHTML="<input type='hidden' name='createaccount' value='15'>";

    form.submit();
}