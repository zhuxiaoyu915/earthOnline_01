//地图设置----生成地图位置信息
function builtMap(width,height,length){
    var num = width * height;
	var width_num = width;
	var height_num = height;
	var lenght = length;
	var init_x = 100;
	var init_y = 50;
	var array =[];
    var x = init_x;
	var y = init_y;

	for(m = 1; m <=width_num ;m++)
	{
	if (m%2 == 1)
	{
	  	//元数据绘制
		for(count = 0;count<height_num;count++){
	        x = x + 0;
	        y = y + length * 1.732;
		    array.push({x:x,y:y});
	    }
		x = x + length*1.5;
		y = init_y - length*1.732/2;
	}else if(m%2 ==0)
	{
	    for(count = 0;count<height_num;count++){
	        x = x + 0;
	        y = y + length * 1.732;
		    array.push({x:x,y:y});
	    }
		x = x + length*1.5;
		y = init_y;
	}
	}
	//添加地图信息
	array.push({line_length:length,map_num:num});
    return array;
}	

//地图设置----绘制地图
function drawMap(mapArray){
    //初始化参数
	var canvas = document.getElementById("baseCanvas");
	var array = mapArray;
	var cxt = canvas.getContext("2d");
    cxt.fillStyle = "#FF0000";
	cxt.strokeStyle = "#0000FF";
	cxt.lineWidth = 2;
	//获得地形方块的个数
	var num = array.length-1;
	//获得每个拼版的长度
	var len = array[num].line_length;
	//绘制拼版图
	cxt.beginPath();
	for(i = 0;i<num; i++)
	{
	    var loc_x = array[i].x;
        var loc_y = array[i].y;
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
		
		cxt.moveTo(p1_x,p1_y);
		cxt.lineTo(p2_x,p2_y);
		cxt.lineTo(p3_x,p3_y);
		cxt.lineTo(p4_x,p4_y);
		cxt.lineTo(p5_x,p5_y);
		cxt.lineTo(p6_x,p6_y);
		cxt.lineTo(p1_x,p1_y);
		cxt.stroke();
	}
	cxt.closePath();
	//绘制技能图标
	cxt.beginPath();
	cxt.strokeStyle = "#9b59b6";
	cxt.fillStyle ="#9b59b6";
	cxt.rect(1050,100,150,50);
	cxt.fill();
	cxt.closePath();
	
	cxt.beginPath();
	cxt.strokeStyle = "#3498db";
	cxt.fillStyle ="#3498db";
	cxt.rect(1050,200,150,50);
	cxt.fill();
	cxt.closePath();
	
	cxt.beginPath();
	cxt.strokeStyle = "#2ecc71";
	cxt.fillStyle ="#2ecc71";
	cxt.rect(1050,300,150,50);
	cxt.fill();
	cxt.closePath();
	
	cxt.beginPath();
	cxt.strokeStyle = "#99bbbb";
	cxt.fillStyle ="#99bbbb";
	cxt.rect(1050,400,150,50);
	cxt.fill();
	cxt.closePath();
	//将技能图标的信息存入localStorage
	var buttonArray =[];
	buttonArray.push({x:1050,y:100,width:150,height:50,color:"#9b59b6"});
	buttonArray.push({x:1050,y:200,width:150,height:50,color:"#3498db"});
	buttonArray.push({x:1050,y:300,width:150,height:50,color:"#2ecc71"});
	buttonArray.push({x:1050,y:400,width:150,height:50,color:"#99bbbb"});
	localStorage.buttonInfo = buttonArray;
}

//地图设置----绘制基础型建筑
function drawBasedArc(canvas,mapArray){
    //如果没有联网下载地图
    if(localStorage.basedArc == ""){
	    //那就进行本地地图读取
	}

}
