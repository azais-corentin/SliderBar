function activator()
    return 'shift'
end

function init()
    setPosition(50)
end

function onPosition()
    if position >= 60 then
        -- Change tab
        keys('ctrl+pgup')
        -- Go back to the middle
        setPosition(50)
    end

    if position <= 50 then
        -- Change tab
        keys('ctrl+pgdown')
        -- Go back to the middle
        setPosition(50)
    end
end