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

[shader("vertex")]
VertexOutput Vert_main(VSInput input)
{
    VertexOutput output;
    float4 localPos = float4(input.in_position * 0.5, 1.0); // skaluj, żeby cała kostka mieściła się w z: 0..1 po prostym triku
    output.pos = localPos;
    output.pos.z = output.pos.z * 0.5 + 0.5; // wepchnij Z z zakresu [-0.5,0.5] w [0,1] dla Vulkan NDC
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
