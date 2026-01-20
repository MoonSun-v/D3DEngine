#pragma once
#include "../pch.h"

class Material
{
public:
    // ----- [ Has Flag ] ----- 
    // Texture Flag
    BOOL hasDiffuse = false;
    BOOL hasNomal = false;
    BOOL hasRoughness = false;
    BOOL hasMetallic = false;
    BOOL hasEmissive = false;

    // PBR roughness가 aiTextureType_SHININESS 텍스처로 들어오는 경우
    BOOL roughnessFromShininess;


    // ----- [ Values ] ----- 
    // Factor
    float emissiveFactor;
    float metallicFactor;
    float roughnessFactor;

    // Override
    BOOL useBaseColorOverride;
    BOOL useEmissiveOverride;
    BOOL useMetallicOverride;
    BOOL useRoughnessOverride;

    Vector3 baseColorOverride;
    float   metallicOverride;
    Vector3 emissiveOverride;
    float   roughnessOverride;

public:
    // TODO :: Script Component에서 사용 가능한 Util Funcs 추가
};

