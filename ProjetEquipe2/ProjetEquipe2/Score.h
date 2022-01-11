#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <istream>
#include <ostream>

class Score
{
    std::string name;
    int score;
    int minutes;
    int seconds;

public:
    Score() : name("----"), score(0), minutes(0), seconds(0) {}
    Score(std::string n, int s) : name(n), score(s), minutes(0), seconds(0) {}
    Score(std::string n, int s, int m, int sec) : name(n), score(s), minutes(m), seconds(sec) {}

    int GetScore()const { return score; }
    std::string getName()const { return name; }
    int GetMin()const { return minutes; }
    int GetSecs()const { return seconds; }

    bool operator<(Score const& s) const { return score < s.score; }

    friend std::istream& operator>>(std::istream& is, Score& s) {
        if (is >> s.name) {
            is.ignore(1);
            if (is >> s.score) {
                is.ignore(1);
            }
            if (is >> s.minutes) {
                is.ignore(1);
                is >> s.seconds;
            }
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const Score& s) {
        return os << s.name << ' ' << s.score << ' ' << s.minutes << ' ' << s.seconds;
    }
};
#endif


