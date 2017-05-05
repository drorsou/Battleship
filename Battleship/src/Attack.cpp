#include "Attack.h"


Attack::Attack(const std::string& path, int player_num)
{
	init = this->loadFromAttackFile(path, player_num);
}


std::pair<int, int> Attack::getNextAttack()
{
	std::pair<int, int> temp;
	temp = list.front();
	list.pop();
	return temp;
}


bool Attack::loadFromAttackFile(const std::string& path, int player_num)
{
	std::string line, pathToFile;
	std::pair<int, int> singleAttack;
	std::pair<std::string, std::string> attackFiles;
	attackFiles = FileReader::findFilesLexicographically("attack");
	/*if(player_num == 0)
		attackFiles = FileReader::findFilesLexicographically("attack");
	else
		attackFiles = FileReader::findFilesLexicographically("attack");*/
	
	std::cout << attackFiles.first << " , " << attackFiles.second << std::endl;
	// Creating an ifstream object and opening file in path attackPath
	if (player_num == 0)
	{
		if (attackFiles.first.empty())
			FileReader::printError(FileReader::Error::AlGO_INIT, path); // TODO - Which error?
		else
			pathToFile = path + "\\" + attackFiles.first;
	}
	else if (player_num == 1)
	{
		if (attackFiles.second.empty())
		{
			if (attackFiles.first.empty())
				FileReader::printError(FileReader::Error::AlGO_INIT, path); // TODO - Which error?
			else
				pathToFile = path + "\\" + attackFiles.first;
		}
		else
			pathToFile = path + "\\" + attackFiles.second;
	}

	std::ifstream fin(pathToFile);

	if (fin.fail()) //error openning file
	{
		std::cout << "Error Occured opening file of attack" << std::endl;		
		return false;
	}
	while (getline(fin, line)) //getline returns null if we get to end of file.
	{

		if (line.size() && line[line.size() - 1] == '\r') //to handle \r\n(getline is only for \n)
			line = line.substr(0, line.size() - 1);
		bool isInvalidLine = false;//if invalid dont add to vector of attack
		
		if (Attack::processLine(line, &singleAttack))
		{
			this->list.push(std::make_pair(singleAttack.first, singleAttack.second)); // push the new coordinate
		}
		else if (singleAttack.first == -1 && singleAttack.second == -1)
		{ // invalid coord			
			continue;
		}
		else
			return false;
	}
	return true;
}


//takes a string and splits it by the delimeter ','. creates a pair of ints of this row 
bool Attack::processLine(const std::string& line, std::pair<int, int> * res) {
	std::vector<std::string> tokens = Attack::split(line, ',');
	int row;
	int col;
	if (tokens.size() != 2)
	{	
		res->first = -1;
		res->second = -1;
		return false; // invalid line format
	}
	if (!Attack::is_number(tokens[0]) || !Attack::is_number(tokens[1]))
	{
		res->first = -1;
		res->second = -1;
		return  false; // ignore this line
	}
	try
	{
		row = stoi(tokens[0]);
		col = stoi(tokens[1]);

	}
	catch (const std::exception&)
	{
		res->first = -1;
		res->second = -1;
		return false;
	}
	if (row < 1 || row > 10 || col < 1 || col > 10)
	{
		res->first = -1;
		res->second = -1;
		return false; // ignore this line
	}
	res->first = row;
	res->second = col;
	//TODO- if its not a number-what to do?it's written there is an exception
	return true;
}


bool Attack::is_number(const std::string &s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}


//splits string s to a vector by delimeter
std::vector<std::string> Attack::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	std::stringstream strSt(s);
	std::string item;

	while (std::getline(strSt, item, delim))
	{
		Attack::removeCharFromString(item, ' ');//earse spaces around the comma
		elems.push_back(item);
	}

	return elems;
}

void Attack::removeCharFromString(std::string &str, char charToRemove) {

	str.erase(remove(str.begin(), str.end(), charToRemove), str.end());
}
