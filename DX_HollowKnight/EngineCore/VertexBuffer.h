#pragma once

// Ό³Έν :
class UVertexBuffer
{
public:
	// constrcuter destructer
	UVertexBuffer();
	~UVertexBuffer();

	// delete Function
	UVertexBuffer(const UVertexBuffer& _Other) = delete;
	UVertexBuffer(UVertexBuffer&& _Other) noexcept = delete;
	UVertexBuffer& operator=(const UVertexBuffer& _Other) = delete;
	UVertexBuffer& operator=(UVertexBuffer&& _Other) noexcept = delete;

protected:

private:

};

