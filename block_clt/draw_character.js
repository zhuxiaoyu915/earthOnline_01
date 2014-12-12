
function draw_character(chara_name,move,loc,map_info){
	//绘制参数
	var canvas = document.getElementById("myCanvas");
	var cxt = canvas.getContext("2d");
	//图片加载
	var chara_image_1 = window.chara_image_1;

	
	var chara_info = [];
	var key = chara_name;
	var desti_chara = localStorage.getItem(key);
	var chara_str = desti_chara.split(";");
	for(i in chara_str){
		chara_info_1 = chara_str[i].split("/");
		chara_info.push(chara_info_1);
	}
	if(move){
		var x = loc.x;
		var y = loc.y;
	}else{
		var loc_num = parseInt(chara_info[1]);
		var x = map_array[loc_num].x;
		var y = map_array[loc_num].y;
	}
	
	var length = map_info[0].length;
	var length_2 = length - 10;
	
	var border_array = [];
	var border1_x = x - length/2;
	var border1_y = y - length * 0.866;
	border_array.push({x:border1_x , y:border1_y});
	var border2_x = x + length/2;
	var border2_y = border1_y;
	border_array.push({x:border2_x , y:border2_y});
	var border3_x = x + length*1;
	var border3_y = y;
	border_array.push({x:border3_x , y:border3_y});
	var border4_x = border2_x;
	var border4_y = y + length * 0.866;
	border_array.push({x:border4_x , y:border4_y});
	var border5_x = border1_x;
	var border5_y = border4_y;
	border_array.push({x:border5_x , y:border5_y});
	var border6_x = x - length*1;
	var border6_y = y;
	border_array.push({x:border6_x , y:border6_y});
	
	var point_array = [];
	var p1_x = x - length_2/2;
	var p1_y = y - length_2 * 0.866;
	point_array.push({x:p1_x , y:p1_y});
	var p2_x = x + length_2/2;
	var p2_y = p1_y;
	point_array.push({x:p2_x , y:p2_y});
	var p3_x = x + length_2*1;
	var p3_y = y;
	point_array.push({x:p3_x , y:p3_y});
	var p4_x = p2_x;
	var p4_y = y + length_2 * 0.866;
	point_array.push({x:p4_x , y:p4_y});
	var p5_x = p1_x;
	var p5_y = p4_y;
	point_array.push({x:p5_x , y:p5_y});
	var p6_x = x - length_2*1;
	var p6_y = y;
	point_array.push({x:p6_x , y:p6_y});
	
	
	//绘制
	//添加底色
	//添加图片
	var bgColor = chara_info[2][0];
	if(move){
		drawBgColor(loc,map_array,canvas,bgColor,bgColor,1,1);
		cxt.drawImage(chara_image_1,loc.x-40,loc.y-30,80,60);
	}else{
		var loc ={x:map_array[loc_num].x,y:map_array[loc_num].y};
		drawBgColor(loc,map_array,canvas,bgColor,bgColor,0,1);
		console.log("绘制绵羊");
		cxt.drawImage(chara_image_1,map_array[loc_num].x-40,map_array[loc_num].y-30,80,60);
		console.log("绘制结束了");
	}
	
	
	//添加血条
	var b = parseInt(chara_info[4][0]);
	var m = parseInt(chara_info[4][1]);
	var color_info = [];
	for(i = b; i>0 ; i--){
		color_info.push("red");
	}
	for(i = m; i>0 ; i--){
		color_info.push("blue");
	}
	
	for(i = 0; i < 6; i++){
		cxt.lineWidth = 3;
		cxt.strokeStyle = "grey";
		cxt.fillStyle = color_info[i];
		if(i > color_info.length - 1){
			cxt.fillStyle = "white";
		}
		cxt.beginPath();
		if(i == 5){
			cxt.moveTo(border_array[5].x , border_array[5].y);
			cxt.lineTo(border_array[0].x , border_array[0].y);
			cxt.lineTo(point_array[0].x , point_array[0].y);
			cxt.lineTo(point_array[5].x , point_array[5].y);
			cxt.lineTo(border_array[5].x , border_array[5].y);
		}else{
			cxt.moveTo(border_array[i].x , border_array[i].y);
			cxt.lineTo(border_array[i+1].x , border_array[i+1].y);
			cxt.lineTo(point_array[i+1].x , point_array[i+1].y);
			cxt.lineTo(point_array[i].x , point_array[i].y);
			cxt.lineTo(border_array[i].x , border_array[i].y);
		}
		cxt.closePath();
		
		cxt.fill();
		cxt.stroke();
	}
	//添加边框
	for(i = 0; i < 6 ; i++){
		cxt.lineWidth = 3;
		cxt.strokeStyle = "#ecf0f1";
		cxt.beginPath();
		
		cxt.closePath();
	}
}