struct EngineVertex
{
    float4 POSITION : POSITION;
    float4 COLOR : COLOR;
};

// ���ؽ� ���̴��� ������ ���ϰ��� �־�� �մϴ�.
// ��ǲ�����2�� �ѱ� ���� ����������ϴµ�.
// �̶��� ��Ģ�� �ֽ��ϴ�.

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