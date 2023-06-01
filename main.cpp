#define ARDUINO_JSON_ENABLE_STD_STRING 1 
#include"Json.h"
#include<string>
#include <iostream>
#include<cstdlib>
#include<fstream>
#include<iterator>
using namespace std;

void createDefaultconfig()
{

	fstream file;
	file.open("settings.json", ios::out);
	if (file.good())
	{
		StaticJsonDocument<1024> doc;

		doc["Try"] = 5;
		doc["Range"] = 1000;
		doc["Hint1"] = 0;
		doc["Resolution"] = "1920x1080";
		doc["Fullscreen"] = true;
		string box;
		serializeJsonPretty(doc, box);
		file << box;
		file.close();
	}

}
void showOptions(StaticJsonDocument<1024>& doc)
{
	for (JsonPair p : doc.as<JsonObject>())
	{
		cout << p.key() << " " << p.value() << '\n';
	}
}
void saveSettings(StaticJsonDocument<1024>& doc)
{
	fstream file;
	file.open("settings.json", ios::out);
	if (file.good())
	{
		string tmp;
		serializeJson(doc, tmp);
		file << tmp;
		file.close();
	}
}
int game(int tab[])
{
	int score =0;
	srand(time(0));
	int randomNumber = rand() % tab[1] + 1;
	int guessed =0;
	// cout << randomNumber<<"\n";
	for (int i = 0; i <= tab[0]; ++i)
	{	
		cout << "It is your " << i + 1 << " turn \n ";
		cout << "Guess a number I think about...";
		cin >> guessed;
		
			if (guessed == randomNumber)
			{
				cout << ("Congratulations!This is number I think about :D \n \t");
				if (i <= tab[0])
				{
					score = i;
				}
				break;
			}
			else if (guessed < randomNumber)
			{
				cout << ("This isn't number I think about...My number is bigger :D \n");
			}
			else if (guessed > randomNumber)
			{
				cout << ("This isn't number I think about...My number is smaller :D \n");
			}
			else if (i > tab[0] - 2)
			{
				cout << "Your hint: \n" << randomNumber - 20 << "< x < " << randomNumber + 20 << endl;

			}
	}
	if (guessed != randomNumber)
	{
		cout << "Your tryout is over. Number I think about was... " << randomNumber << endl;
	}

	return score;
}


int main()
{
	StaticJsonDocument<1024> doc;
	
	fstream file;
	file.open("settings.json", ios::in);
	if (!file.good())
	{
		createDefaultconfig();
	}
	deserializeJson(doc, file);

	int const SIZE = 3;
	int tab[SIZE];
	int choice = 0;
	tab[SIZE];

	int i = 0;
	for (JsonPair p : doc.as<JsonObject>())
	{
		if (i >= SIZE)
			break;
		tab[i] = p.value().as<int>();
		i++;
	}
	

	while (choice != 3)
	{
		cout << ("What do you want?\n ");
		cout << "1.Play game!\n";
		cout << "2.Change settings.\n";
		cout << "3.Exit\n";
		
		cin >> choice;

		switch (choice)
		{
		case 1:
		{
			
			game(tab);



			break;
		}
		case 2:
		{

			showOptions(doc);
			int userInput;
			cout << "What do you want to do ? \n";
			cout << "1. Back\n";
			cout << "2. Change settings \n";
			cin >> userInput;
			if (userInput == 2)
			{
				int i = 0;
				for (JsonPair p : doc.as<JsonObject>())
				{
					
					string newValue;
					cout << "Enter a new value for  " << p.key() << ": ";
					cin >> newValue;
					if (newValue == "n") continue;
					doc[p.key()] = newValue;
					tab[i] = p.value().as<int>();
					i++;
				}
				saveSettings(doc);
			}
			else if (userInput == 1)
			{
				main();
			}
			break;
		}
		case 3:
			choice = 3;
			break;

		}


	}









	
	file.close();
	

	

	return 0;
}