#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "Logger.h"
#include <vector>
#include <tuple>
#include "Score.h"

class FileLogger
{
	std::vector<Score> m_scores;
	std::string scoreFile;
	std::string LastNameUsed;

public:
	FileLogger() = default;
	FileLogger(std::string path);
	~FileLogger() = default;

	std::vector<Score>& GetScores() { return m_scores; }

	bool Check_Score(std::string, int, int, int);
	void save();
	void load();
	void NewScores(std::string name, int score, int minutes, int seconds);
	void draw(sf::RenderWindow*,sf::Font&);
};
#endif
