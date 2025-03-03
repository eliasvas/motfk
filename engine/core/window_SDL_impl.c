#include "window.h"
#include "core/global_state.h"
#include "gfx/ogl.h"

// TODO -- we need some more SDL_Error logs I think
// TODO -- Probably we want _SDL_nwindow_impl_xxx, and link with main implementation via function pointers
// TODO -- maybe add an option for graphics API to be used?

#define SDL_MAIN_NOIMPL
#include <SDL.h>
#include <SDL_main.h>

b32 nwindow_impl_create(nWindow *win) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        return 0;
    }

    #if (OS_WINDOWS)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    #endif

    u32 flags = SDL_WINDOW_OPENGL;
    if (win->opt & N_WINDOW_OPT_RESIZABLE) {
        flags |= SDL_WINDOW_RESIZABLE;
    }
    if (win->opt & N_WINDOW_OPT_BORDERLESS) {
        flags |= SDL_WINDOW_BORDERLESS;
    }
    SDL_Window *window = SDL_CreateWindow(win->name,SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (s32)win->ww, (s32)win->wh, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    win->impl_state = window;

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if (!glcontext) {
        NLOG_ERR("Error creating GL context!");
        return 0;
    }
    SDL_GL_MakeCurrent(window,glcontext);
    ogl_load_gl_functions(SDL_GL_GetProcAddress);

    {
        int majorv, minorv, profilem;
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorv);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorv);
        SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profilem);
        NLOG_INFO("OpenGL %d.%d (ES=%s)", majorv, minorv, (SDL_GL_CONTEXT_PROFILE_ES == profilem) ? "true" : "compatibility");
    }

    SDL_ShowWindow(window);
    {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        NLOG_INFO("Window size: %dx%d", width, height);
    }
    NLOG_INFO("SDL window [%s] created succesfully!", win->name);
    return 1;
}

b32 nwindow_impl_destroy(nWindow *win) {
    SDL_DestroyWindow(win->impl_state);
    return 1;
}

void nwindow_impl_swap(nWindow *win) {
    SDL_GL_SwapWindow(win->impl_state);
}

void nwindow_impl_update_size(nWindow *win) {
    int ww,wh;
    SDL_GetWindowSize(win->impl_state, &ww, &wh);
    win->ww = (f32)ww;
    win->wh = (f32)wh;
}

nWindowEventNode* nwindow_impl_capture_events(nWindow *win) {
    nWindowEventNode *first = NULL;
    nWindowEventNode *last = NULL;
    SDL_Event event;
    s32 scroll_y;
    while (SDL_PollEvent(&event)) {
        nWindowEventNode *node = push_array(get_frame_arena(), nWindowEventNode, 1);
        switch (event.type) {
            case SDL_QUIT:
                exit(1);
            case SDL_KEYUP:
                node->kind = N_WINDOW_EVENT_KIND_KEYBOARD_EVENT;
                node->ke.key = event.key.keysym.scancode;
                node->ke.state = 0;
                sll_queue_push(first, last, node);
                break;
            case SDL_KEYDOWN:
                node->kind = N_WINDOW_EVENT_KIND_KEYBOARD_EVENT;
                node->ke.key = event.key.keysym.scancode;
                node->ke.state = 1;
                sll_queue_push(first, last, node);
                break;
            case SDL_MOUSEMOTION:
                node->kind = N_WINDOW_EVENT_KIND_MOUSE_MOTION_EVENT;
                node->mme.x = event.motion.x;
                node->mme.y = event.motion.y;
                sll_queue_push(first, last, node);
                break;
            case SDL_MOUSEBUTTONDOWN:
                node->kind = N_WINDOW_EVENT_KIND_MOUSE_EVENT;
                node->me.key = event.button.button - SDL_BUTTON_LEFT;
                node->ke.state = 1;
                sll_queue_push(first, last, node);
                break;
            case SDL_MOUSEBUTTONUP:
                node->kind = N_WINDOW_EVENT_KIND_MOUSE_EVENT;
                node->me.key = event.button.button - SDL_BUTTON_LEFT;
                node->ke.state = 0;
                sll_queue_push(first, last, node);
                break;
            case SDL_MOUSEWHEEL:
                node->kind = N_WINDOW_EVENT_KIND_SCROLLWHEEL_EVENT;
                scroll_y = event.wheel.y;
                node->swe.y = scroll_y;
                sll_queue_push(first, last, node);
            default:
                break;
        }
    }
    nwindow_impl_update_size(win);

    return first;
}

void nwindow_impl_toggle_fullscreen(nWindow *win) {
    u64 flag = SDL_GetWindowFlags(win->impl_state);
	u64 is_fullscreen  = flag & SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen(win->impl_state, is_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}