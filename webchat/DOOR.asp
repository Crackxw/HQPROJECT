<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>�Ż� ��ȭ�� �׽�Ʈ</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
<script language="javascript">
<!--
function ReqExitRoom(){
	top.frames[0].JoyOnChat.ReqExitRoom();	
	top.frames[1].location.href="ROOM.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
	
	top.frames[0].document.ObjectForm.strPageLoad.value = "true";
	return true;
}
	
function ReqMyRoomInfo(){
	top.frames[0].JoyOnChat.ReqMyRoomInfo();
	top.frames[0].OpenRoomOption();
}

top.frames[0].JoyOnChat.ReqMyRoomInfo();
//-->
</script>
</head>
<body topmargin="1" leftmargin="1">
<input type="button" name="exit_boor" value="�� ������" onClick="return ReqExitRoom();">
<input type="button" name="room_info" value="���� ��" onClick="return ReqMyRoomInfo();">
</body>
</html>

<script language="javascript">
<!--
//	������ ���� ���ϱ�
//	1. Chatting_Send
//	2. Chatting_Title
//	3. Chatting_User_List
//	4. Chatting_board
//	5. Chatting_Door

//������ ������ �ε�
top.frames[0].document.ObjectForm.strPageLoad.value = "false";
//-->
</script>