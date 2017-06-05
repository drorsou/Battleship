#include  "Utils.h"


bool Utils::checkArgs(int argc, char** argv)
{
	if (argc < 2) // No arguments
		TournamentManager::emptyPath();
	else
	{
		std::queue<std::string> queue;
		Utils::splitString(std::string(argv[1]), queue, '=');

		if (queue.size() == 2) // First argument has a single '='
		{
			if (Utils::setThreadsFromQueue(queue) == false)
				return false;
		}
		else if (queue.size() == 1) // First argument has no '='
		{
			queue.pop();
			TournamentManager::setPath(argv[1]);
			if (argc > 2)
			{
				Utils::splitString(std::string(argv[2]), queue, '=');

				if (queue.size() == 2) // Second argument has a single '='
				{
					if (Utils::setThreadsFromQueue(queue) == false)
						return false;
				}
				else // Second argument is unrecognized
				{
					std::cout << "Error: illegal parameter" << std::endl;
					return false;
				}
			}

		}
		else // First argument is unrecognized
		{
			std::cout << "Error: illegal parameter" << std::endl;
			return false;
		}
	}

	return true;
}


bool Utils::setThreadsFromQueue(std::queue<std::string>& queue)
{
	if (std::string{ "-threads=" }.compare(queue.front()) != 0)
	{
		queue.pop();
		TournamentManager::setThreads(std::stoi(queue.front()));
		return true;
	}

	std::cout << "Error: illegal parameter" << std::endl;
	return false;
}


void Utils::splitString(const std::string& str, std::queue<std::string>& queue, char delim)
{
	std::stringstream stream(str);
	std::string token;
	while (std::getline(stream, token, '='))
	{
		queue.push(token);
	}
}