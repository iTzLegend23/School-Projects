/*
DISCLAIMER:
------------------------------------------------------------------------------------------------------------------
This is a very rough implementation of a simple text-based adventure game and there are bound to be several bugs.
You can see where my thoughts are in the below comments to get a feel of what still needs to be done.
This was my first real dive into a fun project while starting to learn how to code!
I hope you enjoy!
------------------------------------------------------------------------------------------------------------------


What do I need to do?
- Make sure to take care of the potential death of a player while not in a fight
- Decide if you want to make some of the fights more intense based on the enemy the player is fighting
	- If necromancer for example, he might spawn minions that attack you as well
	- The golem might wind up and do a crazy strong attack after 3 turns or something
		- Might not be able to escape from some enemies too

Completed Parts of the game:
- Player class created. Used for both user and enemies --> Check
- Create a room function with multiple options to choose from based on randomness or choice --> Check
	-- At least 6 rooms needed
- Create an initialization function to set the game up --> Check
- Main logic creation to  move the player from one room to the next based on the choices they make --> Check
- Will also need to refresh player (and enemy information) with each action
	- This idea is implemented in the fight function and will also be printed to the screen in the beginning of each room
*/

// Idea: make each room used via using a bool variable that changes to true if the room has been visited
// This allows you to make use of all the rooms you create without really wasting any of them
// Just use logic like: if visited == true...roomChoice = rand() % whatever...break;
// For the boss room you can just give an option to enter the room, or to come back to it later

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

int printCondition, roomChoice;
std::string move;

// Player class. Holds the player's data such as health, strength, etc.
struct player
{
	int HP;
	int MP;
	int strength;
	int armor;
	int magic;
	int magicResist;
	int speed;
	char name[100];
	// char playerClass[7]; // strcmp (player1.playerClass, "knight") == 0
	std::string playerClass;
};

void showProgramHeader();
void init(player& player1, player& enemy); // Game initializtion function
void printData(int printCondition, player& player1, player& enemy); // Function holding most of the print statements
void setPlayer(player& player1); // Initializes player stats based on class choice
void setEnemy(int enemyChoice, player& player1); // Enemy initialization function
void room(player& player1, player& enemy); // Room function moving from room to room
void fight(player& player1, player& enemy); // Function handling battles between player and different enemies


// Main function where game plays
int main()
{
	srand(time(NULL));
	player player1;
	player enemy;
	init(player1, enemy);
	room(player1, enemy);
	room(player1, enemy);
	room(player1, enemy);
	room(player1, enemy);
	room(player1, enemy);
	room(player1, enemy);

	std::cin.ignore();
	std::cout << "Press Enter to exit the program\n";
	std::cin.ignore();
	return 0;
}

// Initialization function to setup the game properly
void init(player& player1, player& enemy)
{
	printCondition = 1;
	printData(printCondition, player1, enemy);
	printCondition = 2;
	printData(printCondition, player1, enemy);
	
	setPlayer(player1);

	printCondition = 3;
	printData(printCondition, player1, enemy);

	std::cin >> player1.name;
	
	printCondition = 4;
	printData(printCondition, player1, enemy);
	printCondition = 5;
	printData(printCondition, player1, enemy);
	
	roomChoice = rand() % 3 + 1;
}

// Prints relevant data to the screen i.e. health, mana, etc.
void printData(int printCondition, player& player1, player& enemy)
{
	switch (printCondition)
	{
	case 0:
	// Prints the players current health (will be done at the beginning of each room)
		std::cout << "HP: " << player1.HP << "\n\n";
		break;
	// This case is the opening statement when the game starts
	case 1:
		std::cout << "Welcome to the dungeon, adventurer!\n"
			<< "Inside treasure and glory awaits.\n"
			<< "But beware - monsters lurk in the dark too.\n"
			<< "Can you make it out alive?\n\n";
		break;
	case 2:
	// Still part of the setup - choosing their class
		std::cout << "What is your class, adventurer?\nType in your choice:\n"
			<< "1) knight\n2) mage\n3) thief\n4) archer\n";
		break;
	case 3:
	// Still part of the setup - choosing their name
		std::cout << "What is your name, adventurer?\n";
		break;
	case 4:
	// Final bit of setup before game technically "begins"
		std::cout << "\nWelcome to the dungeon, " << player1.name << ", the " << player1.playerClass << ".\n"
			<< "You hop off the wagon carrying a group of adventurers and head into the dungeon . . .\n\n";
		break;
	case 5:
	// Case 5 lists all the stats of the player
		std::cout << "Your stats: \n" << "HP: " << player1.HP << "\n"
			<< "MP: " << player1.MP << "\n"
			<< "Str: " << player1.strength << "\n"
			<< "Mgc: " << player1.magic << "\n"
			<< "Amr: " << player1.armor << "\n"
			<< "MR: " << player1.magicResist << "\n"
			<< "Spd: " << player1.speed << "\n\n";
		break;
	case 6:
	// Prints out the description for Room 1 Option 1
		std::cout << "You enter a dimly-lit room, illuminated by a single flickering torch in the corner.\n"
			<< "There is a small pile of bones in the corner, and below the torch a slight glimmer reflects off the ground.\n"
			<< "Aside from a few cobwebs and dust, there is nothing else in the room.\n"
			<< "\nWhat do you do?\n1) Take the torch\n2) Investigate the pile of bones\n3) Check the glimmer\n";
		break;
	case 7:
	// Appropriate response text to choice 1 for Room 1 Option 1
		std::cout << "You pick up the torch and move on.\n"
			<< "+1 Spd\n\n";
		break;
	case 8:
	// Appropriate response text to choice 2 for Room 1 Option 1
		std::cout << "You examine the bones, but there is nothing remarkable about them.\n"
			<< "You move on.\n\n";
		break;
	case 9:
	// Appropriate response text to choice 3 for Room 1 Option 1
		std::cout << "Rummaging around the floor you find an old amulet inscribed with ancient ruins.\n"
			<< "+1 MR\n-3 HP\n\n";
		break;
	case 10:
	// Prints out the description for Room 1 Option 2
		std::cout << "You walk down the entrance into a shallow cave. At the back of the cave, you catch a glimpse of something shiny.\n"
			<< "As you move closer to the object, you make out a strange insignia in the middle of a golden crown.\n"
			<< "Suddenly, a piercing shriek echos behind you. You turn around to see a giant bat blocking your path.\n"
			<< "You have no choice but to fight your way past.\n\n";
		break;
	case 11:
	// Response text for battles, player-side of battle
		std::cout << "What do you do?\n";
		do
		{
			std::cout << "1) Attack with your physical strength\n"
				<< "2) Attack with your magical strength\n3) Attempt to flee\n";
			std::cin >> move;
			if (move == "1")
			{
				if (player1.strength - enemy.armor > 0)
				{
					std::cout << "You attack dealing " << player1.strength - enemy.armor << " damage.\n";
				}
				else
				{
					std::cout << "You attack, but deal no damage...\n";
				}
			}
			else if (move == "2")
			{
				if (player1.magic - enemy.magicResist > 0)
				{
					std::cout << "You attack dealing " << player1.magic - enemy.magicResist << " damage.\n";
				}
				else
				{
					std::cout << "You attack, but deal no damage...\n";
				}
			}
			else if (move == "3")
			{
				std::cout << "You attempt to flee.\n";
			}
			else
			{
				std::cout << "Please enter your choice: \'1\', \'2\', or \'3\'\n";
			}
		} while (move != "1" && move != "2" && move != "3");
		break;
	case 12:
	// Text corresponding to enemy's action in fights
		if (enemy.strength - player1.armor > enemy.magic - player1.magicResist && enemy.strength - player1.armor > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << enemy.strength - player1.armor << " damage.\n";
		}
		else if (enemy.magic - player1.magicResist > enemy.strength - player1.armor && enemy.magic - player1.magicResist > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << enemy.magic - player1.magicResist << " damage.\n";
		}
		else
		{
			std::cout << "The " << enemy.playerClass << " attackes you, but you take no damage.\n";
		}
		break;
	case 13:
	// Corresponds to fleeing successfully from a fight
		std::cout << "You manage to escape, but just barely.\nYou continue on to the next room with your life.\n";
		break;
	case 14:
	// Corresponds to a failed attempt at fleeing from a fight
		std::cout << "You cannot get past the " << enemy.playerClass << ".\nYou are forced to fight and the "
			<< enemy.playerClass << " is relentless.\n";
		break;
	case 15:
	// Text for winning the fight
		std::cout << "You defeat the " << enemy.playerClass << "!\n";
		break;
	case 16:
	// Defeating the Giant Bat:
		std::cout << "\nTurning to the back of the cave, you approach the shining crown and pick it up.\n"
			<< "You place it on your head and feel a surge of magical power flow through you.\n"
			<< "+1 Mgc\n+1 MR\n";
		break;
	case 17:
	// Copy and pasting of current health for both player and enemy (used in fights)
		std::cout << "HP: " << player1.HP << std::endl
			<< enemy.playerClass << "\'s HP: " << enemy.HP << std::endl;
		break;
	case 18:
	// Corresponding text for Room 1 Option 3
		std::cout << "You walk into a long corridor filled on either side with tall marble columns.\n"
			<< "At the end of the corridor a chest lies, unopened.\n"
			<< "\nWhat do you do?\n1) Search the chest\n2) Ignore it - it most certainly is a trap\n3) Toss a rock along the path\n";
		break;
	case 19:
	// Response text to player choosing "1" in Room 1 Option 3
		std::cout << "You walk toward the chest, nothing strange happens.\nYou take a step further and open up the chest.\n"
			<< "Suddenly a hidden trap is triggered and you leap to the side to dodge it.\nYou avoid the trap, but injure yourself in the process.\n-3 HP\n";
		break;
	case 20:
	// Response text to player choosing "2" in Room 1 Option 3
		std::cout << "These types of pitfalls are far to common to fall for, you tell yourself.\nYou continue on, letting the next weary soul fall prey "
			<< "to this room's traps.\n";
		break;
	case 21:
	// Response text to player choosing "3" in Room 1 Option 3
		std::cout << "Clearly this is a trap, but you are an opportunist.\nYou toss a few rocks at different locations around the room.\n"
			<< "Suddenly, the trap is sprung! But with it, the chest is destroyed.\nTerrible luck, but you take a consolation prize: a blazing torch to guide "
			<< "your way\nand a sturdier breastplate still resting upon its unfortunate owner's bones.\n+1 Spd\n+1 Amr\n";
			// It could be fun to put an if statement here that does something different depending on the class the player chose
		break;
	case 22:
	// Corresponding text for Room 2 Option 1
		std::cout << "You continue into another room. The only object seen is a robe that will clearly raise your abilities.\n"
			<< "The only trouble is, it is hanging from the body of a skeleton warrior who just noticed you.\n"
			<< "The enemy charges right for you. You have no choice but to fight.\n";
		break;
	case 23:
	// Defeating the Skeleton Warrior
		std::cout << "Taking a sigh of relief, you look on as the pile of bones you just defeated crash to the ground, finally lifeless as it should be.\n"
			<< "You walk over to the remains and dawn the new cloak - spoils of victory.\n+7 HP\n+1 MR\n";
		break;
	case 24:
	// Corresponding text for Room 2 Option 2
		std::cout << "You walk into another room. At the far end a red vial rests on a pedestal.\n"
			<< "Beyond the strange potion, another passage leads further into the dungeon. The only way to go is forward.\n"
			<< "However, between you and the other side of the room lies a wide chasm. You must get past.\n"
			<< "The only option you can think of is to get a running start and leap over. You back up and bit and start running.\n";
		break;
	case 25:
	// Successfully leap over the chasm and get the vial
		std::cout << "It feels like you are flying for a moment before you look down and see the far edge of the chasm rushing to meet you.\n"
			<< "You made the jump, and after catching your breath, you walk over to the strange red vial and drink.\n"
			<< "+5 HP\n+1 Str\n+1 Mgc\n";
		break;
	case 26:
	// Failing the jump
		std::cout << "It feels like you are flying for a moment before you look down and see the chasm rush to meet you.\n"
			<< "You failed the jump and lose consciousness. Upon awaking, you find yourself mysteriously on the other side of the chasm.\n"
			<< "The vial's color has changed to blue. You reach over and drink.\n"
			<< "-1 Str\n-1 Mgc\n";
		break;
	case 27:
	// Corresponding text for Room 2 Option 3
		std::cout << "You find yourself looking down a long hallway. Along the sides, long dead adventurers hang chained to the walls, only bones remaining.\n"
			<< "At the far end of the hallway a small sack is strewn open and you can see some of its contents. You begin walking toward it.\n"
			<< "Suddenly, an arrow whizzes past you, narrowly missing your head. You snap back around to find one of the skeletons is not quite as dead as it seems.\n"
			<< "You have no choice but to fight.\n";
		break;
	case 28:
	// Defeating the Skeleton Archer
		std::cout << "As the skeleton collapses into a pile of bones, you brush the dust off your cloak and head to the end of the hallway.\n"
			<< "You open the sack, revealing two magic tonics and a wonderfully lightweight set of chainmail. You drink both potions and dawn the chainmail.\n"
			<< "+7 HP\n+1 Str\n+1 Amr\n";
		break;
	case 29:
	// Corresponding text for Room 3 Option 1
		std::cout << "You setup a small campfire in a spacious cavern of the dungeon. The going has been tough and you find yourself hungry.\n"
			<< "The small morsel of meat you have roasting over the fire sure smells delicious. The wolves think so too.\n"
			<< "You look around to find a pack of feral wolves growling at you as they creep closer to you and the fire.\n"
			<< "There is only one concerning one: it is nearly twice your size. It lunges toward you. You have no choice but to fight.\n";
		break;
	case 30:
	// Defeating the Feral Wolf
		std::cout << "With a final blow, the mighty beast drops to the ground, lifeless. Its bretheren scatter and run as you chase them for a short while with a torch from the campfire.\n"
			<< "Catching your breath, you look around and find a strange book. You open it and a rush of magical power overwhelms you. You head back to your campfire to eat.\n"
			<< "Wolf tastes good too...\n+7 HP\n+1 Str\n+1 Mgc\n";
		break;
	case 31:
	// Corresponding text for Room 3 Option 2
		std::cout << "You wander along for a while before ending up in a dimly lit room. You only take a few steps before you notice the putrid smell.\n"
			<< "As you turn around to find a different path, a small earthquake gives way and rocks and debris cover your path. Terrible luck.\n"
			<< "You turn to continue down the path. A few steps later, you hear strange growling and moaning coming from ahead.\n"
			<< "As you make your way to the source of the noise, you discover freshly rotting corpses strewn across the room.\n"
			<< "Only a few steps further reveals one of the corpses is not quite as dead as the rest. It has noticed you. You have no choice but to fight.\n";
		break;
	case 32:
	// Defeating the Undead
		std::cout << "The smell has become overwhelming as the monster finally slumps to the ground. You hurry along, eager to get out of the foul stench.\n"
			<< "A ways down the path, you happen across a poorly buried chest, still partially revealed above the ground. You dig it up and pry it open.\n"
			<< "Inside a sealed scroll and some booze lie. You open the scroll and ancient magic enchants you. The alcohol is a nice plus too.\n"
			<< "+10 HP\n+1 Str\n+1 Amr\n+1 Spd\n";
		break;
	case 33:
	// Corresponding text for Room 3 Option 3
		std::cout << "You find yourself in a small room with tongues of green flame floating in the air. You look to end of the room and find a skeleton mage staring at you, its eyes glowing green.\n"
			<< "It points at you and a stone wall slams agains the entryway, blocking your path. As it stands up from its throne chair, you ready your weapon. You have no choice but to fight.\n";
		break;
	case 34:
	// Defeating the Lich
		std::cout << "With the final blow, the light fades from the Lich's hollow eyes and it tumbles to the ground in a pile of bones. The throne holds a small collection of translucent gems.\n"
			<< "You pry them loose and take them with you. As you hold them, you feel a surge of magical energy. You are rejuvinated and empowered.\n"
			<< "+7 HP\n+1 Mgc\n+1 MR\n";
		break;
	case 35:
	// Corresponding text for Room 4 Option 1
		std::cout << "You walk into a bare room. Suddenly, upon taking another step, the room transforms into a bright spectacle of white and light.\n"
			<< "You find yourself standing before a fantastical fountain: lights flicker about, dancing merrily around. A voice calls, \"Drink.\"\n"
			<< "You step toward the fountain and drink deeply. You are rejuvinated.\n";
		if (player1.HP < 20)
		{
			std::cout << "HP increased to 30.\n";
		}
		else
		{
			std::cout << "+2 Str\n+2 Amr\n+2 Mgc\n+2 MR\n";
		}
		break;
	case 36:
	// Corresponding text for Room 4 Option 2
		std::cout << "You walk into a small, barren room. The only object in the room is an oval slab of stone. Eerie mist twists and falls from the stone.\n"
			<< "You hear a voice beckoning you to come closer and offer a sacrifice. A sacrifice of blood. Your blood. Power is promised in return.\n"
			<< "Do you offer a sacrifice?\n";
		break;
	case 37:
	// Text for blood altar options (Room 4 Option 2)
		do
		{
			std::cout << "Offer a sacrifice? (y\\n)\n";
			std::cin >> move;
			if (move == "y")
			{
				std::cout << "You offer a sacrifice.\n-3 HP\n+1 Str\n";
				player1.HP = player1.HP - 3;
				player1.strength++;
				printCondition = 0;
				printData(printCondition, player1, enemy);
				if (player1.HP <= 0)
				{
					break;
				}
			}
			else if (move == "n")
			{
				std::cout << "You do not offer a sacrifice.\n";
				printCondition = 5;
				printData(printCondition, player1, enemy);
			}
			else
			{
				std::cout << "Please select \'y\' or \'n\'\n";
			}
		} while (move != "n");
		break;
	case 38:
	// Corresponding text to Room 4 Option 3
		std::cout << "You walk into a strange little room. Inside the strange little room sits a strange little goblin in a stange little suit. He beckons you to sit.\n"
			<< "You sit and he eagerly begins explaining rules to a game you must now play. A game of chance. Try you luck and win big.\n"
			<< "Or try your luck and lose big. Your choice.\n";
		break;
	case 39:
	// Playing the game or passing (Room 4 Option 3)
		do
		{
			std::cout << "Roll the dice? (y\\n)\n";
			std::cin >> move;
			if (move == "y")
			{
				int chance = rand() % 6 + 1;
				std::cout << "You roll a " << chance << ".\n";
				if (chance < 3)
				{
					std::cout << "\"Unlucky,\" the strange little goblin says and wacks you across the head. You lose your footing and fall to the ground.\n"
						<< "Before you can recover, the goblin has disapeared, along with your sack of food. Your head is ringing too. Terrible luck.\n"
						<< "-2 HP\n";
					player1.HP = player1.HP - 2;
					if (player1.HP <= 0)
					{
						break;
					}
				}
				else if (chance < 5)
				{
					std::cout << "\"Ooohh lucky!\" The strange little goblin exclaims. He pulls out a small vial and pours some of its contents onto your weapon before you can react.\n"
						<< "You go to strike the strange little creature, but he has already dissapeared. You look back at your weapon to see it is glowing magically.\n"
						<< "+1 Str\n";
					player1.strength++;
				}
				else
				{
					std::cout << "\"Luck favors you this time.\" The strange little goblin says, with a smirk. He pulls out three vials and hands them to you.\n"
						<< "You look up to thank the strange little goblin, but he has already dissapeared. You try one of the vials and find it has a beneficial effect.\n"
						<< "You quickly quaff the other two, which have similar effects.\n"
						<< "+7 HP\n+2 Str\n+2 Mgc\n";
					player1.HP = player1.HP + 7;
					player1.strength = player1.strength + 2;
					player1.magic = player1.magic + 2;
				}
			}
			else if (move == "n")
			{
				std::cout << "You choose not to play. The strange little goblin fumes with rage and lunges at you. You have no choice but to fight.\n";
			}
			else
			{
				std::cout << "Please select \'y\' or \'n\'\n";
			}
		} while (move != "y" && move != "n");
		break;
	case 40:
	// Defeating the Strange Little Goblin
		std::cout << "The strange creature freezes with the final blow and topples over, almost comically. You search his bag and discover three vials.\n"
			<< "You taste them to be sure: they smell, but do their job. You quaff them as quickly as you can.\n"
			<< "+7 HP\n+1 Amr\n+1 MR\n";
		break;
	case 41:
	// Corresponding text to Room 5 Option 1
		std::cout << "You trudge along until you find yourself in a large circular room. It is pretty plain, aside from some unsettling symbols patterned around the room.\n"
			<< "Suddenly, eerie laughter echos around you. You spring around to find a malicious Imp closing in on you. \"How dare you ruin the ritual?!\" It screams.\n"
			<< "You have no choice but to fight.\n";
		break;
	case 42:
	// Defeating the Imp
		std::cout << "The Imp laughs, even as it collapses to the floor. Its body scatters in a pile of ash. In the middle of the room, you spot a precious gem, swirling with energy.\n"
			<< "You take it and hope for the best.\n";
		if (player1.strength > player1.magic)
		{
			std::cout << "As you clutch the gem, you feel your strength increasing while your magic power fades.\n+3 Str\n";
			player1.strength = player1.strength + 3;
			if (player1.magic >= 3)
			{
				std::cout << "-3 Mgc\n";
				player1.magic = player1.magic - 3;
			}
			else
			{
				std::cout << "Your magic power is reduced to 0.\n";
				player1.magic = 0;
			}
		}
		else if (player1.magic > player1.strength)
		{
			std::cout << "As you clutch the gem, you feel your magic power increasing while your strength fades.\n+3 Mgc\n";
			player1.magic = player1.magic + 3;
			if (player1.strength >= 3)
			{
				std::cout << "-3 Str\n";
				player1.strength = player1.strength - 3;
			}
			else
			{
				std::cout << "Your strength is reduced to 0.\n";
				player1.strength = 0;
			}
		}
		else
		{
			std::cout << "As you clutch the gem, you feel your agility and speed increase, while your energy levels fade.\n+3 Spd\n-3 HP\n";
			player1.speed = player1.speed + 3;
			player1.HP = player1.HP - 3;
			if (player1.HP <= 0)
			{
				printCondition = -1;
				printData(printCondition, player1, enemy);
				roomChoice = -1;
				break;
			}
		}
		break;
	case 43:
	// Corresponding text for Room 5 Option 2
		std::cout << "You enter a long forgotten throne room. It looks as though the last thing to happen here was a party as there are still goblets and trays everywhere.\n"
			<< "You rummage around, trying to find anything of value. That\'s when you notice it: hovering in front of the throne, a Beholder as big as you are, its eye still closed.\n"
			<< "You take a step back, not to wake it, but bump a silver cup and the noise echoes through the room. Its eye shoots open and stares you down.\n"
			<< "You have no choice but to fight.\n";
		break;
	case 44:
	// Defeating the Beholder
		std::cout << "Finally, the unholy floating eye collapses to the floor. You catch your breath and look victoriously up at the throne.\n"
			<< "A magic tome lays resting on the side, concealed and out of view, except at the right angle. Lucky you. You walk over and brush the dust off.\n"
			<< "Opening it, a surge of energy coarses through you.\n"
			<< "+10 HP\n+3 Amr\n+3 MR\n";
		break;
	case 45:
	// Corresponding text for Room 5 Option 3
		std::cout << "You walk into a room lined with ancient earthen guards. At the far end of the room, an unopened chest rests. You walk down the room towards the chest.\n"
			<< "As you do, you hear low creaking and look to find the ancient earth golems filling with life once more. However, time has not been kind to them.\n"
			<< "All the earthen guards fall to the floor, broken. Time has left its mark. You turn back to the chest. It seems one earthen guard has withstood the test of time.\n"
			<< "It stands between you and the chest. You have no choice but to fight.\n";
		break;
	case 46:
	// Defeating the Earth Elemental
		std::cout << "With a loud crash, the ancient machine tumbles to the floor. It still has its eye fixed on you, but it is unable to move and no longer poses a threat.\n"
			<< "You walk over to the chest and open it, revealing crystallized magical energy. Consuming raw magical energy is said to greatly increase one\'s prowess, but with significant risk.\n"
			<< "You\'ve come too far now to not take that risk. You consume the crystal.\n"
			<< "+10 HP\n+5 Mgc\n-1 MR\n";
		break;
	case 47:
	// Corresponding text for Room 6 Option 1 (Boss option 1 - Stone Golem)
		std::cout << "This is it. Your adventure has lead to this. You ready yourself as you walk into the final room. One way or the other, everything ends here.\n"
			<< "You push open two large doors and walk through a tall archway into a long hall. In front of you lies a giant ancient stone golem.\n"
			<< "It creaks and hums with ancient magic as you walk closer. The ancient core of this magical automation would set you up for the rest of your life.\n"
			<< "There\'s no backing down now. No running away. It is do or die time. You stand and fight.\n";
		break;
	case 48:
	// Defeating the Stone Golem
		std::cout << "You\'ve done it. The Stone Golem has fallen. You walk over to your vanquished foe and pry the ancient core from its remains.\n"
			<< "The core still pulses with ancient magical energy. You gather your belongins and the ancient core and head out of the dungeon. One of the few to ever return.\n";
		break;
	case 49:
	// Boss fight, player move text options
		std::cout << "What do you do?\n";
		do
		{
			std::cout << "1) Attack with your physical strength\n"
				<< "2) Attack with your magical strength\n"
				<< "3) Focus and channel your strength\n"
				<< "4) Focus and channel your magic\n";
			std::cin >> move;
			if (move == "1")
			{
				if (player1.strength - enemy.armor > 0)
				{
					std::cout << "You attack dealing " << player1.strength - enemy.armor << " damage.\n";
				}
				else
				{
					std::cout << "You attack, but deal no damage...\n";
				}
			}
			else if (move == "2")
			{
				if (player1.magic - enemy.magicResist > 0)
				{
					std::cout << "You attack dealing " << player1.magic - enemy.magicResist << " damage.\n";
				}
				else
				{
					std::cout << "You attack, but deal no damage...\n";
				}
			}
			else if (move == "3")
			{
				std::cout << "You gather your focus, channeling your power.\n+1 Str\n";
			}
			else if (move == "4")
			{
				std::cout << "You gather your thoughts, channeling your energy.\n+1 Mgc\n";
			}
			else
			{
				std::cout << "Please enter your choice: \'1\', \'2\', \'3\', or \'4\'\n";
			}
		} while (move != "1" && move != "2" && move != "3" && move != "4");
		break;
	case 50:
		// Text corresponding to stone golem's action in boss fight
		if (3 * enemy.strength - player1.armor > 3 * enemy.magic - player1.magicResist && 3 * enemy.strength - player1.armor > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << 3 * enemy.strength - player1.armor << " damage.\n";
		}
		else if (3 * enemy.magic - player1.magicResist > 3 * enemy.strength - player1.armor && 3 * enemy.magic - player1.magicResist > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << 3 * enemy.magic - player1.magicResist << " damage.\n";
		}
		else
		{
			std::cout << "The " << enemy.playerClass << " attackes you, but you take no damage.\n";
		}
		break;
	case 51:
	// Corresponding text for Room 6 Option 2 (Boss option 2 - Dragon)
		std::cout << "This is it. Your adventure has lead to this. You ready yourself as you walk into the final room. One way or the other, everything ends here.\n"
			<< "You push open two large doors and walk through a tall archway into a long hall. In front of you lies an ancient dragon, still sleeping.\n"
			<< "It eyes snap open as you walk closer. The dragon sits atop a pile of gold large enough to set you up for the rest of your life.\n"
			<< "There\'s no backing down now. No running away. It is do or die time. You stand and fight.\n";
		break;
	case 52:
	// Defeating the Dragon
		std::cout << "With a mighty roar, the dragon buckles and falls, its head slams to the ground by your feet. You have slain the mighty beast.\n"
			<< "You fashion a make-shift wagon out of pieces in the room and gather all the dragon\'s gold. You head out of the dungeon, one of the few to ever return.\n";
		break;
	case 53:
	// Text corresponding to dragon's action in boss fight
		if (2 * enemy.strength - player1.armor > 2 * enemy.magic - player1.magicResist && 2 * enemy.strength - player1.armor > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << 2 * enemy.strength - player1.armor << " damage.\n";
		}
		else if (2 * enemy.magic - player1.magicResist > 2 * enemy.strength - player1.armor && 2 * enemy.magic - player1.magicResist > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << 2 * enemy.magic - player1.magicResist << " damage.\n";
		}
		else
		{
			std::cout << "The " << enemy.playerClass << " attackes you, but you take no damage.\n";
		}
		break;
	case 54:
	// Corresponding text for Room 6 Option 3 (Boss option 3 - Necromancer)
		std::cout << "This is it. Your adventure has lead to this. You ready yourself as you walk into the final room. One way or the other, everything ends here.\n"
			<< "You push open two large doors and walk through a tall archway into a long hall. In front of you an evil Necromancer stares you down from his throne chair, a wicked grin across his face.\n"
			<< "He beckons you to come closer, taunting you with a ridiculuous pile of gold to his side, easily enough to set you up for life. You walk forward and ready your weapon.\n"
			<< "There\'s no backing down now. No running away. It is do or die time. You stand and fight.\n";
		break;
	case 55:
	// Defeating the Necromancer
		std::cout << "The Necromancer looks at you as he falls, a mix of extreme fury and confusion contort his face. He hits the ground and his summoned minions follow suit, lifeless without his magic.\n"
			<< "You fashion a make-shift wagon and gather the spoils. You head out of the dungeon, one of the few to ever return.\n";
		break;
	case 56:
	// Text corresponding to necromancer's action in boss fight
		if (enemy.strength - player1.armor > enemy.magic - player1.magicResist && enemy.strength - player1.armor > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << enemy.strength - player1.armor << " damage.\n";
		}
		else if (enemy.magic - player1.magicResist > enemy.strength - player1.armor && enemy.magic - player1.magicResist > 0)
		{
			std::cout << "The " << enemy.playerClass << " attacks you dealing " << enemy.magic - player1.magicResist << " damage.\n";
		}
		else
		{
			std::cout << "The " << enemy.playerClass << " attackes you, but you take no damage.\n";
		}
		break;
	case 57:
	// Text corresponding to the necromancer summoning a minion
		std::cout << "The Necromancer raises his hands and chants some words in a language unfamiliar to you. He summons a monster to fight you.\n";
		break;
	case 58:
	// Congratulation & Thank You for playing
		std::cout << "\n\n=========================\n"
			<< " YOU DEFEATED THE DUNGEON\n"
			<< "==========================\n\n"
			<< "Thank you for playing!\nTry the dungeon again, it will be a different experience!\n";
		break;
	default:
		std::cout << "\n\n===============\n   GAME OVER   \n===============\n\n";
		break;
	}
}

// Function used to initialize the player's stats based on the "class" they choose to start out
void setPlayer(player& player1)
{
	do
	{
		std::cin >> player1.playerClass;
		if (player1.playerClass == "knight")
		{
			player1.HP = 25;
			player1.MP = 3;
			player1.strength = 8;
			player1.armor = 9;
			player1.magic = 2;
			player1.magicResist = 4;
			player1.speed = 4;
		}
		else if (player1.playerClass == "mage")
		{
			player1.HP = 18;
			player1.MP = 12;
			player1.strength = 4;
			player1.armor = 3;
			player1.magic = 10;
			player1.magicResist = 7;
			player1.speed = 6;
		}
		else if (player1.playerClass == "thief")
		{
			player1.HP = 15;
			player1.MP = 6;
			player1.strength = 9;
			player1.armor = 3;
			player1.magic = 5;
			player1.magicResist = 3;
			player1.speed = 9;
		}
		else if (player1.playerClass == "archer")
		{
			player1.HP = 20;
			player1.MP = 5;
			player1.strength = 6;
			player1.armor = 4;
			player1.magic = 6;
			player1.magicResist = 6;
			player1.speed = 7;
		}
		else
		{
			std::cout << "Please type in what class you want:\n"
				<< "\'knight\', \'mage\', \'thief\', or \'archer\'\n";

		}
	} while (player1.playerClass != "knight" && player1.playerClass != "mage" && player1.playerClass != "thief" && player1.playerClass != "archer");
}

// Function used to initialize different enemies based on what encounter they are given by a number
void setEnemy(int enemyChoice, player& enemy)
{
	// Choice 1: Giant Bat
	if (enemyChoice == 1)
	{
		enemy.HP = 15;
		enemy.MP = 3;
		enemy.strength = 5;
		enemy.armor = 1;
		enemy.magic = 5;
		enemy.magicResist = 1;
		enemy.speed = 9;
		enemy.playerClass = "Giant Bat";
	}
	// Choice 2: Skeleton Warrior
	else if (enemyChoice == 2)
	{
		enemy.HP = 25;
		enemy.MP = 3;
		enemy.strength = 6;
		enemy.armor = 3;
		enemy.magic = 5;
		enemy.magicResist = 2;
		enemy.speed = 3;
		enemy.playerClass = "Skeleton Warrior";
	}
	// Choice 3: Skeleton Archer
	else if (enemyChoice == 3)
	{
		enemy.HP = 20;
		enemy.MP = 3;
		enemy.strength = 5;
		enemy.armor = 3;
		enemy.magic = 3;
		enemy.magicResist = 3;
		enemy.speed = 6;
		enemy.playerClass = "Skeleton Archer";
	}
	// Choice 4: Skeleton Mage (Lich)
	else if (enemyChoice == 4)
	{
		enemy.HP = 15;
		enemy.MP = 5;
		enemy.strength = 2;
		enemy.armor = 3;
		enemy.magic = 7;
		enemy.magicResist = 5;
		enemy.speed = 5;
		enemy.playerClass = "Lich";
	}
	// Choice 5: Undead
	else if (enemyChoice == 5)
	{
		enemy.HP = 40;
		enemy.MP = 3;
		enemy.strength = 5;
		enemy.armor = 5;
		enemy.magic = 1;
		enemy.magicResist = 3;
		enemy.speed = 2;
		enemy.playerClass = "Undead";
	}
	// Choice 6: Feral Wolf
	else if (enemyChoice == 6)
	{
		enemy.HP = 15;
		enemy.MP = 3;
		enemy.strength = 7;
		enemy.armor = 1;
		enemy.magic = 3;
		enemy.magicResist = 1;
		enemy.speed = 11;
		enemy.playerClass = "Feral Wolf";
	}
	// Choice 7: Strange Little Goblin
	else if (enemyChoice == 7)
	{
		enemy.HP = 30;
		enemy.MP = 5;
		enemy.strength = 6;
		enemy.armor = 4;
		enemy.magic = 5;
		enemy.magicResist = 3;
		enemy.speed = 6;
		enemy.playerClass = "Strange Little Goblin";
	}
	// Choice 8: Imp
	else if (enemyChoice == 8)
	{
		enemy.HP = 15;
		enemy.MP = 5;
		enemy.strength = 7;
		enemy.armor = 2;
		enemy.magic = 7;
		enemy.magicResist = 2;
		enemy.speed = 7;
		enemy.playerClass = "Imp";
	}
	// Choice 9: Earth Elemental
	else if (enemyChoice == 9)
	{
		enemy.HP = 30;
		enemy.MP = 3;
		enemy.strength = 6;
		enemy.armor = 7;
		enemy.magic = 7;
		enemy.magicResist = 7;
		enemy.speed = 3;
		enemy.playerClass = "Earth Elemental";
	}
	// Choice 10: Oculothorax (Giant floating eye monster)
	else if (enemyChoice == 10)
	{
		enemy.HP = 20;
		enemy.MP = 10;
		enemy.strength = 2;
		enemy.armor = 4;
		enemy.magic = 10;
		enemy.magicResist = 8;
		enemy.speed = 4;
		enemy.playerClass = "Beholder";
	}
	// Choice 11: Stone Golem
	else if (enemyChoice == 11)
	{
		enemy.HP = 50;
		enemy.MP = 4;
		enemy.strength = 6;
		enemy.armor = 13;
		enemy.magic = 4;
		enemy.magicResist = 7;
		enemy.speed = 1;
		enemy.playerClass = "Stone Golem";
	}
	// Choice 12: Dragon
	else if (enemyChoice == 12)
	{
		enemy.HP = 40;
		enemy.MP = 7;
		enemy.strength = 6;
		enemy.armor = 10;
		enemy.magic = 6;
		enemy.magicResist = 10;
		enemy.speed = 6;
		enemy.playerClass = "Dragon";
	}
	// Choice 13: Necromancer
	else if (enemyChoice == 13)
	{
		enemy.HP = 30;
		enemy.MP = 10;
		enemy.strength = 4;
		enemy.armor = 7;
		enemy.magic = 10;
		enemy.magicResist = 11;
		enemy.speed = 4;
		enemy.playerClass = "Necromancer";
	}
}

// Room function - room structure
void room(player& player1, player& enemy)
{
	std::string playerChoice;
	int enemyChoice;

	switch (roomChoice)
	{
	case 1:
	// This is Room 1 Option 1. The following code lays out the room logic
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 6;
		printData(printCondition, player1, enemy);
		do
		{
			std::cin >> playerChoice;
			if (playerChoice == "1")
			{
				printCondition = 7;
				printData(printCondition, player1, enemy);
				player1.speed++;
			}
			else if (playerChoice == "2")
			{
				printCondition = 8;
				printData(printCondition, player1, enemy);
			}
			else if (playerChoice == "3")
			{
				printCondition = 9;
				printData(printCondition, player1, enemy);
				player1.magicResist++;
				player1.HP = player1.HP - 3;
			}
			else
			{
				std::cout << "Please type \'1\', \'2\', or \'3\'\n";
			}
		} while (playerChoice != "1" && playerChoice != "2" && playerChoice != "3");
		roomChoice = rand() % 3 + 4;
		break;
	case 2:
	// This is Room 1 Option 2. The room logic is below
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 10;
		printData(printCondition, player1, enemy);
		enemyChoice = 1;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 4;
		break;
	case 3:
	// This is Room 1 Option 3. Room logic follows below
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 18;
		printData(printCondition, player1, enemy);
		do
		{
			std::cin >> playerChoice;
			if (playerChoice == "1")
			{
				printCondition = 19;
				printData(printCondition, player1, enemy);
				player1.HP = player1.HP - 3;
			}
			else if (playerChoice == "2")
			{
				printCondition = 20;
				printData(printCondition, player1, enemy);
			}
			else if (playerChoice == "3")
			{
				printCondition = 21;
				printData(printCondition, player1, enemy);
				player1.speed++;
				player1.armor++;
			}
			else
			{
				std::cout << "Please type \'1\', \'2\', or \'3\'\n";
			}
		} while (playerChoice != "1" && playerChoice != "2" && playerChoice != "3");
		roomChoice = rand() % 3 + 4;
		break;
	case 4:
	// This is Room 2 Option 1
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 22;
		printData(printCondition, player1, enemy);
		enemyChoice = 2;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 7;
		break;
	case 5:
	// This is Room 2 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 24;
		printData(printCondition, player1, enemy);
		std::cin.ignore();
		std::cin.ignore();
		if ((player1.speed + rand() % 3) - 6 >= 0)
		{
			// Success, cleared the jump (speed check)
			printCondition = 25;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 5;
			player1.strength++;
			player1.magic++;
		}
		else
		{
			// Failed the jump, loose some strength (speed check)
			printCondition = 26;
			printData(printCondition, player1, enemy);
			player1.strength--;
			player1.magic--;
		}
		roomChoice = rand() % 3 + 7;
		break;
	case 6:
	// This is Room 2 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 27;
		printData(printCondition, player1, enemy);
		enemyChoice = 3;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 7;
		break;
	case 7:
	// This is Room 3 Option 1
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 29;
		printData(printCondition, player1, enemy);
		enemyChoice = 6;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 10;
		break;
	case 8:
	// This is Room 3 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 31;
		printData(printCondition, player1, enemy);
		enemyChoice = 5;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 10;
		break;
	case 9:
	// This is Room 3 Option 3
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 33;
		printData(printCondition, player1, enemy);
		enemyChoice = 4;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 10;
		break;
	case 10:
	// This is Room 4 Option 1
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 35;
		printData(printCondition, player1, enemy);
		if (player1.HP < 20)
		{
			player1.HP = 30;
		}
		else
		{
			player1.strength = player1.strength + 2;
			player1.armor = player1.armor + 2;
			player1.magic = player1.magic + 2;
			player1.magicResist = player1.magicResist + 2;
		}
		roomChoice = rand() % 3 + 13;
		break;
	case 11:
	// This is Room 4 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 36;
		printData(printCondition, player1, enemy);
		printCondition = 37;
		printData(printCondition, player1, enemy);
		if (player1.HP <= 0)
		{
			printCondition = -1;
			printData(printCondition, player1, enemy);
			roomChoice = -1;
			break;
		}
		roomChoice = rand() % 3 + 13;
		break;
	case 12:
	// This is Room 4 Option 3
		printCondition = 38;
		printData(printCondition, player1, enemy);
		printCondition = 39;
		printData(printCondition, player1, enemy);
		if (player1.HP <= 0)
		{
			printCondition = -1;
			printData(printCondition, player1, enemy);
			roomChoice = -1;
			break;
		}
		if (move == "n")
		{
			enemyChoice = 7;
			setEnemy(enemyChoice, enemy);
			fight(player1, enemy);
		}
		roomChoice = rand() % 3 + 13;
		break;
	case 13:
	// This is Room 5 Option 1
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 41;
		printData(printCondition, player1, enemy);
		enemyChoice = 8;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 16;
		break;
	case 14:
	// This is Room 5 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 43;
		printData(printCondition, player1, enemy);
		enemyChoice = 10;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 16;
		break;
	case 15:
	// This is Room 5 Option 3
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 45;
		printData(printCondition, player1, enemy);
		enemyChoice = 9;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		roomChoice = rand() % 3 + 16;
		break;
	case 16:
	// This is Room 6 Option 1
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 47;
		printData(printCondition, player1, enemy);
		enemyChoice = 11;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		break;
	case 17:
	// This is Room 6 Option 2
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 51;
		printData(printCondition, player1, enemy);
		enemyChoice = 12;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		break;
	case 18:
	// This is Room 6 Option 3
		printCondition = 0;
		printData(printCondition, player1, enemy);
		printCondition = 54;
		printData(printCondition, player1, enemy);
		enemyChoice = 13;
		setEnemy(enemyChoice, enemy);
		fight(player1, enemy);
		break;
	default:

		break;
	}
}

// Fight function handling interactions between player and different enemies
void fight(player& player1, player& enemy)
{
	printCondition = 17;
	printData(printCondition, player1, enemy);
	int turn;
	if (enemy.speed - player1.speed > 0)
	{
		std::cout << "The enemy " << enemy.playerClass << " is faster than you.\n";
		turn = 0;
	}
	else
	{
		turn = 1;
	}
	do
	{
		// Golem fight logic
		if (enemy.playerClass == "Stone Golem")
		{
			// Player's action (same as usual fights)
			if (turn % 2 == 1)
			{
				printCondition = 49;
				printData(printCondition, player1, enemy);
				// Player turn logic for fights
				if (move == "1")
				{
					if (player1.strength - enemy.armor > 0)
					{
						// Player attacks with physical strength
						enemy.HP = enemy.HP - (player1.strength - enemy.armor);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "2")
				{
					if (player1.magic - enemy.magicResist > 0)
					{
						// Player attacks with magical strength
						enemy.HP = enemy.HP - (player1.magic - enemy.magicResist);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "3")
				{
					// Player gathers their strength and +1 Str at the cost of their turn
					player1.strength++;
				}
				else if (move == "4")
				{
					// Player gathers their magical power and +1 Mgc at the cost of their turn
					player1.magic++;
				}
				turn++;
			}
			else
			{
				// Stone Golem turn fight logic
				if (turn % 6 == 4)
				{
					printCondition = 50;
					printData(printCondition, player1, enemy);
					if (3 * enemy.strength - player1.armor > 3 * enemy.magic - player1.magicResist && 3 * enemy.strength - player1.armor > 0)
					{
						player1.HP = player1.HP - (3 * enemy.strength - player1.armor);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else if (3 * enemy.magic - player1.magicResist > 3 * enemy.strength - player1.armor && 3 * enemy.magic - player1.magicResist > 0)
					{
						player1.HP = player1.HP - (3 * enemy.magic - player1.magicResist);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else
					{
						// Enemy does no damage
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					turn++;
				}
				else if (turn % 6 >= 2)
				{
					// Golem is just about to attack
					std::cout << "The " << enemy.playerClass << " begins to glow. Its attack is almost ready.\n";
					turn++;
				}
				else
				{
					// Golem starts winding up for attack
					std::cout << "The " << enemy.playerClass << " let\'s loose a low rumble, readying for a powerful attack.\n";
					turn++;
				}
			}
		}
		// Dragon fight logic
		else if (enemy.playerClass == "Dragon")
		{
			// Player's action (same as usual fights)
			if (turn % 2 == 1)
			{
				printCondition = 49;
				printData(printCondition, player1, enemy);
				// Player turn logic for fights
				if (move == "1")
				{
					if (player1.strength - enemy.armor > 0)
					{
						// Player attacks with physical strength
						enemy.HP = enemy.HP - (player1.strength - enemy.armor);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "2")
				{
					if (player1.magic - enemy.magicResist > 0)
					{
						// Player attacks with magical strength
						enemy.HP = enemy.HP - (player1.magic - enemy.magicResist);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "3")
				{
					// Player gathers their strength and +1 Str at the cost of their turn
					player1.strength++;
				}
				else if (move == "4")
				{
					// Player gathers their magical power and +1 Mgc at the cost of their turn
					player1.magic++;
				}
				turn++;
			}
			else
			{
				// Dragon turn fight logic
				if (turn % 4 == 2)
				{
					printCondition = 53;
					printData(printCondition, player1, enemy);
					if (2 * enemy.strength - player1.armor > 2* enemy.magic - player1.magicResist && 2* enemy.strength - player1.armor > 0)
					{
						player1.HP = player1.HP - (2 * enemy.strength - player1.armor);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else if (2 * enemy.magic - player1.magicResist > 2 * enemy.strength - player1.armor && 2 * enemy.magic - player1.magicResist > 0)
					{
						player1.HP = player1.HP - (2 * enemy.magic - player1.magicResist);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else
					{
						// Enemy does no damage
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					turn++;
				}
				else
				{
					// Dragon starts winding up for attack
					std::cout << "The " << enemy.playerClass << " let\'s loose a low rumble, readying a powerful attack.\n";
					turn++;
				}
			}
		}
		// Necromancer fight logic
		else if (enemy.playerClass == "Necromancer")
		{
			// Player's action (same as usual fights)
			if (turn % 2 == 1)
			{
				printCondition = 49;
				printData(printCondition, player1, enemy);
				// Player turn logic for fights
				if (move == "1")
				{
					if (player1.strength - enemy.armor > 0)
					{
						// Player attacks with physical strength
						enemy.HP = enemy.HP - (player1.strength - enemy.armor);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "2")
				{
					if (player1.magic - enemy.magicResist > 0)
					{
						// Player attacks with magical strength
						enemy.HP = enemy.HP - (player1.magic - enemy.magicResist);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "3")
				{
					// Player gathers their strength and +1 Str at the cost of their turn
					player1.strength++;
				}
				else if (move == "4")
				{
					// Player gathers their magical power and +1 Mgc at the cost of their turn
					player1.magic++;
				}
				turn++;
			}
			else
			{
				// Necromancer turn fight logic
				if (turn % 6 == 4)
				{
					// The necromancer summons a minion to fight you
					printCondition = 57;
					printData(printCondition, player1, enemy);
					player minion;
					minion.playerClass = "Minion";
					minion.HP = 10;
					minion.strength = 7;
					minion.armor = 3;
					minion.magic = 7;
					minion.magicResist = 3;
					minion.speed = 5;
					fight(player1, minion);
					turn = 0;
				}
				else
				{
					printCondition = 56;
					printData(printCondition, player1, enemy);
					if (enemy.strength - player1.armor > enemy.magic - player1.magicResist && enemy.strength - player1.armor > 0)
					{
						player1.HP = player1.HP - (enemy.strength - player1.armor);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else if (enemy.magic - player1.magicResist > enemy.strength - player1.armor && enemy.magic - player1.magicResist > 0)
					{
						player1.HP = player1.HP - (enemy.magic - player1.magicResist);
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					else
					{
						// Enemy does no damage
						std::cout << "\nHP: " << player1.HP << std::endl;
						std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
					}
					turn++;
				}

			}
		}
		else
		{
			// Player's action
			if (turn % 2 == 1)
			{
				printCondition = 11;
				printData(printCondition, player1, enemy);
				// Player turn logic for fights
				if (move == "1")
				{
					if (player1.strength - enemy.armor > 0)
					{
						// Player attacks with physical strength
						enemy.HP = enemy.HP - (player1.strength - enemy.armor);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "2")
				{
					if (player1.magic - enemy.magicResist > 0)
					{
						// Player attacks with magical strength
						enemy.HP = enemy.HP - (player1.magic - enemy.magicResist);
					}
					else
					{
						// Player does no damage
					}
				}
				else if (move == "3")
				{
					int chance = rand() % 3;
					if (player1.speed + chance - enemy.speed + chance > 0)
					{
						// Player runs away and doesn't get any loot, but stays alive
						printCondition = 13;
						printData(printCondition, player1, enemy);
						break;
					}
					else
					{
						// Player fails and looses turn
						printCondition = 14;
						printData(printCondition, player1, enemy);
					}
				}
				turn++;
			}
			// Enemy's action
			else
			{
				printCondition = 12;
				printData(printCondition, player1, enemy);
				if (enemy.strength - player1.armor > enemy.magic - player1.magicResist && enemy.strength - player1.armor > 0)
				{
					player1.HP = player1.HP - (enemy.strength - player1.armor);
					std::cout << "\nHP: " << player1.HP << std::endl;
					std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
				}
				else if (enemy.magic - player1.magicResist > enemy.strength - player1.armor && enemy.magic - player1.magicResist > 0)
				{
					player1.HP = player1.HP - (enemy.magic - player1.magicResist);
					std::cout << "\nHP: " << player1.HP << std::endl;
					std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
				}
				else
				{
					// Enemy does no damage
					std::cout << "\nHP: " << player1.HP << std::endl;
					std::cout << enemy.playerClass << " HP: " << enemy.HP << std::endl;
				}
				turn++;
			}
		}
	} while (enemy.HP > 0 && player1.HP > 0);
	if (player1.HP <= 0)
	{
		// Player looses - game over
		printCondition = -1;
		printData(printCondition, player1, enemy);
	}
	else if (enemy.HP <= 0)
	{
		// Player wins and gets the stuff from the room if there is any
		printCondition = 15;
		printData(printCondition, player1, enemy);
		if (enemy.playerClass == "Giant Bat")
		{
			printCondition = 16;
			printData(printCondition, player1, enemy);
			player1.magicResist++;
			player1.magic++;
		}
		else if (enemy.playerClass == "Skeleton Warrior")
		{
			printCondition = 23;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 7;
			player1.magicResist++;
		}
		else if (enemy.playerClass == "Skeleton Archer")
		{
			printCondition = 28;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 7;
			player1.strength++;
			player1.armor++;
		}
		else if (enemy.playerClass == "Lich")
		{
			printCondition = 34;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 7;
			player1.magic++;
			player1.magicResist++;
		}
		else if (enemy.playerClass == "Undead")
		{
			printCondition = 32;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 10;
			player1.strength++;
			player1.armor++;
			player1.speed++;
		}
		else if (enemy.playerClass == "Feral Wolf")
		{
			printCondition = 30;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 7;
			player1.magic++;
			player1.strength++;
		}
		else if (enemy.playerClass == "Strange Little Goblin")
		{
			printCondition = 40;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 7;
			player1.armor++;
			player1.magicResist++;
		}
		else if (enemy.playerClass == "Imp")
		{
			printCondition = 42;
			printData(printCondition, player1, enemy);
		}
		else if (enemy.playerClass == "Earth Elemental")
		{
			printCondition = 46;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 10;
			player1.magic = player1.magic + 5;
			player1.magicResist--;
		}
		else if (enemy.playerClass == "Beholder")
		{
			printCondition = 44;
			printData(printCondition, player1, enemy);
			player1.HP = player1.HP + 10;
			player1.armor = player1.armor + 3;
			player1.magicResist = player1.magicResist + 3;
		}
		else if (enemy.playerClass == "Stone Golem")
		{
			printCondition = 48;
			printData(printCondition, player1, enemy);
			printCondition = 58;
			printData(printCondition, player1, enemy);
		}
		else if (enemy.playerClass == "Dragon")
		{
			printCondition = 52;
			printData(printCondition, player1, enemy);
			printCondition = 58;
			printData(printCondition, player1, enemy);
		}
		else if (enemy.playerClass == "Necromancer")
		{
			printCondition = 55;
			printData(printCondition, player1, enemy);
			printCondition = 58;
			printData(printCondition, player1, enemy);
		}
		else
		{
			// Should be minion otherwise
			std::cout << "You defeat the Necromancer\'s minion and feel empowered. You turn your attention back to the main threat.\n"
				<< "+1 Str\n+1 Mgc\n";
			player1.strength++;
			player1.magic++;
			turn = 1;
		}
	}
}
