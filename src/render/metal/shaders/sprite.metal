#include <metal_stdlib>
using namespace metal;

struct SpriteVertexIn {
    float2 position [[attribute(0)]];
    float2 uv       [[attribute(1)]];
    float  alpha    [[attribute(2)]];
};

struct SpriteVertexOut {
    float4 position [[position]];
    float2 uv;
    float  alpha;
};

vertex SpriteVertexOut sprite_vertex(SpriteVertexIn in                    [[stage_in]],
                                     constant float4x4& projection        [[buffer(1)]])
{
    SpriteVertexOut out;
    out.position = projection * float4(in.position, 0.0, 1.0);
    out.uv       = in.uv;
    out.alpha    = in.alpha;
    return out;
}

fragment float4 sprite_fragment(SpriteVertexOut  in      [[stage_in]],
                                texture2d<float> tex     [[texture(0)]],
                                sampler          sampler [[sampler(0)]])
{
    float4 color = tex.sample(sampler, in.uv);
    color.a *= in.alpha;
    return color;
}