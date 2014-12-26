//技能
//技能--技能列表展开
function skill_spread(chara,map_info,map_array,loc_num){
	//设置0.5s延迟
	var moveable = neighbor_block(loc_num,map_array,map_info,1,0);
	
	var chara_info = [];
	var chara_str = localStorage.character;
	var chara_content = chara_str.split(";");
	for(i in chara_content){
		var chara_info_detail = chara_content[i].split("|");
		chara_info.push(chara_info_detail);
	}
	
	var skill_key = chara_info[8][0];
	//console.log("技能信息为：" + localStorage.character_skill);
	var skill_array_str = localStorage.getItem(skill_key);
	var skill_array = skill_array_str.split(";");

	var skill_num = parseInt(skill_array[0]);
	var skill_loc =[];
	for(i = 1;i <= skill_num;i++){
		if(moveable[i-1] >= 0){
			var skill_info = [];
			var skill_info_str = skill_array[i];
			var skill_info_1 = skill_info_str.split(",");
			for(m in skill_info_1){
				skill_info_2 = skill_info_1[m].split("|");
				skill_info.push(skill_info_2);
			}
			//检测当前技能状态
			var skill_demand = skill_castable_test(chara_info,skill_info);
			//console.log("技能是否可执行："+ skill_demand);			
			if(skill_demand){
				var color = skill_info[8][0];
			}else{
				var color = "grey";
			}
			skill_loc_num = moveable[i-1];

			var canvas = document.getElementById("skillCanvas");
			var loc_bgc = map_array[skill_loc_num];
			drawBgColor(loc_bgc,map_array,canvas,color,color,0,0.5);
			
			var skill_name = skill_info[0][0];
			var cxt = canvas.getContext("2d");
			cxt.fillStyle = "white";
			cxt.textAlign = "center";
			cxt.fillText(skill_name,map_array[skill_loc_num].x , map_array[skill_loc_num].y + 7);
			if(skill_demand)skill_loc.push(skill_loc_num);//只有满足了释放需求的函数才会被推入待选名单
		}
	}
	return skill_loc;
}

//技能--技能施放
function skill_chosing(map_array,map_info,map_num,skill_num,loc){
	var canvas = document.getElementById("skillCanvas");
	var cxt = canvas.getContext("2d");
	skill_str = localStorage.character_skill;
	skill_group = skill_str.split(";");
	skill_info = skill_group[parseInt(skill_num) + 1].split(",");

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
	
//技能--技能完成选择
function skill_chosed(map_array,map_info,map_num,skill_num,loc){

	var canvas = document.getElementById("skillCanvas");
	var cxt = canvas.getContext("2d");
	var skill_casted = false;//技能没有释放
	//获取角色信息----角色属性
	var chara = localStorage.character;
	var chara_info_content = chara.split(";");
	var chara_info = [];
	for (i in chara_info_content){
		var chara_info_1 = chara_info_content[i].split("|");
		chara_info.push(chara_info_1);
	}
	chara_info.push("character");
	//获取角色信息----角色位置
	var loc_num = localStorage.temporary_character_loc;
	//获取技能信息
	skill_str = localStorage.character_skill;
	skill_group = skill_str.split(";");
	skill_info_all = [];
	for(i = 1; i < skill_group.length; i++){
		var skill_info_str = skill_group[i].split(",");
		var skill_info_1 = [];
		for(m in skill_info_str){
			var skill_info_detail = skill_info_str[m].split("|");
			skill_info_1.push(skill_info_detail);
		}
		skill_info_all.push(skill_info_1);
	}
	skill_info = skill_info_all[skill_num];

	if(skill_info[1][0] == "1"){
		skill_cast_1();
	}
	if(skill_info[1][0] == "2"){
		skill_cast_2();
	}
	if(skill_info[1][0] == "3"){
		skill_cast_3();
	}
	
	function skill_cast_1(){
		//当skill_info[10]==4时，该技能指向特定功能
		if(parseInt(skill_info[10][0]) == 4){
			var a = parseInt(skill_info[10][1]);
			if(a == 1){
				round_finish(map_array , map_info);
			}
			else if (a == 2){
				console.log("lesautre function");
			}
		}else if(parseInt(skill_info[10][0]) == 3){//当skill_info[10] == 3时，该技能为技能改变型
			var skill_change = skill_info[10][1];
			//console.log(skill_change);
			//技能信息：2[不爽(愤怒{不爽[0[0[0[0(0(0[0[-1[0[0([0(0{愤怒[0[0[0[0(0(0[0[-1[0[0([0(0
			var change_skill_info = skill_change.split("{");
			var change_skill_content = change_skill_info[0].split("[");
			var change_num = parseInt(change_skill_content[0]);
			var skill_name = "";
			var change_array = change_skill_content[1].split("(");
			//将可更改技能列表与现在的技能列表匹配
			//!-------为什么不直接写现有技能编号而是写技能名称？----------------
			//是为了保证技能的复用性，这样的话，可以使得角色随意变换现有技能，而不需要每次变换技能都要
			//重新更改非指向性技能的适用范围

			for(i in change_array){
				//读取角色技能列表
				//console.log(change_array);
				for(m in skill_info_all){
					if(skill_info_all[m][0][0] == change_array[i]){//如果面向的技能对象与技能编号匹配，则执行如下
						//读取加成因子
						var skill_plus_str = change_skill_info[parseInt(i)+ 1];
						var skill_plus = [];
						var skill_plus_detail_str = skill_plus_str.split("[");
						for (n in skill_plus_detail_str){
							var skill_p_1 = skill_plus_detail_str[n].split("(");
							skill_plus.push(skill_p_1);
						}
						console.log(skill_plus[6][0]);
						//读取技能
						var skill_compare = skill_info_all[m];
						//加成因子与技能匹配
						for(n =1;n<skill_plus.length;n++){
							for(x in skill_plus[m]){
								if(skill_plus[n][x]!= "0"){
									skill_compare[n][x] = parseInt(skill_compare[n][x]) + parseInt(skill_plus[n][x]);
									skill_info_all[m][n][x] = skill_compare[n][x];
									console.log(skill_compare[n][x]);
								}
							}
						}
					}
				}				
			}
			//设置效果持续时间
			
			//匹配完成后重新生成技能信息
			var compare_result = [];
			var c_r_2 = [];
			c_r_2.push(skill_info_all.length);
			for(i in skill_info_all){
				var c_r_1 = [];
				for(m in skill_info_all[i]){
					var s_c_1 = skill_info_all[i][m].join("|");
					c_r_1.push(s_c_1);
				}
				var s_c_2 = c_r_1.join(",");
				c_r_2.push(s_c_2);
			}
			var skill_info_after_compare = c_r_2.join(";");
			localStorage.character_skill = skill_info_after_compare;
			//console.log(localStorage.character_skill);
		}else if(parseInt(skill_info[10][0]) == 2){
			
		}
		
		//添加技能冷却时间
		skill_info_all = read_skill_array("character_skill",3);
		skill_info_all[parseInt(skill_num) + 1][9][1] = skill_info_all[parseInt(skill_num) + 1][9][0];
		skill_array = package_skill_array(skill_info_all);
		localStorage.character_skill = skill_array;	
	}
	
	function skill_cast_2(){//指向性技能释放
		if(parseInt(skill_info[2]) != 0){
		}
		choseable_block = neighbor_block(loc_num,map_array,map_info,skill_info[3][0],0);
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
					var skill_efficacy = skill_info[4];
					var destination = skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info);
					console.log("目标区域为：" + destination);
					damage_count(map_array,map_info,destination,skill_info,chara_info);
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
	
	//添加技能冷却时间
	if(skill_casted){
		skill_info[9][1] = skill_info[9][0];
		var skill_join_1 = [];
		for(i in skill_info){
			skill_join_1.push(skill_info[i].join("|"));
		}
		var skill_join = skill_join_1.join(",");
		skill_group[parseInt(skill_num) + 1] = skill_join;
		var skill_last = skill_group.join(";");
		localStorage.character_skill = skill_last;
	}
	//人物重绘
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
			console.log("扇形投射");
			for(range = skill_info[3][0];range > 0;range--){
				console.log("环形分析 ");
				var moveable = count_circle(orient_loc,map_array,map_info,range,0);
				for(i in moveable){
					if(moveable[i].loc == desti_loc){
						var move_num = i;//获取当前匹配的拼版在扇形中的序号
						destination.push(moveable[move_num].loc);
						console.log(destination);
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
						console.log(destination);
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
function damage_count(map_array,map_info,destination,skill_info,ori_chara_info){
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
					//console.log("damage:  " + damage);
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
	
	//重新绘制施法角色信息
	var miner_cost = skill_info[7][0];
	//console.log("技能的魔法消耗：" + miner_cost);
	//console.log("现有魔法量为：" + ori_chara_info[4][1]);
	ori_chara_info[4][1] = parseInt(ori_chara_info[4][1]) + parseInt(miner_cost);
	//console.log("计算后的魔法量为：" + ori_chara_info[4][1]);
	var string_array =[];
	for(i =0;i<ori_chara_info.length-1;i++){
		var string_array_1 = ori_chara_info[i].join("|");
		string_array.push(string_array_1);
	}
	var string_new = string_array.join(";");
	var key = ori_chara_info[ori_chara_info.length -1];
	localStorage.setItem(key,string_new);
	//console.log(localStorage.character);
}

//技能--可释放性检测
function skill_castable_test(chara_info,skill_info){
	var skill_demand = 1;//默认skill_demand为1，即满足施放要求
	//1)检测人物状态
	var chara_state = parseInt(chara_info[6][0]);

	var skill_type = parseInt(skill_info[1][0]);

	switch(skill_type){
	case 1://非指向性技能
		if(chara_state == 1||chara_state ==2||chara_state ==3
				||chara_state ==8||chara_state == 11){
			skill_demand = 0;
		}
		break;
	case 2://指向性技能
		if(chara_state == 1||chara_state ==2||chara_state ==3
				||chara_state ==8||chara_state == 11){
			skill_demand = 0;
			console.log("技能与人物状态匹配有问题。");
		}				
		break;
	case 3://被动技能
		if(chara_state == 1||chara_state ==2||chara_state ==3
				||chara_state ==8||chara_state == 11){
			skill_demand = 0;
		}
		break;
	}
	//2)检测技能状态
	var cold_time = parseInt(skill_info[9][1]);//如果技能没有冷却完成那么不满足释放要求
	if(cold_time > 0)skill_demand = 0;
	//console.log("技能冷却时间为：" + cold_time);
	
	var miner_demand = parseInt(skill_info[7][0]);//如果魔法消耗量大于现存魔法量那么不满足释放要求
	var miner_exist = parseInt(chara_info[4][1]);
	if((miner_exist + miner_demand) < 0)skill_demand = 0;
	return skill_demand;
}
