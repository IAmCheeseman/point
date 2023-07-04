# Point Docs

Temp docs for y'all

## callbacks

- `onkeydown(key, isrepeat)` - called when a key is pressed
- `onkeyup(key)` - called when a key is released
- `onload()` - called when everything is set up
- `onstep(dt)` - called every frame
- `ondraw()` - called when you should draw
- TODO: mouse callbacks, window callbacks

## point.input

- `getmousepos()` - returns the x and y of the mouse
- `getglobalmousepos()` - returns the x and y of the mouse relative to the top left corner of your screen
- `iskeydown(key)` - returns if the specified key is pressed
- `ismousedown(button)` - returns if the specified mouse button is pressed

## point.draw

- `setscreensize(w, h)` - sets the resolution of the game
- `getscreensize()` - returns the resolution of the game
- `setpixelperfect(ison)` - keep the game's screen in multiples of 100%
- `getpixelperfect()` - returns if the game's pixel perfect
- `setcolor(r, g, b, a=1)` - sets the draw color to the specified color
- `clear()` - clears the screen
- `line(startx, starty, endx, endy)` - draws a line from the start to end
- `rect(x, y, w, h)` - draws a rectangle
- `filledrect(x, y, w, h)` - draws a filled rectangle
- `texture(index, x, y)` - draws the specified texture

## point.window

- `exit()` - closes the window
- `center()` - centers the window
- `settitle(title)` - sets the window title
- `gettitle()` - returns the window title
- `setpos(x, y)` - sets the window's position
- `getpos()` - returns the window's position
- `setsize(w, h)` - sets the window's size
- `getsize()` - returns the window's size
- `setfullscreen(ison, mode="desktop")` - sets fullscreen to `ison`, valid modes are `"desktop"` or `"fullscreen"`
- `getfullscreen()` - returns whether it's fullscreen or not

## point.texture

- `load(path)` - loads the texture at `path`.
- `setscale(index, x, y)` - sets the specified texture's scale
- `getscale(index, x, y)` - returns the specified texture's scale
- `setscalex(index, x)` - sets the specified texture's x scale
- `getscalex(index, x)` - returns the specified texture's x scale
- `setscaley(index, y)` - sets the specified texture's y scale
- `getscaley(index, y)` - returns the specified texture's y scale
- `setcenter(index, x, y)` - sets the specified texture's center
- `getcenter(index)` - returns the specified texture's center
- `setcenterx(index, x)` - sets the specified texture's x center
- `getcenterx(index)` - returns the specified texture's x center
- `setcentery(index, y)` - sets the specified texture's y center
- `getcentery(index)` - returns the specified texture's y center
- `setangle(index, r)` - sets the specified texture's angle, in radians
- `getangle(index)` - returns the specified texture's angle, in radians
- `setquad(index, x, y, w, h)` - sets the rect of the texture that's drawn
- `getquad(index)` - returns the rect of the texture that's drawn
- `getsize(index)` - returns the width and height of the texture
- `getwidth(index)` returns the width of the texture
- `getheight(index)` - returns the height of the texture

## point.vector

- `dot(x, y, xx, yy)` - dot product
- `length(x, y)` - magnitude
- `normalized(x, y)` - unit vector
- `directionto(x, y, xx, yy)` - direction from one point to another
- `distanceto(x, y, xx, yy)` - distance from one point to another
- `manhattandistanceto(x, y, xx, yy)` - manhattan distance from one point to another
- `angle(x, y)` - angle of a vector, in radians
- `angleto(x, y, xx, yy)` - angle from one vector to another
- `rotated(x, y, radians)` - returns the specified vector rotated by `radians` radians