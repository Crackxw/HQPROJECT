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
<frameset rows="0,*" frameborder="0">
	<frame name="TOP" scrolling="no" src="OBJECT.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>">
	<frame name="MAIN" scrolling="no" src="PAGE_WAIT.asp">
</frameset>
<noframes>
<body bgcolor="#FFFFFF" text="#000000">
	�������� �������� �ʴ� ������ �Դϴ�.
</body>
</noframes> 
</html>