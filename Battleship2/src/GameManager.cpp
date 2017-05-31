#include "GameManager.h"


int GameManager::threads = 4;
std::string GameManager::path;


int main(int argc, char* argv[])
{
	if (Utils::checkArgs(argc, argv) == false)
		return EXIT_FAILURE;

	if (!FileReader::checkIsValidDir(GameManager::getPath()))
	{
		FileReader::printError(FileReader::Error::PATH, GameManager::getPath());
		return EXIT_FAILURE;
	}
	FileReader::writeToVectorTheFilesInDir(GameManager::getPath());


	std::cout << "Number of threads: " << GameManager::getThreads() << std::endl; // Debug line
	std::cout << "Path is: " << GameManager::getPath() << std::endl; // Debug line

	
	// Read dlls and board files
	std::vector<std::string> boardsVector;
	std::vector<std::unique_ptr<IBattleshipGameAlgo>> playersVector;

	FileReader::importFromFilesToVectors(boardsVector, playersVector, GameManager::getPath());

	if (boardsVector.size() < 1)
	{
		FileReader::printError(FileReader::Error::BOARD, GameManager::getPath());
		return EXIT_FAILURE;
	}
	if (playersVector.size() < 2)
	{
		FileReader::printError(FileReader::Error::DLL, GameManager::getPath());
		return EXIT_FAILURE;
	}
	
	std::cout << "Number of legal players: " << playersVector.size() << std::endl;
	std::cout << "Number of legal boards: " << boardsVector.size() << std::endl;




	// Run tournament

	std::vector<std::thread> threadsVector;


	return 0;
}