// TCP server test.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h> // For character press

using namespace std;


const short STRING_TYPE = 1;
const short FLOAT_TYPE = 2;
const short DOUBLE_TYPE = 3;
const short INT_TYPE = 4;

const short ASK_WORD = 1;
const short PLAYER_TIME = 2;
const short SEND_SCORE = 3;
const short GAME_FINISHED = 4;
const short ASK_LOGIN_OR_REGISTER = 5;

const bool SHOW_RANDOM_ORDER = true;
const bool SHOW_WORD_LIST = false;

// functions
int myrandom(int i) { return std::rand() % i; }
vector<char> stringToArray(string sentence)
{
	vector<char> sentence2;
	for (int i = 0; i < sentence.size(); i++)
	{
		sentence2.push_back(sentence[i]);
	}
	return sentence2;
}
string ArrayToString(vector<char> sentence)
{
	string sentence2;
	for (int i = 0; i < sentence.size(); i++)
	{
		sentence2 += sentence[i];
	}
	return sentence2;
}

class TextFile
{
public:
	string path;

	vector<string> lineList;
	TextFile(){}
	TextFile(string myPath)
	{
		path = myPath;
		ifstream myfile;
		myfile.open(myPath);

		string line;

		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				lineList.push_back(line);
			}
			myfile.close();
		}
	}

	// Write a sentence at the end of the file
	void uptdate()
	{
		lineList.clear();
		ifstream myfile;
		myfile.open(path);

		string line;

		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				lineList.push_back(line);
			}
			myfile.close();
		}
	}
	void write(string sentence)
	{
		ofstream myfile;
		myfile.open(path, std::ios::app);
		if (myfile.is_open())
		{
			myfile << sentence;
			myfile << "\n";
			myfile.close();
		}
		else cout << "Unable to open file";
	}
	
};

class Player
{
public :
	//Properties
	string name;
	TextFile data;

	// Constructor
	Player(){}
	Player(string playerName)
	{
		name = playerName;
		string path = "";
		path += "..\\players_data\\";
		path += name;
		path += ".txt";
		data = TextFile(path);
	}

	void addScore(double time)
	{
		//save the score in the player's data
		string timeData = to_string(time);
		data.write(timeData);
	}
	double getLastScore()
	{
		data.uptdate();
		string scoreData = data.lineList[data.lineList.size() - 1];
		string::size_type sz;
		double score = stod(scoreData, &sz);
		return score;
	}
	double getBestScore()
	{
		data.uptdate();

		string scoreData;
		double bestScore = DBL_MAX;
		double score;
		for (int i = 2; i < data.lineList.size(); i++)
		{
			scoreData = data.lineList[i];
			string::size_type sz;
			score = stod(scoreData, &sz);
			if (score < bestScore)
			{
				bestScore = score;
			}
		}
		return bestScore;
	}
	// Functions


};

class GameBoardServer
{
	// For the data, we will use a code to know which kind of data is sent
	// 1 = word to guess
	// 2 = Other player score
	// 3 = end game

public:
	//Properties
	TextFile wordList = TextFile("..\\word_list.txt");
	TextFile playerList = TextFile("..\\player_list.txt");
	Player actualPlayer;
	string actualPlayerName;

	// Constructor
	GameBoardServer() { }


	// Functions
	void sendPacket(string myString, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << STRING_TYPE << myString;
		if ((*client).send(packet) != sf::Socket::Done)

		{
			cout << "couldn't send data" << endl;
		}
	}
	void sendPacket(float myFloat, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << FLOAT_TYPE << myFloat;
		if ((*client).send(packet) != sf::Socket::Done)

		{
			cout << "couldn't send data" << endl;
		}
	}
	void sendPacket(double myDouble, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << DOUBLE_TYPE << myDouble;
		if ((*client).send(packet) != sf::Socket::Done)

		{
			cout << "couldn't send data" << endl;
		}
	}
	void sendPacket(bool myBool, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << myBool;
		if ((*client).send(packet) != sf::Socket::Done)

		{
			cout << "couldn't send data" << endl;
		}
	}

	sf::Packet receivePacket(sf::TcpSocket * client)
	{
		sf::Packet packet;
		sf::Socket::Status socketStatus = (*client).receive(packet); // Receiving the whole packet
		if (socketStatus == sf::Socket::Disconnected) {
		} // If client disconnect

		if (socketStatus != sf::Socket::Done)
		{
			cout << "couldn't receive data" << endl;
		} // Case can't receive data

		return packet;
	}

	double askWordTo(string word, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << ASK_WORD << word;
		if ((*client).send(packet) != sf::Socket::Done)
		{
			cout << "couldn't send data" << endl;
		}

		// Receiving time
		short typeOfRequest;
		double time;

		packet = receivePacket(client);
		packet >> typeOfRequest;
		packet >> time;

		return time;
	}
	void sendScore(double time, sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << SEND_SCORE << time;
		if ((*client).send(packet) != sf::Socket::Done)
		{
			cout << "couldn't send data" << endl;
		}
	}

	bool playerNameUsed(string name)
	{
		bool used = false; // Flag if the name is already taken or not

		for (int i = 0; i < playerList.lineList.size(); i++)
		{

			// Case the name already exist
			if (name == playerList.lineList[i])
			{
				used = true;
				cout << "This name is already taken." << endl;
			}
		}
		return used;
	}
	void addPlayer(string name, string password)
	{
		playerList.write(name);

		// Create a data file for the new player
		string newPath = "";
		newPath += "..\\players_data\\";
		newPath += name;
		newPath += ".txt";

		TextFile newFile(newPath);
		newFile.write(name);
		newFile.write(password);

		actualPlayerName = name;
		actualPlayer = Player(name);
	}
	void sendInfo(sf::TcpSocket * client)
	{
		cout << actualPlayer.getBestScore() << endl;
		cout << actualPlayer.getLastScore() << endl;

		sendPacket(actualPlayer.getBestScore(), client);
		sendPacket(actualPlayer.getLastScore(), client);
	}
	bool login(string name, string password)
	{
		bool success = false;

		// test if the name is in the list
		for (int i = 0; i < playerList.lineList.size(); i++)
		{
			// Case the name already exist
			if (name == playerList.lineList[i])
			{
				success = true;
			}
		}

		if (success)
		{
			string path = "";
			path += "..\\players_data\\";
			path += name;
			path += ".txt";

			TextFile playerData(path);
			if (playerData.lineList[1] == password)
			{
				success = true;
				actualPlayerName = name;
				actualPlayer = Player(name);
				cout << "success to connect to account " << name << endl;
			}
			else
			{
				success = false;
			}
		}

		return success;
	}
	void askLoginOrRegister(sf::TcpSocket * client)
	{
		sf::Packet packet;
		packet << ASK_LOGIN_OR_REGISTER;
		if ((*client).send(packet) != sf::Socket::Done)
		{
			cout << "couldn't send data" << endl;
		}

		sf::Packet anwser = receivePacket(client);
		string choiceNumber;
		anwser >> choiceNumber;

		// Case log in
		if (choiceNumber == "1")
		{
			cout << "Client decide to log in." << endl;

			string name;
			string password;
			anwser = receivePacket(client);
			anwser >> name;

			anwser = receivePacket(client);
			anwser >> password;

			bool loginSuccess = login(name, password);
			cout << loginSuccess;
			sendPacket(loginSuccess, client);

			while (!loginSuccess)
			{
				anwser = receivePacket(client);
				anwser >> name;

				anwser = receivePacket(client);
				anwser >> password;

				loginSuccess = login(name, password);
				cout << loginSuccess;
				sendPacket(loginSuccess, client);
			}

			sendInfo(client);
		}

		// register
		if (choiceNumber == "2")
		{
			string name;
			string password;

			anwser = receivePacket(client);
			anwser >> name;

			// Asking for an unused name
			while (playerNameUsed(name))
			{
				sendPacket(true, client); // Say that the name is used to client
				anwser = receivePacket(client);
				anwser >> name;
			}
			sendPacket(false, client); // success to use this name

			anwser = receivePacket(client); // Receive the password

			addPlayer(name, password); // Create a new .txt for player's data		
		}
	}

	void endGame(sf::TcpSocket * client)
	{
		
	}
};


int main()
{
	GameBoardServer gameBoard = GameBoardServer();
	if (SHOW_WORD_LIST)
	{
		cout << "The list of words is : " << endl;
		for (int i = 0; i < gameBoard.wordList.lineList.size(); i++)
		{
			cout << gameBoard.wordList.lineList[i] << endl;
		}
		cout << endl; 
	}

	sf::TcpListener listener; //Looking for clients

	// bind the listener to a port
	if (listener.listen(53000) != sf::Socket::Done)
	{
		cout << "no connection" << endl;
	}
	else
	{
		cout << "Success to connect on port 53000" << endl;
	}

	// accept a new connection
	cout << "Waiting for client to connect" << endl;
	sf::TcpSocket client;
	if (listener.accept(client) != sf::Socket::Done)
	{
		cout << "can't accept the client" << endl;
	}
	else
	{
		cout << "Success to connect client" << endl;
	}
	

	gameBoard.askLoginOrRegister(&client);


	// Send words to guess
	double time = 0;

	// An array with int [0,1,2,3]... that we scramble so the words from the list are randomly taken.
	#pragma region Random Array

	srand(unsigned(std::time(0)));

	vector<int> randomArray;

	for (int i = 0; i < gameBoard.wordList.lineList.size(); i++)
	{
		randomArray.push_back(i);
	}
	random_shuffle(randomArray.begin(), randomArray.end());
	random_shuffle(randomArray.begin(), randomArray.end(),myrandom);
	if (SHOW_RANDOM_ORDER)
	{
		for (int i = 0; i < gameBoard.wordList.lineList.size(); i++)
		{
			cout << randomArray[i] << endl;
		}
	}
#pragma endregion

	// Ask words 1 by 1
	for (int i = 0; i < gameBoard.wordList.lineList.size(); i++)
	{
		time += gameBoard.askWordTo(gameBoard.wordList.lineList[randomArray[i]], &client);
		cout << "Total time = " << time << endl;

		gameBoard.sendScore(time, &client);
	}
	gameBoard.actualPlayer.addScore(time);
	cout << "last : " << gameBoard.actualPlayer.getLastScore() << endl;
	cout << "best : " << gameBoard.actualPlayer.getBestScore() << endl;

	system("pause");
    return 0;
}

