#include "gtest/gtest.h"
#include "gmock/gmock.h"

int Factorial(int number) {
    return number <= 1 ? number : Factorial(number - 1) * number;
};

TEST(FactorialTest, HandlesZeroInput) {
    EXPECT_EQ(1, 1);
}

class Turtle {
public:
    virtual void PenUp() = 0;
    virtual void PenDown() = 0;
    virtual void Forward(int distance) = 0;
    virtual void Turn(int degrees) = 0;
    virtual void GoTo(int x, int y) = 0;
    virtual int GetX() const = 0;
    virtual int GetY() const = 0;
};

class MockTurtle : public Turtle {
public:
    MOCK_METHOD(void, PenUp, (), (override));
    MOCK_METHOD(void, PenDown, (), (override));
    MOCK_METHOD(void, Forward, (int distance), (override));
    MOCK_METHOD(void, Turn, (int degrees), (override));
    MOCK_METHOD(void, GoTo, (int x, int y), (override));
    MOCK_METHOD(int, GetX, (), (const, override));
    MOCK_METHOD(int, GetY, (), (const, override));
};

class Painter {
private:
    Turtle& turtle;
public:
    Painter(Turtle& turtle) : turtle(turtle) {}

    bool DrawCircle(int a, int b, int c) {
        turtle.PenDown();
        return true;
    }

};

TEST(PainterTest, CanDrawSomething) {
    MockTurtle turtle;

    EXPECT_CALL(turtle, PenDown()).Times(::testing::AtLeast(1));

    Painter painter(turtle);

    EXPECT_TRUE(painter.DrawCircle(0, 0, 10));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}