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

	// Constructor
	GameBoardServer() { }


	// Functions

	double askWordTo(string word, sf::TcpSocket * player)
	{
		
		vector<char> wordArray = stringToArray(word); // Transform the word into an array

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


	//wordGame.askWordTo("potatoe", &client);

	//system("pause");

	// client to server test char[]
	
	// TCP socket:

	const short STRING_TYPE = 1;

	// While the client is connected to the server
	bool active = true;
	while (active) {
		sf::Packet packet;
		sf::Socket::Status socketStatus = client.receive(packet); // Receiving the whole packet
		if (socketStatus == sf::Socket::Disconnected) {
			active = false;
			break;
		} // If client disconnect

		// Case can't receive data
		if (socketStatus != sf::Socket::Done)
		{
			cout << "couldn't receive data" << endl;
		}

		// Case receive data
		else
		{
			short typeOfRequest = -1; // default
			packet >> typeOfRequest; // Receiving the type of request
			switch (typeOfRequest) {
			case STRING_TYPE:
			{
				string receivedString;
				packet >> receivedString;
				cout << receivedString << endl;
				break;
			}
			default:
				break;
			}
		}
	}
	//Disconnect

	// Server to client Vector char

	//vector<char> data = stringToArray("test");
	//wordGame.sendData(&data, &client);
	

	system("pause");
    return 0;
}

