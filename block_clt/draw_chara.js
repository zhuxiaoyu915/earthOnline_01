//绘制人物----初始化所有静态拼版
function init_Chara_block(map_array,map_info){
	//读取主角数据并绘制
	var chara_info_str = localStorage.character;
	chara_info = chara_info_str.split(";");
	var loc_ = map_array[chara_info[1]];
	draw_character("character",0,loc_,map_info);

	//读取其他玩家数据
	var player_info = [];
	//读取NPC数据
	var npc_array_all = [];
	for(i = 1;i<7;i++){
		var str_basic = "npc_";
		var key = str_basic + i;
		var val = localStorage.getItem(key);
		var npc_information = [];
		if(val != ""){
			var npc_info = val.split(";");
			for( m in npc_info){
				npc_info_detail = npc_info[m].split("|");
				npc_information.push(npc_info_detail);
			}
		npc_array_all.push(npc_information);
		}	
	}
	
	var baseCanvas = document.getElementById("baseCanvas");
	for(i in npc_array_all){
		var color = npc_array_all[i][2][0];
		var loc_num = parseInt(npc_array_all[i][1][0]);
		var loc_bgc = map_array[loc_num];
		drawBgColor(loc_bgc,map_array,baseCanvas,color,color,0,1);
		//drawBggColor(loc_num,map_array,color,baseCanvas);
	}
}

//绘制人物----移动拼版
function drawCharacter_move(loc,array,color,canvas,shadowColor){
    var num = array.length-1;
	var len = array[num].line_length;
	var canvas= canvas;
	var cxt = canvas.getContext("2d");
	//添加阴影
    cxt.beginPath(); 
	cxt.shadowOffsetX = 5;
	cxt.shadowOffsetY = 5;
	cxt.shadowBlur = 4;
	cxt.shadowColor = 'rgba(50,50,50,0.5)';
	
	cxt.lineWidth = 5;
	cxt.fillStyle = color;
	cxt.strokeStyle = color;
	//添加背景色	
    var loc_x = loc.x;
    var loc_y = loc.y;
    var p1_x = loc_x - len;
	var p1_y = loc_y;
	var p2_x = loc_x - len/2;
	var p2_y = loc_y - len/2*1.732;
	var p3_x = loc_x + len/2;
	var p3_y = p2_y;
	var p4_x = loc_x + len;
	var p4_y = loc_y;
	var p5_x = p3_x;
	var p5_y = loc_y + len/2*1.732;
	var p6_x = p2_x;
	var p6_y = p5_y;	
	
	cxt.moveTo(p1_x,p1_y);
	cxt.lineTo(p2_x,p2_y);
	cxt.lineTo(p3_x,p3_y);
	cxt.lineTo(p4_x,p4_y);
	cxt.lineTo(p5_x,p5_y);
	cxt.lineTo(p6_x,p6_y);
	cxt.lineTo(p1_x,p1_y);
	cxt.fill();
	cxt.closePath();
}

//绘制人物----人物状态更改
function chara_change(chara_info,map_array,map_info){
	//读取人物的血条
	var blood = parseInt(chara_info[4][0]);
	//看当前角色有没有特殊状态：眩晕、恐惧、残废、周泰
	var special_state = parseInt(chara_info[6][0]);
	if(blood == 0&&special_state != 6){
		//角色清除，因为npc是绘制在baseCanvas层的，所以只需对地图进行重绘
		var loc = parseInt(chara_info[1][0]);
		var canvas = document.getElementById("baseCanvas");
		var loc_bgc = map_array[loc];
		drawBgColor(loc_bgc,map_array,canvas,"blue","white",0,1);
		//drawBggColor(loc,map_array,"white",canvas);
		//设置map_info中的拼版属性，将拼版属性还原为原先的值
		map_info[loc + 1][0] = 0;		
	}else{
		//draw_
	}
}
