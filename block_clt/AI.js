//使用新版本的文本编辑器来测试。
//AI----目标搜寻
function detect_destination(s_b,map_info){
	var dest = -1;
	for(i in s_b){
		if(map_info[s_b[i] + 1][2] == "character"){
			console.log("fucker,catch you!");
			direction = true;
			var dest = s_b[i];
		}
	}
	return dest;
}

//AI----路径选择
function route_find(dest,loc_num,map_array,map_info,npc_info){
	//先生成可移动区域
	var moveability = parseInt(npc_info[5][3]);//移动能力
	var move_arrange_str = npc_info[5][2];
	var move_arrange = move_arrange_str.split(",");//巡逻拼版数组
	var moveable_block = neighbor_block(loc_num,map_array,map_info,moveability,1);//可移动拼版
	
	var destination = -1;
	//检测视距内有无敌对目标
	if(dest == -1){//视距内没有敌对目标
		//读取目标点信息，并检测当前状态有无目标点
		aim_info = npc_info[7];
		if(aim_info[0] == ""){//如果没有目标点
			var m_num = moveable_block.length; 
			if(m_num == 1){//如果只有一个可移动位置，直接移动
				destination = moveable_block[0];
			}else{//将可移动位置与巡逻位置叠加匹配
				var get = false;
				var count = 0;
				while(!get&&count<5){//最多进行五次匹配，如果不成功，则选定路径退回任意一个巡逻点坐标
					var n = parseInt(Math.random()*m_num);

					var choseable_b  = moveable_block[n];
					for(i in move_arrange){
						if(choseable_b == move_arrange[i]){
							destination = choseable_b;
							get = true;
						}
					}
					count++;
				}
				if(count ==5){
				}
			}
		}
		else{//目标存在，沿着目标走
			
		}
	}else{
		//计算当前位置与目标的距离
		var distance = {dis:1000000,loc:-1};
		for(i in moveable_block){//计算出距离敌方最近的一个拼版
			des_loc = dest;
			or_loc = parseInt(moveable_block[i]);
			var dist = (map_array[or_loc].x - map_array[des_loc].x)*(map_array[or_loc].x - map_array[des_loc].x) + (map_array[or_loc].y - map_array[des_loc].y)*(map_array[or_loc].y - map_array[des_loc].y);
			if(dist < distance.dis){
				distance.dis = dist;
				distance.loc = or_loc;
			}
		}
		//console.log("距离目标最近拼版为" + distance.loc);
		destination = distance.loc;
	}
	return destination;
}

