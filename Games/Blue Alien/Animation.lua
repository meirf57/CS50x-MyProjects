Animation = Class{}


-- dimensions for animation
function Animation:init(params)
    -- table of params
    self.texture = params.texture
    self.frames = params.frames or {}
    -- time between frames
    self.interval = params.interval or 0.05
    -- starting at 0
    self.timer = 0
    -- starting with still frame
    self.currentFrame = 1
end


-- index for frame
function Animation:getCurrentFrame()
    return self.frames[self.currentFrame]
end


-- updating with delta time
function Animation:update(dt)
    -- incrementing timer
    self.timer = self.timer + dt

    -- to increment frame
    while self.timer > self.interval do
        self.timer = self.timer - self.interval
        -- setting frame to next (+1 not to skip the last frame)
        self.currentFrame = (self.currentFrame + 1) % #self.frames
        -- no 0 frame so back to 1
        if self.currentFrame == 0 then self.currentFrame = 1 end
    end
end


-- to set to idle frame and start animation from beggining
function Animation:restart()
    self.timer = 0
    self.currentFrame = 1
end