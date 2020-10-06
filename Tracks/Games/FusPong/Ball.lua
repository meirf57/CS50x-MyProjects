Ball = Class{}
-- dimension of Ball class
function Ball:init(x, y, width, height)
    self.x = x
    self.y = y
    self.width = width
    self.height = height

    self.dx = math.random(2) == 1 and -100 or 100
    self.dy = math.random(-50, 50) -- 1 and math.random(-80, -100) or math.random(80, 100)
end
-- when game is reset
function Ball:reset()
    self.x = VIRTUAL_WIDTH / 2 - 3
    self.y = VIRTUAL_HEIGHT / 2
    self.dx = math.random(2) == 1 and -100 or 100
    self.dy = math.random(-50, 50)
end
-- ball movement
function Ball:update(dt)
    self.x = self.x + self.dx * dt
    self.y = self.y + self.dy * dt
end

-- bouncing ball off edges or paddles
function Ball:bounce(box)
    if self.x > box.x + box.width or self.x + self.width < box.x then
        return false
    elseif self.y > box.y + box.height or self.y + self.height < box.y then
        return false
    else
        return true
    end
end

-- drawing ball on screen
function Ball:render()
    love.graphics.rectangle('fill', self.x, self.y, 6, 6)
end