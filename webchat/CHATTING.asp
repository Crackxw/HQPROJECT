<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>거상 대화방</title>
</head>
<frameset rows="10%,76%,*">
	<frame name="Chatting_Title" scrolling="auto" src="">
	<frameset cols="80%,*">
		<frame name="Chatting_board" scrolling="auto" src="">
		<frame name="Chatting_User_List" scrolling="auto" src="">
	</frameset>
	<frameset cols="80%,*">
		<frame name="Chatting_Send" scrolling="no" src="SEND.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
		<frame name="Chatting_Door" scrolling="no" src="">
	</frameset>
</frameset>
</html>
<script language="javascript">
<!--
//	페이지 순서 정하기
//	1. Chatting_Send
//	2. Chatting_Title
//	3. Chatting_User_List
//	4. Chatting_board
//	5. Chatting_Door

//처음 프래임 로딩
top.frames[0].document.ObjectForm.strPageLoad.value = "true";
//-->
</script>

