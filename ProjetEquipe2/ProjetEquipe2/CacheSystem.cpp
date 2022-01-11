#include "CacheSystem.h"

gm::Cache<sf::Texture>gm::CacheSystem::m_texture;
gm::Cache<sf::Font>gm::CacheSystem::m_font;
gm::Cache<sf::SoundBuffer>gm::CacheSystem::m_sound;

template<>
std::string gm::CacheSystem::CheckPath<sf::Texture>(std::string path, std::string name)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().extension() == "")
		{
			if (CheckPath<sf::Texture>(entry.path().string(), name) != "")
				return CheckPath<sf::Texture>(entry.path().string(), name);
		}

		if (entry.path().stem() == name)
		{
			if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg")
				return entry.path().string();
		}
	}

	return "";
}

template<>
std::string gm::CacheSystem::CheckPath<sf::Font>(std::string path, std::string name)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().extension() == "")
		{
			if (CheckPath<sf::Font>(entry.path().string(), name) != "")
				return CheckPath<sf::Font>(entry.path().string(), name);
		}

		if (entry.path().stem() == name)
		{
			if (entry.path().extension() == ".ttf" || entry.path().extension() == ".otf")
				return entry.path().string();
		}
	}

	return "";
}

template<>
std::string gm::CacheSystem::CheckPath<sf::SoundBuffer>(std::string path, std::string name)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.path().extension() == "")
		{
			if (CheckPath<sf::SoundBuffer>(entry.path().string(), name) != "")
				return CheckPath<sf::SoundBuffer>(entry.path().string(), name);
		}

		if (entry.path().stem() == name)
		{
			if (entry.path().extension() == ".wav" || entry.path().extension() == ".ogg")
				return entry.path().string();
		}
	}

	return "";
}