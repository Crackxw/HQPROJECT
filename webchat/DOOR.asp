<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>거상 대화방 테스트</title>
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
<input type="button" name="exit_boor" value="방 나가기" onClick="return ReqExitRoom();">
<input type="button" name="room_info" value="방장 툴" onClick="return ReqMyRoomInfo();">
</body>
</html>

<script language="javascript">
<!--
//	페이지 순서 정하기
//	1. Chatting_Send
//	2. Chatting_Title
//	3. Chatting_User_List
//	4. Chatting_board
//	5. Chatting_Door

//마지막 프래임 로딩
top.frames[0].document.ObjectForm.strPageLoad.value = "false";
//-->
</script>