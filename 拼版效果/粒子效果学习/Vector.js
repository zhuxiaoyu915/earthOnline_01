function Vector(xx, yy)
{
	this.x = xx||0;
	this.y = yy||0;
}

//閲嶇疆
Vector.prototype.reset = function(xx, yy)
{
	this.x=xx;
	this.y=yy;
}
//	----------------getClone----------------------------------------------
//	澶嶅埗
Vector.prototype.getClone = function()
{
	return new Vector(this.x,this.y);
}
// ---------------------cut---------------------------------------------
//鎴柇
Vector.prototype.cut = function(max)
{
	var r = Math.min(max, this.getLength());
	this.setLength(r);	
}
// ---------------------cutNew---------------------------------------------
//鎴柇
Vector.prototype.cutNew = function(max)
{
	var r= Math.min(max, this.getLength());
	var v = this.getClone();
	v.setLength(r);
	return v;	
}

//	----------------equals------------------------------------------------
//	姣旇緝鏄惁鐩哥瓑
Vector.prototype.equals = function(v)
{
	return (this.x==v.x && this.y==v.y);
}
//	----------------plus--------------------------------------------------
//	鍔犳硶,鏀瑰彉褰撳墠瀵硅薄
Vector.prototype.plus = function(v)
{
	this.x += v.x;
	this.y += v.y;	
}
//	----------------plusNew-----------------------------------------------
//	姹傚拰,杩斿洖鏂板璞�
Vector.prototype.plusNew = function(v)
{
	return new Vector(this.x+v.x,this.y+v.y);
}
//	----------------minus-------------------------------------------------
//	鍑忔硶,鏀瑰彉褰撳墠瀵硅薄
Vector.prototype.minus = function(v)
{
	this.x -= v.x;
	this.y -= v.y;
}
//	----------------minusNew----------------------------------------------
//	姹傚樊,杩斿洖鏂板璞�
Vector.prototype.minusNew = function(v)
{
	return new Vector(this.x-v.x,this.y-v.y);
}
//	----------------negate------------------------------------------------
//	姹傞€�,鏀瑰彉褰撳墠瀵硅薄
Vector.prototype.negate = function()
{
	this.x = - this.x;
	this.y = - this.y;
}
//	----------------negateNew---------------------------------------------
//	姹傞€�,杩斿洖鏂板璞�
Vector.prototype.negateNew = function()
{
	return new Vector(-this.x,-this.y);
}
//	----------------scale-------------------------------------------------
//	缂╂斁,鏀瑰彉褰撳墠瀵硅薄
Vector.prototype.scale = function(s)
{
	this.x *= s;
	this.y *= s;
}
//	----------------scaleNew----------------------------------------------
//	缂╂斁,杩斿洖鏂板璞�
Vector.prototype.scaleNew = function(s)
{
	return new Vector(this.x * s, this.y * s);
}
//	----------------getLength---------------------------------------------
//	鑾峰彇鍚戦噺闀垮害
Vector.prototype.getLength = function()
{
	return Math.sqrt(this.x*this.x + this.y*this.y);
}
//	----------------setLength---------------------------------------------	
//	璁剧疆鍚戦噺闀垮害
Vector.prototype.setLength = function(len)
{
	var r = this.getLength();
	if (r) this.scale (len / r);
	else this.x = len;
}
//	----------------getAngle----------------------------------------------
//	鑾峰彇鍚戦噺瑙掑害
Vector.prototype.getAngle = function()
{
	return Math.atan2(this.y, this.x);
}
//	----------------setAngle----------------------------------------------
//	璁剧疆鍚戦噺瑙掑害
Vector.prototype.setAngle = function(ang)
{
	var r = this.getLength();
	this.x = r * Math.cos (ang);
	this.y = r * Math.sin (ang);
}
//	----------------rotate------------------------------------------------
//	鍚戦噺鏃嬭浆锛屾敼鍙樺綋鍓嶅璞�
Vector.prototype.rotate = function()
{  
	var cos, sin;
	var a = arguments;
	if(a.length == 1)
	{
		cos = Math.cos(a[0]);
		sin = Math.sin(a[0]);
	} 
	else
	{
		cos = a[0]
		sin = a[1]
	}
	var rx = this.x * cos - this.y * sin;
	var ry = this.x * sin + this.y * cos;
	this.x = rx;
	this.y = ry;
} 
//	----------------rotateNew---------------------------------------------
//	鍚戦噺鏃嬭浆锛岃繑鍥炴柊瀵硅薄
Vector.prototype.rotateNew = function(ang)
{
	var v=new Vector(this.x,this.y);
	v.rotate(ang);
	return v;
}
//	----------------dot---------------------------------------------------
//	鐐圭Н
Vector.prototype.dot = function(v)
{
	return this.x * v.x + this.y * v.y;
}
//	----------------getNormal---------------------------------------------
//	娉曞悜閲�
Vector.prototype.getNormal = function()
{
	return new Vector(-this.y,this.x);
}
//	----------------isPerpTo----------------------------------------------
//	鍨傜洿楠岃瘉
Vector.prototype.isPerpTo = function(v)
{
	return (this.dot (v) == 0);
}
//	----------------angleBetween------------------------------------------
//	鍚戦噺鐨勫す瑙�
Vector.prototype.angleBetween = function(v)
{
	var dp = this.dot (v); 
	var cosAngle = dp / (this.getLength() * v.getLength());
	return Math.acos (cosAngle); 
}









