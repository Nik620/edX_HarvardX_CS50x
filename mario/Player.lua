--[[
    Represents our player in the game, with its own sprite.
]]

Player = Class{}

local WALKING_SPEED = 140
local JUMP_VELOCITY = 250 -- was 450 **UPDATED**
local dance = 100 -- **UPDATE**

function Player:init(map)
    
    self.x = 0
    self.y = 0
    self.width = 16
    self.height = 20

    -- offset from top left to center to support sprite flipping
    self.xOffset = 8
    self.yOffset = 10

    -- reference to map for checking tiles
    self.map = map
    self.texture = love.graphics.newImage('graphics/blue_alien.png')

    -- sound effects
    -- deathnote & flag rise **UPDATE**
    self.sounds = {
        ['jump'] = love.audio.newSource('sounds/jump.wav', 'static'),
        ['hit'] = love.audio.newSource('sounds/hit.wav', 'static'),
        ['coin'] = love.audio.newSource('sounds/coin.wav', 'static'),
        ['death'] = love.audio.newSource('sounds/death.wav', 'static'),
        ['flag'] = love.audio.newSource('sounds/powerup-reveal.wav', 'static')
    }

    -- animation frames
    self.frames = {}

    -- current animation frame
    self.currentFrame = nil

    -- used to determine behavior and animations
    self.state = 'idle'

    -- determines sprite flipping
    self.direction = 'left'

    -- x and y velocity
    self.dx = 0
    self.dy = 0

    -- position on top of map tiles
    self.y = map.tileHeight * ((map.mapHeight - 2) / 2) - self.height
    self.x = map.tileWidth * 10
    self.ground = map.tileHeight * ((map.mapHeight - 2) / 2) -- added ground level **UPDATE**

    -- initialize all player animations
    self.animations = {
        ['idle'] = Animation({
            texture = self.texture,
            frames = {
                love.graphics.newQuad(0, 0, 16, 20, self.texture:getDimensions())
            }
        }),
        ['walking'] = Animation({
            texture = self.texture,
            frames = {
                love.graphics.newQuad(128, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(144, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(160, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(144, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 0.15
        }),
        ['jumping'] = Animation({
            texture = self.texture,
            frames = {
                love.graphics.newQuad(32, 0, 16, 20, self.texture:getDimensions())
            }
        }),
        -- added ducking function **UPDATE**
        ['ducking'] = Animation({
            texture = self.texture,
            frames = {
                love.graphics.newQuad(48, 0, 16, 20, self.texture:getDimensions())
            }
        }),
        -- added victory dance **UPDATE**
        ['victory'] = Animation({
            texture = self.texture,
            frames = {
                love.graphics.newQuad(96, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(80, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(96, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(80, 0, 16, 20, self.texture:getDimensions()),
                love.graphics.newQuad(96, 0, 16, 20, self.texture:getDimensions())
            },
            interval = 0.10
        })
    }

    -- initialize animation and current frame we should render
    self.animation = self.animations['idle']
    self.currentFrame = self.animation:getCurrentFrame()

    -- behavior map we can call based on player state
    self.behaviors = {
        ['idle'] = function(dt)
            -- add up button functionality to trigger jump state 
            if love.keyboard.wasPressed('up') then -- **UPDATED, WAS SPACE**
                self.dy = -JUMP_VELOCITY
                self.state = 'jumping'
                self.animation = self.animations['jumping']
                self.sounds['jump']:play()
            elseif love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -WALKING_SPEED
                self.state = 'walking'
                self.animations['walking']:restart()
                self.animation = self.animations['walking']
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = WALKING_SPEED
                self.state = 'walking'
                self.animations['walking']:restart()
                self.animation = self.animations['walking']
            elseif love.keyboard.isDown('down') then -- **UPDATE**
                self.dx = 0
                self.state = 'ducking'
                self.animations['ducking']:restart()
                self.animation = self.animations['ducking']
            elseif self.x >= 400 then -- victory **UPDATE**
                self.dx = 0
                if  dance > 0 then
                    self.state = 'victory'
                    --self.animations['victory']:restart()
                    self.animation = self.animations['victory']
                    self.sounds['flag']:play()
                    dance = dance - 1
                else
                    self.state = 'idle'
                    self.animation = self.animations['idle']
                end
            elseif self.y > self.ground then -- failState **UPDATE**
                failState = true
                self.state = 'fail'
            else
                self.dx = 0
            end
        end,
        ['walking'] = function(dt)
            -- keep track of input to switch movement while walking, or reset
            -- to idle if we're not moving
            if love.keyboard.wasPressed('up') then -- **UPDATED, WAS SPACE**
                self.dy = -JUMP_VELOCITY
                self.state = 'jumping'
                self.animation = self.animations['jumping']
                self.sounds['jump']:play()
            elseif love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -WALKING_SPEED
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = WALKING_SPEED
            elseif love.keyboard.isDown('down') then -- **UPDATE**
                self.dx = 0
                self.state = 'ducking'
                self.animations['ducking']:restart()
                self.animation = self.animations['ducking']
            elseif self.x >= 400 then -- victory **UPDATE**
                self.dx = 0
                if  dance > 0 then
                    self.state = 'victory'
                    --self.animations['victory']:restart()
                    self.animation = self.animations['victory']
                    self.sounds['flag']:play()
                    dance = dance - 1
                else
                    self.state = 'idle'
                    self.animation = self.animations['idle']
                end
            elseif self.y > self.ground  then -- failState **UPDATE**
                failState = true
                self.state = 'fail'
            else
                self.dx = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
            end

            -- check for collisions moving left and right
            self:checkRightCollision()
            self:checkLeftCollision()

            -- check if there's a tile directly beneath us
            if not self.map:collides(self.map:tileAt(self.x, self.y + self.height)) and
                not self.map:collides(self.map:tileAt(self.x + self.width - 1, self.y + self.height)) then
                
                -- if so, reset velocity and position and change state
                self.state = 'jumping'
                self.animation = self.animations['jumping']

            end
        end,
        ['jumping'] = function(dt)
            -- break if we go below the surface
            if self.y > 300 then
                return
            end

            if love.keyboard.isDown('left') then
                self.direction = 'left'
                self.dx = -WALKING_SPEED
            elseif love.keyboard.isDown('right') then
                self.direction = 'right'
                self.dx = WALKING_SPEED
            elseif self.y > self.ground  then  -- failState **UPDATE**
                failState = true
                self.state = 'fail'
            end

            -- apply map's gravity before y velocity
            self.dy = self.dy + self.map.gravity

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
        end,
        -- ducking function **UPDATE**
        ['ducking'] = function(dt)
            if love.keyboard.isDown('down') then
                self.dx = 0
                self.state = 'ducking'
                self.animation = self.animations['ducking']
            else
                self.dx = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
            end
        end, 
        -- Victory march **UPDATE**
        ['victory'] = function(dt)
            self.dx = 0
            map.state = 'rise'
            if dance > 0 then
                self.animation = self.animations['victory']
                dance = dance - 1
            elseif dance == 0 or love.keyboard.isDown('left') or 
                love.keyboard.isDown('right') or
                love.keyboard.isDown('up') or
                love.keyboard.isDown('down') then
                self.dx = 0
                self.state = 'idle'
                self.animation = self.animations['idle']
            end
        end,
        -- fail check **UPDATE**
        ['fail'] = function(dt)
            if love.keyboard.wasPressed('space') then
                self:reset()
            end
        end
    }
end

-- reset function if falls through map **UPDATE**
function Player:reset()
    self.y = map.tileHeight * ((map.mapHeight - 2) / 2) - self.height
    self.x = map.tileWidth * 10
    self.dx = 0
    self.dy = 0
    self.state = 'idle'
    self.animation = self.animations['idle']
    failState = false
    map.camX = 0
    map.camY = -3
end

function Player:update(dt)
    self.behaviors[self.state](dt)
    self.animation:update(dt)
    self.currentFrame = self.animation:getCurrentFrame()
    self.x = self.x + self.dx * dt

    self:calculateJumps()
    
    -- apply velocity
    self.y = self.y + self.dy * dt
end

-- jumping and block hitting logic
function Player:calculateJumps()
    
    -- if we have negative y velocity (jumping), check if we collide
    -- with any blocks above us
    if self.dy < 0 then
        if self.map:tileAt(self.x, self.y).id ~= TILE_EMPTY or
            self.map:tileAt(self.x + self.width - 1, self.y).id ~= TILE_EMPTY then
            -- reset y velocity
            self.dy = 0

            -- change block to different block
            local playCoin = false
            local playHit = false
            if self.map:tileAt(self.x, self.y).id == JUMP_BLOCK then
                self.map:setTile(math.floor(self.x / self.map.tileWidth) + 1,
                    math.floor(self.y / self.map.tileHeight) + 1, JUMP_BLOCK_HIT)
                playCoin = true
            else
                playHit = true
            end
            if self.map:tileAt(self.x + self.width - 1, self.y).id == JUMP_BLOCK then
                self.map:setTile(math.floor((self.x + self.width - 1) / self.map.tileWidth) + 1,
                    math.floor(self.y / self.map.tileHeight) + 1, JUMP_BLOCK_HIT)
                playCoin = true
            else
                playHit = true
            end

            if playCoin then
                self.sounds['coin']:play()
            elseif playHit then
                self.sounds['hit']:play()
            end
        end
    end
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

function Player:render()
    local scaleX

    -- set negative x scale factor if facing left, which will flip the sprite
    -- when applied
    if self.direction == 'right' then
        scaleX = 1
    else
        scaleX = -1
    end

    -- draw sprite with scale factor and offsets
    love.graphics.draw(self.texture, self.currentFrame, math.floor(self.x + self.xOffset),
        math.floor(self.y + self.yOffset), 0, scaleX, 1, self.xOffset, self.yOffset)
end
