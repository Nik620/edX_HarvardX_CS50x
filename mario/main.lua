--[[
    Super Mario Bros. Demo
    Author: Colton Ogden
    Original Credit: Nintendo

    Demonstrates rendering a screen of tiles.

    Edits documented with **UPDATE** or **UPDATED**
    Edits by: Nik P

    Functionalities added:
    -player can duck
    -player can respawn
    -failure & victory messages added
    -pyramid and animated flagpole added
    -dance animation upon level completion

]]

Class = require 'class'
push = require 'push'

require 'Animation'
require 'Map'
require 'Player'

-- close resolution to NES but 16:9
VIRTUAL_WIDTH = 432
VIRTUAL_HEIGHT = 243

-- actual window resolution
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

-- seed RNG
math.randomseed(os.time())

-- makes upscaling look pixel-y instead of blurry
love.graphics.setDefaultFilter('nearest', 'nearest')

-- an object to contain our map data
map = Map()

-- failstate check **UPDATE**
failState = false

-- performs initialization of all objects and data needed by program
function love.load()
    -- sets up virtual screen resolution for an authentic retro feel
    push:setupScreen(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, {
        fullscreen = false,
        resizable = true
    })

    -- font addition, replaced old font for new sizing **UPDATE**
    smallFont = love.graphics.newFont('fonts/font.ttf', 8)
    largeFont = love.graphics.newFont('fonts/font.ttf', 16)
    love.graphics.setFont(smallFont)

    love.window.setTitle('Super Mario 50')

    -- keys
    love.keyboard.keysPressed = {}
    love.keyboard.keysReleased = {}
end

-- called whenever window is resized
function love.resize(w, h)
    push:resize(w, h)
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

-- called whenever a key is pressed
function love.keypressed(key)
    if key == 'escape' then
        love.event.quit()
    elseif key == 'space' then -- added to reset character **UPDATE**
        map.player:reset()
    end

    love.keyboard.keysPressed[key] = true
end

-- called whenever a key is released
function love.keyreleased(key)
    love.keyboard.keysReleased[key] = true
end

-- called every frame, with dt passed in as delta in time since last frame
function love.update(dt)
    map:update(dt)

    gameState = map.state -- added for state **UPDATE**

    -- reset all keys pressed and released this frame
    love.keyboard.keysPressed = {}
    love.keyboard.keysReleased = {}
end

-- called each frame, used to render to the screen
function love.draw()
    -- begin virtual resolution drawing
    push:apply('start')

    -- clear screen using Mario background blue
    love.graphics.clear(108/255, 140/255, 255/255, 255/255)

    -- renders our map object onto the screen
    love.graphics.translate(math.floor(-map.camX + 0.5), math.floor(-map.camY + 0.5))
    map:render()

    -- render victory message **UPDATE**
    if gameState == 'wave' then
        love.graphics.setFont(largeFont)
        love.graphics.printf('Congratulations!', 0, 10, VIRTUAL_WIDTH, 'right')
        love.graphics.setFont(smallFont)
        love.graphics.printf('You Win', 0, 40, VIRTUAL_WIDTH, 'right')
        love.graphics.printf('Press Esc to quit', 0, 50, VIRTUAL_WIDTH, 'right')
    elseif failState == true then
        love.graphics.setFont(largeFont)
        love.graphics.printf('Failure...', 0, 10, VIRTUAL_WIDTH, 'center')
        love.graphics.setFont(smallFont)
        love.graphics.printf('Press space to respawn', 0, 40, VIRTUAL_WIDTH, 'center')
        love.graphics.printf('Press Esc to quit', 0, 50, VIRTUAL_WIDTH, 'center')
    end

    -- end virtual resolution
    push:apply('end')
end
