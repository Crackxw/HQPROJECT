<html>
<head>
<title>거상 대화방 테스트</title>
<script language="javascript">
<!--
function OnScroll(){
	window.scrollBy(0,20);
}
-->
</script>
</head>
<body>
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

var PageLoad = top.frames[0].document.ObjectForm.strPageLoad.value;

if (PageLoad == "true" || PageLoad == ""){
	top.frames[1].frames[4].location.href= "DOOR.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
}
//-->
</script>