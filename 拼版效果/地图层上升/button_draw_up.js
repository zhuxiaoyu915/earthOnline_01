function draw_up(){

	var map_info = localStorage.map_info.split(",");
	var map_array = built_base_map_array(map_info[0],map_info[1],map_info[2],map_info[3],map_info[4]);
	
	var n = 4;
	//底图重绘
	var canvas = document.getElementById("base_canvas");
	context = canvas.getContext("2d");

	var strokeColor = "#99bbbb";
	var fillColor = "#99bbbb";
	drawBgColor(n,map_array,map_info,context,strokeColor,fillColor);
	
	//顶层图片设置间隙重绘
	var event_canvas = document.getElementById("event_canvas");
	event_context = event_canvas.getContext("2d");
	event_context.fillStyle ="#ecf0f1";
	event_context.strokeStyle ="#ecf0f1";
	var count = 0;
	var x = map_array[n].x;
	var y = map_array[n].y;
	var length = map_info[2];
	var a = 1;
	var b = 1;
	var base_1 = new Image();
	base_1.onload = function(){
			//context.drawImage(base_1,x,y,100,80);
	}
	base_1.src = 'images/tree.png';
	
	function rise_up(){
		if(count < 25){
		    event_context.clearRect(0,0,1000,500);
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

			
			
			event_context.beginPath();
			event_context.shadowOffsetX = a;
			event_context.shadowOffsetY = b;
			event_context.shadowBlur = 4;
			event_context.shadowColor = 'rgba(0,0,0,0.5)';
			event_context.lineWidth = 4;
			event_context.fillStyle = "#ecf0f1";
			event_context.strokeStyle = "#ecf0f1";

			event_context.moveTo(p1_x,p1_y);
			event_context.lineTo(p2_x,p2_y);
			event_context.lineTo(p3_x,p3_y);
			event_context.lineTo(p4_x,p4_y);
			event_context.lineTo(p5_x,p5_y);
			event_context.lineTo(p6_x,p6_y);
			event_context.lineTo(p1_x,p1_y);
			//event_context.stroke();
			event_context.fill();
			event_context.closePath();
			
			event_context.shadowOffsetX = 0;
			event_context.shadowOffsetY = 0;
			event_context.shadowBlur = 0;
			event_context.shadowColor = 'rgba(0,0,0,0.5)';
			event_context.drawImage(base_1,x-50,y-40,100,80);
			
			x = x-1;
			y = y-1;
			a = a+1.5;
			b = b+1.5;
			count ++;
		}else{
		window.clearInterval(intervalID);
		}
	}
	var intervalID = window.setInterval(rise_up,40);
	
	
	
	//event_context.fillRect(100,100,200,200);
}