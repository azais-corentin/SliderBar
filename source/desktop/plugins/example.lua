-- Called at the first plugin loading.
-- Describes the key(s) pressed to activate this plugin
function activator()
    return 'shift' -- The key(s) that activates the plugin
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
delta = 0
lastPosition = 0

-- There are several functions which are called everytime something happens, 
-- and the plugin is activated (the activator key is pressed)
-- onPosition(), everytime a new position is received
-- onVelocity(), everytime a new velocity is received

-- Some global variables you can use:
-- position, in percent (0 - 100)
-- velocity, in percent/s (0 - 100) [Only use if requestVelocity() was called in init()]

function onPosition()
    -- Increments the delta
    delta = delta + lastPosition - position

    if delta >= 5 then
        -- do something
        delta = 0
    end

    -- You can access some global values already set:
    -- position, in percent (0 - 100)
    -- velocity, in percent/s (0 - 100)
    if position >= 60 then
        keys('ctrl+pgup')
    end

    if position <= 50 then
        keys('ctrl+pgdown')
    end

    lastPosition = position
end