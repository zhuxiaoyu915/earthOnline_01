//技能
//技能--技能列表展开
function skill_spread(chara,map_info,map_array,loc_num){
	//设置0.5s延迟
	var n = 1;
	var moveable = neighbor_block(loc_num,map_array,map_info,n,0);
	var skill_num = 0;
	var skill_loc =[];
	for(i = 0;i<4;i++){
		if(moveable[i] >= 0){	
		skill_loc_num = moveable[i];
		draw_skill(skill_num,skill_loc_num);
		skill_loc.push(skill_loc_num);
		skill_num++;
		}
	}
	return skill_loc;
}

//技能--技能图标绘制
function draw_skill(skill_num,skill_loc_num){
	//读取技能列表
	skill_str = localStorage.skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[skill_num].split(",");
	//根据技能的属性来设置技能绘制属性
	switch(skill_num){
		case 0:
			color = "#9b59b6";
			break;
		case 1:
			color = "#3498db";
			break;
		case 2:
			color = "#2ecc71";
			break;
		case 3:
			color = "#99bbbb";
			break;
	}
	var loc_bgc = map_array[skill_loc_num];
	var canvas = document.getElementById("skillCanvas");
	drawBgColor(loc_bgc,map_array,canvas,color,color,0,0.5);
	//drawBggColor(skill_loc_num,map_array,color,canvas,0,0.5);
	var skill_name = skill_info[0];
	var cxt = canvas.getContext("2d");
	cxt.fillStyle = "white";
	cxt.textAlign = "center";
	cxt.fillText(skill_name,map_array[skill_loc_num].x , map_array[skill_loc_num].y + 7);	 
}

//技能--技能施放
function skill_chosing(map_array,map_info,map_num,skill_num,loc){
	var canvas = document.getElementById("skillCanvas");
	var cxt = canvas.getContext("2d");
	skill_str = localStorage.skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[skill_num].split(",");
	//skill_info分析
	//----------skill_info[0]技能名称
	//----------skill_info[1]技能分类--非指向性--指向性--被动
	//----------skill_info[2]是否受其他技能影响

	if(skill_info[1] == "1"){
		skill_cast_1();
	}
	if(skill_info[1] == "2"){
		skill_cast_2();
	}
	if(skill_info[1] == "3"){
		skill_cast_3();
	}
	
	function skill_cast_1(){
	}
	function skill_cast_2(){
		//判断技能是否会被影响
		if(parseInt(skill_info[2]) != 0){
		//读取影响效果
		//return 0;
		}
		var n = parseInt(skill_info[3]);//n 为技能投放半径
		var loc_num = localStorage.temporary_character_loc;
		choseable_block = neighbor_block(loc_num,map_array,map_info,n,0);
		choseable_block.push(loc_num);
		cxt.clearRect(0,0,1400,700);
		var color = "#cdd3d7";
		var loc_bgc = map_array[loc_num];
		drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
		//drawBggColor(loc_num,map_array,color,canvas);
		for(i in choseable_block){
			var num = choseable_block[i];
			if(num>=0 && loc.x>map_array[num].x-25 &&loc.x<map_array[num].x + 25 && loc.y>map_array[num].y -50*0.866 && loc.y < map_array[num].y + 50*0.866){
				var color = "#3498db";
				var loc_bgc = map_array[num];
				drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
				//drawBggColor(num,map_array,color,canvas);
			}else{
				var color = "green";
				var loc_bgc = map_array[choseable_block[i]];
				drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
				//drawBggColor(choseable_block[i],map_array,color,canvas);
			}
		}
		cxt.strokeStyle = "red";
		cxt.beginPath();
		cxt.arc(loc.x,loc.y,25,0,Math.PI * 2,true);
		cxt.closePath();
		cxt.stroke();
		cxt.lineWidth = 3;
		cxt.beginPath();
		cxt.arc(loc.x,loc.y,15,0,Math.PI *2,true);
		cxt.moveTo(loc.x-25,loc.y);
		cxt.lineTo(loc.x+25,loc.y);
		cxt.moveTo(loc.x,loc.y+25);
		cxt.lineTo(loc.x,loc.y-25);
		cxt.closePath();
		cxt.stroke();
	}
	function skill_cast_3(){
	}
}
	
//技能--技能施放结束
function skill_chosed(canvas,map_array,map_info,map_num,skill_num,loc){
	var cxt = canvas.getContext("2d");
	skill_str = localStorage.skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[skill_num].split(",");
	var skill_casted = false;
	if(skill_info[1] == "1"){
		skill_cast_1();
	}
	if(skill_info[1] == "2"){
		skill_cast_2();
	}
	if(skill_info[1] == "3"){
		skill_cast_3();
	}
	
	function skill_cast_1(){
	}
	function skill_cast_2(){
		if(parseInt(skill_info[2]) != 0){
		}
		var loc_num = localStorage.temporary_character_loc;
		choseable_block = neighbor_block(loc_num,map_array,map_info,skill_info[3],0);
		choseable_block.push(loc_num);
		cxt.clearRect(0,0,1400,700);
		for(i in choseable_block){
			var num = choseable_block[i];
			if(num>=0 && loc.x>map_array[num].x-25 &&loc.x<map_array[num].x + 25 && loc.y>map_array[num].y -50*0.866 && loc.y < map_array[num].y + 50*0.866){
				if(num == loc_num){//如果指向目标本身，则取消本次操作。
					cxt.clearRect(0,0,1400,700);
					skill_move = false;
					skill_spread(canvas,chara,map_info,map_array,loc_num);
					color_char = "#cdd3d7";
					var loc_bgc = map_array[loc_num];
					drawBgColor(loc_bgc,map_array,canvas,color_char,color_char,0,1);
					//drawBggColor(loc_num,map_array,color_char,canvas);
				}else{
					var orient_loc = loc_num;
					var desti_loc = num;
					var skill_efficacy = skill_info[4].split("|");
					var destination = skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info);
					damage_count(map_array,map_info,destination,skill_info);
					localStorage.loc_changable = 0;
				}
				skill_casted = true;
			}
		}
		if(!skill_casted){
			cxt.clearRect(0,0,1400,700);
			skill_move_begin = false;
			console.log("在技能释放错误位置之后的smb为" +skill_move_begin);
			skill_spread(canvas,chara,map_info,map_array,loc_num);
			color_char = "#cdd3d7";
			var loc_bgc = map_array[loc_num];
			drawBgColor(loc_bgc,map_array,canvas,color_char,color_char,0,1);
			//drawBggColor(loc_num,map_array,color_char,canvas);
		}
	}
	function skill_cast_3(){
	}
	//返回一个鼠标是否可以移动的值
	return false;
}

//技能--技能特效计算--获取（多个）目标点坐标
function skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info){
	var num = parseInt(skill_efficacy[1]);
	var destination = [];
	if(num != 1){//判断为单目标还是多目标
		if(parseInt(skill_efficacy[2]) == 2){//路径伤害的执行函数	
		}else{//扇形伤害的执行函数
			var range = skill_info[3];//技能的施放范围
			destination = get_range_dest();
			for(i in destination){//对目标进行绘制
				draw_skill_efficacy(orient_loc,destination[i],skill_efficacy[0],map_array,map_info);
			}
		}
	}
	function get_range_dest(){//对范围内的拼版，分层检测匹配，然后对匹配层内拼版进行排序
		for(range = skill_info[3];range > 0;range--){
			var moveable = count_circle(orient_loc,map_array,map_info,range,0);
			for(i in moveable){
				if(moveable[i].loc == desti_loc){
					var move_num = i;//获取当前匹配的拼版在扇形中的序号
					for(i = skill_efficacy[1];i>0;i--){
						if(move_num < 0){
							move_num = move_num + moveable.length;
						}
						if(move_num >= moveable.length){
							move_num = move_num - moveable.length;
						}
						destination.push(moveable[move_num].loc);
						move_num ++;						
					}
					return destination;
				}
			}
		}
	}
	return destination;
}

//技能--伤害结算
function damage_count(map_array,map_info,destination,skill_info){
	for(i in destination){
		var block_info = map_info[destination[i] + 1];
		var influ_level = skill_info[5];
		
		if( influ_level >= block_info[0]){
			//block_info[0]为块中对象的属性
			var block_object = parseInt(block_info[0]);
			if(block_object == 0){//如果是空白块
				console.log("击中一个空白块");
				if(influ_level > 2){//如果技能等级为影响地形等级，就采取效果
				}
			}else if(block_object ==1){//如果是人物站在空白块中
				console.log("击中了一个站在空白块上的人");
				kickANakedMan();
				function kickANakedMan(){
					var damage = parseInt(skill_info[6]);//读取技能伤害
					//读取block_info中的关联人物信息
					var chara_name = block_info[2];
					var chara_info = [];
					var character = localStorage.getItem(chara_name);
					var chara_str = character.split(";");
					for(i in chara_str){
						var chara_detail = chara_str[i].split("|");
						chara_info.push(chara_detail);
					}
					//计算伤害效果
					chara_info[4][0] = parseInt(chara_info[4][0]) + damage;
					//计算完之后需要实时展示
					//人物加血，减少血量，死亡效果执行，要立刻做人物重绘
					chara_change(chara_info,map_array,map_info);
					var string_array = [];
					for(i in chara_info){
						var string_array_1 = chara_info[i].join("|");
						string_array.push(string_array_1);
					}
					string_new = string_array.join(";");
					localStorage.setItem(chara_name,string_new);
					
					//console.log(localStorage.getItem());
				}	
			}else if(block_object ==2){//如果目标为建筑
				if(influ_level > 1){//如果目标技能等级为影响建筑等级
				}
			}else if(block_object ==4){//如果目标位特殊地形快
				if(influ_level = 4){//如果目标技能等级为影响游戏等级
				}
			}	
		}		
	}
}
