function initial() {
	msg = document.getElementById("msg");
	document.getElementById("closebtn").disabled = true;
	//绘图部分初始化
	var started = false;
    var mText_canvas = null;
    var x = 0, y =0;
    var canvas = document.getElementById("myCanvas");		
    context = canvas.getContext("2d");
    context.fillStyle="blue";
    var rain_num = 0;
	//本地存储的数据消息
	localStorage.basedArc = "";
	//人物属性设置
	localStorage.character = "探险者;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.loc_changable = 1;//人物位置是否是可以移动的
	localStorage.temporary_character_loc = "";
	localStorage.temporary_loc = "";
	localStorage.turnOn = 1;
	localStorage.move =1;
	localStorage.buttonInfo = "";
		//游戏玩家
	localStorage.player_1 = "守护1;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.player_2 = "守护2;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.player_3 = "守护3;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.player_4 = "守护4;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.player_5 = "守护5;2;#cdd3d7|||;3|3;3|3;0";
	localStorage.player_6 = "守护5;2;#cdd3d7|||;3|3;3|3;0";
	//NPC设置
	localStorage.npc_1 = "先锋;17;#f7de92|||;3|3;1|3;0|2|16,17,18,21,22|1|1|1;0;|0";//土黄
	localStorage.npc_2 = "护卫;38;#d592f7|||;3|3;3|3;0|2|32,33,37,38,39|1|1|1;5;|0";//淡紫
	localStorage.npc_3 = "护卫;40;#d592f7|||;3|3;1|3;0|2|35,36,40,41|1|1|1;5;|0";   //淡紫
	localStorage.npc_4 = "霹雳;59;#08f3f0|||;3|3;3|3;0|2|52,53,54,58,59|1|1|1;4;|0";//亮蓝
	localStorage.npc_5 = "深渊;51;#8b056d|||;3|3;3|3;0|2|45,46,47,48,51|1|1|1;9;|0";//紫红
	localStorage.npc_6 = "神罗;55;#f3cf08|||;3|3;3|3;0|2|50,55,56|1|1|1;7;|0";      //金黄
	localStorage.npc_move_turn = 1;
	//颜色记录
	localStorage.move_base_blue = "#3498db";//角色移动时候，显示可以匹配的拼版的颜色
	localStorage.character_color = "#cdd3d7";//角色本身的拼版颜色
	localStorage.character_mover = "#88aaaa";//移动中拼版的颜色
	localStorage.skill_color_0 = "#9b59b6";//技能0颜色：紫
	localStorage.skill_color_1 = "#3498db";//技能1颜色：蓝
	localStorage.skill_color_1 = "#2ecc71";//技能1颜色：绿
	localStorage.skill_color_1 = "#99bbbb";//技能1颜色：灰
	//技能信息
	//0:名称，1：释放效果（=0，无状态改变；=1，自身状态更改；=2：指向性技能状态);2:自身状态更改具体效果（=1:隐身）
	localStorage.skill = "弱者气场,1,1,1,0,0;愤怒,2,0,1,3;火刺,2,1,1,1|3|1,1,-1;抱怨,3,3,1,1,1,1,1";
	
	//技能影响信息
	localStorage.skill_influ = "0;0;1,1,0,0;0";
			
}