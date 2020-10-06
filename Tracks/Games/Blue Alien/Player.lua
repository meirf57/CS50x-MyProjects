Player = Class{}

-- Animation
require 'Animation'
-- movement speed
local MOVE_SPEED = 100
local JUMP_VELOCITY = 300
local GRAVITY = 15

-- dimensions of player
function Player:init(map)

    --size
    self.x = 0
    self.y = 0
    self.width = 16
    self.height = 20

    -- to check position in map
    self.map = map
    self.gameState = gameState

    -- offset from top left to center to support sprite flipping
    self.xOffset = 8
    self.yOffset = 10

    -- texture / frame data
    self.texture = love.graphics.newImage('graphics/blue_alien.png')

    -- sound effects
    self.sounds = {
        ['jump'] = love.audio.newSource('sounds/jump.wav', 'static'),
        ['hit'] = love.audio.newSource('sounds/hit.wav', 'static'),
        ['coin'] = love.audio.newSource('sounds/coin.wav', 'static'),
        ['death'] = love.audio.newSource('sounds/death.wav', 'static')
    }
    
    -- animation frames
    self.frames = {}

    -- current animation frame
    self.currentFrame = nil

    --Animation state default
    self.state = 'idle'
    self.direction = 'right'

    -- x / y velocity
    self.dx = 0
    self.dy = 0

    -- position on top of tiles
    self.x = map.tileWidth * 10
    self.y = map.tileHeight * ((map.mapHeight - 2) / 2) - self.height
    
    -- Animation params
    self.animations = {
        ['idle'] = Animation {
            texture = self.texture,
            frames = {
                love.graphics.newQuad(0, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 1
        },
        ['walk'] = Animation {
            texture = self.texture,
            frames = {
                love.graphics.newQuad(128, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(144, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(160, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(144, 0, 16, 20, self.texture:getDimensions()),
            },
            interval = 0.15
        },
        ['crouch'] = Animation {
            texture = self.texture,
            frames = {
                love.graphics.newQuad(48, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 1
        },
        ['jump'] = Animation {
            texture = self.texture,
            frames = {
                love.graphics.newQuad(32, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 1
        },
        ['land'] = Animation { --*
            texture = self.texture,
            frames = {
                love.graphics.newQuad(64, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(48, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 0.15
        },
    } 

    -- default animation standing idle
    self.animation = self.animations['idle']
    self.currentFrame = self.animation:getCurrentFrame()

    -- animation state functions
    self.behaviors = {
        ['idle'] = function(dt)
            if love.keyboard.wasPressed('up') then
                self.dy = -JUMP_VELOCITY
                self.state = 'jump'
                self.animation = self.animations['jump']
                self.sounds['jump']:play()
            -- left
            elseif love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -MOVE_SPEED
                self.state = 'walk'
                self.animations['walk']:restart()
                self.animation = self.animations['walk']
            -- right
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = MOVE_SPEED
                self.state = 'walk'
                self.animations['walk']:restart()
                self.animation = self.animations['walk']
            -- down (static crouch)
            elseif love.keyboard.isDown('down') then
                self.dx = 0
                self.state = 'crouch'
                self.animation = self.animations['crouch']
            else
                self.dx = 0
            end
        end,

        ['walk'] = function(dt)
            if love.keyboard.wasPressed('up') then
                self.dy = -JUMP_VELOCITY
                self.state = 'jump'
                self.animation = self.animations['jump']
                self.sounds['jump']:play()
            -- left
            elseif love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -MOVE_SPEED
            -- right
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = MOVE_SPEED
            -- down (if walk and crouch == slide)
            elseif love.keyboard.isDown('down') then
                self.dx = 0
                self.state = 'crouch'
                self.animation = self.animations['crouch']
            else
                self.dx = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
            end

            -- check for collisions moving left and right
            self:checkRightCollision()
            self:checkLeftCollision()
        
            if not self.map:collides(self.map:tileAt(self.x, self.y + self.height)) and
                not self.map:collides(self.map:tileAt(self.x + self.width - 1, self.y + self.height)) then
                
                -- if so, reset velocity and position and change state
                self.state = 'jump'
                self.animation = self.animations['jump']
                if self.y  > self.map.mapHeight / 2 then
                    self.sounds['death']:play()
                end
            end
        end,

        ['crouch'] = function(dt)
            if love.keyboard.wasPressed('up') then
                self.dy = -JUMP_VELOCITY
                self.state = 'jump'
                self.animation = self.animations['jump']
                self.sounds['jump']:play()
            -- left
            elseif love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -MOVE_SPEED
                self.state = 'walk'
                self.animations['walk']:restart()
                self.animation = self.animations['walk']
            -- right
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = MOVE_SPEED
                self.state = 'walk'
                self.animations['walk']:restart()
                self.animation = self.animations['walk']
            -- down (static crouch)
            elseif love.keyboard.isDown('down') then
                self.dx = 0
                self.state = 'crouch'
                self.animation = self.animations['crouch']
            else
                self.dx = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
            end
        end,

        ['jump'] = function(dt)
            -- break if we go below the surface
            if self.y > 300 then
                return
            end
            -- left
            if love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -MOVE_SPEED
            -- right
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = MOVE_SPEED
            end

            self.dy = self.dy + GRAVITY

            -- check if there's a tile directly beneath us
            if self.map:collides(self.map:tileAt(self.x, self.y + self.height)) or
                self.map:collides(self.map:tileAt(self.x + self.width - 1, self.y + self.height)) then
                
                -- if so, reset velocity and position and change state
                self.dy = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
                self.y = (self.map:tileAt(self.x, self.y + self.height).y - 1) * self.map.tileHeight - self.height
            end

            -- check for collisions moving left and right
            self:checkRightCollision()
            self:checkLeftCollision()
        end
    }
end


-- updating..
function Player:update(dt)
    self.behaviors[self.state](dt)
    self.animation:update(dt)
    self.currentFrame = self.animation:getCurrentFrame()
    self.x = self.x + self.dx * dt

    -- checking if tile block was hit
    if self.dy < 0 then
        if self.map:tileAt(self.x, self.y).id ~= TILE_EMPTY or
            self.map:tileAt(self.x + self.width - 1, self.y).id ~= TILE_EMPTY then
            -- reset up velocity
            self.dy = 0

            -- setting sound effects
            local playCoin = false
            local playHit = false

            -- change right block to hit block
            if self.map:tileAt(self.x, self.y).id == JUMP_BLOCK then
                self.map:setTile(math.floor(self.x / self.map.tileWidth) + 1,
                    math.floor(self.y / self.map.tileHeight) + 1, JUMP_HIT)
                playCoin = true
            else
                playHit = true
            end
            -- change left block to hit block
            if self.map:tileAt(self.x + self.width - 1, self.y).id == JUMP_BLOCK then
                self.map:setTile(math.floor((self.x + self.width - 1) / self.map.tileWidth) + 1,
                    math.floor(self.y / self.map.tileHeight) + 1, JUMP_HIT)
                playCoin = true
            else
                playHit = true
            end

            -- playing sound effects
            if playCoin then
                self.sounds['coin']:play()
            elseif playHit then
                self.sounds['hit']:play()
            end
        end
    end

    --apply velocity
    self.y = self.y + self.dy * dt
end


-- checks two tiles to our left to see if a collision occurred
function Player:checkLeftCollision()
    if self.dx < 0 then
        -- check if there's a tile directly beneath us
        if self.map:collides(self.map:tileAt(self.x - 1, self.y)) or
            self.map:collides(self.map:tileAt(self.x - 1, self.y + self.height - 1)) then
            
            -- if so, reset velocity and position and change state
            self.dx = 0
            self.x = self.map:tileAt(self.x - 1, self.y).x * self.map.tileWidth
        end
    end
end

-- checks two tiles to our right to see if a collision occurred
function Player:checkRightCollision()
    if self.dx > 0 then
        -- check if there's a tile directly beneath us
        if self.map:collides(self.map:tileAt(self.x + self.width, self.y)) or
            self.map:collides(self.map:tileAt(self.x + self.width, self.y + self.height - 1)) then
            
            -- if so, reset velocity and position and change state
            self.dx = 0
            self.x = (self.map:tileAt(self.x + self.width, self.y).x - 1) * self.map.tileWidth - self.width
        end
    end
end


-- drawing avatar
function Player:render()

    local scaleX
    if self.direction == 'right' then
        scaleX = 1
    else
        scaleX = -1
    end

    -- draw sprite with scale factor and offsets
    love.graphics.draw(self.texture, self.currentFrame, math.floor(self.x + self.xOffset),
        math.floor(self.y + self.yOffset), 0, scaleX, 1, self.xOffset, self.yOffset)
end