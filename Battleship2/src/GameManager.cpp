#include "GameManager.h"


int GameManager::threads = 4;


void splitString(const std::string& str, std::queue<std::string>& queue, char delim)
{
	std::stringstream stream(str);
	std::string token;
	while (std::getline(stream, token, '='))
	{
		queue.push(token);
	}
}



int main(int argc, char* argv[])
{
	std::string path;

	/*
	* Check for arguments
	*/

	if (argc < 2) // No arguments
		path = ".";
	else
	{
		std::queue<std::string> queue;
		splitString(std::string(argv[1]), queue, '=');

		if (queue.size() == 2) // First argument has a single '='
		{
			if (GameManager::setThreadsFromQueue(queue) == false)
				return EXIT_FAILURE;
		}
		else if (queue.size() == 1) // First argument has no '='
		{
			queue.pop();
			path.append(argv[1]);
			if (argc > 2)
			{
				splitString(std::string(argv[2]), queue, '=');

				if (queue.size() == 2) // Second argument has a single '='
				{	
					if (GameManager::setThreadsFromQueue(queue) == false)
						return EXIT_FAILURE;
				}
				else // Second argument is unrecognized
				{
					std::cout << "Error: illegal parameter" << std::endl;
					return EXIT_FAILURE;
				}
			}

		}
		else // First argument is unrecognized
		{
			std::cout << "Error: illegal parameter" << std::endl;
			return EXIT_FAILURE;
		}
	}
	

	if (!FileReader::checkIsValidDir(path))
	{
		FileReader::printError(FileReader::Error::PATH, path);
		return false;
	}


	// Create threads
	// Read boards
	// Read DLLs
	// Run tournament
	std::cout << "Number of legal players: " << GameManager::getThreads() << std::endl;
	return 0;
}


bool GameManager::setThreadsFromQueue(std::queue<std::string>& queue)
{
	if (std::string{ "-threads=" }.compare(queue.front()) != 0)
	{
		queue.pop();
		GameManager::setThreads(std::stoi(queue.front()));
		return true;
	}

	std::cout << "Error: illegal parameter" << std::endl;
	return false;
}