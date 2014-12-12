//基础函数----绘制模块的背景色
function drawBggColor(loc_num,map_array,color,canvas,shadow,alpha){
    var alpha = alpha||1;
	var num = map_array.length-1;
	var len = map_array[num].line_length;
	var cxt = canvas.getContext("2d");
	cxt.lineWidth = 5;
	cxt.fillStyle = color;
	cxt.strokeStyle = color;
	//阴影设置
	cxt.shadowOffsetX = 0;
	cxt.shadowOffsetY = 0;
	cxt.shadowBlur = 0;
	cxt.shadowColor = 'rgba(50,50,50,0.5)';
	cxt.globalAlpha = alpha;
	
	//添加背景色	
    var loc_x = map_array[loc_num].x;
    var loc_y = map_array[loc_num].y;
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
	cxt.beginPath();
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

function drawBgColor(loc,map_array,canvas,strokeColor,fillColor,shadow,alpha){
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
	
	context.lineWidth = 4;
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
		}else{
			window.clearInterval(intervalID);
		}
	}
	 var intervalID = window.setInterval(draw_begin , frame_time);
}

//基础函数----流程结束
function move_finish(map_array,map_info){
	//对角色修改进行保存
	var chara_info = [];
	var character = localStorage.character;
	var chara_str = character.split(";");
	for(i in chara_str){
		var chara_detail = chara_str[i].split("/");
		chara_info.push(chara_detail);
	}
	//清除原位置在map_info中产生的残留信息
	var loc_num = parseInt(chara_info[1][0]);
	map_info[loc_num + 1][0] = 0;
	map_info[loc_num + 1][2] = "";
	
	chara_info[1][0] = localStorage.temporary_character_loc;
				
	var string_array = [];
	for(i in chara_info){
		var string_array_1 = chara_info[i].join("/");
		string_array.push(string_array_1);
	}
	string_new = string_array.join(";");
	localStorage.character = string_new;
	//重新写入character在现在位置的信息
	var loc_num = parseInt(chara_info[1][0]);
	map_info[loc_num + 1][0] = 1;
	map_info[loc_num + 1][2] = "character";
}
