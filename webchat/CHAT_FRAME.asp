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
<frameset rows="0,*" frameborder="0">
	<frame name="TOP" scrolling="no" src="OBJECT.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
	<frame name="MAIN" scrolling="no" src="PAGE_WAIT.asp">
</frameset>
<noframes>
<body bgcolor="#FFFFFF" text="#000000">
	프래임을 지원하지 않는 브라우저 입니다.
</body>
</noframes> 
</html>