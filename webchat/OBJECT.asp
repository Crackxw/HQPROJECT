<% Option Explicit %>
<% Response.Expires = 0 %>
<%	Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>�Ż� ��ȭ��</title>
<SCRIPT for="JoyOnChat" event="ResLogin(Result)" language="JavaScript">
<!--
//���ü��� ���� ����	: 0
//���ü��� ���� ����	: 99
if (Result == 0){
	top.frames[1].location.href = "ROOM.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
}
else if (Result == 99){
	alert("���ü��� ���� ����");
}
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResMakeRoom(Result, RoomNum)" language="JavaScript">
<!--
//�� ����� ���� 				: 0
//�� �̻� ���� ���� �� ���� ��	: 1
//��й�ȣ ���� �̻��ϰ� ������	: 2
//�Էµȹ� �Ӽ� ���� �̻��Ҷ�		: 3
//������ �ʴ� �������϶�		: 4
if (Result == 0){
	top.frames[1].location.href="CHATTING.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>"
	return false;
}
else if (Result == 1){
	alert("�� �̻� ��ȭ���� ����� �����ϴ�.");	
	return false;
}
else if (Result == 2){
	alert("��й�ȣ�� �Ҹ�Ȯ�մϴ�.");
	return false;
}
else if (Result == 3){
	alert("�� Ÿ���� �Ҹ�Ȯ�մϴ�.");
	return false;
}
else if (Result == 4){
	alert("������ ���� ������ �Դϴ�.");
	return false;
}
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResJoinRoom(Result)" language="JavaScript">
<!--
//�ִ� �ο� �ʰ�				: 5
//���� ��������				: 6
//������ �ʴ� �� ��ȣ�϶�	: 7
//Ʋ�� ��� ��ȣ				: 8

var strURL_5;
var strURL_6;
var strURL_7;
var strURL_8;

if (Result == 5){
	strURL_5 = "POP_MSG.asp?MGG=�ִ��ο� �ʰ�";
	window.open(strURL_5,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
else if (Result == 6){
	strURL_6 = "POP_MSG.asp?MGG=���� �����ֽ��ϴ�.";
	window.open(strURL_6,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
else if (Result == 7){
	strURL_7 = "POP_MSG.asp?MGG=������ ���� �� ��ȣ�Դϴ�.";
	window.open(strURL_7,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
else if (Result == 8){
	strURL_8 = "POP_MSG.asp?MGG=��й�ȣ�� �����ʽ��ϴ�.";
	window.open(strURL_8,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
else{
	top.frames[1].location.href="CHATTING.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
	return false;
}
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResBeginRoomList()" language="JavaScript">
<!--
//�� ����Ʈ �ޱ� ����
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResRoomList(RoomNum, RoomCode, RoomSubject, OwnerID, OwnerNickName, RoomType, MaxUserNum, CurrentUserNum)" language="JavaScript">
<!--
var RommType_Text;
var insertRoomList;
var ReqJoinRoom_value;

if (RoomType == 0){
	RommType_Text = "�Ϲ� ��ȭ��";
}
else if (RoomType == 1){
	RommType_Text = "��� ��ȭ��";
}

ReqJoinRoom_value = (RoomNum + "," + RoomCode + "," + RoomType);

insertRoomList = ("<input type='button' value='������' onClick=ReqJoinRoomReady(" + ReqJoinRoom_value + ");>" +
					" �� ��ȣ : " + RoomNum + 
					" �� ���� : " + RoomSubject + " (" + RommType_Text + ") " +
					" ���� ID : " + OwnerID +
					" ���� �г��� : " + OwnerNickName +
					" (" + MaxUserNum + " / " + CurrentUserNum + ")<br>");

top.frames[1].frames[0].document.body.insertAdjacentHTML('beforeEnd', insertRoomList);	

//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResEndRoomList()" language="JavaScript">
<!--
//�� ����Ʈ �ޱ� ��
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="NotifyUserJoined(UserID, NickName)" language="JavaScript">
<!--
//�� ����
var NotifyUserJoined_text
var iinsertCharMSG 

NotifyUserJoined_text = ("[" + UserID + "] �� ����");
iinsertCharMSG = 	("<b><font color='blue'>" + NotifyUserJoined_text + "</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', iinsertCharMSG);

//�ٸ������ �� ���� ������ ����� ����Ʈ�� �ٽ� �ҷ��´�.
//Room_User_List������ JoyOnChat.ReqUserList();�� ȣ���Ѵ�.
top.frames[1].frames[2].location.reload();
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResBeginUserList(count)" language="JavaScript">
<!--
//�� ������ ����Ʈ �ޱ� ����
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResUserList(Parser)" language="JavaScript">
<!--
//input hidden�� �����ϳ��.
document.ObjectForm.strParser.value = Parser;

var insertUserList;
var insertOwnerList;
var ResUserList;

var OwnerID;
var OwnerName;

var len = Parser.length;

var szID = new Array;
var szName = new Array;
var szType = new Array;

var ch;
var bReadSlash = false;
var strCode = -1;
var index = -1;

for(var i=0; i<Parser.length; i++){
	ch = Parser.charAt(i)

	if(bReadSlash == true) {
		switch(ch) {
			case 'i':	// ID
				index++;
				szID[index] = '';
				strCode = 0;
				break;
			case 'n':	// NAME
				szName[index] = '';
				strCode = 1;
				break;
			case 't':	// TYPE
				szType[index] = '';
				strCode = 2;
				break;
		}
		
		bReadSlash = false;

		if(ch != "/") continue;
		
	} else if(ch == "/"){
		bReadSlash = true;
		continue;
	}

	switch( strCode ) {
		case 0:
			szID[index] += ch;
			break;
		case 1:
			szName[index] += ch;
			break;
		case 2:
			szType[index] += ch;
			break;
	}
}

var k = 1
for( i=0; i<= index; i++){
	top.frames[1].frames[3].document.sendform.target.options[k] = new Option( '', '', false);
	k=k+1
}

var j=1;
for( i=0; i<= index; i++){
	//alert("ID : " + szID[i] + "\n\nNAME : " + szName[i] + "\n\nType : " + szType[i]);
	
	ResUserList = szID[i] + " / " + szName[i] + " / " + szType[i];
	insertUserList = 	("<a href='#' onClick='PutMenu();'>" + ResUserList + "</a><br>");

	top.frames[1].frames[2].document.body.insertAdjacentHTML('beforeEnd', insertUserList);

//	if (szID[i] != "<%=strID%>"){
		top.frames[1].frames[3].document.sendform.target.options[j] = new Option( szID[i], szID[i], false);
		j=j+1
//	}

}
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResEndUserList()" language="JavaScript">
<!--
//�� ������ ����Ʈ �ޱ� ��
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResChatMsg(strUserID, strChatMsg, sFontType, sTextSize, sTextColor)" language="JavaScript">
<!--
var text_style

//	sFontType 	1: ����	2: ����	3: ����	4: �ü�

if (sFontType == 1){text_style = "face=\"����\" ";}
else if (sFontType == 2){text_style = "face=\"����\" ";}
else if (sFontType == 3){text_style = "face=\"����\" ";}
else if (sFontType == 4){text_style = "face=\"�ü�\" ";}
else if (sFontType == 0){text_style = "";}

//	sTextColor	1: #FFEA00 ���
//				2: #FF7E00 ������
//				3: #FF3333 ����
//				4: #990000 ����
//				5: #FF00F6 ��ȫ
//				6: #68269A ����
//				7: #33339D ����
//				8: #268F81 û�ϻ�
//				9: #0060FF �Ķ�
//				10: #156B0F ���
//				11: #90D133 ����
//				12: #989898 ȸ��
//				13: #C0C0C0 ����ȸ��

if (sTextColor == 1){text_style += "color=\"#FFEA00\" ";}
else if (sTextColor == 2){text_style += "color=\"#FF7E00\" ";}
else if (sTextColor == 3){text_style += "color=\"#FF3333\" ";}
else if (sTextColor == 4){text_style += "color=\"#990000\" ";}
else if (sTextColor == 5){text_style += "color=\"#FF00F6\" ";}
else if (sTextColor == 6){text_style += "color=\"#68269A\" ";}
else if (sTextColor == 7){text_style += "color=\"#33339D\" ";}
else if (sTextColor == 8){text_style += "color=\"#268F81\" ";}
else if (sTextColor == 9){text_style += "color=\"#0060FF\" ";}
else if (sTextColor == 10){text_style += "color=\"#156B0F\" ";}
else if (sTextColor == 11){text_style += "color=\"#90D133\" ";}
else if (sTextColor == 12){text_style += "color=\"#989898\" ";}
else if (sTextColor == 13){text_style += "color=\"#C0C0C0\" ";}
else if (sTextColor == 0){text_style += "";}


//	sTextSize	1: 8pt 2: 10pt 3: 12pt 4: 14pt 5: 16pt 6: 18pt 7: 20pt 8: 22pt
				
if (sTextSize == 1){text_style += "size=\"8pt\" ";}
else if (sTextSize == 2){text_style += "size=\"10pt\" ";}
else if (sTextSize == 3){text_style += "size=\"12pt\" ";}
else if (sTextSize == 4){text_style += "size=\"14pt\" ";}
else if (sTextSize == 5){text_style += "size=\"16pt\" ";}
else if (sTextSize == 6){text_style += "size=\"18pt\" ";}
else if (sTextSize == 7){text_style += "size=\"20pt\" ";}
else if (sTextSize == 8){text_style += "size=\"22pt\" ";}
else if (sTextSize == 0){text_style += "";}

var insertCharMSG 
insertCharMSG = ("<b>[ " + strUserID + " ]</b> : <font " + text_style + ">" + strChatMsg + " </font><br>");

//alert(insertCharMSG);	
top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', insertCharMSG);	
		
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResWhisper (Result, UserID, ChatMsg)" language="JavaScript">
<!--
// �ӼӸ�

var iinsertCharMSG 
var strURL_99 = "POP_MSG.asp?MGG=�ӼӸ� ���� ����";
	
if (Result == 99){
	window.open(strURL_99,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
else{
	iinsertCharMSG = ("<b><font color='green'> [ " + UserID + " / �ӼӸ� ] : " + ChatMsg + "</font></b><br>");
	
	top.frames[1].frames[1].OnScroll();
	top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', iinsertCharMSG);
}
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="NotifyWhisper(UserID, ChatMsg)" language="JavaScript">
<!--
// �ӼӸ�
var insertCharMSG 

insertCharMSG = ("<b><font color='green'> [ " + UserID + " / �ӼӸ� ] : " + ChatMsg + "</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', insertCharMSG);
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResMyRoomInfo(RoomSubject, ssOpenRoom, ssMaxUserNum, ssCurrentUserNum, PassWord)" language="JavaScript">
<!--
//input hidden�� �� ������ �����س��´�.
document.ObjectForm.strRoomTitle.value = RoomSubject;
document.ObjectForm.intRoomType.value = ssOpenRoom;
document.ObjectForm.intMaxUserNumber.value = ssMaxUserNum;
document.ObjectForm.intCurrUserNumber.value = ssCurrentUserNum;
document.ObjectForm.strRoomPass.value = PassWord;

top.frames[1].frames[0].location.href="CHAT_TITLE.asp?RoomTitle=" + RoomSubject;
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="NotifyRoomSubjectChanged(RoomSubject)" language="JavaScript">
<!--
//���̸� �ٲ����� input hidden�� �����ϰ� �� �̸��� reload��Ų��.

var insertMSG

insertMSG = ("<b><font color='green'>[�� ���� ���� : " + RoomSubject + "]</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', insertMSG);

document.ObjectForm.strRoomTitle.value = RoomSubject;
top.frames[1].frames[0].location.href = "CHAT_TITLE.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>&RoomTitle=" + RoomSubject;
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="NotifyRoomMasterChanged(UserID)" language="JavaScript">
<!--
// ���� ����
var insertMSG 

insertMSG = ("<b><font color='green'> [ ���� ��ü : " + UserID + " ]</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', insertMSG);
top.frames[1].frames[2].location.reload();
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResChangeRoomMaster(Result)" language="JavaScript">
<!--
//���� ��ü
var strURL_0 = "POP_MSG.asp?MGG=���� ��ü ����";
var strURL_99 = "POP_MSG.asp?MGG=���� ��ü ����";

if (Result == 0){
	alert("���屳ü ����");
//	window.open(strURL_0,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
//	top.frames[1].frames[2].location.reload();
//	return false;
}
else if (Result == 99){
	alert("���屳ü ����");
//	window.open(strURL_99,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
//	return false;
}
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResChangeRoomInfo(Result)" language="JavaScript">
<!--
//�� ���� ����
var strURL_99 = "POP_MSG.asp?MGG=������ ���� ����";
if (Result == 99){
	var change_room_title = open(strURL_99,"POP_MSD", "width=1500,height=100,marginwidth=0,marginheight=0,border=0");
	return false;
}
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="NotifyRoomAttrChanged(MaxUserNum, RoomType)" language="JavaScript">
<!--
//�� ���� ����
var RommType_Text
var insertMSG 

if (RoomType == 0){
	RommType_Text = "�Ϲ� ��ȭ��";
}
else if (RoomType == 1){
	RommType_Text = "��� ��ȭ��";
}
insertMSG = ("<b><font color='green'> [ ������ ���� / �ִ��ο� :" + MaxUserNum + " / �� Ÿ�� : " + RommType_Text + " ]</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', insertMSG);
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="ResKickUserOut(Result)" language="JavaScript">
<!--
//����
//-->  
</SCRIPT>

<SCRIPT for="JoyOnChat" event="NotifyKickedOut()" language="JavaScript">
<!--
// ����
JoyOnChat.ReqExitRoom();
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="NotifyUserExited (Result, UserID)" language="JavaScript">
<!--
//�濡�� ����

var NotifyUserExited_text
var iinsertCharMSG 

if (Result == 0 || Result == 2){
	NotifyUserExited_text = ("[" + UserID + "] �� ����");
}
else if (Result == 1){
	NotifyUserExited_text = ("[" + UserID + "] �����");
}
	iinsertCharMSG = 	("<b><font color='red'>" + NotifyUserExited_text + "</font></b><br>");

top.frames[1].frames[1].OnScroll();
top.frames[1].frames[1].document.body.insertAdjacentHTML('beforeEnd', iinsertCharMSG);

//�ٸ������ �� ������ ����� ����Ʈ�� �ٽ� �ҷ��´�.
top.frames[1].frames[2].location.reload();
//-->
</SCRIPT> 

<SCRIPT for="JoyOnChat" event="ResExitRoom()" language="JavaScript">
<!--
//�濡�� ������.
//-->
</SCRIPT>

<SCRIPT for="JoyOnChat" event="Exiting()" language="JavaScript">
<!--
//�Ż� ������ �����Ѵ�.
self.close();
//-->
</SCRIPT>

<script language="javascript">
<!--

var isInstall = false;
var runIt = false;

function ocxchk() {
	if (typeof(document.all("JoyOnChat"))!="undefined" && document.all("JoyOnChat").object!=null){ 
		isInstall = true;
	}
	else{
		isInstall = false;
	}
}

function add_big() {
	if(isInstall&&!runIt) {
		top.frames[1].location.href="USER_INFO.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>"
		runIt = true;
	} 
	else {
		alert("�Ż� ���� ��Ʈ���� ��ġ���� �ʾҽ��ϴ�.!!!\n������ �ϱ� ���ؼ��� �ݵ�� ��Ʈ���� ��ġ �ϼž� �մϴ�.");
	}  
}


function OpenRoomOption(){

	var strParser = document.ObjectForm.strParser.value;
	var RoomSubject = document.ObjectForm.strRoomTitle.value;
	var ssOpenRoom = document.ObjectForm.intRoomType.value;
	var ssMaxUserNum = document.ObjectForm.intMaxUserNumber.value = ssMaxUserNum;
	var ssCurrentUserNum = document.ObjectForm.intCurrUserNumber.value = ssCurrentUserNum;
	var PassWord = document.ObjectForm.strRoomPass.value = PassWord;
	
	var strURL = "CHANGE_ROOM_INFO.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>&RoomSubject=" + RoomSubject + "&ssOpenRoom=" + ssOpenRoom + "&ssMaxUserNum=" + ssMaxUserNum + "&ssCurrentUserNum=" + ssCurrentUserNum + "&PassWord=" + PassWord + "&Parser=" + strParser;
	var change_room_title = open(strURL,"change_room_title", "width=500,height=300,marginwidth=0,marginheight=0,border=0");
}
//-->
</script>
</head>
<body onload="ocxchk();add_big();">
<OBJECT id=JoyOnChat style="LEFT: 0px; TOP: 0px" codeBase=HTTP://gm.joyon.com/GersangChat/JoyOnChat.cab#version=1,0,0,7 height=0 width=0 classid="clsid:269ACE65-4849-4832-9A4C-CAA98D8E73C1">
</OBJECT>
<form name="ObjectForm">
	<input type="hidden" name="strParser" value="">
	<input type="hidden" name="strRoomTitle" value="">
	<input type="hidden" name="intRoomType" value="">
	<input type="hidden" name="intMaxUserNumber" value="">
	<input type="hidden" name="intCurrUserNumber" value="">
	<input type="hidden" name="strRoomPass" value="">
	<input type="hidden" name="strPageLoad" value="">
</form>
</body>
</html>