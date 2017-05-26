#pragma once
#include <string>
#include <queue>
#include <sstream>
#include <iostream>
#include "FileReader.h"


class GameManager
{
	static int threads;


public:
	GameManager() {};

	static void setThreads(int threads_) { threads = threads_; }
	static int getThreads() { return threads; }

	/* Pre: Get a queue of at least 2 strings
	 * Post: If the first string equals to 'threads' (otherwise return false), then self.threads is changed to the second string
	 */
	static bool setThreadsFromQueue(std::queue<std::string>& queue);
};