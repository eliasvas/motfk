#ifndef SHADERS_INL
#define SHADERS_INL
// OpenGL ES 3.0 shaders

// TODO -- in the far future, we can use standard GLSL, and depending on 
//backend (desktop/ES3/ES2/?) to transpile, shouldn't be hard 

static const char *fullscreen_col_vert= 
"#version 300 es\n"
"precision mediump float;\n"
"layout (location = 0) in vec2 in_pos; // NDC screen coords [-1,+1] \n"
"uniform vec4 color;\n"
"uniform float zoom_factor;\n"
"out vec4 c;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(zoom_factor * in_pos.xy, 0.0, 1.0);\n"
"   c = color;\n"
"}\n\0";

static const char *fullscreen_col_frag= 
"#version 300 es\n"
"precision mediump float;\n"
"in vec4 c;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = c;\n"
"}\n\0";





static const char *batch_vert= 
"#version 300 es\n"
"precision mediump float;\n"
"layout (location = 0) in vec2 in_pos;\n"
"layout (location = 1) in vec2 in_texcoord;\n"
"layout (location = 2) in vec4 in_color;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"out vec2 tc;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"   gl_Position = proj * view * vec4(in_pos.x, in_pos.y, 0.0, 1.0);\n"
"   tc = in_texcoord;\n"
"   color = in_color;\n"
"}\0";

static const char *batch_frag= 
"#version 300 es\n"
"precision mediump float;\n"
"uniform sampler2D Tex;\n"
"in vec2 tc;\n"
"in vec4 color;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   vec4 tex_col = texture(Tex, tc/vec2(textureSize(Tex,0)));\n"
"   FragColor = tex_col*color;\n"
"}\n\0";



static const char* gui_vs =
"#version 300 es\n"
"precision mediump float;\n"
"layout(location = 0) in vec2 inPos0;\n"
"layout(location = 1) in vec2 inPos1;\n"
"layout(location = 2) in vec2 inUV0;\n"
"layout(location = 3) in vec2 inUV1;\n"
"layout(location = 4) in vec4 inColor;\n"
"layout(location = 5) in float inCornerRadius;\n"
"layout(location = 6) in float inEdgeSoftness;\n"
"layout(location = 7) in float inBorderThickness;\n"
"\n"
"uniform vec2 winDim;\n"
"\n"
"out vec2 fragUV;\n"
"out vec2 fragDstPos;\n"
"out vec2 fragDstCenter;\n"
"out vec2 fragHalfSize;\n"
"out vec4 fragColor;\n"
"out float fragCornerRadius;\n"
"out float fragEdgeSoftness;\n"
"out float fragBorderThickness;\n"
"\n"
"const vec2 vertices[4] = vec2[4](\n"
"    vec2(-1, -1),\n"
"    vec2(-1, +1),\n"
"    vec2(+1, -1),\n"
"    vec2(+1, +1)\n"
");\n"
"\n"
"void main() {\n"
"    vec2 dstHalfSize = (inPos1 - inPos0) / 2.0;\n"
"    vec2 dstCenter = (inPos1 + inPos0) / 2.0;\n"
"    vec2 dstPos = vertices[gl_VertexID] * dstHalfSize + dstCenter;\n"
"\n"
"    gl_Position = vec4(2.0 * dstPos / winDim - 1.0, 0.0, 1.0);\n"
"    gl_Position.y *= -1.0;\n"
"\n"
"    vec2 uvHalfSize = (inUV1 - inUV0) / 2.0;\n"
"    vec2 uvCenter = (inUV1 + inUV0) / 2.0;\n"
"    vec2 uvPos = vertices[gl_VertexID] * uvHalfSize + uvCenter;\n"
"\n"
"    fragUV = uvPos;\n"
"    fragColor = inColor;\n"
"    fragCornerRadius = inCornerRadius;\n"
"    fragEdgeSoftness = inEdgeSoftness;\n"
"    fragBorderThickness = inBorderThickness;\n"
"    fragDstPos = dstPos;\n"
"    fragDstCenter = dstCenter;\n"
"    fragHalfSize = dstHalfSize;\n"
"}\n";

static const char* gui_fs =
"#version 300 es\n"
"precision mediump float;\n"
"in vec2 fragUV;\n"
"in vec2 fragDstPos;\n"
"in vec2 fragDstCenter;\n"
"in vec2 fragHalfSize;\n"
"in vec4 fragColor;\n"
"in float fragCornerRadius;\n"
"in float fragEdgeSoftness;\n"
"in float fragBorderThickness;\n"
"\n"
"out vec4 outColor;\n"
"\n"
"uniform sampler2D texture0;\n"
"\n"
"float roundedRectSDF(vec2 samplePos, vec2 rectCenter, vec2 rectHalfSize, float r) {\n"
"    vec2 d2 = abs(rectCenter - samplePos) - rectHalfSize + vec2(r, r);\n"
"    return min(max(d2.x, d2.y), 0.0) + length(max(d2, 0.0)) - r;\n"
"}\n"
"\n"
"void main() {\n"
"    ivec2 texSize = textureSize(texture0, 0);\n"
"    float col = texture(texture0, fragUV / vec2(texSize)).r;\n"
"    vec4 tex = vec4(col, col, col, col);\n"
"\n"
"    float softness = fragEdgeSoftness + 0.001;\n"
"    float cornerRadius = fragCornerRadius;\n"
"    vec2 softnessPadding = vec2(max(0.0, softness * 2.0 - 1.0), max(0.0, softness * 2.0 - 1.0));\n"
"\n"
"    float dist = roundedRectSDF(fragDstPos, fragDstCenter, fragHalfSize - softnessPadding, cornerRadius);\n"
"    float sdfFactor = 1.0 - smoothstep(0.0, 2.0 * softness, dist);\n"
"\n"
"    float borderFactor = 1.0;\n"
"    if (fragBorderThickness != 0.0) {\n"
"        vec2 interiorHalfSize = fragHalfSize - vec2(fragBorderThickness);\n"
"        float interiorRadiusReduceF = min(interiorHalfSize.x / fragHalfSize.x, interiorHalfSize.y / fragHalfSize.y);\n"
"        float interiorCornerRadius = fragCornerRadius * interiorRadiusReduceF * interiorRadiusReduceF;\n"
"\n"
"        float insideD = roundedRectSDF(fragDstPos, fragDstCenter, interiorHalfSize - softnessPadding, interiorCornerRadius);\n"
"        float insideF = smoothstep(0.0, 2.0 * softness, insideD);\n"
"        borderFactor = insideF;\n"
"    }\n"
"\n"
"    outColor = fragColor * tex * sdfFactor * borderFactor;\n"
"    if (outColor.a < 0.01) discard;\n"
"}\n";

#endif