function activator()
    return 'Shift+Ctrl', Toggle
end

function settings()
    return {
        scrollFactor = 'Gain from position to scroll delta'
    }
end

scrollFactor = 1.

function init()
    setPosition(50)
end

lastPosition = 0

function onPosition()
    delta = lastPosition - position

    if not isAutoMoving then
        mouseScroll(scrollFactor * delta)
    end

    lastPosition = position
end
