#pragma once

#include <string>

namespace storytiled {

    class World {
    private:
        const char* tiledVersion;
        const char* type;
        int width;
        int height;
        int tileWidth;
        int tileHeight;
        bool infinite;

    public:
        [[nodiscard]] const char* getTiledVersion() const;

        void setTiledVersion(const char* tiledVersion);

        [[nodiscard]] const char* getType() const;

        void setType(const char* type);

        [[nodiscard]] int getWidth() const;

        void setWidth(int width);

        [[nodiscard]] int getHeight() const;

        void setHeight(int height);

        [[nodiscard]] int getTileWidth() const;

        void setTileWidth(int tileWidth);

        [[nodiscard]] int getTileHeight() const;

        void setTileHeight(int tileHeight);

        [[nodiscard]] bool isInfinite() const;

        void setInfinite(bool infinite);
    };

}


