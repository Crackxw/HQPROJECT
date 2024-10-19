<html>
<head>
<title>거상 대화방</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
</head>
<script language="javascript">
<!--
function ReqMakeChatRoom(){

	var RoomTitle
	var UserNum
	var RoomStyle
	var SecretNum
	
	if (document.MakeRoomForm.room_title.value == ""){
		alert("insert Room Title !!");
		document.MakeRoomForm.room_title.focus();
		return false;
	}
	else if (document.MakeRoomForm.UserNum.value == ""){
		alert("Select User Join Count !!");
		alert("select User Number !!");
		document.MakeRoomForm.UserNum.focus();
		return false;
	}
	else if (document.MakeRoomForm.Room_Style[0].checked == false && document.MakeRoomForm.Room_Style[1].checked == false){
		alert("Check Room Style !!");
		document.MakeRoomForm.Room_Style[0].focus();
		return false;
	}
	else if (document.MakeRoomForm.Room_Style[1].checked == true){
		if (document.MakeRoomForm.SecretNum.value == ""){
			alert("Insert Secret Number !!");
			document.MakeRoomForm.SecretNum.focus();
			return false;
		}
	}
	
	if (document.MakeRoomForm.Room_Style[0].checked == true){ RoomStyle = 0;}
	else if (document.MakeRoomForm.Room_Style[1].checked == true){ RoomStyle = 1;}
		
	RoomTitle	= document.MakeRoomForm.room_title.value;
	UserNum		= document.MakeRoomForm.UserNum.value;
	SecretNum	= document.MakeRoomForm.SecretNum.value;
	
	top.frames[0].document.ObjectForm.strRoomTitle.value = RoomTitle;
	top.frames[0].JoyOnChat.ReqMakeRoom(RoomTitle, RoomStyle, UserNum, SecretNum);
	return true;
}

//-->
</script>
<body topmargin="0" leftmargin="0">
<table border="0" width="100%">
	<tr>
		<td></td>
	</tr>
</table>

<table border="0">
<form name="MakeRoomForm">
	<tr>
		<td>Room Title</td>
		<td colspan="2"><input type="text" size="60" name="room_title"></td>
	</tr>
	<tr>
		<td>User Join Count</td>
		<td><select name="UserNum">
				<option value="4">4</option>
				<option value="5">5</option>
				<option value="6">6</option>
				<option value="7">7</option>
				<option value="8">8</option>
				<option value="9">9</option>
				<option value="10">10</option>
				<option value="11">11</option>
				<option value="12">12</option>
			</select>		
		</td>
		<td>
			General Room <input type="radio" name="Room_Style" value="general" checked>&nbsp;&nbsp;
			Secret Room <input type="radio" name="Room_Style" value="secret">&nbsp;
			Secret Number <input type="password" name="SecretNum">
		</td>
	</tr>
	<tr>
		<td></td>
		<td colspan="2"><input type="submit" value="Create Room" name="make_room_submit" onClick="return ReqMakeChatRoom()"></td>
	</tr>
</form>
</table>
</body>
</html>