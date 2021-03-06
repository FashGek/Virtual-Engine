#pragma once
#include "IGPUResource.h"
class FrameResource;
struct StructuredBufferElement
{
	uint64_t stride;
	uint64_t elementCount;
	static constexpr StructuredBufferElement Get(uint64_t stride, uint64_t elementCount)
	{
		return { stride, elementCount };
	}
};

enum StructuredBufferState
{
	StructuredBufferState_UAV = D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
	StructuredBufferState_Indirect = D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
	StructuredBufferState_Read = D3D12_RESOURCE_STATE_GENERIC_READ
};

class StructuredBuffer final : public IGPUResource
{
private:
	std::vector<StructuredBufferElement> elements;
	std::vector<uint64_t> offsets;
public:
	StructuredBuffer(
		ID3D12Device* device,
		StructuredBufferElement* elementsArray,
		UINT elementsCount,
		bool isIndirect = false,
		bool isReadable = false
	);
	uint64_t GetStride(UINT index) const;
	uint64_t GetElementCount(UINT index) const;
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress(UINT element, UINT index) const;
	uint64_t GetAddressOffset(UINT element, UINT index) const;
	void TransformBufferState(ID3D12GraphicsCommandList* commandList, StructuredBufferState beforeState, StructuredBufferState afterState) const;
};
/*
class StructuredBufferStateBarrier final
{
	StructuredBuffer* sbuffer;
	D3D12_RESOURCE_STATES beforeState;
	D3D12_RESOURCE_STATES afterState;
	ID3D12GraphicsCommandList* commandList;
public:
	StructuredBufferStateBarrier(ID3D12GraphicsCommandList* commandList, StructuredBuffer* sbuffer, StructuredBufferState beforeState, StructuredBufferState afterState) :
		beforeState((D3D12_RESOURCE_STATES)beforeState), afterState((D3D12_RESOURCE_STATES)afterState), sbuffer(sbuffer),
		commandList(commandList)
	{
		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			sbuffer->GetResource(),
			this->beforeState,
			this->afterState
		));
	}
	~StructuredBufferStateBarrier()
	{
		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			sbuffer->GetResource(),
			afterState,
			beforeState
		));
	}
};*/