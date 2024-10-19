<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>거상 대화방</title>
<script language="javascript">
<!--
function onStart( loginid, NickName )
{
	top.frames[0].JoyOnChat.strSetLoginID(loginid);
	top.frames[0].JoyOnChat.strSetNickName(NickName);
	top.frames[0].JoyOnChat.strSetServerIP("220.85.18.73");
	top.frames[0].JoyOnChat.ssSetServerPort(3352);
	top.frames[0].JoyOnChat.Start(); 
}
//-->
</script>
</head>
<body onload="onStart('<%=strID%>','<%=strName%>');">
</body>
</html>