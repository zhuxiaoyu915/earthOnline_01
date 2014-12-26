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
				npc_info_detail = npc_information[m].split("|");
				npc_info.push(npc_info_detail);
			}
			npc_info.push(key);
			var blood_num = parseInt(npc_info[4][0]);
		}
		if(npc_info[6][0] =="6"){
			active_num++;
		}else if(blood_num > 0){
			active_num++;
		}
	}
	
	for(i=0;i<active_num;i++){		
		setTimeout(function a(){chara_auto_move(map_array,map_info,skill_info);},600*(i+1));
	}
	setTimeout(function b(){localStorage.turnOn = 1;},600*(i+1) + 500);
}

//流程----角色自动运行
function chara_auto_move(map_array,map_info,skill_info){
	//读取下一个可执行的npc
	var moveable_npc =0;
	while(moveable_npc ==0){
		var move_turn = parseInt(localStorage.npc_move_turn);
		var str_basic = "npc_";
		var key = str_basic + move_turn;
		var val = localStorage.getItem(key);
		var npc_info = [];
		if(val != ""){
			var npc_information = val.split(";");
			for( m in npc_information){
				npc_info_detail = npc_information[m].split("|");
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
		var s_b = neighbor_block(loc_num,map_array,map_info,sight_length,0);//生成视距模块
		s_b.push(loc_num);
		var dest = detect_destination(s_b , map_info);//在视距内遍历
		//移动
		var destination = route_find(dest,loc_num,map_array,map_info,npc_info);//寻径
		if(destination == -1){
			destination = loc_num;
		}
		//移动----擦除npc在baseCanvas上的位置
		var baseCanvas = document.getElementById("baseCanvas");
		var loc_bgc = map_array[loc_num];
		drawBgColor(loc_bgc,map_array,baseCanvas,"blue","white",0,1);
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
				var num = npc_info.length;
				var npc_name = npc_info[num -1];
				var canvas = document.getElementById("npcCanvas");
				draw_character(npc_name,1,loc,map_info,canvas);
				count++;
			}else if (count == frame_num){
				context.clearRect(0,0,1400,700);
				var baseCanvas = document.getElementById("baseCanvas");
				var color = npc_info[2][0];
				var num = npc_info.length;
				var npc_name = npc_info[num-1];
				var loc_bgc = map_array[destination];
				//console.log("loc_bgc的值为： " + loc_bgc);
				//console.log("map_info为：" + map_info);
				draw_character(npc_name,0,loc_bgc,map_info,baseCanvas);
				count++;
			}else{
				window.clearInterval(intervalID);
			}
		}
		var intervalID = window.setInterval(draw_begin , frame_time);
		
		//移动----更新map属性（map_info,旧属性去除，新属性添加）
		var old_info = parseInt(map_info[loc_num + 1][0]);
		old_info--;
		map_info[loc_num + 1][0] = old_info;
		map_info[loc_num + 1][2] ="";
		var new_info = parseInt(map_info[destination + 1][0]);
		new_info++;
		map_info[destination + 1][0] = new_info;
		var num = npc_info.length;
		map_info[destination + 1][2] = npc_info[num -1];
		//移动----更新npc属性（npc_info 和 localStorage）
		npc_info[1][0] = destination;
		var npc_inforr = [];
		for(i = 0;i <npc_info.length -1;i++){
			var npc_infor = npc_info[i].join("|");
			npc_inforr.push(npc_infor);
		}
		var npc_full_info = npc_inforr.join(";");
		localStorage.setItem(key,npc_full_info);
		//攻击
		//攻击----读取可选技能列表
		//console.log("npc_info为: " + npc_info[8][0]);
		var skill_list_key = npc_info[8][0];
		var skill_list_str = localStorage.getItem(skill_list_key);
		var skill_list = skill_list_str.split(";");
		var skill_array = [];
		for(i = 1;i < skill_list.length;i++){
			var skill_info = [];
			var skill_information = skill_list[i].split(",");
			for(m in skill_information){
				skill_info_detail = skill_information[m].split("|");
				skill_info.push(skill_info_detail);
			}
			skill_array.push(skill_info);
		}
		//攻击----选取攻击技能
		//攻击----选取过程暂时跳过，直接选取一号技能施放
		if(dest != -1){
			//执行攻击动作
			for(i in skill_array){
				var skill_info = skill_array[i];
				var skill_demand = skill_castable_test(npc_info,skill_info);

				if(skill_array[i][1]== 2 && skill_array[i][6] < 0){//指向性伤害类
					var skill_info = skill_array[i];
					directional_cast();
					function directional_cast(){
						var orient_loc = npc_info[1];
						var desti_loc = dest;
						var skill_efficacy = skill_info[4];
						var canvas = document.getElementById("skillCanvas");
						var destination = skill_efficacy_count(canvas,map_array,map_info,orient_loc,desti_loc,skill_efficacy,skill_info);
						damage_count(map_array,map_info,destination,skill_info,npc_info);
					}
				}
			}
		}
		
		//停止
	}
	move_turn++;
	localStorage.npc_move_turn = move_turn;
}
