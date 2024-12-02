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
#include <iomanip>
using namespace std;

const int MAXCHAR = 30;
const string FILENAME = "471-vgsales.csv";

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
void printVideoGame(const vector<VideoGame>& games, int index);
void printVideoGame(const VideoGame& game);
void printVideoGame(const VideoGame& game, int i, ofstream &fout);

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
  int const MAX_TO_IGNORE = 100;
  string line;
  while (getline(file, line)) {
    stringstream ss(line);
    VideoGame game;
    ss.ignore(MAX_TO_IGNORE,','); // Rank
    getline(ss, game.name, ',');  // Name
    getline(ss, game.platform, ','); // Platform                                    
    ss.ignore(MAX_TO_IGNORE,','); // Year
    ss.ignore(MAX_TO_IGNORE,','); // Genre
    ss.ignore(MAX_TO_IGNORE,','); // Publisher
    ss >> game.naSales;           // NA_Sales
    games.push_back(game);
    printVideoGame(game);
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

void printVideoGame(const VideoGame& game, int i,  ofstream &fout){
  const char SEP = ',';
  fout << i << SEP;
  fout << game.name << SEP;
  fout << game.platform << SEP;
  fout << game.naSales << SEP;
  fout << endl;
}

void printVideoGame(const VideoGame& game) {
   cout << right << fixed << setprecision(2);
   cout << setw(50) << game.name.substr(0, 45);
   cout << setw(10) << game.platform;
   cout << setw(10) << game.naSales << endl;
}

void printVideoGame(const vector<VideoGame>& games, int index) {
   const VideoGame& game = games[index];
   cout << right << fixed << setprecision(2);
   cout << setw(7) << (index + 1);
  printVideoGame(game);
}

// Print the list of video games
void printVideoGames(const vector<VideoGame>& games) {
  if (games.empty()) {
    cout << "No video games to display!" << endl;
    return;
  }

  cout << "Video game name; NA sales; Platform" << endl;
  for (size_t i = 0; i < games.size(); ++i) {
    printVideoGame(games, i);
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
      printVideoGame(games, randomIndex);
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
      printVideoGame(game);
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
  printVideoGame(*maxGame);
}

// Save the video games back to the file
void saveVideoGames(const vector<VideoGame>& games) {
  ofstream file(FILENAME + ".bkp");
  if (!file.is_open()) {
    cout << "Error opening file for saving!" << endl;
    return;
  }

  int i = 0;
  for (const auto& game : games) {
    printVideoGame(game, i, file);
    i++;
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

