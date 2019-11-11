#pragma once


namespace storytime {

    class Timestep {
    private:
        float time;

    public:
        Timestep(float time = 0.0f);

        [[nodiscard]] float getSeconds() const;

        [[nodiscard]] float getMilliseconds() const;

        operator float() const;
    };

}


