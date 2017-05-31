#pragma once
#include <string>
#include <queue>
#include <sstream>
#include <iostream>
#include "GameManager.h"

class Utils
{
public:

	/* Pre: Get number 'argc' of the program arguments and their pointers 'argv'
	 * Post: Set the GameManager variables according to the arguments,
	 * returns false if there was an error and true otherwise
	 */
	static bool Utils::checkArgs(int argc, char** argv);

	/* Pre: Get a queue of strings
	 * Post: If the first one equals to "-thread" then the second must be an integer,
	 * which is set at the number of threads in GameManager
	 * returns false if there was an error and true otherwise
	 */
	static bool Utils::setThreadsFromQueue(std::queue<std::string>& queue);

	/* Pre: String 'str', queue of strings 'queue', delimiter character 'delim'
	 * Post: Split the string by the delimiter, and put the substrings in the queue
	 *
	 */
	static void splitString(const std::string& str, std::queue<std::string>& queue, char delim);

};