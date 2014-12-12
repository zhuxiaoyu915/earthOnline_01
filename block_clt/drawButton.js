//绘图
function drawButton()
{	
   	var started = false;//mouse_event
	var character_move_begin = false;//mouse_event
	var temporary_state = false;//mouse_event
	var up_num = -1;//mouse_event
	var skill_loc =[];
	var skill_move_begin = false;//mouse_event
	
	var skill_chosen_num = -1;//mouse_event 断定的技能的编号
	var skill_map_num = -1;//mouse_event 选定的技能在地图上的位置
	
	var str = localStorage.message;
	var str_old = str;
	//str_num = str.split(";");
	
	var ptArray = [];
	var chosen_num = -1;
	var content = document.getElementById("content");
	//基础数据绘制
    var basecanvas = document.getElementById("baseCanvas");
    var basecxt = basecanvas.getContext("2d");
	//上层数据绘制
	var canvas = document.getElementById("myCanvas");
	var cxt = canvas.getContext("2d");
	
	//特效数据绘制
	var efficanvas = document.getElementById("effiCanvas");
	var efficxt = efficanvas.getContext("2d");
    cxt.font = "15px Time New Roman";
    var map_height = 5;
	var map_width = 12;
	var map_length = 50;
	var map_info = map_info_built();
	function map_info_built(){
	//============================================================================
	//=================     map_info的手动设置    =================================
	var map_info =[];
	var map_base_info = ({height:map_height,width:map_width,length:map_length});
	map_info.push(map_base_info);
	var map_num_count = 0;
	for(i = 0;i<map_width;i++){
	    for(m =0;m<map_height;m++){
		var single_propriety =[];
		single_propriety[0] = 0;
		single_propriety[1] = 1;
		single_propriety[2] = "";
		single_propriety[3] = "";
		single_propriety[4] = map_num_count;
		map_info.push(single_propriety);
		map_num_count++;
		}
	}
	//读取NPC
	var npc_array_all = [];
	for(i = 1;i<7;i++){
		var str_basic = "npc_";
		var key = str_basic + i;
		var val = localStorage.getItem(key);
		var npc_information = [];
		if(val != ""){
			var npc_info = val.split(";");
			for( m in npc_info){
				npc_info_detail = npc_info[m].split("/");
				npc_information.push(npc_info_detail);
			}
			npc_information.push(key);
		npc_array_all.push(npc_information);
		}	
	}
	//将ncp数据导入map_info
	for(i in npc_array_all){
		var loc_n = parseInt(npc_array_all[i][1][0]);
		map_info[loc_n + 1][0] = 1;
		map_info[loc_n + 1][2] = npc_array_all[i][8];
		
	}
	
	//将人物数据导入map_info
	var loc_c = [];
	var loc_c_str = localStorage.character;
	loc_c_info = loc_c_str.split(";");
	for(i in loc_c_info){
		var loc_s = loc_c_info[i].split("/");
		loc_c.push(loc_s);
	}
	var loc_n = parseInt(loc_c[1][0]);
	map_info[loc_n + 1][0] = 1;
	map_info[loc_n + 1][2] = "character";
	
	return map_info;
	//=============================================================================
	//=============================================================================
	}
	//生成一个N格地图中心点图
	map_array = builtMap(map_width,map_height,map_length);
	//根据点图生成地图
	drawMap(map_array);
	//添加事件监听
	content.addEventListener("mousedown", doMouseDown, false);
	content.addEventListener('mousemove', doMouseMove, false);
	content.addEventListener('mouseup',   doMouseUp, false);
	drawBasedArc(canvas,map_array);
	init_Chara_block(map_array,map_info);
	//console.log(map_info);

function getPointOnCanvas(canvas, x, y) {
	var bbox = canvas.getBoundingClientRect();
	return {x: x - bbox.left * (canvas.width  / bbox.width),
			y: y - bbox.top  * (canvas.height / bbox.height)
		   };
}
		
function doMouseDown(event) {
    
	var x = event.pageX;
	var y = event.pageY;
	var canvas = document.getElementById("myCanvas");
	var loc = getPointOnCanvas(canvas, x, y);
	//console.log("mouse down at point( x:" + loc.x + ", y:" + loc.y + ")");
	started = true;

	for(i = 0;i< map_width * map_height; i++){
		if(loc.x > (parseInt(map_array[i].x)-map_length/2) && loc.x < (parseInt(map_array[i].x) + map_length/2) && loc.y > (parseInt(map_array[i].y) - map_length* 0.866) && loc.y < (parseInt(map_array[i].y) + map_length*0.866) )
		{
			chosen_num = i;
		}
	}
	
	var move =localStorage.move;
	character_move_begin = false;
	
	switch(localStorage.turnOn){
		case '1':
			turnOn_1();
			break;
		case '2':
			turnOn_2();
			break;
		case '3':
			console.log("sd");
			break;
	}
	
	function turnOn_1(){
		var character_info = localStorage.character;
	    chara = character_info.split(";");

		if(temporary_state){
			var character_loc = localStorage.temporary_character_loc;
		}else{
			character_loc = parseInt(chara[1]);
		}
		
	    if(chosen_num == character_loc){
		    move = chara_ready(canvas,chara,map_info,map_array,loc);
			character_move_begin = true;	
	    }else if(skill_loc != ""){
			for(i in skill_loc){
				if(chosen_num == skill_loc[i]){
					skill_chosen_num = i;
					skill_map_num = skill_loc[i];
					localStorage.turnOn = 2;
					skill_chosing(canvas,map_array,map_info,skill_map_num,skill_chosen_num,loc);
					skill_move_begin = true;
				}			
			}
		}
	}
	
	function turnOn_2(){
		var character_info = localStorage.character;
	    chara = character_info.split(";");

		if(temporary_state){
			var character_loc = localStorage.temporary_character_loc;
		}else{
			character_loc = parseInt(chara[1]);
		}
		
	    if(chosen_num == character_loc){//点选角色位置
			var loc_changeable = localStorage.loc_changable;//在施法后，loc_change值设为0，不能再进行位置的更改
			if(loc_changeable == 1){//如果 = 1,表明没有进行过施法，可以继续移动
				move = chara_ready(canvas,chara,map_info,map_array,loc);
				character_move_begin = true;
			}else{
				//console.log("人物角色在释放技能后不能再进行移动");
				var context = canvas.getContext("2d");
				context.clearRect(0,0,1400,700);
				move_finish(map_array , map_info);
				localStorage.turnOn =3;
			}
	    }else if(skill_loc != ""){//在drawButton中定义，记录各个技能的位置
			for(i in skill_loc){
				if(chosen_num == skill_loc[i]){
					var skill_num = i;
					var map_num = skill_loc[i];
					localStorage.turnOn = 2;
					skill_chosing(canvas,map_array,map_info,skill_map_num,skill_chosen_num,loc);
					skill_move_begin = true;
				}			
			}
		}
	}
	
	function turnOn_3(){
		console.log("观察者状态，点击弹出被点击坐标的详细信息");
		//清空所有其余信息
	}
}

function doMouseMove(event) {
    var x = event.pageX;
	var y = event.pageY;
	var canvas = document.getElementById("myCanvas");
	var loc = getPointOnCanvas(canvas, x, y);
	var character_info = localStorage.character;
	chara = character_info.split(";");
	if(started){
		switch(localStorage.turnOn){
			case '1':			
				if(character_move_begin){
					cxt = canvas.getContext("2d");
					cxt.clearRect(0,0,1400,700);
					move = chara_ready(canvas,chara,map_info,map_array,loc);
				}    
				break;
			case '2':
				if(skill_move_begin){
					cxt = canvas.getContext("2d");
					cxt.clearRect(0,0,1400,700);
					skill_chosing(canvas,map_array,map_info,skill_map_num,skill_chosen_num,loc);
				}else if (character_move_begin){
					cxt = canvas.getContext("2d");
					cxt.clearRect(0,0,1400,700);
					move = chara_ready(canvas,chara,map_info,map_array,loc);
				}
				break;
			case '3':
				console.log("每个拼版的基本信息");
				break;
		}
	}
}

function doMouseUp(event) {
    var x = event.pageX;
	var y = event.pageY;
	var canvas = document.getElementById("myCanvas");
	var cxt = canvas.getContext("2d");
	var loc = getPointOnCanvas(canvas, x, y);
	var up_num = -1;
	
	switch(localStorage.turnOn){
		case '1':
			whenCharaFinishMove();			
			break;
		case '2':
			if(character_move_begin){
				whenCharaFinishMove();
			}else if(skill_move_begin){
				cxt = canvas.getContext("2d");
				cxt.clearRect(0,0,1400,700);
				skill_move_begin = skill_chosed(canvas,map_array,map_info,skill_map_num,skill_chosen_num,loc);
				loc_info_now = localStorage.temporary_loc;//获取当前坐标
				character_info = localStorage.character;
				draw_leader(loc_info_now,character_info,map_array,map_info);
				skill_finish_button();//"结束了"三个字
				
				skill_loc_2 = skill_spread(canvas,chara,map_info,map_array,loc_info_now);
				//console.log("技能释放之后，skill_move_begin值为：" + skill_move_begin);
			}
			break;
		case '3':
			loc_info_now = localStorage.temporary_loc;
			color = "#cdd3d7";
			var canvas = document.getElementById("myCanvas");
			var loc_bgc = map_array[loc_info_now];
			drawBgColor(loc_bgc,map_array,canvas,color,color,0,1);
			//drawBggColor(loc_info_now,map_array,color,canvas);
			var canvas = document.getElementById("effiCanvas");
			var cxt = canvas.getContext("2d");
			cxt.clearRect(0,0,1400,700);
			localStorage.turnOn = 4;//权宜之计
			//其他角色流程
			localStorage.npc_move_turn = 1;
			wait_routine(map_array,map_info);
			break;
	}
	function skill_finish_button(){
		var canvas = document.getElementById("myCanvas");
		var context = canvas.getContext("2d");
		loc = localStorage.temporary_character_loc;
		cxt.fillStyle = "white";
		cxt.textAlign = "center";
		cxt.fillText("结束了",map_array[loc].x , map_array[loc].y + 7);	 
	}
	function whenCharaFinishMove(){
		//console.log("chara移动完毕");
		for(i = 0;i< map_width * map_height; i++){
			if(loc.x > (parseInt(map_array[i].x)-map_length/2) && 
					loc.x < (parseInt(map_array[i].x) + map_length/2) && 
					loc.y > (parseInt(map_array[i].y) - map_length* 0.866) && 
					loc.y < (parseInt(map_array[i].y) + map_length*0.866)){
			up_num = i;
			}
		}
		started = false;
		var finish_info = chara_ready_finish(canvas,chara,map_info,map_array,up_num,character_move_begin);
		//返回值finish_info是一个数组：
		//----temporary_state ：是否有临时状态的变量（主要是为了避免发生鼠标移动到可选范围之外后抬起的情况。在这种情况下，默认为没有临时状态）
		//----temporary_loc 临时位置 
		temporary_state = finish_info[1];
		
		if(temporary_state){
			//人物位置的即时更新
			var loc_c = [];
			var loc_c_str = localStorage.character;
			loc_c = loc_c_str.split(";");
	
			if(localStorage.temporary_loc == ""){
				loc_old = parseInt(loc_c[1]);
			}else {
				loc_old = parseInt(localStorage.temporary_loc);
			}
			var state = parseInt(map_info[loc_old + 1][0]);
			map_info[loc_old + 1][0] = state -1;
			map_info[loc_old + 1][2] = "";
			var loc_new = parseInt(finish_info[0]);
			state = parseInt(map_info[loc_new + 1][0]);
			map_info[loc_new + 1][0] = state + 1;
			//技能列表展开
			skill_loc = skill_spread(canvas,chara,map_info,map_array,finish_info[0]);
			
		}
		localStorage.temporary_loc = finish_info[0];
	}
}	
}
