#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

using ResourceName = std::string;
using ResourceCount = unsigned int;
using FileName = std::string;
using FilePath = std::string;

//hexadecimal numbers
//https://www.dropbox.com/s/qe0688gbpum5ue9/Screenshot%202016-10-05%2021.18.40.png?dl=0
//https://www.dropbox.com/s/t7zj5s4idk6h0tn/Screenshot%202016-10-05%2021.46.07.png?dl=0

template <class T>
class ResourceManager
{
public:
	ResourceManager() {}
	virtual ~ResourceManager() {}

	//Disallow copying of this class
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	bool requireResource(const std::string& id)
	{
		auto res = find(id);
		if (res) {
			res->second++;
			return true;
		}

		auto iter = m_filePaths.find(id);
		if (iter != m_filePaths.cend())
		{
			T* resource = loadFromFile(iter->second);
			if (resource)
			{
				m_resources.emplace(iter->first, std::make_pair(resource, 1));
				return true;
			}
		}
		return false;
	}

	T* getResource(const std::string& id)
	{
		auto res = find(id);
		return (res ? res->first : nullptr);
	}

	void releaseResource(const std::string& id)
	{
		auto res = find(id);
		if (res)
		{
			if (res->second)
			{
				--res->second;
			}
			//So as not to waste valuable memory
			//If need again, simply load new from file
			//Could be chance that program never uses the memory again
			else
			{
				delete res->first;
				res->first = nullptr;
				//removeResource(id);
			}
		}
	}

protected:
	std::unordered_map<ResourceName, std::pair<T*, ResourceCount>> m_resources;
	std::unordered_map<FileName, FilePath> m_filePaths;

	virtual T* loadFromFile(const std::string& fileName) = 0;

	void purgeResources()
	{
		for (auto &i : m_resources)
		{
			delete i.second.first;
		}
		m_resources.clear();
	}

	void registerFilePath(const std::string& name, const std::string& location)
	{
		if (!m_filePaths.emplace(std::make_pair(name, location)).second)
		{
			std::cerr << "Failed to register file path: '" << name << "'. Location: '" << location << "'\n";
		}
	}
	
private:
	//Note to self - Use find as a pointer, otehrwise copying it would be too expensive for this simple check
	std::pair<T*, ResourceCount>* find(const std::string& id)
	{
		auto iter = m_resources.find(id);
		return (iter != m_resources.cend() ? &iter->second : nullptr);
	}

	void removeResource(const std::string& id)
	{
		auto iter = m_resources.find(id);
		if (iter != m_resources.end())
		{
			//delete iter->second->first;
			//iter->second->first = nullptr;
			m_resources.erase(iter);
		}
	}
};