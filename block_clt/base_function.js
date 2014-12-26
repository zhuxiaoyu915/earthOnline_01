//基础函数----绘制模块的背景色
function drawBgColor(loc,map_array,canvas,strokeColor,fillColor,shadow,alpha){
	//console.log("执行了吗？");
	var x = loc.x;
	var y = loc.y;
	var num = map_array.length -1;
	var length = map_array[num].line_length;
	var context = canvas.getContext("2d");
	
	var p1_x = x - length/2;
	var p1_y = y - length * 0.866;
	var p2_x = x + length/2;
	var p2_y = p1_y;
	var p3_x = x + length*1;
	var p3_y = y;
	var p4_x = p2_x;
	var p4_y = y + length * 0.866;
	var p5_x = p1_x;
	var p5_y = p4_y;
	var p6_x = x - length*1;
	var p6_y = y;
	
	context.lineWidth = 2;
	context.fillStyle = fillColor;
	context.strokeStyle = strokeColor;
	context.globalAlpha = alpha;
	
	if(shadow){
		context.shadowOffsetX = 10;
		context.shadowOffsetY = 10;
		context.shadowBlur = 4;
		context.shadowColor = 'rgba(0,0,0,0.5)';
	}else{
		context.shadowOffsetX = 0;
		context.shadowOffsetY = 0;
		context.shadowBlur = 0;
		context.shadowColor = 'rgba(0,0,0,0.5)';
	}
		context.beginPath();
		context.moveTo(p1_x,p1_y);
		context.lineTo(p2_x,p2_y);
		context.lineTo(p3_x,p3_y);
		context.lineTo(p4_x,p4_y);
		context.lineTo(p5_x,p5_y);
		context.lineTo(p6_x,p6_y);
		context.lineTo(p1_x,p1_y);
		context.fill();
	context.shadowOffsetX = 0;
	context.shadowOffsetY = 0;
	context.shadowBlur = 0;
	context.shadowColor = 'rgba(0,0,0,0.5)';
	
	context.stroke();
	context.closePath();
}

//基础函数----生成邻接拼版数组
function neighbor_block(loc_num,map_array,map_info,n,limit){
	
	var map_array = map_array;
	var moveable =[];
	var num = 3*n*(n+1);
	
	for(i = 0;i<map_array.length-1&&moveable.length<num;i++){
		var x = map_array[i].x;
		var y = map_array[i].y;
		var dest_x = map_array[loc_num].x;
		var dest_y = map_array[loc_num].y;
		if(Math.pow((x - dest_x),2) + Math.pow((y - dest_y),2) < (Math.pow(n,2)* Math.pow(map_array[map_array.length - 1].line_length,2) * 3.1)){
			if(limit ==0){
				if(i != loc_num){
					moveable.push(i);
				}
			}
			if(limit ==1){
				if(i != loc_num&& map_info[i+1][0] == 0){
					moveable.push(i);
				}
			}
			if(limit ==2){
			}
		}
	}
	return moveable;
}

//基础函数----根据原点生成扇形
function count_circle(loc_num,map_array,map_info,n,limit){
	
	var map_array = map_array;
	var moveable =[];
	var num = 6 * n;
	
	for(i = 0;i<map_array.length-1&&moveable.length<num;i++){
		var x = map_array[i].x;
		var y = map_array[i].y;
		var dest_x = map_array[loc_num].x;
		var dest_y = map_array[loc_num].y;
		if(Math.pow((x - dest_x),2) + Math.pow((y - dest_y),2) < (Math.pow(n,2)* Math.pow(map_array[map_array.length - 1].line_length,2) * 3.1)){
			if(Math.pow((x - dest_x),2) + Math.pow((y - dest_y),2) > (Math.pow(n-1,2)* Math.pow(map_array[map_array.length - 1].line_length,2) * 3.1)){
				//计算夹角
				var a = x - dest_x; 
				var b = y - dest_y;
				var c = Math.pow(a*a + b*b , 1/2);
				var sin = b/c;
				var cos = a/c;
				var tan = b/a;
				var q = Math.asin(sin);
				if(a >= 0 && b > 0){
					var angle = q * (180/Math.PI);
				}else if (a < 0 && b>0){
					var angle = 180 - q*(180/Math.PI);
				}else if (a <= 0 && b<0){
					var angle = 180 - q*(180/Math.PI);
				}else if (a > 0 && b<0){
					var angle = 360  + q*(180/Math.PI);
				}				
				moveable.push({loc:i,angle:angle});
			}
		}	
	}
	
	function sortNumber(a,b){
		return a.angle - b.angle;
	}	
	moveable.sort(sortNumber);
	return moveable;
}

//基础函数----特效绘制
function draw_skill_efficacy(orient_loc,desti_loc,skill_effi_num,map_array,map_info){
	var efficanvas = document.getElementById("effiCanvas");
	var context = efficanvas.getContext("2d");
	var grd = context.createRadialGradient(150,75,0,150,75,100);
	var radius = map_info[0].length;
	if(skill_effi_num == 1){
		//特效为火焰
		grd.addColorStop(0,"#0000FF");
		grd.addColorStop(1,"#00FF00");
		context.fillStyle = grd;
		var orient_rad = radius * 0.1;
		var desti_rad = radius * 0.7;
	}else if(skill_effi_num ==2){
		//特效为冰霜
		grd.addColorStop(0,"#AAAAAA");
		grd.addColorStop(1,"#00FF00");
		context.fillStyle = grd;
		var orient_rad = radius * 0.1;
		var desti_rad = radius * 0.7;
	}else if(skill_effi_num ==3){
		//特效为暗影
		grd.addColorStop(0,"#FF0000");
		grd.addColorStop(1,"#0000FF");
		context.fillStyle = grd;
		var orient_rad = radius * 0.25;
		var desti_rad = radius * 0.7;
		var frame_rad = (desti_rad - orient_rad)/15;
	}
	//计算距离
	var orient_info = map_array[orient_loc];
	var desti_info = map_array[desti_loc];
	var distance = Math.pow(((desti_info.y -orient_info.y)*(desti_info.y - orient_info.y) + (desti_info.x -orient_info.x)*(desti_info.x - orient_info.x)),1/2);

	var frame_num = 15;
	var frame_time = 30;
	var frame_distance = distance/frame_num;
	var frame_distance_x = (desti_info.x - orient_info.x)/ frame_num;
	var frame_distance_y = (desti_info.y - orient_info.y)/ frame_num;
	
	var draw_rad = orient_rad;
	var draw_loc = orient_loc;
	var count = 0;
	var draw_loc_array_x = map_array[draw_loc].x;
	var draw_loc_array_y = map_array[draw_loc].y;
	
	//判断是否为npc操作
	var state_now = localStorage.turnOn;
	if(state_now == "4"){
		setTimeout(function a(){var intervalID = window.setInterval(draw_begin , frame_time);},500);
	}else{
		var intervalID = window.setInterval(draw_begin , frame_time);
	}
	
	
	function draw_begin(){
		if(count < frame_num){
			context.beginPath();
			context.strokeStyle = "blue";
			context.arc(draw_loc_array_x , draw_loc_array_y , draw_rad , 0, Math.PI*2 , true);
			context.closePath();
			context.fill();
			context.stroke();
			draw_loc_array_x = draw_loc_array_x + frame_distance_x;
			draw_loc_array_y = draw_loc_array_y + frame_distance_y;
			draw_rad = draw_rad +2;
			count++;
		}else if(count == frame_num){
			context.clearRect(0,0,1400,700);
			count++;
		}else{
			window.clearInterval(intervalID);
		}
	}
	
}

//基础函数----流程结束
function round_finish(map_array,map_info){
	//对角色修改进行解析并修改
	var chara_info = [];
	var character = localStorage.character;
	var chara_str = character.split(";");
	for(i in chara_str){
		var chara_detail = chara_str[i].split("|");
		chara_info.push(chara_detail);
	}
	//清除原位置在map_info中产生的残留信息
	var loc_num = parseInt(chara_info[1][0]);
	map_info[loc_num + 1][0] = 0;
	map_info[loc_num + 1][2] = "";
	
	chara_info[1][0] = localStorage.temporary_character_loc;
	//修改完成后保存到localStorage中			
	var string_array = [];
	for(i in chara_info){
		var string_array_1 = chara_info[i].join("|");
		string_array.push(string_array_1);
	}
	string_new = string_array.join(";");
	localStorage.character = string_new;
	//重新写入character在现在位置的信息
	var loc_num = parseInt(chara_info[1][0]);
	map_info[loc_num + 1][0] = 1;
	map_info[loc_num + 1][2] = "character";
	
	//人物重绘
	var canvas = document.getElementById("myCanvas");
	loc_info_now = localStorage.temporary_loc;
	var loc_bgc = map_array[loc_info_now];
	draw_character("character",0,loc_bgc,map_info);
	//特效清除
	var canvas = document.getElementById("effiCanvas");
	var cxt = canvas.getContext("2d");
	cxt.clearRect(0,0,1400,700);
	//技能显示清除
	var canvas2 = document.getElementById("skillCanvas");
	var cxt2 = canvas2.getContext("2d");
	cxt2.clearRect(0,0,1400,700);
	//清空技能位置信息
	skill_loc ="";
	
	//回合结算
	
	//-----------非指向性技能在冷却时间结束（即效果消失之后）处理-----------------
	//增益（减益）效果可能持续多个回合，所以需要在效果结束之后进行效果重置
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
	if(parseInt(skill_info[9][1]) == 1){
		if(parseInt(skill_info[1][0]) == 1){//如果为非指向性效果
			if(parseInt(skill_info[10][0]) == 3){//如果是指向本身技能的效果
				//逆向生成

				var skill_change = skill_info[10][1];
				var change_skill_info = skill_change.split("{");
				var change_skill_content = change_skill_info[0].split("[");
				var change_num = parseInt(change_skill_content[0]);
				var skill_name = "";
				var change_array = change_skill_content[1].split("(");
				for(i in change_array){
					for(m in skill_info_all){
						if(skill_info_all[m][0][0] == change_array[i]){
							var skill_plus_str = change_skill_info[parseInt(i)+ 1];
							var skill_plus = [];
							var skill_plus_detail_str = skill_plus_str.split("[");
							for (n in skill_plus_detail_str){
								var skill_p_1 = skill_plus_detail_str[n].split("(");
								skill_plus.push(skill_p_1);
							}
							console.log(skill_plus[6][0]);
							var skill_compare = skill_info_all[m];
							for(n =1;n<skill_plus.length;n++){
								for(x in skill_plus[m]){
									if(skill_plus[n][x]!= "0"){
										skill_compare[n][x] = parseInt(skill_compare[n][x]) - parseInt(skill_plus[n][x]);
										skill_info_all[m][n][x] = skill_compare[n][x];
										console.log(skill_compare[n][x]);
									}
								}
							}
						}
					}				
				}
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
			}
		}
	}
	//-----------非指向性技能在冷却时间结束（即效果消失之后）处理-----------------
	//回合结算----技能冷却时间重置
	var skill_group_info =[];
	var skill_str = localStorage.character_skill;
	var skill_group = skill_str.split(";");
	for(i = 1;i <skill_group.length;i++){
		var skill_array = skill_group[i].split(",");
		var skill_info = [];
		var skill_info_forchange = [];
		for(m in skill_array){
			skill_info_forchange = skill_array[m].split("|");
			skill_info.push(skill_info_forchange);
		}
		if(parseInt(skill_info[9][1])){//如果skill_info[9][1]不为0（存在冷却时间），那么skill_info--
			skill_info[9][1] = parseInt(skill_info[9][1]) -1;
			console.log(skill_info[0][0]);
		}
		skill_group_info.push(skill_info);
	}
	
	var skill_array_2 =[];
	var length = skill_group_info.length;
	skill_array_2.push(length);
	for(i =0;i<skill_group_info.length;i++){
		var skill_array_1 = [];
		for(m in skill_group_info[i]){
				skill_str_1 = skill_group_info[i][m].join("|");
				skill_array_1.push(skill_str_1);
		}
		skill_str_2 = skill_array_1.join(",");
		skill_array_2.push(skill_str_2);
	}
	var skill_str_3 = skill_array_2.join(";");
	//console.log("重新组合后的技能信息为：" + skill_str_3);
	localStorage.character_skill = skill_str_3;
	localStorage.turnOn = 4;
	//进入其他角色流程
	localStorage.npc_move_turn = 1;
	wait_routine(map_array,map_info);
	//console.log("在round_finish执行过程当中turnOn的值为：" + localStorage.turnOn);
	localStorage.turnOn = 3;
	//console.log("在round_finish执行结束后turnOn的值为：" + localStorage.turnOn);
	console.log("skill_loc :" + skill_loc);
}


//读取本地的技能信息，并生成多维数组的函数
function read_skill_array(chara_array_name,level){
	var skill_array_1 = [];
	var skill_array_2 = [];
	var level = level||3;
	skill_array_str = localStorage.getItem(chara_array_name);
	if(level == 1){
		skill_array_1 = skill_array_str.split(";");
		return skill_array_1;
	}else if (level == 2){
		skill_array_1 = skill_array_str.split(";");
		for(i in skill_array_1){
			var skill_a_2 = skill_array_1[i].split(",");
			skill_array_2.push(skill_a_2);
		}
		return skill_array_2;
	}else if (level == 3){
		skill_array_1 = skill_array_str.split(";");
		var skill_array_4 = [];
		for(i in skill_array_1){			
			var skill_a_2 = skill_array_1[i].split(",");
			var skill_array_3 = [];
			for(m in skill_a_2){
				var skill_a_3 = skill_a_2[m].split("|");
				skill_array_3.push(skill_a_3);
			}
			skill_array_4.push(skill_array_3);
		}
		return skill_array_4;
	}
}

//将数组打包成字符串
function package_skill_array(skill_array_all , level){
	var level = level||3;
	if(level == 3){
		var c_r_2 = [];
		for(i in skill_info_all){
			var c_r_1 = [];
			for(m in skill_info_all[i]){
				var s_c_1 = skill_info_all[i][m].join("|");
				c_r_1.push(s_c_1);
			}
			var s_c_2 = c_r_1.join(",");
			c_r_2.push(s_c_2);
		}
		var skill_info_after = c_r_2.join(";");
		return skill_info_after;
	} 
}
