# Point
A blazingly slow and unsafe game engine for Lua written in C.

Here's a snippet of the engine:
```lua
local x, y = 0, 0
local texture = point.texture.load("testsprite.png")

function point.onstep(dt)
    local ix, iy = 0, 0
    if point.input.iskeydown(point.key.w) then iy = iy - 1 end
    if point.input.iskeydown(point.key.a) then ix = ix - 1 end
    if point.input.iskeydown(point.key.s) then iy = iy + 1 end
    if point.input.iskeydown(point.key.d) then ix = ix + 1 end

    ix, iy = point.vector.normalized(ix, iy)

    x = x + ix * 220 * dt
    y = y + iy * 220 * dt
end

function point.ondraw()
    point.draw.setcolor(1, 1, 0)
    point.draw.clear()

    point.draw.texture(texture, x, y)
end
```

Use [this](https://github.com/IAmCheeseman/point/blob/master/point.yml) as documentation.

## TODO

- Make textures, fonts, audio userdata
- Make the engine userdata and get rid of the globals