-- CS50 2020, Games/Pong, Colton Ogden

-- Author/Notes: Meir Fainzilber

-- constant values, size of screen, speed of paddles

-- https://github.com/vrld/hump from Matthias Richter
Class = require 'class'

-- push used to make screen as virtual resolution 
-- https://github.com/Ulydev/push
push = require 'push'
--
require 'Ball'
require 'Paddle'
--
WINDOW_HEIGHT = 720
WINDOW_WIDTH = 1280
--
VIRTUAL_WIDTH = 432
VIRTUAL_HEIGHT = 243
--
PADDLE_SPEED = 200

-- loading game with initial data
function love.load()
    -- seeding math...
    math.randomseed(os.time())

    -- Title on screen window
    love.window.setTitle('Pong')

    -- point graphics, not blurry
    love.graphics.setDefaultFilter('nearest', 'nearest')

    -- setting font sizes
    smallfont = love.graphics.newFont('Font.ttf', 8)
    scorefont = love.graphics.newFont('Font.ttf', 32)
    victoryfont = love.graphics.newFont('Font.ttf', 24)

    -- Sounds Table
    sounds = {
        ['paddle_hit'] = love.audio.newSource('paddle_hit.wav', 'static'),
        ['point_score'] = love.audio.newSource('point_score.wav', 'static'),
        ['wall_hit'] = love.audio.newSource('wall_hit.wav', 'static'),
    }

    -- setting initial score
    score1 = 0
    score2 = 0

    -- victor
    winner = 0

    -- using push.lua for sreen fitting
    push:setupScreen(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, {
        fullscreen = false,
        vsync = true,
        resizable = true
    })

    -- setting paddles (position w, position h, size width, size length)
    paddle1 = Paddle(5, 20, 5, 24)
    paddle2 = Paddle(VIRTUAL_WIDTH - 10, VIRTUAL_HEIGHT - 44, 5, 24)
    -- setting ball (pos w, pos h, size)
    ball = Ball(VIRTUAL_WIDTH / 2 - 3, VIRTUAL_HEIGHT / 2, 6, 6)

    -- setting game state
    gameState = 'start'
end

-- resizing
function love.resize(width, height)
    push:resize(width, height)
end

-- movement of paddles / ball
function love.update(dt)

    -- if ball hits paddle 1 bounce back add speed
    if ball:bounce(paddle1) then
        ball.dx = -ball.dx * 1.03
        ball.x = paddle1.x + 5
        -- keep velocity going in the same direction, but randomize it
        if ball.dy < 0 then
            ball.dy = -math.random(10, 150)
        else
            ball.dy = math.random(10, 150)
        end
        sounds['paddle_hit']:play()
    end
    -- if ball hits paddle 2 bounce back add speed
    if ball:bounce(paddle2) then
        ball.dx = -ball.dx * 1.03
        ball.x = paddle2.x - 4
        -- keep velocity going in the same direction, but randomize it
        if ball.dy < 0 then
            ball.dy = -math.random(10, 150)
        else
            ball.dy = math.random(10, 150)
        end
        sounds['paddle_hit']:play()
    end
    -- if ball hits top/bottom bounce back
    if ball.y <= 0 then
        ball.dy = -ball.dy
        ball.y = 0
        sounds['wall_hit']:play()
    elseif ball.y >= VIRTUAL_HEIGHT - 6 then
        ball.dy = -ball.dy
        ball.y = VIRTUAL_HEIGHT -6
        sounds['wall_hit']:play()
    end
    -- if ball goes past width reset ball update score
    if ball.x <= 0 then
        sounds['point_score']:play()
        gameState = 'start'
        score2 = score2 + 1
        if score2 >= 5 then
            gameState = 'victory'
            winner = 2
        end
        ball:reset()
    elseif ball.x >= VIRTUAL_WIDTH then
        sounds['point_score']:play()
        gameState = 'start'
        score1 = score1 + 1
        if score1 >= 5 then
            gameState = 'victory'
            winner = 1
        end
        ball:reset()
    end


    -- movement paddles
    paddle1:update(dt)
    paddle2:update(dt)

    -- player 1
    if love.keyboard.isDown('w') then
        paddle1.dy = -PADDLE_SPEED
    elseif love.keyboard.isDown('s') then
        paddle1.dy = PADDLE_SPEED
    else
        paddle1.dy = 0
    end

    -- AI 1
    if ball.x < VIRTUAL_WIDTH / 3 then
        if paddle1.y > ball.y then
            paddle1.dy = -PADDLE_SPEED / 2
        elseif paddle1.y < ball.y then
            paddle1.dy = PADDLE_SPEED / 2
        end
    end

    -- player 2
    if love.keyboard.isDown('up') then
        paddle2.dy = -PADDLE_SPEED
    elseif love.keyboard.isDown('down') then
        paddle2.dy = PADDLE_SPEED
    else
        paddle2.dy = 0
    end

    --[[ AI 2
    if ball.x > VIRTUAL_WIDTH - (VIRTUAL_WIDTH / 3) then
        if paddle2.y > ball.y then
            paddle2.dy = -PADDLE_SPEED / 2
        elseif paddle2.y < ball.y then
            paddle2.dy = PADDLE_SPEED / 2
        end
    end
    --]]

    -- ball position
    if gameState == 'play' then
        ball:update(dt)
    end
end

-- function of 1 time key press
function love.keypressed(key)
    -- esc to exit game
    if key == 'escape' then
        love.event.quit()
    end
    -- enter to serve ball
    if key == 'enter' or key == 'return' then
        if gameState == 'start' then
            gameState = 'play'
        elseif gameState == 'victory' then
            gameState = 'start'
            score1 = 0
            score2 = 0
        end   
    end
end

-- drawining items on screen
function love.draw()
    -- push func for screen fit
    push:apply('start')

    -- setting background color to grayish/blue (RGB is less than 1)
    love.graphics.clear(60 / 255, 179 / 255, 113 / 255, 1)

    -- ball
    ball:render()
    -- paddles
    love.graphics.setColor(1, 0, 0, 1)
    paddle1:render()
    love.graphics.setColor(0, 0, 1, 1)
    paddle2:render()
    love.graphics.setColor(1, 1, 1, 1)

    displayfield()
    
    -- words at top
    displaytopwords()
    -- scores
    displayscores()
    -- FPS
    displayFPS()

    push:apply('end')
end

--FPS
function displayFPS()
    love.graphics.setColor(85 / 255, 107 / 255, 47 / 255, 0.5)
    love.graphics.setFont(smallfont)
    love.graphics.print('FPS: ' .. tostring(love.timer.getFPS()), 40, 20)
    love.graphics.setColor(1, 1, 1, 1)
end

-- Scores
function displayscores()
    if gameState == 'start' then
        love.graphics.setFont(scorefont)
        -- p1 score (left)
        love.graphics.print(score1, VIRTUAL_WIDTH / 2 - 70, VIRTUAL_HEIGHT / 3)
        -- p2 score (right)
        love.graphics.print(score2, VIRTUAL_WIDTH / 2 + 50, VIRTUAL_HEIGHT / 3)
    elseif gameState == 'play' then
        love.graphics.setFont(smallfont)
        -- p1 score (left)
        love.graphics.print(score1, VIRTUAL_WIDTH - 70, 20)
        -- p2 score (right)
        love.graphics.print(score2, VIRTUAL_WIDTH - 50, 20)
    end
end

-- Words on top (game state)
function displaytopwords()
    love.graphics.setFont(smallfont)
    love.graphics.setColor(212 / 255, 175 / 255, 55 / 255, 1)
    if gameState == 'start' then
        love.graphics.printf("Ping Pong!", 0, 20, VIRTUAL_WIDTH, 'center')
        love.graphics.printf("Press enter to serve", 0, 32, VIRTUAL_WIDTH, 'center')
    elseif gameState == 'play' then
        love.graphics.printf("Playing..", 0, 20, VIRTUAL_WIDTH, 'center')
    elseif gameState == 'victory' then
        love.graphics.setFont(victoryfont)
        love.graphics.printf('Player ' .. winner .. ' wins!', 0, 20, VIRTUAL_WIDTH, 'center')
        love.graphics.setFont(smallfont)
        love.graphics.printf("Press enter to restart", 0, 46, VIRTUAL_WIDTH, 'center')
        love.graphics.setFont(scorefont)
        -- p1 score (left)
        love.graphics.print(score1, VIRTUAL_WIDTH / 2 - 70, VIRTUAL_HEIGHT / 3)
        -- p2 score (right)
        love.graphics.print(score2, VIRTUAL_WIDTH / 2 + 50, VIRTUAL_HEIGHT / 3)
    end
    love.graphics.setColor(1, 1, 1, 1)
end

-- creating backgroud field
function displayfield()
    love.graphics.rectangle('line', 0, 0, VIRTUAL_WIDTH, VIRTUAL_HEIGHT)
    love.graphics.setColor(1, 0, 0, 1)
    love.graphics.rectangle('fill', 0, VIRTUAL_HEIGHT / 3 - 10, 2, VIRTUAL_HEIGHT / 3 + 20)
    love.graphics.setColor(0, 0, 1, 1)
    love.graphics.rectangle('fill', VIRTUAL_WIDTH - 2, VIRTUAL_HEIGHT / 3 - 10, VIRTUAL_WIDTH, VIRTUAL_HEIGHT / 3 + 20)
    love.graphics.setColor(1, 1, 1, 1)
    love.graphics.circle('line', VIRTUAL_WIDTH / 2 , VIRTUAL_HEIGHT / 2 + 2, 25)
    love.graphics.line(VIRTUAL_WIDTH / 2, 0, VIRTUAL_WIDTH / 2, VIRTUAL_HEIGHT)
end