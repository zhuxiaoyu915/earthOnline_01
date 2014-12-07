var context = null; // global variable 2d context
var started = false;
var mText_canvas = null;
var x = 0, y =0;
window.add
window.onload = function() {
	var canvas = document.getElementById("base_canvas");
	context = canvas.getContext("2d");

	localStorage.map_info = "3,3,50,100,100";
	localStorage.map_base_sit_info = "0,1;1,1;2,1;3,1;4,1;5,1;6,1;7,1;8,1;9,1";

	var map_info = localStorage.map_info.split(",");
	var map_array = built_base_map_array(map_info[0],map_info[1],map_info[2],map_info[3],map_info[4]);
	draw_base_map(context,map_array,map_info);
    canvas.addEventListener("mousedown", doMouseDown, false);
    canvas.addEventListener('mousemove', doMouseMove, false);
    canvas.addEventListener('mouseup',   doMouseUp, false);
}

function built_base_map_array(width,height,length,x_loc,y_loc){	
	var map_array =[];
	for(i =0;i<width;i++){
		for(m = 0; m < height;m++){
			x = parseInt(x_loc) + i * length * 1.5;
			if(i%2 == 0){
				y = parseInt(y_loc) + m*length*1.732;
			}else{
				y = parseInt(y_loc) + m*length*1.732 -length*0.866;
			}
			map_array.push({x:x,y:y});
		}	
	}
	return map_array;
}

function draw_base_map(context,map_array,map_info,map_base_sit){
	var length = map_info[2];
	for(i = 0; i < map_array.length; i++){
		var fillColor = "#ecf0f1";
		var strokeColor = "#99bbbb";
		drawBgColor(i,map_array,map_info,context,strokeColor,fillColor);	
	}
	var base_1 = new Image();
	base_1.onload = function(){
		for(i = 0; i < map_array.length; i++){
			var x = map_array[i].x - 50;
		    var y = map_array[i].y - 40;
			context.drawImage(base_1,x,y,100,80);
		}
	}
	base_1.src = 'images/tree.png';
}

function drawBgColor(i,map_array,map_info,context,strokeColor,fillColor){
	var x = map_array[i].x;
	var y = map_array[i].y;
	var length = map_info[2];
	
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
	context.beginPath();
	context.moveTo(p1_x,p1_y);
	context.lineTo(p2_x,p2_y);
	context.lineTo(p3_x,p3_y);
	context.lineTo(p4_x,p4_y);
	context.lineTo(p5_x,p5_y);
	context.lineTo(p6_x,p6_y);
	context.lineTo(p1_x,p1_y);
	context.stroke();
	context.fill();
	context.closePath();
}