-- Called at the first plugin loading.
-- Describes the key(s) pressed to activate this plugin.
function activator()
    return 'Shift', Hold -- The key(s) that activates the plugin & the type of activation:
    -- Hold for holding the key to keep it activated
    -- Toggle for toggling the activation
end

-- This function allows your plugins to be configured at runtime.
-- Return a table with the name of the setting (which must be a predefined
-- global variable) associated with a short text describing the variable.
function settings()
    return {
        maxDelta = 'Maximum delta value',
        maxPosDelta = 'Maximum position delta from the middle position'
    }
end

-- Called each time the plugin is activated.
function init()
    -- Self explanatory
    setPosition(50)

    -- This will request the SliderBar to calculate and send velocity values
    -- everytime it can. It will then call the onVelocity() callback.
    -- requestVelocity()
end

-- You can create your own global variables, they won't interfere with other
-- scripts.
maxDelta = 0
maxPosDelta = 10

delta = 0
lastPosition = 0

-- You can manipulate the keyboard and mouse using these functions:
-- keyDown('key')
-- keyUp('key')
-- keyPress('key1+key2+...')
-- mouseScroll(delta)

-- There are several functions which are called everytime something happens, 
-- and the plugin is activated (the activator key is pressed)
-- onPosition(), everytime a new position is received
-- onVelocity(), everytime a new velocity is received

-- Some global variables you can use:
-- position, in percent (0 - 100)
-- velocity, in percent/s (0 - 100) [Only use if requestVelocity() was called in init()]
-- isAutoMoving, is true if the sliderbar is moved by the motor
function onPosition()
    if isAutoMoving then
        return
    end

    -- Increments the delta
    delta = delta + lastPosition - position
    lastPostion = position

    if delta >= maxDelta then
        -- do something
        delta = 0
    elseif delta <= -maxDelta then
        -- do something else
        delta = 0
    end

    -- You can access some global values already set:
    -- position, in percent (0 - 100)
    -- velocity, in percent/s (0 - 100)
    -- isAutoMoving, tells if the slider is still moving from a setPosition call
    if position >= 50 + maxPosDelta then
        keyPress('Ctrl+PgUp')
    elseif position <= 50 - maxPosDelta then
        keyPress('Ctrl+PgDown')
    end
end
