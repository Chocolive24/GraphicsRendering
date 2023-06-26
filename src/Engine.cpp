#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "triangle-sapp.glsl.h"

#include "../include/Engine.h"
#include "Graphics.cpp"
#include "Input.cpp"

namespace Input
{
    bool keys[256] = { false };

    Vector2F mousePos = Vector2F::zero;

    void KeyDownCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_KEY_DOWN)
        {
            if (event->key_code < 256)
            {
                keys[event->key_code] = true;
            }
        }
    }

    void KeyUpCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_KEY_UP)
        {
            if (event->key_code < 256)
            {
                keys[event->key_code] = false;
            }
        }
    }

    void MousePositionCallback(const sapp_event* event)
    {
        if (event->type == SAPP_EVENTTYPE_MOUSE_MOVE) 
        {
            mousePos.x = event->mouse_x;
            mousePos.y = event->mouse_y;
        }
    }
}

void GameFrame();

// application state
static struct 
{
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
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

    // create shader from code-generated sg_shader_desc
    sg_shader shd = sg_make_shader(triangle_shader_desc(sg_query_backend()));

    state.pip = sg_make_pipeline((sg_pipeline_desc)
    {
        .index_type = SG_INDEXTYPE_UINT32,
        .shader = shd,
        // if the vertex layout doesn't have gaps, don't need to provide strides and offsets
        .layout = {
            .attrs = {
                [ATTR_vs_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_vs_color0].format   = SG_VERTEXFORMAT_FLOAT4
            }
        },
        .label = "triangle-pipeline"
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
    sg_apply_bindings(&state.bind);

    Graphics::ClearDrawBuffer();

    GameFrame();

    sg_update_buffer(state.bind.vertex_buffers[0], (sg_range) { .ptr = vertexBuffer, .size = vertexBufferUsed * sizeof(*vertexBuffer) });
    sg_update_buffer(state.bind.index_buffer, (sg_range) { .ptr = indexBuffer, .size = indexBufferUsed * sizeof(*indexBuffer) });

    sg_draw(0, indexBufferUsed, 1);
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
            Input::MousePositionCallback(event);
            Input::KeyDownCallback(event);
            Input::KeyUpCallback(event);
        },
        .width = 640,
        .height = 480,
        .window_title = "Triangle (sokol-app)",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}