#pragma once
#include <queue>
#include <thread>
#include "FileReader.h"
#include "Utils.h"


class GameManager
{
	static int threads;
	static std::string path;

public:
	GameManager() {};

	static void setThreads(int threads_) { threads = threads_; }
	static int getThreads() { return threads; }
	static void emptyPath() { path = "."; }
	static void setPath(char* path_) { path.append(path_); }
	static std::string getPath() { return path; }
};