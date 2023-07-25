#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "basic-sapp.glsl.h"

#include "../include/Engine.h"
#include "Graphics.cpp"
#include "Input.cpp"
#include <iostream>

namespace Input
{
    // Variables 
    // -----------------------------------------------------------------------------------------------------------------------------------------------

    bool keyStates[256] = { false };
    bool previousKeyStates[256] = { false };

    Vector2F mousePos = Vector2F::zero;

    // -----------------------------------------------------------------------------------------------------------------------------------------------

    // Callbacks 
    // -----------------------------------------------------------------------------------------------------------------------------------------------

    // Keyboard 
    // --------------------------------------------------------------------------------------

    void KeyDownCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_KEY_DOWN)
        {
            if (event->key_code < 256)
            {
                keyStates[event->key_code] = true;
            }
        }
    }

    void KeyUpCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_KEY_UP)
        {
            if (event->key_code < 256)
            {
                keyStates[event->key_code] = false;
            }
        }
    }

    // --------------------------------------------------------------------------------------

    // Mouse 
    // --------------------------------------------------------------------------------------

     void MouseMoveCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) 
        {
            mousePos.x = event->mouse_x;
            mousePos.y = event->mouse_y;
        }
    }

    // --------------------------------------------------------------------------------------

    // -----------------------------------------------------------------------------------------------------------------------------------------------

    // Functions 
    // -----------------------------------------------------------------------------------------------------------------------------------------------

    bool KeyBeingPressed(sapp_keycode key)
    {
        return keyStates[key];
    }

    bool KeyWasPressed(sapp_keycode key)
    {
        return !previousKeyStates[key] && keyStates[key];
    }

    bool KeyWasReleased(sapp_keycode key)
    {
        return previousKeyStates[key] && !keyStates[key];
    }

    // -----------------------------------------------------------------------------------------------------------------------------------------------
}

#include <assert.h>
#include <malloc.h>
#include <Windows.h> 
#include <stdint.h>
#include <stdio.h>
#include <Shlwapi.h> // Include this header for PathCombine function

#pragma comment(lib, "Shlwapi.lib") // Link to the Shlwapi library

bool RdxFileExists(const char* path) 
{
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

int64_t RdxLastModified(const char* path) 
{
	if (!RdxFileExists(path)) 
    {
		return -1;
	}

	FILETIME ftLastWriteTime;

    // Open the file, specifying the desired access rights, sharing mode, and other parameters.
	HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE && GetFileTime(hFile, NULL, NULL, &ftLastWriteTime)) 
    {
		ULARGE_INTEGER uli;
		uli.LowPart  = ftLastWriteTime.dwLowDateTime;
		uli.HighPart = ftLastWriteTime.dwHighDateTime;
		CloseHandle(hFile);

		return static_cast<int64_t>(uli.QuadPart / 10000000ULL - 11644473600ULL);
	}

	return -1;
}

#define GAME_STATE_MAX_BYTE_SIZE 4096

void GameFrame();

// application state
static struct 
{
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;

    sg_image tilemapTexture;
    sg_image penguinTexture;

} state;

static void init(void) 
{
    sg_setup((sg_desc)
    {
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });
    
    state.bind.vertex_buffers[0] = sg_make_buffer((sg_buffer_desc)
    {
        .size = sizeof(vertexBuffer),
        .usage = SG_USAGE_DYNAMIC,
        .label = "triangle-vertices",
    });

    state.bind.index_buffer = sg_make_buffer((sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .size = sizeof(indexBuffer),
        .usage = SG_USAGE_DYNAMIC,
        .label = "triangle-indices",
    });

    
    // create a checkerboard texture

    // uint32_t pixels[4*4] = {
    //     0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000,
    //     0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
    //     0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF, 0xFF000000,
    //     0xFF000000, 0xFFFFFFFF, 0xFF000000, 0xFFFFFFFF,
    // };
    
    Bitmap tilemapImg = Graphics::LoadImg("assets/tilemap.png");
    Bitmap penguinImg = Graphics::LoadImg("assets/Penguin.png");

    // NOTE: SLOT_tex is provided by shader code generation
    state.tilemapTexture = sg_make_image((sg_image_desc)
    {
        .width  = tilemapImg.pixelSizeX,
        .height = tilemapImg.pixelSizeY,
        .data.subimage[0][0] =  sg_range{ tilemapImg.pixels, tilemapImg.DataSize() },
        .label = "tilemap-texture"
    });

    state.penguinTexture = sg_make_image((sg_image_desc)
    {
        .width  = penguinImg.pixelSizeX,
        .height = penguinImg.pixelSizeY,
        .data.subimage[0][0] =  sg_range{ penguinImg.pixels, penguinImg.DataSize() },
        .label = "penguin-texture"
    });

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));

    state.pip = sg_make_pipeline((sg_pipeline_desc)
    {
        .index_type = SG_INDEXTYPE_UINT32,
        .shader = shd,
        // if the vertex layout doesn't have gaps, don't need to provide strides and offsets
        .layout = 
        {
            .attrs = 
            {
                [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_color0].format   = SG_VERTEXFORMAT_FLOAT4,
                [ATTR_vs_uv0].format      = SG_VERTEXFORMAT_FLOAT2,
            }
        },
        .label = "triangle-pipeline",

        // Set up the alpha blending so that the output color of the fragment pixel is blended 
        // into the framebuffer proportionnally to its alpha.
        // This is the classic transparency blending.
        .colors[0].blend = (sg_blend_state)
        {
            .enabled = true,
            .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
            .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .src_factor_alpha = SG_BLENDFACTOR_ONE,
            .dst_factor_alpha = SG_BLENDFACTOR_ZERO,
        },
    });

    // a pass action to clear framebuffer to black
    state.pass_action = (sg_pass_action) 
    {
        .colors[0] = { .load_action=SG_LOADACTION_CLEAR, .clear_value={0.0f, 0.0f, 0.0f, 1.0f } }
    };
}

void frame(void) 
{
    sg_begin_default_pass(&state.pass_action, sapp_width(), sapp_height());
    sg_apply_pipeline(state.pip);

    Graphics::ClearDrawBuffer();
    drawCommandCount = 0;
    currentDrawCommand = {};

    memcpy(Input::previousKeyStates, Input::keyStates, sizeof(Input::keyStates));

    GameFrame();
    Graphics::DrawRect(Vector2F(sapp_width() / 2 - 8.f, sapp_height() / 2 - 60.f), 160.f, 120.f, Color::white, &state.penguinTexture);
    Graphics::DrawRect(Vector2F(100.f,  100.f), 100.f, 100.f, Color::white, &state.tilemapTexture);
    Graphics::DrawRect(Vector2F(200.f,  200.f),  50.f,  30.f, Color::white, &state.penguinTexture);
    Graphics::DrawRect(Vector2F(300.f,  300.f), 20.f, 120.f, Color::white, &state.tilemapTexture);

    
    Graphics::DrawQuad({500.f, 400.f}, {600.f, 400.f}, {600.f, 450.f}, {500.f, 450.f}, Color::green);
    Graphics::DrawQuad({500.f, 400.f}, {600.f, 400.f}, {600.f, 450.f}, {500.f, 450.f}, Color::red);
    Graphics::DrawQuad({400.f, 300.f}, {500.f, 300.f}, {500.f, 350.f}, {400.f, 350.f}, Color::red);

    sg_update_buffer(state.bind.vertex_buffers[0], (sg_range) { .ptr = vertexBuffer, .size = vertexBufferUsed * sizeof(*vertexBuffer) });
    sg_update_buffer(state.bind.index_buffer,      (sg_range) { .ptr = indexBuffer,  .size = indexBufferUsed *  sizeof(*indexBuffer) });

    for (int i = 0; i < drawCommandCount; i++)
    {
        printf("cmd no %i \n", i);
        DrawCommand* cmd = &drawCommandBuffer[i];

        // Bind our textures before the sg_apply_binding
        // NOTE: SLOT_tex is provided by shader code generation
        state.bind.fs_images[SLOT_tex] = *cmd->texture;
        sg_apply_bindings(&state.bind);

        // sg_draw
        // arg1: offset to start reading the indices in the index buffer
        // arg2: the number of indices to draw
        // arg3: the number of instances to draw (in your case, 1 for each draw command)
        sg_draw(cmd->indexStart, cmd->indexCount, 1);
    }
    
    state.bind.fs_images[SLOT_tex] = state.penguinTexture;
    sg_apply_bindings(&state.bind);

    //sg_draw(24, 12, 1); // draw the 2 penguins manually to test.
    sg_draw(36,  6, 1); // test to draw the last indices before some previous ones.
    sg_draw(27,  6, 1); // draw custom penguin.

    sg_end_pass();
    sg_commit();
}

void cleanup(void) 
{
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) 
{
    (void)argc; (void)argv;
    
    return (sapp_desc)
    {
        .win32_console_create = true,
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = [](const sapp_event* event) 
        {
            Input::MouseMoveCallback(event);
            Input::KeyDownCallback(event);
            Input::KeyUpCallback(event);
        },
        .width = 640,
        .height = 480,
        .window_title = "Game",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}