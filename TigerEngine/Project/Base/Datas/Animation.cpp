#include "Animation.h"
#include <stdexcept>

void Animation::CreateNodeAnimation(aiAnimation* pAiAnimation)
{
	m_name = pAiAnimation->mName.C_Str();
	m_tick = pAiAnimation->mTicksPerSecond; // 해당 값이 0인 FBX가 들어올 수도 있다고 함. 
    if (m_tick == 0)
        m_tick = 25.0f; // Assimp 권장 기본값

	m_duration = pAiAnimation->mDuration / m_tick;

	// 본에 대한 키 애니메이션 저장
	for (int i = 0; i < pAiAnimation->mNumChannels; i++)
	{
		aiNodeAnim* pAiNodeAnim = pAiAnimation->mChannels[i];

		NodeAnimation nodeAnim;
		nodeAnim.m_nodeName = pAiNodeAnim->mNodeName.C_Str();
		nodeAnim.CreateKeys(pAiNodeAnim, m_tick);
		m_nodeAnimations.push_back(nodeAnim);
		m_mappingNodeAnimations.insert({ nodeAnim.m_nodeName, i });
	}
}

bool Animation::GetNodeAnimationByName(string boneName, NodeAnimation& out)
{
	auto anim = m_mappingNodeAnimations.find(boneName);
	if (anim == m_mappingNodeAnimations.end())
	{
		return false;
	}

	int index = anim->second;
	out = m_nodeAnimations[index];

	return true;
}
