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

/**
 * #include <iostream> is a standard library that allows the user to have the ability to interact
 * with the console/CMD 
 */
#include <iostream>

/**
 * #include <string> give you access to the std::string type that is used for storing
 * and working with text
 */
#include <string>

/**
 * This library gives you access to all the standard functions in in c++ which is also used in 
 * C library which is known as `stdlib.h`
 */
#include <cstdlib>

/**
 * #include <ctime> is a library that gives you access to the time & date functions
 */
#include <ctime>

using namespace std; 
/**
 * This line of code allows the developer to pull namespace from the std library
 * with out having to write std:: everytime when righting a string or using cout.
 */ 

int main () {

     /**
      * This is just a mental note #02:
      * << output
      * >> is input 
      */

     // Creating a Radomised number generator
     srand(static_cast<unsigned int>(time(nullptr)));

     // int setup for the player. 
     string playerName;
     int health = 100;
     int gold = 0;
     int choice; //choice has not been assigned as yet

     cout << "The lost kingdom\n"; // Title of the game

     cout << "enter your character name: ";
     cin >> playerName; // stores the players name using cin

     /**
      * cin is used to receive an input from the user trough the console.
      */

     cout << "\nWelcome, " << playerName << "!\n"; 
     cout << "You have entered the Lost Kingdom.\n";
     cout << "Your mission is to find the legendary Sword.\n\n";

     /**
      * Here is a statement checking if the health is greater the 0 so i can be
      * put to the next stage of the game.
      *
      * Chapter One
      */
     while (health > 0) {
          
          cout << "\nMenu\n";
          cout << "Players Name: " << playerName << "\n"; // Displaying Players Name
          cout << "Health: " << health << "\n"; // Displaying the Health
          cout << "Gold: " << gold << "\n"; // Displaying the gold

          /**
           * This are the current option the player can choice from.
           */
          cout << "1. Explore the forest\n";
          cout << "2. Visit the village\n";
          cout << "3. Open Investory.\n";
          cout << "4. End Game\n";

          cout << "Choose an option: ";
          cin >> choice; // input for the choice from the option above. 

          if (choice == 1) {
               
               cout << "\nYou enter the Dark Forest...\n";
               
               int event = rand() % 4;

               /**
                * Another Metal note:
                * rand() generates a random integer number for eg:
                * 7887
                * 512
                * 888751
                * 
                * the % is called modulo operator
                * it gives the event the reminding after division. 
                * 
                * rand() random number divide by 4 gives you the out come.
                */

               if (event == 0){
                    
                    cout << "You found a bag of gold\n";

                    /**
                     * Created a int called found gold and then ran the rand() divined by 50 + 10;
                     */
                    int foundGold = rand() % 50 + 10;

                    /**
                     * add the gold found and the current gold in my pocket together.
                     */
                    gold += foundGold;
                    
                    cout << "You Found " << foundGold << " gold!\n";

               } else if (event == 1) {

                    cout << "A Goblin attacks you!\n";

                    /** 
                     * Goblins Health
                     */
                    int enemyHealth = 50;

                    /**
                     * Senoiro check for both the goblin and me.
                     */
                    while (enemyHealth > 0 && health > 0) {

                         /**
                          * When my Health and goblin health is greater the 0; countinue with the attack.  
                          */                         
                         cout << "Your Health: " << health << "\n";
                         cout << "Goblins Heath: " << enemyHealth << "\n";

                         cout << "\nAttack\n";
                         cout << "2. Run\n";
                         
                         /**
                          * Choice to choose which path to take when the Goblin. 
                          */
                         cout << "Choose: ";
                         cin >> choice;

                         if (choice == 1) {
                              
                              /**
                               * Player and goblin Damage Variables.
                               */
                              int playerDamage = rand() % 20 + 10;
                              int enemyDamage = rand() % 15 + 5;

                              /**
                               * Subtracting the enemies Health using the players Damage.
                               */
                              enemyHealth -= playerDamage;

                              /**
                               * Show to the Console when the hit was made.
                               */
                              cout << "\nYou hit the goblin for "
                                   << playerDamage << "damage!\n";

                              /**
                               * This shows the player got hit by the enemy ( goblin )
                               */
                              if (enemyHealth > 0) {

                                   health -= enemyDamage;

                                   cout << "The Goblin hits you for "
                                        << enemyDamage << "damage!\n";
                              }
                         }
                    }
               }
          } else if (choice == 2) {
               
               /**
                * Dialog for option 2. Crystal village
                */
               cout << "You have Entered the Crystal Village near the dark forest.\n";

               /**
                * Town options for the Crystal Village.
                */
               cout << "1. Enter the Blacksmith\n";
               cout << "2. Enter the Magic Shop\n";
               cout << "3. Enter the Aventures Guild\n";
               cout << "4. Enter the Iteam Shope\n";
               cout << "5. Enter the Dark Forest\n";
               cout << "6. More Options\n";

               cout << "Choice an option: ";
               cin >> choice;

               /**
                * Mental note #Final: My Brain is not working currently at this stage of the project as I have
                * not had any caffiene or coffee and i currenly very dangerous at this point in the development. 
                * So please `STAY AWAY` from me and keep a 10 feet distance when approaching me :)
                */

               /* To be Continued :) */
          }
     } 
}