bgColor = '#000';
gravity = 0.1;
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
};

charging = false;
pos1 = {x:0,y:0};
showInstructions = true;//展示示例
onmousedown = function(e){
  pos1.x = mouse.x;
  pos1.y = mouse.y;
  charging = true;
  showInstructions = false;
  console.log("x坐标为：" + x);
  console.log("y坐标为：" + y);
};

onmouseup = function(){
  if(charging){
    newFireball(
      mouse.x,
      mouse.y,
      (pos1.x-mouse.x)*0.03,
      (pos1.y-mouse.y)*0.03,
      600
    );
    charging = false;
  }
};

time = 0;
requestAnimationFrame(loop = function(){
  ctx.setTransform(1,0,0,1,0,0);
  ctx.globalCompositeOperation = 'source-over';//图像覆盖方式：新图像会覆盖旧图片
  ctx.globalAlpha = 1;//透明度为不透明
  ctx.fillStyle = bgColor;
  ctx.fillRect(0,0,width,height);
  //重设屏幕中心点的坐标
  ctx.translate(o.x,o.y);
  
  if(charging){
    var c = Math.floor(30+mouse.d/2);
	console.log("C的值为：" + c);

    ctx.strokeStyle = 'rgba('+c+','+c+','+c+',1)';
	console.log("strokeStyle的值为：" + ctx.strokeStyle);
    ctx.lineWidth = 4;
    ctx.beginPath();
    ctx.moveTo(pos1.x,pos1.y);
    ctx.lineTo(mouse.x,mouse.y);
    ctx.lineCap = 'round';
    ctx.stroke();
  }

  ctx.globalCompositeOperation = 'lighter';
  for(var i in particles){
    var p = particles[i];
    ctx.beginPath();
    ctx.arc(p.x,p.y,p.r,0,Math.PI*2);//实际上只是画圆
    ctx.globalAlpha = p.o;
    ctx.fillStyle = p.c;
    ctx.fill();
  }
  
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
    var numParticles = Math.sqrt(f.xv*f.xv+f.yv*f.yv)/5;//粒子数目与速度有关，速度越快，粒子数越多，成正比
    if(numParticles<1)numParticles=1;
    var numParticlesInt = Math.ceil(numParticles),//Math.ceil(向上取整数）
        numParticlesDif = numParticles/numParticlesInt;
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
    var boundary;
    if(f.y<(boundary = edge.top+7)){
      f.y = boundary;
      f.yv *= -1;
    }else if(f.y>(boundary = edge.bottom-7)){
      f.y = boundary;
      f.yv *= -1;
    }
    if(f.x>(boundary = edge.right-7)){
      f.x = boundary;
      f.xv *= -1;
    }else if(f.x<(boundary = edge.left+7)){
      f.x = boundary;
      f.xv *= -1;
    }
    if(--f.life<0)delete fireballs[f.index];
  }
  
  requestAnimationFrame(loop);
});