// TCP server test.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include<fstream>
#include <conio.h> // For character press

using namespace std;

// functions
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
	vector<string> lineList;
	TextFile(string path)
	{
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
};

class Player
{
public :
	//Properties
	int playerID;
	string pseudo;
	sf::TcpSocket adress;

	// Constructor
	Player() {}

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
	TextFile wordList = TextFile("..\\list.txt");
	int numberOfPlayers;


	GameBoardServer() { }

	void sendData(vector<char> * data, sf::TcpSocket * player)
	{
		if ((*player).send(data, (*data).size()) != sf::Socket::Done)
		{
			cout << "Couldn't send data" << endl;
		}
		else
		{
			cout << "Success to send data" << endl;
		}
	}
	void sendData(int * data, sf::TcpSocket * player)
	{
		if ((*player).send(data, 4) != sf::Socket::Done)
		{
			cout << "Couldn't send data" << endl;
		}
		else
		{
			cout << "Success to send data" << endl;
		}
	}
	void sendData(double * data, sf::TcpSocket * player)
	{
		if ((*player).send(data, 4) != sf::Socket::Done)
		{
			cout << "Couldn't send data" << endl;
		}
		else
		{
			cout << "Success to send data" << endl;
		}
	}
	void sendData(bool * data, sf::TcpSocket * player)
	{
		if ((*player).send(data, 4) != sf::Socket::Done)
		{
			cout << "Couldn't send data" << endl;
		}
		else
		{
			cout << "Success to send data" << endl;
		}
	}

	void receiveData(vector<char> * data, sf::TcpSocket * player)
	{
		std::size_t received;
		if ((*player).receive(data, 100, received) != sf::Socket::Done)
		{
			cout << "couldn't receive data vectorChar" << endl;
		}
		else
		{
			std::cout << "Received " << received << " bytes for vectorChar" << endl;
		}
	}
	void receiveData(int * data, sf::TcpSocket * player)
	{
		std::size_t received;
		if ((*player).receive(data, 100, received) != sf::Socket::Done)
		{
			cout << "couldn't receive data int" << endl;
		}
		else
		{
			std::cout << "Received " << received << " bytes for int" << endl;
		}
	}
	void receiveData(double * data, sf::TcpSocket * player)
	{
		std::size_t received;
		if ((*player).receive(data, 100, received) != sf::Socket::Done)
		{
			cout << "couldn't receive data double" << endl;
		}
		else
		{
			std::cout << "Received " << received << " bytes for double" << endl;
		}
	}
	void receiveData(bool * data, sf::TcpSocket * player)
	{
		std::size_t received;
		if ((*player).receive(data, 100, received) != sf::Socket::Done)
		{
			cout << "couldn't receive data bool" << endl;
		}
		else
		{
			std::cout << "Received " << received << " bytes for bool" << endl;
		}
	}

	double askWordTo(string word, sf::TcpSocket * player)
	{
		int code = 1; // Code to send a word
		sendData(&code, player);
		vector<char> wordArray = stringToArray(word); // Transform the word into an array

		bool received = false;
		receiveData(&received, player);

		if (received)
		{
			sendData(&wordArray, player);
		}
		else { cout << "Code not received" << endl; }

		return 1.0;
	}
	void turn()
	{

	}


};


int main()
{
	GameBoardServer wordGame = GameBoardServer();

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


	wordGame.askWordTo("potatoe", &client);

	system("pause");

	// client to server
	/*
	char data[4];
	std::size_t received;
	// TCP socket:
	if (client.receive(data, 100, received) != sf::Socket::Done)
	{
		cout << "couldn't receive data" << endl;
	}
	else
	{
		std::cout << "Received " << received << " bytes" << std::endl;
	}

	cout << data[0] << data[1] << data[2] << data[3] << endl;

	system("pause");
	*/

	// Server to client
	/*
	if (client.send(data, 4) != sf::Socket::Done)
	{
		cout << "couldn't send data" << endl;
	}
	*/
    return 0;
}

