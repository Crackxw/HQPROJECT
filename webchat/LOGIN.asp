<html>
<head>
<title>거상 대화방 로그인</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
<script language="javascript">
<!--
function onLogin(){
	var objID	= document.loginform.ID
	var objName	= document.loginform.name
	var objPass	= document.loginform.pass
	
	var strURL	= "CHAT_FRAME.asp?ID=" + objID.value + "&name=" + objName.value + "&pass=" + objPass.value
	
	if (objID.value == ""){
		alert("insert ID !!");
		objID.focus();
		return false;
	}
	else if (objName.value == ""){
		alert("insert name !!");
		objName.focus();
		return false;
	}
	else if (objPass.value == ""){
		alert("insert pass !!");
		objPass.focus();
		return false;
	}
	else{
		window.open(strURL,"GERSANG_CHAT",width=700,height=600,marginwidth=0,marginheight=0,border=0);
		return false;
	}
}
//-->
</script>
</head>
<body>
<table border="0">
<form method="post" name="loginform" onSubmit="return onLogin();">
	<tr><td>ID</td><td><input type="text" name="ID" size="20" value="dejavu"></td></tr>
	<tr><td>name</td><td><input type="text" name="name" size="20" value="김동춘"></td></tr>
	<tr><td>pass</td><td><input type="password" name="pass" size="20" value="dejavu"></td></tr>
	<tr><td colspan="2" align="right"><input type="submit" name="login" value="login"></td></tr>
</form>
</table>
</body>
</html>