<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strRoomNum, intRoomCode
	strRoomNum = Request("RoomNum")
	intRoomCode = request("RoomCode")	%>
<html>
<head>
<title>거상 대화방</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
<script language="javascript">
<!--
function GoRoomPass(){
	var RoomNum = document.RoomPassFrom.RoomNum.value;
	var RoomCode = document.RoomPassFrom.RoomCode.value;
	var RoomPass = document.RoomPassFrom.RoomPass.value;
	
	if (RoomPass.length == 0){
		alert("Room Pass !!");
		document.RoomPassFrom.RoomPass.focus();
		return false;
	}
	else{
		opener.ReqJoinRoom(RoomNum,RoomCode,RoomPass);
		window.close();
	}
}
//-->
</script>
</head>
<body topmargin="1" leftmargin="1">
<table>
<form name="RoomPassFrom">
	<tr><td>
		<input type="hidden" name="RoomNum" value="<%=strRoomNum%>">
		<input type="hidden" name="RoomCode" value="<%=intRoomCode%>">
	</td></tr>
	<tr><td>비밀번호 : <input type="password" name="RoomPass">&nbsp;&nbsp;&nbsp;
	<input type="button" name="Room Pass" value="방 입장" onClick="return GoRoomPass()"></td></tr>
</form>
</table>
</body>
</html>