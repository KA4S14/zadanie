#define ARDUINO_JSON_ENABLE_STD_STRING 1 
#include "Json.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include<algorithm>
using namespace std;

void createDefaultconfig()
{
    fstream file;
    file.open("settings.json", ios::out);
    if (file.good())
    {
        StaticJsonDocument<1024> doc;

        JsonObject settings = doc.createNestedObject("settings");
        settings["Try"] = 5;
        settings["Range"] = 1000;
        settings["Hint1"] = 0;
        settings["Resolution"] = 1920;
        settings["Resolution2"] = 1080;
        settings["Frames"] = 60;

        JsonObject scores = doc.createNestedObject("scores");

        string box;
        serializeJsonPretty(doc, box);
        file << box;
        file.close();
    }
}

void showOptions(const JsonObject& settings)
{
    for (JsonPair p : settings)
    {
        cout << p.key() << " " << p.value().as<string>() << '\n';
    }
}

void saveSettings(StaticJsonDocument<1024>& doc)
{
    fstream file;
    file.open("settings.json", ios::out);
    if (file.good())
    {
        string tmp;
        serializeJsonPretty(doc, tmp);
        file << tmp;
        file.close();
    }
}





int game(int tab[], string name)
{
    int score = 0;
    srand(time(0));
    int randomNumber = rand() % tab[1] + 1;
    int guessed = 0;
    cout << randomNumber << "\n";
    for (int i = 0; i < tab[0]; ++i)
    {
        cout << "It is your " << i + 1 << " turn \n ";
        cout << "Guess a number I think about...";
        cin >> guessed;

        if (guessed == randomNumber)
        {
            cout << ("Congratulations !This is number I think about : D \n ");
            if (i <= tab[0])
            {
                score = i+1;
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
        cout << "Your tryout " << name << " is over. Number I think about was... " << randomNumber << endl;
    }
    return score;
}


int main()
{
    const int MAX_SCORES = 10;
    StaticJsonDocument<1024> doc;
    JsonObject settings;
    JsonObject scores;

    fstream file;
    file.open("settings.json", ios::in);
    if (!file.good())
    {
        createDefaultconfig();
    }
    deserializeJson(doc, file);
    settings = doc["settings"].as<JsonObject>(); 
    scores = doc["scores"].as<JsonObject>();

    string nickname;
    const int SIZE = 3;
    int tab[SIZE];
    int choice = 0;
    tab[SIZE];
    

    int i = 0;
    for (JsonPair p : settings) {
        if (i >= SIZE) break;
        tab[i] = p.value().as<int>();
        i++;
    }


    while (choice != 4)
    {
        cout << ("What do you want?\n ");
        cout << "1. Play game!\n";
        cout << "2. Show settings.\n";
        cout << "3. Show scores\n";
        cout << "4. Exit\n";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Give me your name: ";
            cin >> nickname;
            int score = game(tab, nickname);


            scores[nickname] = score;

            saveSettings(doc);

            break;
        }
        case 2:
        {
            showOptions(settings);
            int userInput;
            cout << "What do you want to do ? \n";
            cout << "1. Back\n";
            cout << "2. Change settings \n";
            cin >> userInput;
            if (userInput == 2)
            {


                for (JsonPair p : settings)
                {
                    cout << "Enter a new value for " << p.key() << ": ";
                    if (p.value().is<string>())
                    {
                        string newValue;
                        cin >> newValue;
                        settings[p.key()] = newValue;
                    }
                    else if (p.value().is<int>())
                    {
                        int newValue;
                        cin >> newValue;
                        settings[p.key()] = newValue;
                    }
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
        {
            cout << "Scores:\n";
            for (JsonPair score : scores)
            {
                auto nickname = score.key();
                int value = score.value().as<int>();
                cout << "Nickname: " << nickname << ", Score: " << value << endl;
            }
            break;
        }
        case 4:
            choice = 4;
            break;
        }
    }

    file.close();

    return 0;
}
