#include "PreCompile.h"
#include "ContentsRenderer.h"

ContentsRenderer::ContentsRenderer()
{
	CreateRenderUnit();
	SetMesh("Rect");
	SetMaterial("MyMaterial");

	MyColor.Albedo = float4(1.0f, 1.0f, 1.0f, 1.0f);

	GetRenderUnit().ConstantBufferLinkData("FMyColor", MyColor);
}

ContentsRenderer::~ContentsRenderer()
{
}

