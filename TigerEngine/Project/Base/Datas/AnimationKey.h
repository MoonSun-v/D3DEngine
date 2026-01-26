#pragma once
#include "../pch.h"

using namespace DirectX::SimpleMath;

//class AnimationKey
//{
//public:
//	float m_time;			//
//	Vector3 m_position;		//
//	Quaternion m_rotation;	// 
//	Vector3 m_scale;		// 
//};

// ------------------- Key Structs -------------------
struct PositionKey
{
    float Time = 0.0f;
    Vector3 Value = Vector3::Zero;
};
struct RotationKey
{
    float Time = 0.0f;
    Quaternion Value = Quaternion::Identity;
};
struct ScaleKey
{
    float Time = 0.0f;
    Vector3 Value = Vector3::One;
};