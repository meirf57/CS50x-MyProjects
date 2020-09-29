-- CS50 2020, Games/Mario, Colton Ogden

-- Author/Notes: Meir Fainzilber



-- constant values, size of screen, speed of paddles

-- https://github.com/vrld/hump from Matthias Richter
Class = require 'class'

-- push used to make screen as virtual resolution 
-- https://github.com/Ulydev/push
push = require 'push'

-- classes and function in file needed
-- to split images to pixels
require 'Util'
-- to animate object
require 'Animation'
-- generating image
require 'Map'
-- player object
require 'Player'

-- setting up dimensions
WINDOW_HEIGHT = 720
WINDOW_WIDTH = 1280
--
VIRTUAL_WIDTH = 432
VIRTUAL_HEIGHT = 243

-- seeding random 
math.randomseed(os.time())

-- so images are less blurry
love.graphics.setDefaultFilter('nearest', 'nearest')

-- backgroung image/map
map = Map()



-- loading program
function love.load()

    love.window.setTitle('Blue Alien')

    -- so it has a smaller screen like video games
    push:setupScreen(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, {
        fullscreen = false,
        resizable = false,
        vsync = true
    })

    -- table that stores all key's pressed or released
    love.keyboard.keysPressed = {}
    love.keyboard.keysReleased = {}
end


-- global key pressed function
function love.keyboard.wasPressed(key)
    if (love.keyboard.keysPressed[key]) then
        return true
    else
        return false
    end
end

-- global key released function
function love.keyboard.wasReleased(key)
    if (love.keyboard.keysReleased[key]) then
        return true
    else
        return false
    end
end


 -- to exit game 
function love.keypressed(key)
    -- esc to exit game
    if key == 'escape' then
        love.event.quit()
    end

    if key == 'enter' or key == 'return' then
        if gameState == 'Victory' then
            gameState = 'play'
        end
    end

    -- returns true for key pressed
    love.keyboard.keysPressed[key] = true
end


-- called whenever a key is released
function love.keyreleased(key)
    love.keyboard.keysReleased[key] = true
end



-- updating..
function love.update(dt)
    map:update(dt)

    --empty key pressed table
    love.keyboard.keysPressed = {}
    love.keyboard.keysReleased = {}
end



-- Drawing on screen
function love.draw()
    -- push fitting function
    push:apply('start')

    -- setting background color to sky blue
    love.graphics.clear(108 / 255, 140 / 255, 1, 1)
    
    -- displaying map/image
    love.graphics.translate(math.floor(-map.camX + 0.5), math.floor(-map.camY + 0.5))
    map:render()

    push:apply('end')
end