//************************************************************************************
// Author: Jonathan Reiniger
// Assignment: Final Project
// Date: November 15, 2024
// Description: This program gives statistical information about video games sales. This program reads from a file with kaggle data about the name and rank of the video games. The data gets loaded into an array in the program, and the user inputs their option into the menu. You can add video games to the list, print the list, find the video game with the most sales, or quit the program. At the end, the program writes the updated data back to the kaggle data and then quits. 
// Input: Char videoGame[MAXCHAR], naSales as float, Index as int, Platform as string, Option as string
// Output: List as strings, Max as string
// Sources: Final Project Sample
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits> 
#include <ctime>
#include <set>
using namespace std;

const int MAXCHAR = 30;
const string FILENAME = "vgsales.csv";

// Structure to hold video game data
struct VideoGame {
    string name;
    float naSales;
    string platform;
};

// Declare Functions
void displayMenu();
void loadvideoGames(vector<VideoGame>& games);
void addvideoGame(vector<VideoGame>& games);
void deletevideoGame(vector<VideoGame>& games);
void printVideoGames(const vector<VideoGame>& games);
void printRandomVideoGames(const vector<VideoGame>& games);
void searchVideoGame(const vector<VideoGame>& games);
void findMaxSales(const vector<VideoGame>& games);
void saveVideoGames(const vector<VideoGame>& games);
int getValidatedIndex(int maxIndex);

int main() {
    // Display welcome message at the start
    cout << "Welcome!" << endl;

    cout << "This program gives statistical information about video games sales including the rank and sales of the video games." << endl;
    cout << "You can add video games to the list, print the list, find the video games with the most sales, or quit the program." << endl;

    vector<VideoGame> games;
    char option;

    loadvideoGames(games);

    do {
      displayMenu();
      cout << "Choose an option: ";
      cin >> option;
      cin.ignore(numeric_limits<streamsize>::max(), '\n');

      switch (toupper(option)) {
        case 'A':
          addvideoGame(games);
          break;
        case 'D':
            deletevideoGame(games);
            break;
        case 'P':
            printRandomVideoGames(games); // Printing random games in case 'P'
            break;
        case 'S':
            searchVideoGame(games);
            break;
        case 'M':
            findMaxSales(games);
            break;
        case 'E':
            cout << "Thank you for using the program!\n";
            break;
        default:
            cout << "Invalid option! Please choose a valid option.\n";
      }
    } while (toupper(option) != 'E');

    saveVideoGames(games);
    return 0;
}

// Display the menu options to the user
void displayMenu() {
  cout << "\nChoose an option from below: " << endl;
  cout << "(A)DD" << endl;
  cout << "(D)ELETE" << endl;
  cout << "(P)RINT RANDOM" << endl; // Updated to reflect random print
  cout << "(S)EARCH" << endl;
  cout << "(M)AX" << endl;
  cout << "(E)ND" << endl;
}

// Load video games from the file into the vector
void loadvideoGames(vector<VideoGame>& games) {
  ifstream file(FILENAME);
  if (!file.is_open()) {
    cout << "Error opening file!" << endl;
    return;
  }

  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    VideoGame game;
    getline(ss, game.name, ';');
    ss >> game.naSales;
    ss.ignore();
    getline(ss, game.platform, ';');
    games.push_back(game);
  }
  file.close();
}

// Add a new video game to the list
void addvideoGame(vector<VideoGame>& games) {
  VideoGame game;

  cout << "Enter the video game name (30 characters or less): ";
  getline(cin, game.name);

  if (game.name.length() > MAXCHAR) {
      game.name = game.name.substr(0, MAXCHAR);
  }

  cout << "Enter the NA sales: ";
  while (!(cin >> game.naSales)) {
    cout << "Invalid input! Please enter a valid number for NA sales: ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  cin.ignore();

  cout << "Enter the Platform: ";
  getline(cin, game.platform);

  games.push_back(game);
}

// Delete a video game from the list
void deletevideoGame(vector<VideoGame>& games) {
  if (games.empty()) {
    cout << "No games available to delete. \n";
    return;
  }

  // Only print 6 random games, not the entire list
  printRandomVideoGames(games);

  cout << "Enter the index of the video game to remove: ";
  int index = getValidatedIndex(6);  // Restrict the index to 1-6 for the random games
  games.erase(games.begin() + index);  // Remove the selected game
  cout << "Video game removed." << endl;

  // Print the updated list after the game is removed
  cout << "Here is your new list:" << endl;
  printVideoGames(games);
}

// Print the list of video games
void printVideoGames(const vector<VideoGame>& games) {
  if (games.empty()) {
    cout << "No video games to display!" << endl;
    return;
  }

  cout << "Video game name; NA sales; Platform" << endl;
  for (size_t i = 0; i < games.size(); ++i) {
      cout << i + 1 << ". " << games[i].name << "; " << games[i].naSales << "; " << games[i].platform << endl;
  }
}

// Function to print 6 random video games
void printRandomVideoGames(const vector<VideoGame>& games) {
  if (games.size() < 6) {
    cout << "Not enough games in the list to display 6 random games!" << endl;
    return;
  }

  srand(static_cast<unsigned>(time(0)));
  set<int> selectedIndices;

  while (selectedIndices.size() < 6) {
    int randomIndex = rand() % games.size();

    if (selectedIndices.find(randomIndex) == selectedIndices.end()) {
      selectedIndices.insert(randomIndex);
      const VideoGame& game = games[randomIndex];
      cout << game.name << "; " << game.naSales << "; " << game.platform << endl;
    }
  }
}

// Search for a video game by name
void searchVideoGame(const vector<VideoGame>& games) {
  string searchName;
  cout << "Enter the video game name: ";
  getline(cin, searchName);

  bool found = false;
  for (const auto& game : games) {
    if (game.name.find(searchName) != string::npos) {
      cout << "Here is the information:" << endl;
      cout << game.name << " " << game.naSales << " " << game.platform << endl;
      found = true;
      break;
    }
  }

  if (!found) {
    cout << "Video game not found!" << endl;
  }
}

// Find the video game with the maximum North American sales
void findMaxSales(const vector<VideoGame>& games) {
  if (games.empty()) {
    cout << "No games in the list!" << endl;
    return;
  }

  const VideoGame* maxGame = &games[0];
  for (const auto& game : games) {
    if (game.naSales > maxGame->naSales) {
      maxGame = &game;
    }
  }

  cout << "The video game with the most NA sales is: " << endl;
  cout << maxGame->name << " " << maxGame->naSales << " " << maxGame->platform << endl;
}

// Save the video games back to the file
void saveVideoGames(const vector<VideoGame>& games) {
  ofstream file(FILENAME);
  if (!file.is_open()) {
    cout << "Error opening file for saving!" << endl;
    return;
  }

  for (const auto& game : games) {
    file << game.name << ";" << game.naSales << ";" << game.platform << ";" << endl;
  }
  file.close();
}

// Validate the index when deleting a video game
int getValidatedIndex(int maxIndex) {
  int index;
  while (true) {
    cin >> index;
    if (index >= 1 && index <= maxIndex) {
      return index - 1; // Adjust to 0-based index
    } else {
      cout << "Invalid index. Please try again: ";
    }
  }
}

