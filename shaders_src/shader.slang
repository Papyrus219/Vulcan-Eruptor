struct VSInput
{
    float3 in_position;
    float3 normals;
    float2 in_tex_coord;
};

struct VertexOutput
{
    float4 pos: SV_Position;
    float3 normals;
    float2 frag_tex_coord;
};

struct Unifrom_buffer_mvp
{
    float4 model;
    float4 view;
    float4 projection;
};
ConstantBuffer<Unifrom_buffer_mvp> ub_mvp;

[shader("vertex")]
VertexOutput Vert_main(VSInput input)
{
    VertexOutput output;
    output.pos = mul(ub_mvp.projection, mul(ub_mvp.view, mul(ub_mvp.model, float4(input.in_position, 1.0))));
    output.frag_tex_coord = input.in_tex_coord;
    output.normals = input.normals;
    return output;
}

Sampler2D texture;

[shader("fragment")]
float4 Frag_main(VertexOutput invVert): SV_Target
{
    return float4(1.0, 0.5, 0.2, 1.0);
}
