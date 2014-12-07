function getPointOnCanvas(canvas, x, y) {
			var bbox = canvas.getBoundingClientRect();
			return { x: x - bbox.left * (canvas.width  / bbox.width),
					y: y - bbox.top  * (canvas.height / bbox.height)
					};
		}
		
function doMouseDown(event) {
	var x = event.pageX;
	var y = event.pageY;
	var canvas = document.getElementById("base_canvas");
	var loc = getPointOnCanvas(canvas, x, y);
	console.log("mouse down at point( x:" + loc.x + ", y:" + loc.y + ")");
	draw_character(1,loc);
	//context.moveTo(loc.x, loc.y);
	started = true;
}
		
function doMouseMove(event) {
	var x = event.pageX;
	var y = event.pageY;
	var canvas = document.getElementById("event_canvas");
	var context = canvas.getContext("2d");
	var loc = getPointOnCanvas(canvas, x, y);
	if (started) {
		context.clearRect(0,0,1000,500);
		draw_character(1,loc);
	}
}
		
function doMouseUp(event) {
	console.log("mouse up now");
    if (started) {
    	doMouseMove(event);
        started = false;
	}
}