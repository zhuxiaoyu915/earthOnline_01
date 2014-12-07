		var context = null; // global variable 2d context
		var started = false;
		var mText_canvas = null;
		var x = 0, y =0;
		window.add
		window.onload = function() {
			localStorage.string_1 = "名字;n;#FF0000/sdkfjskdf/sdf/sd;sdf/dfsdf";
			var array = [];
			var string = localStorage.string_1;
			var array_1 = string.split(";");
			for(i in array_1){
				array_2 = array_1[i].split("/");
				array.push(array_2);
			}
			console.log(array);
			var array_4 = [];
			console.log(array_1[2]);
			
			for(i in array_1){
				array_3 = array[i].join("/");
				array_4.push(array_3);
			}
			var string_2 = array_4.join(";");
			console.log(string_2);
		}