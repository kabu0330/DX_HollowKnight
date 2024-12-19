struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 COLOR : COLOR;
};

// 버텍스 쉐이더는 무조건 리턴값이 있어야 합니다.
// 인풋어셈블러2로 넘길 값을 리턴해줘야하는데.
// 이때도 규칙이 있습니다.

struct VertexShaderOutPut
{
    float4 SVPOSITION : SV_POSITION;
    float4 COLOR : COLOR;
};

VertexShaderOutPut VertexToWorld(EngineVertex _Vertex)
{
    VertexShaderOutPut OutPut;
	
	// _Vertex 0.5, 0.5
    OutPut.SVPOSITION = _Vertex.POSITION;
    OutPut.COLOR = _Vertex.COLOR;

    return OutPut;
}

float4 PixelToWorld(VertexShaderOutPut _Vertex) : SV_Target0
{
    if (_Vertex.SVPOSITION.x < 640)
    {
        if (_Vertex.SVPOSITION.y < 360)
        {
            return float4(1.0f, 1.0f, 0.0f, 1.0f);
        }
        else
        {
            return float4(1.0f, 0.0f, 0.0f, 1.0f);
        }
    }
    else
    {
        if (_Vertex.SVPOSITION.y < 360)
        {
            return float4(1.0f, 0.0f, 1.0f, 1.0f);
        }
        else
        {
            return float4(0.0f, 1.0f, 0.0f, 1.0f);
        }
    }
    



}