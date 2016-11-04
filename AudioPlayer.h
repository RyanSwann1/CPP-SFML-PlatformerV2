#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <SFML\Audio.hpp>

struct AudioClip
{
	AudioClip(const std::string& audioClipName, sf::Sound* audioClip)
		: m_name(audioClipName),
		m_audioClip(audioClip)
	{}
	
	std::string m_name;
	sf::Sound* m_audioClip;
};

using AudioClips = std::vector<AudioClip>;
//using AudioClips = std::vector<std::pair<std::string, sf::Sound*>>;
struct SharedContext;
class AudioPlayer
{
public:
	AudioPlayer(SharedContext* sharedContext);
	~AudioPlayer();

	void registerOwner(const std::string& ownerID);
	void registerAudioClip(const std::string& audioOwner, const std::string& ClipName);
	void play(const std::string& audioClipName, const bool& loop);
	void stop(const std::string& audioClipName);

private:
	std::unordered_map<std::string, AudioClips> m_owners; //String is owner - IE'Player'
	SharedContext* m_sharedContext;
	std::string m_currentOwner;
	AudioClips* m_currentAudioClips;

	sf::Sound* findAudioClip(const std::string& audioClipName);
	void loadInAudioClips(const std::string& fileName);
};