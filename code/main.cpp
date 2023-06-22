#define SOKOL_IMPL
#define SOKOL_GLCORE33
#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "triangle-sapp.glsl.h"

#include "Graphics.cpp"
#include "Math.h"

// application state
static struct 
{
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

float vertexBuffer[1000];
int   vertexBufferUsed = 0;

uint32_t indexBuffer[1000];
int      indexBufferUsed = 0;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

void AppendVertex(float x, float y, Color color)
{
    vertexBuffer[vertexBufferUsed++] = x;
    vertexBuffer[vertexBufferUsed++] = y;
    vertexBuffer[vertexBufferUsed++] = 0.5f;
    vertexBuffer[vertexBufferUsed++] = color.r;
    vertexBuffer[vertexBufferUsed++] = color.g;
    vertexBuffer[vertexBufferUsed++] = color.b;
    vertexBuffer[vertexBufferUsed++] = color.a;
}

void DrawRect(float x, float y, float width, float height, Color color)
{
    int startIndex = vertexBufferUsed / (3 + 4); // /7 = 1 vertex.

    AppendVertex(x, y, color);
    AppendVertex(x + width, y, color);
    AppendVertex(x + width, y + height, color);
    AppendVertex(x, y + height, color);

    indexBuffer[indexBufferUsed++] = startIndex + 0;
    indexBuffer[indexBufferUsed++] = startIndex + 1;
    indexBuffer[indexBufferUsed++] = startIndex + 2;

    indexBuffer[indexBufferUsed++] = startIndex + 0;
    indexBuffer[indexBufferUsed++] = startIndex + 2;
    indexBuffer[indexBufferUsed++] = startIndex + 3;
}

void DrawLine(Vector2F startPos, Vector2F endPos, Color color)
{
    Vector2F direction = endPos - startPos;
    printf("%f %f\n", direction.x, direction.y);
}

void DrawCircle(Vector2F center, float radius, int numSegments, Color color)
{
    int startIndex = vertexBufferUsed / 7; // 7 = 3 (position) + 4 (color) attributes per vertex

    const float angleStep = Calcul::TAU / numSegments;

    for (int i = 0; i < numSegments; i++)
    {
        float angle = i * angleStep;
        float x = center.x + radius * cos(angle);
        float y = center.y + radius * sin(angle);
        AppendVertex(x, y, color);
    }

    // Connect the last vertex with the first vertex to close the circle
    AppendVertex(center.x + radius, center.y, color);

    for (int i = 0; i < numSegments; i++)
    {
        indexBuffer[indexBufferUsed++] = startIndex + i;
        indexBuffer[indexBufferUsed++] = startIndex + i + 1;
        indexBuffer[indexBufferUsed++] = startIndex + numSegments;
    }
}

static void init(void) 
{
    sg_setup((sg_desc)
    {
        .context = sapp_sgcontext(),
        .logger.func = slog_func,
    });

    DrawRect(-0.25f, -0.25f, 0.5f, 0.5f, Color::yellow);
    DrawRect(-0.75f, -0.75f, 0.5f, 0.5f, Color::purple);
    DrawRect( 0.25f,  0.25f, 0.5f, 0.5f, Color::cyan);

    DrawCircle(Vector2F::zero, 0.5f, 5, Color::red);

    DrawLine(Vector2F::zero, Vector2F::one, Color::blue);

    state.bind.vertex_buffers[0] = sg_make_buffer((sg_buffer_desc)
    {
        .data = sg_range{ vertexBuffer, vertexBufferUsed * sizeof(*vertexBuffer) },
        .label = "triangle-vertices"
    });

    state.bind.index_buffer = sg_make_buffer((sg_buffer_desc)
    {
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = sg_range{ indexBuffer, indexBufferUsed * sizeof(*indexBuffer) },
        .label = "triangle-indices"
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
        .width = 640,
        .height = 480,
        .window_title = "Triangle (sokol-app)",
        .icon.sokol_default = true,
        .logger.func = slog_func,
    };
}