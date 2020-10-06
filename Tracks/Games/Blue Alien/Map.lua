Map = Class{}


-- numbering images of sprite sheet
-- empty / brick tiles
TILE_BRICK = 1
TILE_EMPTY = 4
-- cloud tiles
CLOUD_LEFT = 6
CLOUD_RIGHT = 7
-- bush tiles
BUSH_LEFT = 2
BUSH_RIGHT = 3
-- pillar tiles
PILLAR_TOP = 10
PILLAR_BOTTOM = 11
-- Jump block(s)
JUMP_BLOCK = 5
JUMP_HIT = 9
-- Flag Pole
POLE_TOP = 8
POLE_MID = 12
POLE_BOTTOM = 16
-- Flag(s)
FLAG_1 = 13
FLAG_2 = 14
FLAG_3 = 15

local SCROLL_SPEED = 62



-- items in map
function Map:init()

    -- dimensions to work with
    self.spritesheet = love.graphics.newImage('graphics/spritesheet.png')
     -- function in Utils to break up image pixels
    self.sprites = generateQuads(self.spritesheet, 16, 16)
    -- audio source
    self.music = love.audio.newSource('sounds/music.wav', 'static')
    self.powerup = love.audio.newSource('sounds/powerup-reveal.wav', 'static')
    -- setting font sizes
    smallfont = love.graphics.newFont('fonts/font.ttf', 8)
    victoryfont = love.graphics.newFont('fonts/font.ttf', 24)

    self.tileWidth = 16
    self.tileHeight = 16
    self.mapWidth = 30 
    self.mapHeight = 28

    -- for list of tiles
    self.tiles = {}

    -- player
    self.player = Player(self)

    -- for visual movement
    self.camX = 0
    self.camY = -3

    self.mapWidthPixels = self.mapWidth * self.tileWidth
    self.mapHeightPixels = self.mapHeight * self.tileHeight

    -- filling with empty tiles
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            self:setTile(x, y, TILE_EMPTY)
        end
    end

    -- variable to randomize image layout
    local x = 1
    -- while loop to go through screen and creat random layout
    while x < self.mapWidth do

        -- flag pole
        if x == self.mapWidth - 2 then
            self:setTile(x, self.mapHeight / 2 - 6, POLE_TOP)
            self:setTile(x, self.mapHeight / 2 - 5, POLE_MID)
            self:setTile(x, self.mapHeight / 2 - 4, POLE_MID)
            self:setTile(x, self.mapHeight / 2 - 3, POLE_MID)
            self:setTile(x, self.mapHeight / 2 - 2, POLE_MID)
            self:setTile(x, self.mapHeight / 2 - 1, POLE_BOTTOM)

            -- column bricks
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            x = x + 1
            if math.random(10) == 1 then
                self:setTile(x, self.mapHeight / 2 - 6, FLAG_1)
            elseif math.random(15) == 1 then
                self:setTile(x, self.mapHeight / 2 - 6, FLAG_3)
            else
                self:setTile(x, self.mapHeight / 2 - 6, FLAG_2)
            end
        end

        -- brick pyramid
        if x == self.mapWidth - 8 then
            local z = x
            local h = self.mapHeight / 2 - 1

            while z < x + 4 and h > self.mapHeight / 2 - 5 do
                self:setTile(x, h, TILE_BRICK)
                for y = h, self.mapHeight do
                    self:setTile(x, y, TILE_BRICK)
                end
                x = x + 1
                h = h - 1
            end
        end

        -- 2 % cloud (2 from edges)
        if x < self.mapWidth - 2 then
            if math.random(20) == 1 then
                -- choose random spot above floor objects
                local cloudstart = math.random(self.mapHeight / 2 - 6)
                -- setting cloud left / right
                self:setTile(x, cloudstart, CLOUD_LEFT)
                self:setTile(x + 1, cloudstart, CLOUD_RIGHT)
            end
        end

        -- 5 % pillar
        if math.random(20) == 1 then
            -- pillar blocks top / bottom
            self:setTile(x, self.mapHeight / 2 - 2, PILLAR_TOP)
            self:setTile(x, self.mapHeight / 2 - 1, PILLAR_BOTTOM)
            -- setting bricks under pillar
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end

            -- set x +1 (next column)
            x = x + 1
    
        -- 10 % bush
        elseif math.random(10) == 1 and x < self.mapWidth - 3 then
            local bushlevel = self.mapHeight / 2 -1
            -- place bush tile left then column bricks
            self:setTile(x, bushlevel, BUSH_LEFT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            -- set x + 1 (next column)
            x = x + 1
            -- place bush tile right then column bricks
            self:setTile(x, bushlevel, BUSH_RIGHT)
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            -- set x + 1 (next column)
            x = x + 1

        -- 10 % gap
        elseif math.random(10) ~= 1 then

            -- column bricks
            for y = self.mapHeight / 2, self.mapHeight do
                self:setTile(x, y, TILE_BRICK)
            end
            -- 15 % jump bock
            if math.random(15) == 1 then
                self:setTile(x, self.mapHeight / 2 - 4, JUMP_BLOCK)
            end
            -- set x + 1 (next column)
            x = x + 1
        else
            -- set x + 2 (to make 2 tile gap)
            x = x + 2
        end
    end

    -- background music
    self.music:setLooping(true)
    self.music:setVolume(0.20)
    self.music:play()

    gameState = 'play' 
end



-- return whether a given tile is collidable
function Map:collides(tile)
    -- define our collidable tiles
    local collidables = {
        TILE_BRICK, JUMP_BLOCK, JUMP_HIT,
        PILLAR_TOP, PILLAR_BOTTOM, POLE_TOP,
        POLE_MID, POLE_BOTTOM
    }

    -- iterate and return true if our tile type matches
    for _, v in ipairs(collidables) do
        if tile.id == v then
            if tile.id == POLE_MID then
                gameState = 'Victory'
                self.powerup:play()
            elseif tile.id == POLE_TOP then
                gameState = 'Victory'
                self.powerup:play()
            elseif tile.id == POLE_BOTTOM then
                gameState = 'Victory'
                self.powerup:play()
            end
            return true
        end
    end
    -- else
    return false
end



-- movement of image
function Map:update(dt)
    -- updating player
    self.player:update(dt)
    -- make camera follow player
    self.camX = math.max(0, math.min(self.player.x - VIRTUAL_WIDTH / 2,
        math.min(self.mapWidthPixels - VIRTUAL_WIDTH, self.player.x)))
end



-- setting tile at location
function Map:setTile(x, y, tile)
    self.tiles[(y - 1) * self.mapWidth + x] = tile
end



-- checks tile type at position
function Map:tileAt(x, y)
    return {
        x = math.floor(x / self.tileWidth) + 1,
        y = math.floor(y / self.tileHeight) + 1,
       id = self:getTile(math.floor(x / self.tileWidth) + 1, math.floor(y / self.tileHeight) + 1)
    }
end


-- getting tile for position
function Map:getTile(x, y)
    return self.tiles[(y - 1) * self.mapWidth + x]
end



-- creating image of map
function Map:render()
    for y = 1, self.mapHeight do
        for x = 1, self.mapWidth do
            local tile = self:getTile(x, y)
            if tile ~= TILE_EMPTY then
                love.graphics.draw(self.spritesheet, self.sprites[tile],
                    (x - 1) * self.tileWidth, (y - 1) * self.tileHeight)
            end
        end
    end

    displayVictory()

    self.player:render()
 end


 function displayVictory()
    if gameState == 'Victory' then
        love.graphics.setColor(1, 1, 1, 1)
        love.graphics.setFont(victoryfont)
        love.graphics.printf("VICTORY!", 0, 20, VIRTUAL_WIDTH, 'center')
        love.graphics.setFont(smallfont)
        love.graphics.printf("Press enter to play again",  0, 46, VIRTUAL_WIDTH, 'center')
    end
end