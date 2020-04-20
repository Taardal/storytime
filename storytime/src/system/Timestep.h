#pragma once


namespace storytime
{
    class Timestep
    {
    private:
        float time;

    public:
        Timestep(float time = 0.0f);

        [[nodiscard]] float GetSeconds() const;

        [[nodiscard]] float GetMilliseconds() const;

        operator float() const;
    };

}


