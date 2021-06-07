#pragma once


namespace storytime
{
    class Timestep
    {
    private:
        float time;

    public:
        Timestep(float time = 0.0f);

        float GetSeconds() const;

        float GetMilliseconds() const;

        operator float() const;
    };

}


