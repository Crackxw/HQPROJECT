<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>�Ż� ��ȭ��</title>
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
//	������ ���� ���ϱ�
//	1. Chatting_Send
//	2. Chatting_Title
//	3. Chatting_User_List
//	4. Chatting_board
//	5. Chatting_Door

//ó�� ������ �ε�
top.frames[0].document.ObjectForm.strPageLoad.value = "true";
//-->
</script>

