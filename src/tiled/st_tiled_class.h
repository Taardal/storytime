#pragma once

namespace Storytime {

    using TiledClassMemberValue = std::variant<bool, int, float, std::string>;

    struct TiledClassMember {
        std::string name = "";
        std::string type = "";
        TiledClassMemberValue value{};
    };

    struct TiledClass {
        std::string color = "";
        bool draw_fill = false;
        int id = 0;
        std::vector<TiledClassMember> members{};
        std::string name = "";
        std::string type = "";
        std::vector<std::string> use_as{};
    };
}

template<typename T>
void from_tiled_class(const Storytime::TiledClass&, T&) {
    static_assert(sizeof(T) == 0, "Unimplemented TiledClass deserialization");
}

template<typename T>
T from_tiled_class(const Storytime::TiledClass& tiled_object) {
    T value;
    from_tiled_class(tiled_object, value);
    return value;
}
