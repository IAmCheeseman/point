#include <math.h>
#include "draw.h"
#include "texture.h"
#include "font.h"

static Renderer* renderer = NULL;

static int rect(lua_State* L) {
    int x = lua_tonumber(L, 1);
    int y = lua_tonumber(L, 2);
    int w = lua_tonumber(L, 3);
    int h = lua_tonumber(L, 4);

    SDL_Rect rect = { x, y, w, h };

    if (SDL_RenderDrawRect(renderer->sdl_renderer, &rect) < 0) {
        fprintf(stderr, "Failed to render rect: %s\n", SDL_GetError());
        exit(1);
    }

    return 0;
}

static int fillrect(lua_State* L) {
    int x = lua_tonumber(L, 1);
    int y = lua_tonumber(L, 2);
    int w = lua_tonumber(L, 3);
    int h = lua_tonumber(L, 4);

    SDL_Rect rect = { x, y, w, h };

    if (SDL_RenderFillRect(renderer->sdl_renderer, &rect) < 0) {
        fprintf(stderr, "Failed to render filled rect: %s\n", SDL_GetError());
        exit(1);
    }

    return 0;
}

static int circle(lua_State* L) {
    int x = lua_tonumber(L, 1);
    int y = lua_tonumber(L, 2);
    int radius = lua_tonumber(L, 3);

    int dx = radius;
    int dy = 0;
    int p = 1 - radius;

    if (radius < 0) {
        SDL_RenderDrawPoint(renderer->sdl_renderer,  dx + x, -dy + y); // Right
        SDL_RenderDrawPoint(renderer->sdl_renderer, -dx + x, -dy + y); // Left
        SDL_RenderDrawPoint(renderer->sdl_renderer,  dy + x,  dx + y); // Down
        SDL_RenderDrawPoint(renderer->sdl_renderer, -dy + x, -dx + y); // Up
    }

    while (dx > dy) {
        dy++;

        if (p <= 0) {
            p += 2 * dy + 1;
        } else {
            dx--;
            p += 2*dy - 2*dx + 1;
        }

        if (dx < dy) break;

        SDL_RenderDrawPoint(renderer->sdl_renderer,  dx + x,  dy + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer, -dx + x,  dy + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer,  dx + x, -dy + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer, -dx + x, -dy + y);

        if (dx == dy) continue;

        SDL_RenderDrawPoint(renderer->sdl_renderer, -dy + x,  dx + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer,  dy + x,  dx + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer,  dy + x, -dx + y);
        SDL_RenderDrawPoint(renderer->sdl_renderer, -dy + x, -dx + y);
    }

    SDL_RenderDrawPoint(renderer->sdl_renderer, -radius + x, y);
    SDL_RenderDrawPoint(renderer->sdl_renderer,  radius + x, y);
    SDL_RenderDrawPoint(renderer->sdl_renderer, x, -radius + y);
    SDL_RenderDrawPoint(renderer->sdl_renderer, x,  radius + y);

    return 0;
}

static int fillcircle(lua_State* L) {
    int x = lua_tonumber(L, 1);
    int y = lua_tonumber(L, 2);
    int radius = lua_tonumber(L, 3);

    int dx = radius;
    int dy = 0;
    int p = 1 - radius;

    if (radius < 0) {
        SDL_RenderDrawLine(renderer->sdl_renderer, dx + x, -dy + y, -dx + x, -dy + y);
    }

    while (dx > dy) {
        dy++;

        if (p <= 0) {
            p += 2 * dy + 1;
        } else {
            dx--;
            p += 2*dy - 2*dx + 1;
        }

        if (dx < dy) break;

        SDL_RenderDrawLine(renderer->sdl_renderer, -dx + x,  dy + y,  dx + x,  dy + y);
        SDL_RenderDrawLine(renderer->sdl_renderer,  dx + x, -dy + y, -dx + x, -dy + y);

        if (dx == dy) continue;

        SDL_RenderDrawLine(renderer->sdl_renderer, -dy + x,  dx + y,  dy + x,  dx + y);
        SDL_RenderDrawLine(renderer->sdl_renderer,  dy + x, -dx + y, -dy + x, -dx + y);
    }

    SDL_RenderDrawLine(renderer->sdl_renderer, -radius + x, y, radius + x, y);

    return 0;
}

static int clear(lua_State* L) {
    if (SDL_RenderClear(renderer->sdl_renderer) < 0) {
        fprintf(stderr, "Failed to clear: %s\n", SDL_GetError());
        exit(1);
    }
    
    return 0;
}

static int setcolor(lua_State* L) {
    int r = (int)(lua_tonumber(L, 1) * 255);
    int g = (int)(lua_tonumber(L, 2) * 255);
    int b = (int)(lua_tonumber(L, 3) * 255);
    int a = (int)(luaL_optnumber(L, 4, 1) * 255);

    if (SDL_SetRenderDrawColor(renderer->sdl_renderer, r, g, b, a) < 0) {
        fprintf(stderr, "Failed to set color: %s\n", SDL_GetError());
        exit(1);
    }

    return 0;
}

static int line(lua_State* L) {
    int sx = lua_tonumber(L, 1);
    int sy = lua_tonumber(L, 2);
    int ex = lua_tonumber(L, 3);
    int ey = lua_tonumber(L, 4);

    if (SDL_RenderDrawLine(renderer->sdl_renderer, sx, sy, ex, ey) < 0) {
        fprintf(stderr, "Failed to draw line: %s\n", SDL_GetError());
        exit(1);
    }

    return 0;
}

static int texture(lua_State* L) {
    Texture* texture = (Texture*)luaL_checkudata(L, 1, TEXTURE_NAME);
    int x = lua_tonumber(L, 2);
    int y = lua_tonumber(L, 3);

    int w, h;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &w, &h);

    SDL_Rect src = (SDL_Rect){ 
        (int)texture->quadx, 
        (int)texture->quady, 
        (int)texture->quadw,
        (int)texture->quadh
    };
    SDL_Rect dst = (SDL_Rect){ 
        x - texture->pivotx * texture->scalex, 
        y - texture->pivoty * texture->scaley, 
        w * texture->scalex, 
        h * texture->scaley 
    };
    SDL_Point center = (SDL_Point){ texture->pivotx * texture->scalex, texture->pivoty * texture->scaley };
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (texture->fliph)
        flip |= SDL_FLIP_HORIZONTAL;
    if (texture->flipv)
        flip |= SDL_FLIP_VERTICAL;
    
    SDL_RenderCopyEx(renderer->sdl_renderer, texture->sdl_texture, &src, &dst, texture->angle * 180 / M_PI, &center, flip);

    return 0;
}

void render_text(Font* font, const char* text, int x, int y) {
    SDL_Color color;
    SDL_GetRenderDrawColor(renderer->sdl_renderer, &color.r, &color.g, &color.b, &color.a);
    SDL_Surface* text_surface = TTF_RenderText_Blended(font->sdl_font, text, color);
    if (text_surface == NULL) {
        fprintf(stderr, "Failed to render text: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer->sdl_renderer, text_surface);
    if (text_texture == NULL) {
        fprintf(stderr, "Failed to render text: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_FreeSurface(text_surface);

    int w, h;
    SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);

    SDL_Rect dst = (SDL_Rect){ x, y, w, h };
    SDL_RenderCopy(renderer->sdl_renderer, text_texture, NULL, &dst);

    SDL_DestroyTexture(text_texture);
}

static int text(lua_State* L) {
    Font* font = (Font*)luaL_checkudata(L, 1, FONT_NAME);
    const char* text = lua_tostring(L, 2);
    int x = lua_tonumber(L, 3);
    int y = lua_tonumber(L, 4);

    render_text(font, text, x, y);

    return 0;
}

static int textlines(lua_State* L) {
    Font* font = (Font*)luaL_checkudata(L, 1, FONT_NAME);
    int x = lua_tonumber(L, 2);
    int y = lua_tonumber(L, 3);
    int spacing = lua_tonumber(L, 4);
    
    int length = lua_rawlen(L, 5);

    for (int i = 1; i <= length; i++) {
        lua_rawgeti(L, 5, i);
        const char* text = lua_tostring(L, -1);
        render_text(font, text, x, y + (i - 1) * font->point_size + spacing * (i - 1));
    }

    return 0;
}

static int setpixelperfect(lua_State* L) {
    bool state = lua_toboolean(L, 1);
    renderer->is_pixel_perfect = state;

    return 0;
}

static int getpixelperfect(lua_State* L) {
    lua_pushboolean(L, renderer->is_pixel_perfect);

    return 1;
}

static int setscreensize(lua_State* L) {
    int w = lua_tonumber(L, 1);
    int h = lua_tonumber(L, 2);

    if (renderer->is_rendering) {
        fprintf(stderr, "Cannot change screen size while drawing.\n");
        exit(1);
    }

    set_screen_size(renderer, w, h);

    return 0;
}

static int getscreensize(lua_State* L) {
    lua_pushnumber(L, renderer->screen_width);
    lua_pushnumber(L, renderer->screen_height);

    return 2;
}

static const luaL_Reg draw[] = {
    { "setscreensize", setscreensize },
    { "getscreensize", getscreensize },
    { "getpixelperfect", getpixelperfect },
    { "setpixelperfect", setpixelperfect },
    { "clear", clear },
    { "setcolor", setcolor },
    { "line", line },
    { "rect", rect },
    { "fillrect", fillrect },
    { "circle", circle },
    { "fillcircle", fillcircle },
    { "texture", texture },
    { "text", text },
    { "textlines", textlines },
    { NULL, NULL },
};

void init_draw_lib(lua_State *L, EngineState* engine) {
    renderer = &engine->renderer;

    lua_getglobal(L, "point");
    luaL_newlib(L, draw);
    lua_setfield(L, -2, "draw");
    lua_pop(L, 1);
}