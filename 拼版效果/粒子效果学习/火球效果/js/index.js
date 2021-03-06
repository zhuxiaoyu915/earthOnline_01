bgColor = '#000';
gravity = 0.01;
particleColor = '#f73';

canvas = document.querySelector('canvas');
ctx = canvas.getContext('2d');

(onresize = function(){//当调整窗口大小时候调用该函数
  width = canvas.width = canvas.clientWidth;
  height = canvas.height = canvas.clientHeight;
  //Math.floor为求一个浮点数的整数部分
  //onresize的目的是自动转换坐标系？改成屏幕中心为坐标原点
  o = {x:Math.floor(width/2),y:Math.floor(height/2)};
  edge = {top:-o.y,right:width-o.x,bottom:height-o.y,left:-o.x}
})();

particles = {};
newParticle = (function(){
  var nextIndex = 0;
  return function(x,y,r,o,c,xv,yv,rv,ov){
    particles[++nextIndex] = {
      index: nextIndex,
      x: x,
      y: y,
      r: r,
      o: o,
      c: c,
      xv: xv,
      yv: yv,
      rv: rv,
      ov: ov
    };
  };
})();

fireballs = {};
newFireball = (function(){
  var nextIndex = 0;
  return function(x,y,xv,yv,life){
    fireballs[++nextIndex] = {
      index: nextIndex,
      x: x,
      y: y,
      xv: xv,
      yv: yv,
      life: life
    };
  };
})();

mouse = {x:0,y:0,d:0};
onmousemove = function(e){
  mouse.x = e.clientX-o.x;
  mouse.y = e.clientY-o.y;
  var dx = mouse.x - pos1.x,
      dy = mouse.y - pos1.y;
  mouse.d = Math.sqrt(dx*dx+dy*dy);
  //console.log("mouse.d为：" + mouse.d);
};

charging = false;
pos1 = {x:0,y:0};
showInstructions = true;//展示示例
onmousedown = function(e){
  pos1.x = mouse.x;
  pos1.y = mouse.y;
  charging = true;
};

onmouseup = function(){
  if(charging){
    newFireball(
      mouse.x,
      mouse.y,
      (pos1.x-mouse.x)*0.03,
      (pos1.y-mouse.y)*0.03,
      60
    );
    charging = false;
  }
  frame_num = 15;//帧数为15
  frame_time = 30;//每帧时间为30ms
  var intervalID = window.setInterval(draw_begin , frame_time);
};

function draw_begin(){
	var count = 0;
	if(count < frame_num){
		requestAnimationFrame();
		count++;
	}else{
		window.clearInterval(intervalID);
	}
}

time = 0;
function requestAnimationFrame(){
	ctx.setTransform(1,0,0,1,0,0);
  ctx.globalCompositeOperation = 'source-over';//图像覆盖方式：新图像会覆盖旧图片
  ctx.globalAlpha = 1;//透明度为不透明
  ctx.fillStyle = bgColor;
  ctx.fillRect(0,0,width,height);
  //重设屏幕中心点的坐标
  ctx.translate(o.x,o.y);

  ctx.globalCompositeOperation = 'lighter';
  
  //粒子绘制
  for(var i in particles){
    var p = particles[i];
    ctx.beginPath();
    ctx.arc(p.x,p.y,p.r,0,Math.PI*2);
    ctx.globalAlpha = p.o;
    ctx.fillStyle = p.c;
    ctx.fill();
  }
  
  //粒子物理属性（位置、半径）的实时更新
  for(var i in particles){
    var p = particles[i];
    p.x += p.xv;
    p.y += p.yv;
    p.r += p.rv;
    p.o += p.ov;
    if(p.r<0)delete particles[p.index];
    if(p.o<0)delete particles[p.index];
  }
  
  for(var i in fireballs){
    f = fireballs[i];
    var numParticles = Math.sqrt(f.xv*f.xv+f.yv*f.yv)/5;//粒子数目: 与速度有关，速度越快，粒子数越多，成正比
    if(numParticles<1)numParticles=1;
    var numParticlesInt = Math.ceil(numParticles),//Math.ceil(向上取整数）
        numParticlesDif = numParticles/numParticlesInt;
	console.log("begin");
	console.log("numParticles:" + numParticles);
	console.log("numParticlesInt:" + numParticlesInt);
	console.log("numParticlesDif:" + numParticlesDif);
	console.log("end");
    for(var j=0;j<numParticlesInt;j++){
      newParticle(
        f.x-f.xv*j/numParticlesInt,//从f.x 到f.x - f.xv之间均匀分布
        f.y-f.yv*j/numParticlesInt,//从f.y 到f.y - f.yv之间均匀分布
        7,
        numParticlesDif,
        particleColor,
        Math.random()*0.6-0.3,
        Math.random()*0.6-0.3,
        -0.3,
        -0.05*numParticlesDif
      );
    }
    f.x += f.xv;
    f.y += f.yv;
    f.yv += gravity;

    if(--f.life<0)delete fireballs[f.index];
  }
}
