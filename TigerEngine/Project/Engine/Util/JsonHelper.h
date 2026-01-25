#pragma once
#include "pch.h"

class JsonHelper
{
public:
    template<typename T>
    static nlohmann::json MakeSaveData(const T* typePtr);

    template<typename T>
    static void SetDataFromJson(T* typePtr, nlohmann::json data);
};

template<typename T>
nlohmann::json JsonHelper::MakeSaveData(const T* typePtr)
{
    nlohmann::json datas;

    rttr::type t = rttr::type::get(*typePtr);
    datas["type"] = t.get_name().to_string();
    datas["properties"] = nlohmann::json::object(); // 객체 생성

    for (auto& prop : t.get_properties())
    {
        std::string propName = prop.get_name().to_string();
        rttr::variant value = prop.get_value(*typePtr);

        if (value.is_type<float>())
        {
            auto v = value.get_value<float>();
            datas["properties"][propName] = v;
        }
        else if (value.is_type<int>())
        {
            auto v = value.get_value<int>();
            datas["properties"][propName] = v;
        }
        else if (value.is_type<bool>())
        {
            auto v = value.get_value<bool>();
            datas["properties"][propName] = v;
        }
        else if (value.is_type<Color>())
        {
            auto v = value.get_value<Color>();
            datas["properties"][propName] = { v.x, v.y, v.z, v.w };
        }
        else if (value.is_type<std::string>())
        {
            auto v = value.get_value<std::string>();
            datas["properties"][propName] = v;
        }
    }

    return datas;
}

template<typename T>
inline void JsonHelper::SetDataFromJson(T* typePtr, nlohmann::json data)
{
    auto propData = data["properties"];

    rttr::type t = rttr::type::get(*typePtr);
    for (auto& prop : t.get_properties())
    {
        std::string propName = prop.get_name().to_string();
        rttr::variant value = prop.get_value(*typePtr);
        if (!propData.contains(propName)) continue;

        if (value.is_type<float>())
        {
            float data = propData[propName];
            prop.set_value(*typePtr, data);
        }
        else if (value.is_type<int>())
        {
            int data = propData[propName];
            prop.set_value(*typePtr, data);
        }
        else if (value.is_type<bool>())
        {
            bool data = propData[propName];
            prop.set_value(*typePtr, data);
        }
        else if (value.is_type<Color>())
        {
            Color color = { propData["Color"][0], propData["Color"][1], propData["Color"][2], propData["Color"][3] };
            prop.set_value(*typePtr, color);
        }
        else if (value.is_type<std::string>())
        {
            std::string data = propData[propName];
            prop.set_value(*typePtr, data);
        }
    }
}
