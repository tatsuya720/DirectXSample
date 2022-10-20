struct VSOutput
{
    float4 Pos: SV_Position;
};

VSOutput VS(float4 pos : POSITION)
{
    VSOutput Out;
    Out.Pos = pos;

    return Out;
}

float4 PS(VSOutput In) : SV_Target0
{
    return float4(0, 1, 0, 1);
}