#pragma once
#include "../Common/MetaLib.h"
#include "PipelineComponent.h"
#include "../RenderComponent/RenderTexture.h"
#include "../Singleton/PSOContainer.h"
#include "../RenderComponent/CBufferPool.h"
class LightingComponent;
class LocalShadowComponent : public PipelineComponent
{
public:
	enum class ShadowType : bool
	{
		Spot = false,
		Cube = true
	};
	struct SpotCommand
	{
		float4x4 view;
		float3 right, up, forward, pos;
		float fov;
		float nearZ;
		float farZ;
	};
	struct PointCommand
	{
		float3 position;
		float range;
		float nearClipPlane;
	};
	struct ShadowCommand
	{
		ShadowType type;
		RenderTexture* shadowmap;
		uint lightIndex;
		union
		{
			SpotCommand spotLight;
			PointCommand pointLight;
		};
	};
private:
	std::vector<TemporalResourceCommand> tempResources;
	LightingComponent* lightingComp;
public:
	StackObject<RenderTexture> cubemapDepth;
	StackObject<PSOContainer> psoContainer;
	StackObject<CBufferPool> cullCBuffer;
	std::vector<ShadowCommand> shadowCommands;
	uint ProjectionShadowParams;
	virtual CommandListType GetCommandListType() { return CommandListType_Graphics; }
	virtual void Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	virtual void Dispose();
	virtual std::vector<TemporalResourceCommand>& SendRenderTextureRequire(const EventData& evt);
	virtual void RenderEvent(const EventData& data, ThreadCommand* commandList);
};