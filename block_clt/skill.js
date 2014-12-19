//技能
//技能--技能列表展开
function skill_spread(chara,map_info,map_array,loc_num){
	//设置0.5s延迟
	var moveable = neighbor_block(loc_num,map_array,map_info,1,0);
	var chara_info = [];
	for(i in chara){
		var chara_info_detail = chara[i].split("|");
		chara_info.push(chara_info_detail);
	}
	var skill_content = chara_info[8];
	var skill_num = parseInt(skill_content[0]);
	var skill_loc =[];
	for(i = 1;i <= skill_num;i++){
		if(moveable[i-1] >= 0){
			var skill_info = [];
			var key = skill_content[i];
			var skill_info_str = localStorage.getItem(key);
			var skill_info_1 = skill_info_str.split(",");
			for(m in skill_info_1){
				skill_info_2 = skill_info_1[m].split("|");
				skill_info.push(skill_info_2);
			}
			//检测当前技能状态
			//1)检测人物状态
			var chara_state = chara_info[6][0];
			//2)读取技能信息
			var skill_demand = 1;//默认skill_demand为1，即满足施放要求
			var skill_type = parseInt(skill_info[1][0]);
			switch(skill_type){
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			}
			
			
			
			
			
			var color = skill_info[8][0];	
			skill_loc_num = moveable[i-1];
			console.log("可移动范围为：" + moveable);
			console.log("技能位置为：" + skill_loc_num);
			var canvas = document.getElementById("skillCanvas");
			var loc_bgc = map_array[skill_loc_num];
			drawBgColor(loc_bgc,map_array,canvas,color,color,0,0.5);
			
			var skill_name = skill_info[0][0];
			var cxt = canvas.getContext("2d");
			cxt.fillStyle = "white";
			cxt.textAlign = "center";
			cxt.fillText(skill_name,map_array[skill_loc_num].x , map_array[skill_loc_num].y + 7);
			skill_loc.push(skill_loc_num);
		}
	}
	console.log(skill_loc);
	return skill_loc;
}

//技能--技能施放
function skill_chosing(map_array,map_info,map_num,skill_num,loc){
	var canvas = document.getElementById("skillCanvas");
	var cxt = canvas.getContext("2d");
	skill_str = localStorage.skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[skill_num].split(",");
	//console.log("技能总信息为:" + skill_group);
	//skill_info分析
	//----------skill_info[0]技能名称
	//----------skill_info[1]技能分类--非指向性--指向性--被动
	//----------skill_info[2]是否受其他技能影响

	if(skill_info[1] == "1"){//非指向性技能
		skill_cast_1();
	}
	if(skill_info[1] == "2"){//指向性技能
		skill_cast_2();
	}
	if(skill_info[1] == "3"){//被动技能
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
		for(i in choseable_block){
			var num = choseable_block[i];
			if(num>=0 && loc.x>map_array[num].x-25 &&loc.x<map_array[num].x + 25 && loc.y>map_array[num].y -50*0.866 && loc.y < map_array[num].y + 50*0.866){
				var color = "#3498db";
				var loc_bgc = map_array[num];
				drawBgColor(loc_bgc,map_array,canvas,color,color,0,0.8);
			}else{
				var color = "green";
				var loc_bgc = map_array[choseable_block[i]];
				drawBgColor(loc_bgc,map_array,canvas,color,color,0,0.4);
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
function skill_chosed(map_array,map_info,map_num,skill_num,loc){
	var canvas = document.getElementById("skillCanvas");
	var cxt = canvas.getContext("2d");
	skill_str = localStorage.skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[skill_num].split(",");
	var skill_casted = false;
	var loc_num = localStorage.temporary_character_loc;
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
	function skill_cast_2(){//指向性技能释放
		if(parseInt(skill_info[2]) != 0){
		}
		choseable_block = neighbor_block(loc_num,map_array,map_info,skill_info[3],0);
		choseable_block.push(loc_num);
		cxt.clearRect(0,0,1400,700);
		for(i in choseable_block){
			var num = choseable_block[i];
			if(num>=0 && loc.x>map_array[num].x-25 &&loc.x<map_array[num].x + 25 && loc.y>map_array[num].y -50*0.866 && loc.y < map_array[num].y + 50*0.866){
				if(num == loc_num){//如果指向目标本身，则取消本次操作。
					cxt.clearRect(0,0,1400,700);
				}else{
					var orient_loc = loc_num;
					var desti_loc = num;
					var skill_efficacy = skill_info[4].split("|");
					var destination = skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info);

					var chara_skill_info = [];
					for(i in skill_info){
						var skill_info_detail = skill_info[i].split("|");
						chara_skill_info.push(skill_info_detail);
					}
					damage_count(map_array,map_info,destination,chara_skill_info);
					localStorage.loc_changable = 0;
				}
				skill_casted = true;
			}
		}
		if(!skill_casted){
			cxt.clearRect(0,0,1400,700);
		}
	}
	function skill_cast_3(){
	}
	
	var loc_bgc = map_array[loc_num];
	draw_character("character",0,loc_bgc,map_info);
	
	return false;//返回一个鼠标是否可以移动的值
}

//技能--技能特效计算--获取（多个）目标点坐标
function skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info){

	var num = parseInt(skill_efficacy[1]);
	var destination = [];

	destination = get_range_dest();
	for(i in destination){//对目标进行绘制
		draw_skill_efficacy(orient_loc,destination[i],skill_efficacy[0],map_array,map_info);
	}

	function get_range_dest(){//对范围内的拼版，分层检测匹配，然后对匹配层内拼版进行排序
		var cast_style = parseInt(skill_efficacy[2]);
		if(cast_style == 1){//扇形投射
			for(range = skill_info[3];range > 0;range--){
				var moveable = count_circle(orient_loc,map_array,map_info,range,0);
				for(i in moveable){
					if(moveable[i].loc == desti_loc){
						var move_num = i;//获取当前匹配的拼版在扇形中的序号
						destination.push(moveable[move_num].loc);
						var count_num = skill_efficacy[1];
						var side_num = parseInt((skill_efficacy[1] -1 ) / 2);
						//顺时针添加
						for(m = side_num;m>0;m--){
							move_num ++;
							if(move_num < 0){
								move_num = move_num + moveable.length;
							}
							if(move_num >= moveable.length){
								move_num = move_num - moveable.length;
							}
							destination.push(moveable[move_num].loc);
						}
						//逆时针添加
						move_num = i;
						for(m = side_num;m>0;m--){
							move_num --;
							if(move_num < 0){
								move_num = move_num + moveable.length;
							}
							if(move_num >= moveable.length){
								move_num = move_num - moveable.length;
							}
							destination.push(moveable[move_num].loc);
						}
						return destination;
					}
				}
			}
		}else if (cast_style == 2){//贯通投射
			//检测角度
			//var ready_loc = 0;
			var desti_info_x = map_array[desti_loc].x;
			var desti_info_y = map_array[desti_loc].y;
			var orient_info_x = map_array[orient_loc].x;
			var orient_info_y = map_array[orient_loc].y;
			var angle = parseInt(((desti_info_x - orient_info_x)*(desti_info_x - orient_info_x))/((desti_info_y - orient_info_y)*(desti_info_y - orient_info_y)));
			if(angle == 0||angle == 3){
				var x_index = desti_info_x - orient_info_x;
				var y_index = desti_info_y - orient_info_y;
				var step_num = 10;
				var x_step = x_index /step_num;
				var y_step = y_index /step_num;
				
				var loc_x = orient_info_x;
				var loc_y = orient_info_y;
				for(m = 0;m<10;m++){
					loc_x = loc_x + x_step;
					loc_y = loc_y + y_step;
					//检测loc_x,loc_y的位置，看是否与地图匹配
					var map_length = map_info[0].length;
					for(i in map_array){
						if(loc_x > (parseInt(map_array[i].x)-map_length/2) 
								&& loc_x < (parseInt(map_array[i].x) + map_length/2) 
								&& loc_y > (parseInt(map_array[i].y) - map_length* 0.866) 
								&& loc_y < (parseInt(map_array[i].y) + map_length*0.866)){
							var ready_loc = i;
							if(ready_loc == orient_loc)ready_loc = 0;
							
						}
					}
					if(ready_loc){
						var repeated = 0; 
						
						for(p in destination){
							if(ready_loc == destination[p]){
								repeated = 1;
							}
						}
						if(!repeated)destination.push(ready_loc);
					}
				}
			}else{
				destination.push(desti_loc);
			}
			return destination;
		}else if (cast_style == 3){//区域投射
			destination.push(desti_loc);//在区域投射中，至少会保证命中中央目标，所以会占据一个destination
			//计算num最多有几层
			var layer_num = 1;
			for(n = 1;3*n*(n+1)< num -1;n++){
				layer_num = n + 1;
			}
			console.log("层数为：" + layer_num);
			//计算moveable拼版
			var moveable_through = neighbor_block(desti_loc,map_array,map_info,layer_num,0);
			//moveable_through.push(desti_loc);
			var length = moveable_through.length;
			for(var i= 1;i<= num -1 ; i++){
				//随机从moveable中选取一个拼版
				var a = parseInt(Math.random() * length);
				//重复检测
				var repeat = 0;
				for(m in destination){
					if(destination[m] == moveable_through[a]){
						i--;
						repeat = 1;
					}
				}
				if(!repeat){//如果该数字不重复，则添加到目标之中
					destination.push(moveable_through[a]);
				}
			}
			console.log("区域范围为：" + destination);
		}
	}
	return destination;
}

//技能--伤害结算
function damage_count(map_array,map_info,destination,skill_info){
	for(i in destination){
		var block_info = map_info[parseInt(destination[i]) + 1];
		var influ_level = skill_info[5][0];
		if( influ_level >= block_info[0]){
			//block_info[0]为块中对象的属性
			var block_object = parseInt(block_info[0]);
			if(block_object == 0){//如果是空白块
				console.log("击中一个空白块");
				if(influ_level > 2){//如果技能等级为影响地形等级，就采取效果
				}
			}else if(block_object ==1){//如果是人物站在空白块中
				console.log("击中了一个站在空白块上的人");
				kickANakedMan(destination[i]);
				function kickANakedMan(desti){
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
					//角色信息重写
					var string_array = [];
					for(i in chara_info){
						var string_array_1 = chara_info[i].join("|");
						string_array.push(string_array_1);
					}
					string_new = string_array.join(";");
					localStorage.setItem(chara_name,string_new);
					//效果展示
					//效果展示----分为角色和地形两个方面
					if(chara_info[4][0] > 0||chara_info[6][0] == 6){//人物还活着的情况下，通常只需要绘制角色效果（击退效果不算）
						
						if(chara_name == "character"){
							var loc_bgg = map_array[desti];
							setTimeout(function a(){draw_character(chara_name,0,loc_bgg,map_info);},800);
						}else{
							var canvas = document.getElementById("baseCanvas");
							var cxt = canvas.getContext("2d");
							var loc_bgg = map_array[desti];
							console.log("destination为：" + desti);
							draw_character(chara_name,0,loc_bgg,map_info,canvas);
						}
					}else if(chara_info[4][0]<= 0){//人物确定死亡情况下，绘制地图内容
						//更改地图信息
						chara_loc = parseInt(chara_info[1][0]);
						console.log("人物死亡了");
						var old_info = parseInt(map_info[chara_loc + 1][0]);
						old_info--;
						map_info[chara_loc + 1][0] = old_info;
						map_info[chara_loc + 1][2] ="";
						//重绘地图内容
						if(chara_name == "character"){
							var loc_bgc = map_array[chara_loc];
							var redrawCanvas = document.getElementById("myCanvas");
							drawBgColor(loc_bgc,map_array,redrawCanvas,"blue","white",0,1);
							setTimeout(function c(){alert("gameover");},1000);
						}else{
							var loc_bgc = map_array[chara_loc];
							var redrawCanvas = document.getElementById("baseCanvas");
							drawBgColor(loc_bgc,map_array,redrawCanvas,"blue","white",0,1);
						}
					}
				}
				
			}else if(block_object ==2){//如果目标为建筑
				if(influ_level > 1){//如果目标技能等级为影响建筑等级
				}
			}else if(block_object ==3){//如果目标为人物站在建筑上
			
			}else if(block_object ==4){//如果目标位特殊地形快
				if(influ_level = 4){//如果目标技能等级为影响游戏等级
				}
			}	
		}		
	}
}
