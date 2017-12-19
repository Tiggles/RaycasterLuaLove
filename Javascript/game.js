"use strict";

var canvas = document.getElementById("gameCanvas");
var ctx = canvas.getContext("2d");

const SCREEN_WIDTH = 800;
const SCREEN_HEIGHT = 600;

var running = true

var posX = 22.0;
var posY = 12.0;
var dirX = -1.0;
var dirY = 0.0;
var planeX = 0.0;
var planeY = 0.66;
var time = 0.0;
var oldTime = 0.0;

const worldMap =
[
  [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1],
  [1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1],
  [1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1],
  [1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]
]

var lastTime = Date.now()

var up = false;
var left = false;
var down = false;
var right = false;

window.addEventListener('keydown', function(event) {
	switch (event.keyCode) {
		case 37:
			left = true;
			console.log("Left");
			break;
		case 38:
			up = true;
			break;
		case 39:
			right = true;
			break;
		case 40:
			down = true;
			break;

	}
}, false);

window.addEventListener('keyup', function(event) {
	switch (event.keyCode) {
		case 37:
			left = false;
			break;
		case 38:
			up = false;
			break;
		case 39:
			right = false;
			break;
		case 40:
			down = false;
			break;

	}
}, false);


var deltaTime = 0

function update() {
	deltaTime = Date.now() - lastTime
	lastTime = deltaTime
	var moveSpeed = 5
	var rotSpeed = 3



	if (up) {
		if (worldMap[Math.floor(posX + dirX * moveSpeed)][Math.floor(posY)] == 0) { posX = posX + dirX * moveSpeed }
		if (worldMap[Math.floor(posX)][Math.floor(posY + dirY * moveSpeed)] == 0) { posY = posY + dirY * moveSpeed }
	}

	if (down) {
		if (worldMap[Math.floor(posX - dirX * moveSpeed)][Math.floor(posY)] == 0) { posX = posX - dirX * moveSpeed }
		if (worldMap[Math.floor(posX)][Math.floor(posY - dirY * moveSpeed)] == 0) { posY = posY - dirY * moveSpeed }
	}

  var oldDirX = 0
  var oldPlaneX = 0

	if (right) {
		oldDirX = dirX
      	dirX = dirX * Math.cos(-rotSpeed) - dirY * Math.sin(-rotSpeed)
      	dirY = oldDirX * Math.sin(-rotSpeed) + dirY * Math.cos(-rotSpeed)
      	oldPlaneX = planeX
      	planeX = planeX * Math.cos(-rotSpeed) - planeY * Math.sin(-rotSpeed)
      	planeY = oldPlaneX * Math.sin(-rotSpeed) + planeY * Math.cos(-rotSpeed)	
	}

	if (left) {
      oldDirX = dirX
      dirX = dirX * Math.cos(rotSpeed) - dirY * Math.sin(rotSpeed)
      dirY = oldDirX * Math.sin(rotSpeed) + dirY * Math.cos(rotSpeed)
      oldPlaneX = planeX
      planeX = planeX * Math.cos(rotSpeed) - planeY * Math.sin(rotSpeed)
      planeY = oldPlaneX * Math.sin(rotSpeed) + planeY * Math.cos(rotSpeed)
    }
}

function draw() {
	for (var x = 0; x < SCREEN_WIDTH; x++) {
	     var cameraX = 2 * x / SCREEN_WIDTH - 1;
       var rayPosX = posX;
       var rayPosY = posY;
       var rayDirX = dirX + planeX * cameraX
       var rayDirY = dirY + planeY * cameraX
  
        var mapX = Math.floor(rayPosX);
        var mapY = Math.floor(rayPosY);

        var deltaDistX = Math.sqrt(1.0 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        var deltaDistY = Math.sqrt(1.0 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

        var hit = false;

        var stepX = 0
        var stepY = 0
        var sideDistX = 0
        var sideDistY = 0

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        }

        var side = 0

        while (hit == false) {
            if (sideDistX < sideDistY) {
                sideDistX = sideDistX + deltaDistX;
                mapX = mapX + stepX;
                side = 0;
            } else {
                sideDistY = sideDistY + deltaDistY
                mapY = mapY + stepY;
                side = 1;
            }
            if (worldMap[mapX][mapY] > 0) { hit = true }
        }

      var perpWallDist = 0
      var lineHeight = 0
      var drawStart = 0
      var drawEnd = 0

    	if (side == 0) { perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX }
      else { perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY }

        lineHeight = Math.floor(SCREEN_HEIGHT / perpWallDist)

    	drawStart = -lineHeight / 2  + SCREEN_HEIGHT / 2;
		if (drawStart < 0) { drawStart = 0 }
		drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
		if (drawEnd >= SCREEN_HEIGHT) { drawEnd = SCREEN_HEIGHT - 1; }
		var mapValue = worldMap[mapX][mapY];
    // TODO FIXME NOTE I think the error is here:
    if (side == 1) { mapValue = Math.floor(mapValue / 2) }
    if (mapValue == 0) {
        ctx.strokeStyle="#FF0000";
    } else if (mapValue == 1) {
        ctx.strokeStyle="#FFFF00";
    } else if (mapValue == 2) {
        ctx.strokeStyle="#FF00FF";
    } else if (mapValue == 3) {
        ctx.strokeStyle="#00FFFF";
    } else {
        ctx.strokeStyle="#FFFFFF";
    }
		ctx.moveTo(x, drawStart);
		ctx.lineTo(x, drawEnd);
		ctx.stroke(); 
	}
}

var i = 0;

function main() {
	draw()
	update()
  setTimeout(main, 200);
}

main()

var requestAniFrame = window.requestAnimationFrame || 
    window.webkitRequestAnimationFrame || 
    window.mozRequestAnimationFrame    || 
    window.oRequestAnimationFrame      || 
    window.msRequestAnimationFrame     || 
    function(callback) {
      window.setTimeout(callback, 1000 / BOXED_GAME.constants.FPS_LIMIT);
    };
