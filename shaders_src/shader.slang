struct VSInput
{
    float3 in_position;
    float2 in_tex_coord;
};

struct VertexOutput
{
    float4 pos: SV_Position;
    float2 frag_tex_coord;
};

[shader("vertex")]
VertexOutput Vert_main(VSInput input)
{
    VertexOutput output;
    float4 localPos = float4(input.in_position, 1.0);

    output.frag_tex_coord = input.in_tex_coord;
    return output;
}

Sampler2D texture;

[shader("fragment")]
float4 Frag_main(VertexOutput invVert): SV_Target
{
    return invVert.pos;
}
