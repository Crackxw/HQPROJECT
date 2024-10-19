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
<frameset rows="80%,*">
	<frameset cols="80%,*">
		<frame name="Room_List" scrolling="auto" src="ROOM_LIST.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
		<frame name="Front_User_List" scrolling="auto" src="FRONT_USER_LIST.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
	</frameset>
	<frameset cols="80%,*">
		<frame name="Room_Make" scrolling="no" src="ROOM_MAKE.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
		<frame name="Room_Exit" scrolling="no" src="EXIT.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
	</frameset>
</frameset>
</html>