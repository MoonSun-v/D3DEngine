#pragma once
#include "pch.h"
#include "Datas/Mesh.h"
#include "Datas/Bone.h"
#include "Datas/FBXResourceData.h"
#include "Datas/ConstantBuffer.hpp"

struct RenderItem
{
    // model
    ModelType modelType;
    const Mesh* mesh;
    Material material;
    
    // matrix
    Matrix world;       // static, rigid, skeletal
    Matrix model;       // rigid

    // skeleton
    int boneCount;
    int refBoneIndex;
    const PoseMatrixCB* poses;
    const OffsetMatrixCB* offsets;
};

/// <summary>
/// RenderQueue는 처리한 렌더 item들을 보관한다.
/// </summary>
class RenderQueue
{
public:
    void AddOpaqueQueue(const RenderItem& item)
    {
        opaqueQueue.push_back(item);
    }

    void AddTransparentQueue(const RenderItem& item)
    {
        transparentQueue.push_back(item);
    }

    const auto& GetOpaqueQueue()  const { return opaqueQueue; }
    const auto& GetTransparentQueue()  const { return transparentQueue; }

    /// <summary>
    /// Queue 클래스에 있는 모든 배열 초기화
    /// </summary>
    void Clear()
    {
        opaqueQueue.clear();
        transparentQueue.clear();
    };

private:
    // 각 타입별로 컨테이너 추가하기
    std::vector<RenderItem> opaqueQueue;        // 불투명 오브젝트 -> Deffered Rendering
    std::vector<RenderItem> transparentQueue;   // 투명 오브젝트 -> Forward Rendering
};

