		var context = null; // global variable 2d context
		var started = false;
		var mText_canvas = null;
		var x = 0, y =0;
		window.add
		window.onload = function() {
			var canvas = document.getElementById("event_canvas");
			console.log(canvas.parentNode.clientWidth);
			canvas.width = canvas.parentNode.clientWidth;
			canvas.height = canvas.parentNode.clientHeight;
			
			if (!canvas.getContext) {
			    console.log("Canvas not supported. Please install a HTML5 compatible browser.");
			    return;
			}
			
			// get 2D context of canvas and draw rectangel
			context = canvas.getContext("2d");
			context.fillStyle="blue";
			context.fillRect(100,10,50,50);
			context.fillRect(300,10,50,50);
			//x = canvas.width/2;
			//y = canvas.height/2;
	
	      
	        // mouse event
	        canvas.addEventListener("mousedown", doMouseDown, false);
	        canvas.addEventListener('mousemove', doMouseMove, false);
	        canvas.addEventListener('mouseup',   doMouseUp, false);
		}
		
		function getPointOnCanvas(canvas, x, y) {
			var bbox = canvas.getBoundingClientRect();
			return { x: x - bbox.left * (canvas.width  / bbox.width),
					y: y - bbox.top  * (canvas.height / bbox.height)
					};
		}
		
		
		function clearCanvas() {
			context.clearRect(0, 0, 500, 500)
		}
		
		function doMouseDown(event) {
			var x = event.pageX;
			var y = event.pageY;
			var canvas = event.target;
			var loc = getPointOnCanvas(canvas, x, y);
			console.log("mouse down at point( x:" + loc.x + ", y:" + loc.y + ")");
			context.beginPath();
			context.clearRect(0,0,canvas.width,canvas.height);
			context.fillRect(300,10,50,50);
			context.closePath();
			context.beginPath();
			context.fillStyle = "#0000F1";
			context.fillRect(loc.x-25,loc.y - 25,50,50);
			//context.moveTo(loc.x, loc.y);
			started = true;
		}
		
		function doMouseMove(event) {
			var x = event.pageX;
			var y = event.pageY;
			var canvas = event.target;
			var loc = getPointOnCanvas(canvas, x, y);
			if (started) {
				//context.lineTo(loc.x, loc.y);
				//context.stroke();
				context.clearRect(0,0,canvas.width,canvas.height);
				context.fillRect(300,10,50,50);
				context.fillRect(loc.x-25,loc.y - 25,50,50);
			}
		}
		
		function doMouseUp(event) {
			console.log("mouse up now");
		    if (started) {
		    	doMouseMove(event);
		        started = false;
			}
		}