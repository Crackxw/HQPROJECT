<% Option Explicit %>
<% Response.Expires = 0 %>
<% Dim strID, strName, strPass
	strID = Request("ID")
	strName = request("name")
	strPass = request("pass")	%>
<html>
<head>
<title>�Ż� ��ȭ��</title>
<style>
body, td, input	{font-family:Verdana, Arial, Helvetica, sans-serif; font-size:11px; }
</style>
</head>
<body topmargin="0" leftmargin="0" onLoad="ReqUserList();">
<SCRIPT type=text/javascript>
<!--
var ie5=window.createPopup
var eyesys="";
var preitem="";
var menucount=false

function PutMenu(){
	if (menucount==false){
		menucount=true;
		init();
	}
	else{
		menucount=false;
		return false
	}
}
	
	
	
function init(){
	mx=event.clientX;
	my=event.clientY;
	menx=window.screenLeft+mx;
	meny=window.screenTop+my;
	sysmen=window.createPopup();
	sysmen.document.write(eyesys);
	sysmen.show(menx,meny,eyesys_width,document.getElementById('men').offsetHeight);
	return false
}

function eyesys_init(){
	if (ie5){
		eyesys+=("<style type='text/css'>.textul{position:absolute;top:0px;color:"+eyesys_titletext+";writing-mode:	tb-rl;padding-top:10px;filter: flipH() flipV() dropShadow( Color=000000,offX=-2,offY=-2,positive=true);z-Index:10;width:100%;height:100%;font: bold 12px sans-serif}.gradientul{position:relative;top:0px;left:0px;width:100%;background-color:"+eyesys_titlecol2+";height:100%;z-Index:9;FILTER: alpha( style=1,opacity=0,finishOpacity=100,startX=100,finishX=100,startY=0,finishY=100)}.contra{background-color:"+eyesys_titlecol1+";border:1px inset "+eyesys_bg+";height:98%;width:18px;z-Index:8;top:0px;left:0px;margin:2px;position:absolute;}.men{position:absolute;top:0px;left:0px;padding-left:18px;background-color:"+eyesys_bg+";border:2px outset "+eyesys_bg+";z-Index:1;}.men a{margin:1px;cursor:default;padding-bottom:4px;padding-left:1px;padding-right:1px;padding-top:3px;text-decoration:none;height:100%;width:100%;color:"+eyesys_cl+";font:normal 12px sans-serif;}.men a:hover{background:"+eyesys_bgov+";color:"+eyesys_clov+";} BODY{overflow:hidden;border:0px;padding:0px;margin:0px;}.ico{border:none;float:left;}</style><div class='men'>")
	}
}
	
function eyesys_item(txt,ico,lnk){
	if (ie5){
	//���� ������ ���� x.gif
		if(!ico)ico='http://220.85.18.165/images/x.gif';
			preitem+=("<a href='#' onmousedown='parent.window.location.href=\""+lnk+"\"'><img src='"+ico+"' width='16' height='16' class='ico'> "+txt+"</a>")
	}
}
	
function eyesys_close(){
	if (ie5){
	eyesys+=preitem;
	eyesys+=("</div><div class='contra'><div class='gradientul'></div><div class='textul' id='titlu'>"+eyesys_title+"</div></div>");
	document.write("<div id='men' style='width:"+eyesys_width+"'></div>");
	document.getElementById('men').innerHTML=preitem
	}
}

function ReqUserList(){
	top.frames[0].JoyOnChat.ReqUserList();	
	return true;
}
//-->
</SCRIPT>

<style>
<!--
#men {
BORDER-RIGHT: 2px outset; BORDER-TOP: 2px outset; Z-INDEX: 1; LEFT: 0px; VISIBILITY: hidden; BORDER-LEFT: 2px outset; BORDER-BOTTOM: 2px outset; POSITION: absolute; TOP: 0px
}
#men A {
PADDING-RIGHT: 1px; PADDING-LEFT: 1px; PADDING-BOTTOM: 4px; MARGIN: 1px 1px 1px 16px; FONT: 12px sans-serif; WIDTH: 100%; PADDING-TOP: 3px; HEIGHT: 100%; TEXT-DECORATION: none
}
.ico {
BORDER-RIGHT: medium none; BORDER-TOP: medium none; FLOAT: left; BORDER-LEFT: medium none; BORDER-BOTTOM: medium none
}
//-->
</style>

<SCRIPT type=text/javascript>
<!--
//�޴� ����
eyesys_title="Gersang"	
//���� ����
eyesys_titlecol1="black"			
//���� ������ ��ܻ�
eyesys_titlecol2="blue"				 
// ���� ������ �ϴܻ�
eyesys_titletext="white"			
// ������ ���ڻ�
eyesys_bg="#DFDFDF"				
//�޴��� ����
eyesys_bgov="#006699"			
// ���콺�� Over �Ǿ����� �޴��� ���� 
eyesys_cl="black" 			
//�޴��� ���ڻ�
eyesys_clov="white" 	
// ���콺�� Over �Ǿ����� �޴��� ���ڻ�
eyesys_width=120		
// �޴��� ��

//�޴��� ���� �κ�
//�޴� ������ ���� : eyesys_item(����,������,��ũ)
//���� �������� ������  null�� �ϸ�ȴ�.
eyesys_init()
eyesys_item('����� ����',null,'')
eyesys_item('���� ������',null,'')

eyesys_item('',null,'')
eyesys_close()
//-->
</SCRIPT>
</body>
</html>

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
	top.frames[1].frames[1].location.href= "BOARD.asp?ID=<%=strID%>&name=<%=strName%>&pass=<%=strPass%>";
}
//-->
</script>