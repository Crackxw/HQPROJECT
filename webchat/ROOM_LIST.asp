<html>
<head>
<title>�Ż� ��ȭ��</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
<script language="javascript">
<!--
function ReqRoomList(){
	top.frames[0].JoyOnChat.ReqRoomList();
}
function ReqJoinRoomReady(RoomNum,RoomCode,RoomType){
	// ��� ��ȭ���̶�� ��й�ȣ�� �޾ƿ´�.
	var RoomPassURL;
	var RoomPass;
	
	RoomPassURL = "POP_PASS.asp?RoomNum=" + RoomNum + "&RoomCode=" + RoomCode;
	//top.frames[0].document.ObjectForm.strRoomTitle.value = RoomSubject;
	
	if (RoomType == 1){
		window.open(RoomPassURL,"RoomPass","width=250,height=80,border=0");
		return false;
	}
	else{
		RoomPass = "";
		ReqJoinRoom(RoomNum,RoomCode,RoomPass);
	}
}

function ReqJoinRoom(RoomNum,RoomCode,RoomPass){
	top.frames[0].JoyOnChat.ReqJoinRoom(RoomNum,RoomCode,RoomPass);
}
//-->
</script>
</head>
<body onLoad="ReqRoomList();">
<table>
	<tr><td>
		<input type="button" name="ReqRoomList" value="ReqRoomList" onClick="window.location.reload()">
	</td></tr>
</table>
</body>
</html>