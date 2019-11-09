#pragma once


namespace Darkle {

    class Timestep {
    private:
        double time;

    public:
        Timestep(double time = 0.0);

        [[nodiscard]] double getSeconds() const;

        [[nodiscard]] double getMilliseconds() const;

        operator double() const;
    };

}


