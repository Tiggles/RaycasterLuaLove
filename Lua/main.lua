worldMap =
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}

function love.load()
    posX = 22.0;
    posY = 12.0;
    dirX = -1.0;
    dirY = 0.0;
    planeX = 0.0;
    planeY = 0.66;
    height = 540
    width = 960
    love.window.setMode(width, height)
end

function love.update(dt)
    fps = love.timer.getFPS()
    moveSpeed = dt * 5;
    rotSpeed = dt * 3;
    
    up = love.keyboard.isDown("w")
    left = love.keyboard.isDown("a")
    right = love.keyboard.isDown("d")
    down = love.keyboard.isDown("s")
    esc = love.keyboard.isDown("escape")

    if up then
        if worldMap[math.floor(posX + dirX * moveSpeed) + 1][math.floor(posY) + 1] == 0 then posX = posX + dirX * moveSpeed end
        if worldMap[math.floor(posX) + 1][math.floor(posY + dirY * moveSpeed) + 1] == 0 then posY = posY + dirY * moveSpeed end
    end

    if down then
       if worldMap[math.floor(posX - dirX * moveSpeed) + 1][math.floor(posY) + 1] == 0 then posX = posX - dirX * moveSpeed end
       if worldMap[math.floor(posX) + 1][math.floor(posY - dirY * moveSpeed) + 1] == 0 then posY = posY - dirY * moveSpeed end
    end

    if right then
      oldDirX = dirX
      dirX = dirX * math.cos(-rotSpeed) - dirY * math.sin(-rotSpeed)
      dirY = oldDirX * math.sin(-rotSpeed) + dirY * math.cos(-rotSpeed)
      oldPlaneX = planeX
      planeX = planeX * math.cos(-rotSpeed) - planeY * math.sin(-rotSpeed)
      planeY = oldPlaneX * math.sin(-rotSpeed) + planeY * math.cos(-rotSpeed)
    end

    if left then
      oldDirX = dirX
      dirX = dirX * math.cos(rotSpeed) - dirY * math.sin(rotSpeed)
      dirY = oldDirX * math.sin(rotSpeed) + dirY * math.cos(rotSpeed)
      oldPlaneX = planeX
      planeX = planeX * math.cos(rotSpeed) - planeY * math.sin(rotSpeed)
      planeY = oldPlaneX * math.sin(rotSpeed) + planeY * math.cos(rotSpeed)
    end

    if esc then
      love.event.quit()
    end
end

function love.draw()
    love.graphics.setColor(102 /255, 123 /255, 123/255)
    love.graphics.rectangle( "fill", 0, 0, width, height / 2 )
    love.graphics.setColor(156/255, 123/255, 255/255)
    love.graphics.rectangle( "fill", 0, height / 2, width, height / 2)
    for x = 0, width do
        cameraX = 2 * x / width - 1;
        rayPosX = posX;
        rayPosY = posY;
        rayDirX = dirX + planeX * cameraX
        rayDirY = dirY + planeY * cameraX

        mapX = math.floor(rayPosX)
        mapY = math.floor(rayPosY)

        deltaDistX = math.sqrt(1.0 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
        deltaDistY = math.sqrt(1.0 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

        hit = false;

        if rayDirX < 0 then
            stepX = -1;
            sideDistX = (rayPosX - mapX) * deltaDistX
        else
            stepX = 1;
            sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX
        end
        if rayDirY < 0 then
            stepY = -1;
            sideDistY = (rayPosY - mapY) * deltaDistY;
        else
            stepY = 1;
            sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
        end
        while hit == false do
            if sideDistX < sideDistY then
                sideDistX = sideDistX + deltaDistX;
                mapX = mapX + stepX;
                side = 0;
            else
                sideDistY = sideDistY + deltaDistY
                mapY = mapY + stepY;
                side = 1;
            end
            if worldMap[mapX + 1][mapY + 1] > 0 then hit = true end
        end
        if side == 0 then perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
        else perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY end
        
        lineHeight = math.floor(height / perpWallDist)

        drawStart = -lineHeight / 2  + height / 2;
        if drawStart < 0 then drawStart = 0 end
        drawEnd = lineHeight / 2 + height / 2;
        if drawEnd >= height then drawEnd = height - 1; end
        
        local mapValue = worldMap[mapX + 1][mapY + 1];
        if side == 1 then mapValue = math.floor(mapValue / 2) end
        if mapValue == 0 then
            love.graphics.setColor( 255/255, 255/255, 0 )
        elseif mapValue == 1 then
            love.graphics.setColor( 255/255, 0, 0)
        elseif mapValue == 2 then
            love.graphics.setColor( 0, 255/255, 0)
        elseif mapValue == 3 then
            love.graphics.setColor( 0, 0, 255/255)
        else
            love.graphics.setColor(255/255, 255/255, 255/255)
        end                
        love.graphics.line(x, drawStart, x, drawEnd)
    end
end
