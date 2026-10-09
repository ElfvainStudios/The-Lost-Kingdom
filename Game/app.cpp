/**
 * =================================
 * Project Details
 * =================================
 * Developer: Darren Van Der Velde
 * Author: Darren van der Velde
 * GitHub Profile: @Darrenvandervelde
 * GitHub Repo: 
 * License: 
 * Product Key: 
 * 
 * =================================
 * Introduction To THE LOST KINGDOM.
 * =================================
 * 
 * Last Kingdom is a rpg text based console game developed in C++. The goal with project is 
 * understand the function and logic of the basic's of C++ for a text-based game. This is a fun
 * project to learn and develop C++ skills as a developer who has only touch javascript as there 
 * primary coding language. :)
 * 
 * Expanding my knowleage in different area's to see where is my streghth are and wher i need to improve
 * skils. 
 * 
 * Here is a basic overview of the text based game and what is included in this project:
 * 
 * =================================
 * Project Overview
 * =================================
 * 
 * 
 * =================================
 * Project Stack
 * =================================
 * 
 * Mental Note #01:
 * this is the start up guide for running this console base game. ( THE LOST KINGDOM ).
 * `g++ app.cpp -o app.exe` builds the app in to and executable program.
 * .\app.exe this runs the executable app through the CMD or Console.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std; // Using namespace to I don't have to type out STD everytime :)

int main () {

     srand(static_cast<unsigned int>(time(nullptr))); // Creating a Radomised number generator.

     /**
      * List of Players States
      */
     string playerName;
     int health = 100;
     int gold = 0;
     int attack = 15;
     int defence = 10;
     int luck = 10;
     int choice;

     cout << "The lost kingdom\n"; // Title of the game

     cout << "enter your character name: ";
     cin >> playerName; // stores the players name using cin

     cout << "\nWelcome, " << playerName << "!\n"; 
     cout << "You have entered the Lost Kingdom.\n";
     cout << "Your mission is to find the legendary Sword.\n\n";
}