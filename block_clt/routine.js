//流程
//流程--旁观者流程
function wait_routine(map_array,map_info){
	//检测可活动目标
	var str_basic = "npc_";
	var active_num = 0;
	for(i = 1; i < 7; i++){
		var key = str_basic + i; 
		var val = localStorage.getItem(key);
		var npc_info = [];
		if(val != ""){
			var npc_information = val.split(";");
			for( m in npc_information){
				npc_info_detail = npc_information[m].split("/");
				npc_info.push(npc_info_detail);
			}
			npc_info.push(key);
			var blood_num = parseInt(npc_info[4][0]);
		}
		if(npc_info[6][0] =="6"){
			active_num++;
		}else if(blood_num > 0){
			active_num++;
			console.log(npc_info[8]);
		}
	}
	for(i=0;i<active_num;i++){		
		setTimeout(function a(){chara_auto_move(map_array,map_info);},600*(i+1));
	}
	localStorage.turnOn = 1;
}

//流程----角色自动运行
function chara_auto_move(map_array,map_info){
	//自动初始化
	//读取下一个可珍惜的npc
	var moveable_npc =0;
	while(moveable_npc ==0){
		var move_turn = parseInt(localStorage.npc_move_turn);
		var str_basic = "npc_";
		var key = str_basic + move_turn;
		//console.log("key为： " + key);
		var val = localStorage.getItem(key);
		var npc_info = [];
		if(val != ""){
			var npc_information = val.split(";");
			for( m in npc_information){
				npc_info_detail = npc_information[m].split("/");
				npc_info.push(npc_info_detail);
			}
			npc_info.push(key);
		}
		//判断该npc是否执行流程
		//获得npc的血量
		var blood_num = parseInt(npc_info[4][0]);
		var chara_state = npc_info[6][0];
		if(blood_num > 0||chara_state == "6"){
			moveable_npc = 1;
		}else if(blood_num ==0&&chara_state =="10"){
			//这是新死之人的判断（在正式场景不会出现）
		}else{
			move_turn++;
			localStorage.npc_move_turn = move_turn;
		}
	}
		
	var moveable = 1;//可以移动
	var skillable = 1;//可以施放技能
	var attackable = 1;//可以物理攻击
	var resentable = 1;//抗性
	var sight_length = parseInt(npc_info[5][1]);//视野距离
	var move_arrange_str = npc_info[5][2];//巡逻范围字符串
	var move_arrange = move_arrange_str.split(",");//可移动范围数组
	var moveability = parseInt(npc_info[5][3]);//一次能移动多少拼版
	var senseability = parseInt(npc_info[5][4]);//感知深度（普通地形，隐藏类地形）
	var attack_damage = parseInt(npc_info[5][5]);//攻击力
	switch(npc_info[6][0]){
		case "0":{
			//console.log("正常");
			break;
			}
		case "1":{
			//console.log("冰冻");
			movebale = 0;
			skillable = 0;
			attactable = 0;
			break;}
		case "2":{
			//console.log("恐惧");
			skillable = 0;
			attackable = 0;
			break;}
		case "3":{
			//console.log("眩晕");
			movebale = 0;
			skillable = 0;
			attactable = 0;
			break;}
		case "4":{
			//console.log("愤怒");
			break;
			}
		case "5":{
			//console.log("振奋");
			break;
			}
		case "6":{
			//console.log("强行");
			break;
			}
		case "7":{
			//console.log("漂浮");
			break;
			}
		case "8":{
			//console.log("残废");
			break;
			}
		case "9":{
			//console.log("潜行");
			break;
			}
	}	
	if(npc_info[6][0] != "6"&& npc_info[4][0] == "0")
	{
		loc_num = parseInt(npc_info[1][0]);
		var baseCanvas = document.getElementById("baseCanvas");
		var loc_bgc = map_array[loc_num];
		drawBgColor(loc_bgc,map_array,baseCanvas,"blue","white",0,1);
		//改成1了？
		//drawBggColor(loc_num,map_array,"white",baseCanvas,1,1);
		console.log(npc_info[0][0] + "已经死亡,不再进行移动");
	}else{
		var direction = false;
		//按顺序执行动作：
		//检测
		loc_num = parseInt(npc_info[1][0]);
		var s_b = neighbor_block(loc_num,map_array,map_info,sight_length,0);
		s_b.push(loc_num);
		var dest = detect_destination(s_b , map_info);
		//移动
		var destination = route_find(dest,loc_num,map_array,map_info,npc_info);
		//console.log("loc_num 为： " + loc_num + ". destination 为：" + destination);
		if(destination == -1){
			destination = loc_num;
		}
		//移动----擦除npc在baseCanvas上的位置
		var baseCanvas = document.getElementById("baseCanvas");
		var loc_bgc = map_array[loc_num];
		drawBgColor(loc_bgc,map_array,baseCanvas,"blue","white",0,1);
		//drawBggColor(loc_num,map_array,"white",baseCanvas,1,1);
		//移动----循环绘制npc块
		var npccanvas = document.getElementById("npcCanvas");
		var context = npccanvas.getContext("2d");
		//移动----循环绘制npc块----计算距离

		var orient_info = map_array[loc_num];
		var desti_info = map_array[destination];
		//移动----循环绘制npc块----计算帧属性
		var frame_num = 15;
		var frame_time = 30;
		var frame_distance_x = (desti_info.x - orient_info.x)/ frame_num;
		var frame_distance_y = (desti_info.y - orient_info.y)/ frame_num;
		//移动----循环绘制npc块----绘制开始
		var count = 0;
		var draw_loc_array_x = orient_info.x;
		var draw_loc_array_y = orient_info.y;
		function draw_begin(){
			if(count < frame_num){
				context.clearRect(0,0,1400,700);
				draw_loc_array_x = draw_loc_array_x + frame_distance_x;
				draw_loc_array_y = draw_loc_array_y + frame_distance_y;
				var loc = {x:draw_loc_array_x, y:draw_loc_array_y};
				var color = npc_info[2][0];
				drawCharacter_move(loc,map_array,color,npccanvas,1);
				count++;
			}else if (count == frame_num){
				context.clearRect(0,0,1400,700);
				var baseCanvas = document.getElementById("baseCanvas");
				var color = npc_info[2][0];
				var loc_bgc = map_array[destination];
				drawBgColor(loc_bgc,map_array,baseCanvas,color,color,0,1);
				//drawBggColor(destination,map_array,color,baseCanvas,1,1);
				count++;
			}else{
				window.clearInterval(intervalID);
			}
		}
		var intervalID = window.setInterval(draw_begin , frame_time);
		
		//移动----更新map属性（map_info,旧属性去除，新属性添加）
		//console.log(map_info);
		var old_info = parseInt(map_info[loc_num + 1][0]);
		old_info--;
		map_info[loc_num + 1][0] = old_info;
		map_info[loc_num + 1][2] ="";
		var new_info = parseInt(map_info[destination + 1][0]);
		new_info++;
		map_info[destination + 1][0] = new_info;
		map_info[destination + 1][2] = npc_info[8];
		//console.log(map_info);
		//移动----更新npc属性（npc_info 和 localStorage）
		npc_info[1][0] = destination;
		var npc_inforr = [];
		for(i = 0;i <npc_info.length -1;i++){
			var npc_infor = npc_info[i].join("/");
			npc_inforr.push(npc_infor);
		}
		var npc_full_info = npc_inforr.join(";");
		localStorage.setItem(key,npc_full_info);
		//攻击
		//停止
	}
	move_turn++;
	localStorage.npc_move_turn = move_turn;
}
