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
	localStorage.character = "探险者;2;#cdd3d7|chara_image_1||;3|3;3|3;0|2|1|1|1|1;0;|0;character_skill";
	localStorage.character_skill = "6;结束回合,1,0,0,3|1,1,0,0,#5b59b6|,0|0,4|1;" +
			"不爽,2,0,2,3|1|2,1,-1,-1,#9b59b6|,1|0;" +
			"愤怒,2,0,2,3|7|3,1,-1,-1,#3498db|,1|0;" +
			"李布,2,1,1,1|3|1,1,-1,-2,#2ecc71|,1|0;" +
			"抱怨,2,1,1,1|3|1,1,-1,-1,#99bbbb|,1|0;" +
			"弱者气场,1,0,0,3|1,1,0,0,#5b59b6|,1|0,3|2[不爽(愤怒[3(0{不爽[0[0[0[0(0(0[0[-1[0[0(0[0(0{愤怒[0[0[0[0(0(0[0[-1[0[0(0[0(0"; 
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
	localStorage.npc_1 = "先锋;17;#f7de92|npc_image_1||;3|3;2|1;0|2|16,17,18,21,22|1|1|1;0;|0;npc_1_skill";//土黄
	localStorage.npc_2 = "护卫;38;#d592f7|npc_image_2||;3|3;3|3;0|2|32,33,37,38,39|1|1|1;0;|0;npc_2_skill";//淡紫
	localStorage.npc_3 = "护卫;40;#d592f7|npc_image_3||;3|3;1|3;0|2|35,36,40,41|1|1|1;0;|0;npc_3_skill";   //淡紫
	localStorage.npc_4 = "霹雳;59;#08f3f0|npc_image_4||;3|3;3|3;0|2|52,53,54,58,59|1|1|1;0;|0;npc_4_skill";//亮蓝
	localStorage.npc_5 = "深渊;51;#8b056d|npc_image_5||;3|3;3|3;0|2|45,46,47,48,51|1|1|1;0;|0;npc_5_skill";//紫红
	localStorage.npc_6 = "神罗;55;#f3cf08|npc_image_6||;3|3;3|3;0|2|50,55,56|1|1|1;0;|0;npc_6_skill";      //金黄
	localStorage.npc_move_turn = 1;
	//NPC技能设置
	localStorage.npc_1_skill = "1;攻击,2,0,1,2|1|1,1,-1,-1,#99bbbb|,1|0";
	localStorage.npc_2_skill = "1;强攻,2,0,1,2|1|1,1,-3,-1,#99bbbb|,1|0";
	localStorage.npc_3_skill = "1;强攻,2,0,1,2|1|1,1,-3,-1,#99bbbb|,1|0";
	localStorage.npc_4_skill = "1;致命,2,0,2,2|1|1,1,-2,-1,#99bbbb|,1|0";
	localStorage.npc_5_skill = "1;不爽,2,0,2,3|1|2,1,-1,-1,#9b59b6|,1|0";
	localStorage.npc_6_skill = "1;远射,2,0,2,2|1|1,1,-1,-1,#99bbbb|,1|0";
	//颜色记录
	localStorage.move_base_blue = "#3498db";//角色移动时候，显示可以匹配的拼版的颜色
	localStorage.character_color = "#cdd3d7";//角色本身的拼版颜色
	localStorage.character_mover = "#88aaaa";//移动中拼版的颜色
	localStorage.skill_color_0 = "#9b59b6";//技能0颜色：紫
	localStorage.skill_color_1 = "#3498db";//技能1颜色：蓝
	localStorage.skill_color_1 = "#2ecc71";//技能1颜色：绿
	localStorage.skill_color_1 = "#99bbbb";//技能1颜色：灰
//	//技能信息
//	localStorage.skill = "不爽,2,0,2,3|1|2,1,-1,1,#9b59b6|;愤怒,2,0,2,3|7|3,1,-1,1,#3498db|;李布,2,1,1,1|3|1,1,-1,1,#2ecc71|;抱怨,3,3,1,1,1,1,1,1,#99bbbb|";
//	//技能信息----技能列表
//	localStorage.skill_1 = "攻击,2,0,1,2|1|1,1,-1,1,#99bbbb|,1|0";
//	localStorage.skill_2 = "强攻,2,0,1,2|1|1,1,-3,1,#99bbbb|,1|0";
//	localStorage.skill_3 = "远射,2,0,2,2|1|1,1,-1,1,#99bbbb|,1|0";
//	localStorage.skill_4 = "致命,2,0,2,2|1|1,1,-2,1,#99bbbb|,1|0";
//	localStorage.skill_5 = "不爽,2,0,2,3|1|2,1,-1,1,#9b59b6|,1|0";
//	localStorage.skill_6 = "愤怒,2,0,2,3|7|3,1,-1,1,#3498db|,1|0";
//	localStorage.skill_7 = "李布,2,1,1,1|3|1,1,-1,1,#2ecc71|,1|0";
//	localStorage.skill_8 = "抱怨,3,3,1,1,1,1,1,#99bbbb|,1|0";
	
	//技能影响信息
	localStorage.skill_influ = "0;0;1,1,0,0;0";
			
}