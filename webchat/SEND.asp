<HTML>
<HEAD>
<TITLE>�Ż� ��ȭ��</TITLE>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
</HEAD>

<SCRIPT LANGUAGE=JavaScript>
<!--

function onSend(){
	if (event.keyCode == 8){
		// �ڷΰ�����..
		return false;
	}
}

function defualtFont(){
	window.location.reload();
}

function addFontSel(form){
//	var where = form.selectedIndex;
//	if( where > 0 ){
//		form.selectedIndex = 0;
//		addFont(form.options[where].value);
//	}
}

function addFont(clr){
//	if(clr == null || clr == ""){
//	return;
//	}

//	document.sendform.msg.focus();
//	document.sendform.msg.value += "/" + clr + "/";
//	return;	
}

function addBgSel(form){
	var where = form.selectedIndex;
	
	if(0 == where)
		color = "black";
	else
		color = form.options[where].value;
	
	document.sendform.msg.focus();
	return;
}

function sendmsg(){
	var msg = document.sendform.msg;
	var targetUser = document.sendform.target.value;
	var sFontType = document.sendform.fontSel.value;
	var sTextSize = document.sendform.sizeSel.value;
	var sTextColor = document.sendform.defcolor.value;
	
	if(msg.length == 0 || msg.value == ""||msg.value == null){
		return false;
	}
	//alert(msg.value);
	
	if (document.sendform.target.selectedIndex == 0){
		//top.frames[0].JoyOnChat.ReqChatMsg(msg.value);
		
		top.frames[0].JoyOnChat.ReqChatMsg(msg.value, sFontType, sTextSize, sTextColor);
		document.sendform.msg.value = "";
		return false;
	}
	else{
		//alert(targetUser + " : " + msg.value);
		if (targetUser == ""){
			alert("�ӼӸ��� ����� ������ �ּ���.");
			return false;
		}
		else{
			top.frames[0].JoyOnChat.ReqWhisper(targetUser, msg.value);
			document.sendform.msg.value = "";
			return false;
		}
	}
}

function msgFocus(){
	document.sendform.msg.focus();
}

//-->
</SCRIPT>

<BODY BGCOLOR=#DFDFDF TOPMARGIN=0 LEFTMARGIN=0 onLoad="msgFocus()" onKeyDown="onSend()"><CENTER>
<FORM name=sendform method=post onSubmit="return sendmsg();">

<TABLE WIDTH=100% BORDER=0 CELLPADDING=0 CELLSPACING=0>
	<TR VALIGN=TOP HEIGHT=25>
		<TD>
		<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
			<TR VALIGN=TOP>
				<TD ALIGN=RIGHT NOWRAP>
					<SELECT NAME=target>
						<OPTION> - ��� - </OPTION>
					</SELECT>
					<INPUT MAXLENGTH=128 SIZE=100 NAME=msg STYLE="WIDTH=420">
				</TD>
				<TD ALIGN=RIGHT></TD>
			</TR>
		</TABLE>
		</TD>
	</TR>
</TABLE>

<TABLE WIDTH=100% BORDER=0 CELLPADDING=0 CELLSPACING=0>
	<TR>
		<TD><IMG BORDER="0" WIDTH="17" HEIGHT="17" SRC="images/icon1-1.gif"></TD>
		<TD><SPAN onClick='defualtFont();' onMouseOver="this.style.cursor='hand'"><B>�����·�</B></SPAN></TD>
		<TD><IMG BORDER=0 WIDTH=17 HEIGHT=17 SRC=images/icon2-1.gif></TD>	
		<TD>
			<SELECT NAME=fontSel onChange="addFontSel(this);">
				<OPTION VALUE="0"><FONT FACE=Arial>�۲�</FONT></OPTION>
				<OPTION VALUE="1">����</OPTION>
				<OPTION VALUE="2">����</OPTION>
				<OPTION VALUE="3">����</OPTION>
				<OPTION VALUE="4">�ü�</OPTION>
			</SELECT>
		</TD>
		<TD><IMG BORDER="0" WIDTH="17" HEIGHT="17" SRC="images/icon3-1.gif"></TD>
		<TD>
			<SELECT NAME="sizeSel" onChange="addFontSel(this);">
				<OPTION VALUE="0">ũ��</OPTION>
				<OPTION VALUE="1">8 pt</OPTION>
				<OPTION VALUE="2">10 pt</OPTION>
				<OPTION VALUE="3">12 pt</OPTION>
				<OPTION VALUE="4">14 pt</OPTION>
				<OPTION VALUE="5">16 pt</OPTION>
				<OPTION VALUE="6">18 pt</OPTION>
				<OPTION VALUE="7">20 pt</OPTION>
				<OPTION VALUE="8">22 pt</OPTION>
			</SELECT>
		</TD>
<!--
		<TD><IMG BORDER="0" WIDTH="17" HEIGHT="17" SRC="images/icon4-1.gif"></TD>
		<TD><SPAN onClick='eval("addFont(\"��\")");' onMouseOver="this.style.cursor='hand'"><B>���ϰ�</B></SPAN></TD>
		<TD><IMG BORDER="0" WIDTH="17" HEIGHT="17" SRC="images/icon5-1.gif"></TD>
		<TD><SPAN onClick='eval("addFont(\"��\")");' onMouseOver="this.style.cursor='hand'"><B>�����</B></SPAN></TD>
-->
		<TD><IMG BORDER="0" WIDTH="17" HEIGHT="17" SRC="images/icon6-1.gif"></TD>
		<TD>
			<SELECT NAME="defcolor" onChange="addBgSel(this);">		
				<OPTION VALUE="0"> -- �⺻��</OPTION>
				<OPTION VALUE="1" STYLE="BACKGROUND: #FFEA00;">���</OPTION>
				<OPTION VALUE="2" STYLE="BACKGROUND: #FF7E00;">������</OPTION>
				<OPTION VALUE="3" STYLE="COLOR=#FFFFFF; BACKGROUND: #FF3333;">����</OPTION>
				<OPTION VALUE="4" STYLE="COLOR=#FFFFFF; BACKGROUND: #990000;">����</OPTION>
				<OPTION VALUE="5" STYLE="BACKGROUND: #FF00F6;">��ȫ</OPTION>
				<OPTION VALUE="6" STYLE="COLOR=#FFFFFF; BACKGROUND: #68269A;">����</OPTION>
				<OPTION VALUE="7" STYLE="COLOR=#FFFFFF; BACKGROUND: #33339D;">����</OPTION>
				<OPTION VALUE="8" STYLE="BACKGROUND: #268F81;">û�ϻ�</OPTION>
				<OPTION VALUE="9" STYLE="BACKGROUND: #0060FF;">�Ķ�</OPTION>
				<OPTION VALUE="10" STYLE="COLOR=#FFFFFF; BACKGROUND: #156B0F;">���</OPTION>
				<OPTION VALUE="11" STYLE="BACKGROUND: #90D133;">����</OPTION>
				<OPTION VALUE="12" STYLE="BACKGROUND: #989898;">ȸ��</OPTION>
				<OPTION VALUE="13" STYLE="BACKGROUND: #C0C0C0;">����ȸ��</OPTION>
			</SELECT>
		</TD>
<!--
		<TD>
			<TABLE BORDER=2 CELLPADDING=0 CELLSPACING=0>
				<TR>            
					<TD WIDTH=9 BGCOLOR=#FFEA00><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#FF7E00><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#FF3333><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#990000><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#FF00F6><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#68269A><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#33339D><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
				</TR>
				<TR>
					<TD WIDTH=9 BGCOLOR=#268F81><A HREF=javascript:eval("addFont(\"û\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#0060FF><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#156B0F><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#90D133><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#989898><A HREF=javascript:eval("addFont(\"ȸ\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#C0C0C0><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
					<TD WIDTH=9 BGCOLOR=#000000><A HREF=javascript:eval("addFont(\"��\")");>
						<IMG WIDTH=12 HEIGHT=6 SRC=skin/x.gif BORDER=0></A></TD>
				</TR>
			</TABLE>
		</TD>
-->
	</TR>
</TABLE>

<TABLE WIDTH=100% BORDER=0 CELLPADDING=0 CELLSPACING=0>
	<TR><TD></TD></TR>
</TABLE>
</form>
</BODY>
</HTML>
<script language="javascript">
<!--
//	������ ���� ���ϱ�
//	1. Chatting_Send
//	2. Chatting_Title
//	3. Chatting_User_List
//	4. Chatting_board
//	5. Chatting_Door

var PageLoad = top.frames[0].document.ObjectForm.strPageLoad.value;

if (PageLoad == "true" || PageLoad == ""){
	top.frames[1].frames[0].location.href = "CHAT_TITLE.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
}
//-->
</script>
