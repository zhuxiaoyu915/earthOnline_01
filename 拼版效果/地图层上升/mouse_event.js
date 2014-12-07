function getPointOnCanvas(canvas, x, y) {
			var bbox = canvas.getBoundingClientRect();
			return { x: x - bbox.left * (canvas.width  / bbox.width),
					y: y - bbox.top  * (canvas.height / bbox.height)
					};
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