#include "FileLogger.h"
#include <fstream>
#include <sstream>
#include "Functions.h"
#include "CacheSystem.h"
#include <filesystem>

bool FileLogger::Check_Score(std::string name, int score, int minutes, int seconds)
{
	Score m_score = Score(name, score, minutes, seconds);

	load();
	for (auto i = std::begin(m_scores); i != std::end(m_scores); i++) {
		if (*i < m_score)
			return true;
	}
	return false;
}

void FileLogger::save()
{
	const char* userpath = std::getenv("APPDATA");
	scoreFile = scoreFile.assign(userpath) + "/GreatSteamrogue/Scores";

	std::replace(std::begin(scoreFile), std::end(scoreFile), '\\', '/');

	std::ofstream ScoreFile;
	ScoreFile.open(scoreFile + "/" + "score.txt");

	for (auto i = std::begin(m_scores); i != std::end(m_scores); i++) {
		ScoreFile << *i;
		if (i++ != std::end(m_scores)) {
			ScoreFile << std::endl;
			i--;
		}
	}

	m_scores.clear();
	ScoreFile.close();
}

void FileLogger::load()
{
	const char* userpath = std::getenv("APPDATA");
	scoreFile = scoreFile.assign(userpath) + "/GreatSteamrogue/Scores";

	std::replace(std::begin(scoreFile), std::end(scoreFile), '\\', '/');

	std::filesystem::create_directories(scoreFile);

	std::ifstream ScoreFile;
	ScoreFile.open(scoreFile + "/" + "score.txt", std::ios::in);

	if (ScoreFile.fail()) {
		std::ofstream ScoreFile1(scoreFile + "/" + "score.txt");

		Score New;
		for (int i = 0; i < 11; i++) {
			ScoreFile1 << New;
			if (i < 10)
				ScoreFile1 << std::endl;
		}
		ScoreFile1.close();

		ScoreFile.open(scoreFile + "/" + "score.txt", std::ios::in);
	}

	Score score_buffer;

	if (ScoreFile.is_open()) {
		while (!ScoreFile.eof()) {
			ScoreFile >> score_buffer;
			m_scores.push_back(score_buffer);
		}
		ScoreFile.close();
	}
}

void FileLogger::NewScores(std::string name, int score, int minutes, int seconds)
{
	Score m_score = Score(name, score, minutes, seconds);

	load();

	for (auto i = std::begin(m_scores); i != std::end(m_scores); i++) {
		if (*i < m_score)
			std::swap(*i, m_score);
	}

	save();
}

void FileLogger::draw(sf::RenderWindow* window,sf::Font& font)
{
	for (int i = 0; i < m_scores.size(); i++) {
		window->draw(func::CreateText(m_scores.at(i).getName(), font, 30, sf::Vector2f(500, 352 + (i * 40.5)), sf::Color::Black));
		window->draw(func::CreateText(std::to_string(m_scores.at(i).GetScore()), font, 30, sf::Vector2f(1120, 352 + (i * 40.5)), sf::Color::Black));
		window->draw(func::CreateText(std::to_string(m_scores.at(i).GetMin()) + "min", font, 30, sf::Vector2f(1490, 352 + (i * 40.5)), sf::Color::Black));
		window->draw(func::CreateText(std::to_string(m_scores.at(i).GetSecs()) + "sec", font, 30, sf::Vector2f(1590, 352 + (i * 40.5)), sf::Color::Black));
	}
}