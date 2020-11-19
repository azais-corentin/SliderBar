function activator()
    return 'Shift', Hold
end

function settings()
    return {
        maxPosDelta = 'Maximum position delta from the middle position'
    }
end

maxPosDelta = 5;

function init()
    setPosition(50)
end

function onPosition()
    if isAutoMoving then
        return
    end

    if position >= 50 + maxPosDelta then
        -- Change tab
        keyPress('ctrl+pgup')
        -- Go back to the middle
        setPosition(50)
    elseif position <= 50 - maxPosDelta then
        -- Change tab
        keyPress('ctrl+pgdown')
        -- Go back to the middle
        setPosition(50)
    end
end
