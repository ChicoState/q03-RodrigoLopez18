#include <gtest/gtest.h>
#include "Guesser.h"

class GuesserTest : public ::testing::Test
{
protected:
    GuesserTest() {}
    virtual ~GuesserTest() {}
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST(GuesserTest, SmokeTest)
{
    Guesser object("Secret");
    ASSERT_EQ(1 + 1, 2);
}

// Match function tests
TEST(GuesserTest, MultipleInstances)
{
    Guesser object("password");
    object.match("password");
    ASSERT_GT(object.remaining(), 0u);
}

TEST(GuesserTest, NoMoreAttempts)
{
    Guesser object("Rodrigo");
    ASSERT_FALSE(object.match("Rodri"));  //ran out of attempts
    ASSERT_FALSE(object.match("Rodr"));
    ASSERT_FALSE(object.match("Rodrig"));
    ASSERT_EQ(0u, object.remaining());
}

TEST(GuesserTest, IncorrectGuess)
{
    Guesser object("Secret");
    ASSERT_FALSE(object.match("known"));
}

TEST(GuesserTest, MatchOnLastAttempt)
{
    Guesser object("n");
    ASSERT_FALSE(object.match("rod")); 
    ASSERT_FALSE(object.match("mee"));
    ASSERT_TRUE(object.match("n"));
}

TEST(GuesserTest, BruteForce)
{
    Guesser object("rodrigo");
    ASSERT_FALSE(object.match("thisisabruteforceattempttobreak")); // locks
    ASSERT_FALSE(object.match("rodrigo")); // still false because locked
}

TEST(GuesserTest, CharacterMismatch)
{
    Guesser object("password");
    ASSERT_FALSE(object.match("passwords"));
}

TEST(GuesserTest, EmptySecret)
{
    Guesser object("");
    ASSERT_TRUE(object.match(""));
}


TEST(GuesserTest, DistanceFirstAttempt)
{
    Guesser object("password");
    ASSERT_TRUE(object.match("password")); // distance is 0, match succeeds
}

TEST(GuesserTest, DistanceShortChar)
{
    Guesser object("password");
    ASSERT_FALSE(object.match("passwords")); // decrement attempts
    ASSERT_EQ(2u, object.remaining());
}

TEST(GuesserTest, DistanceBruteForce)
{
    Guesser object("password");
    ASSERT_FALSE(object.match("mmmmmm")); 
    ASSERT_EQ(0u, object.remaining()); 
}

TEST(GuesserTest, DistanceCount32)
{
    Guesser object("12345123451234512345123451234500");
    ASSERT_TRUE(object.match("12345123451234512345123451234500")); // match exact 32
    Guesser object2("12345123451234512345123451234500");
    ASSERT_FALSE(object2.match("")); // empty input, big distance, locks
    ASSERT_EQ(0u, object2.remaining());
}

TEST(GuesserTest, RanOutOfAttempts)
{
    Guesser object("password");
    object.match("passwor");
    ASSERT_EQ(2u, object.remaining());
    object.match("passworf");
    ASSERT_EQ(1u, object.remaining());
    object.match("passworm");
    ASSERT_EQ(0u, object.remaining());
    ASSERT_FALSE(object.match("password")); // should stay locked
    ASSERT_EQ(0u, object.remaining());
}
