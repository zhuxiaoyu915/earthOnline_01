//操作状态设置----人物准备好移动
function chara_ready(chara,map_info,map_array,loc){
	var canvas = document.getElementById("moveCanvas");
	var cxt = canvas.getContext("2d");
	cxt.clearRect(0,0,1400,700);
	var loc_num = parseInt(chara[1]);
	var moveable = neighbor_block(loc_num,map_array,map_info,1,1);
	moveable.push(loc_num);
	if(localStorage.temporary_character_loc != ""){
		var chara_loc = parseInt(localStorage.temporary_character_loc);
		moveable.push(chara_loc);
	}
	for(i in moveable){
		var num = moveable[i];
		if(num>=0 && loc.x>map_array[num].x-25 &&loc.x<map_array[num].x + 25 && loc.y>map_array[num].y -50*0.866 && loc.y < map_array[num].y + 50*0.866){
			var color = "#3498db";
			var loc_bgc = map_array[num];
			drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
		}else{
			var color = "#99bbbb";
			var loc_bgc = map_array[num];
			drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
		}
	}
	draw_character("character",1,loc,map_info);
	return moveable;
}

//操作状态设置----人物移动结束
function chara_ready_finish(canvas,chara,map_info,map_array,up_num,character_move_begin){
	if(character_move_begin){
		var finished = false;
		var cxt = canvas.getContext("2d");
		var loc_num = parseInt(chara[1]);
		var moveable = neighbor_block(loc_num,map_array,map_info,1,1);
		moveable.push(loc_num);	
		if(localStorage.temporary_character_loc != ""){
			var chara_loc = parseInt(localStorage.temporary_character_loc);
			moveable.push(chara_loc);
		}
		var loc_now = loc_num;
		cxt.clearRect(0,0,1400,700);
		for(i in moveable){
			var num = parseInt(moveable[i]);
			if(num == up_num){
				var loc_bgc = map_array[up_num];
				draw_character("character",0,loc_bgc,map_info);
				finished = true;
				temporary_state = true;
				localStorage.temporary_character_loc = num;	
				loc_now = num;
			}
		}
		if(!finished){ 
			console.log("没有放在正确的格子上啊喂！");
			color_char = "#cdd3d7";
			localStorage.temporary_character_loc = loc_num;
			//重绘角色
			var loc_bgc = map_array[loc_num];
			draw_character("character",0,loc_bgc,map_info);
			temporary_state = false;
		}
		
		character_move_begin = false;
        var finish_info = [];
		finish_info.push(loc_now);
		finish_info.push(temporary_state);
		return finish_info;
	}else{
		console.log("又瞎点了");
	}
}
