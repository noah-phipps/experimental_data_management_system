#include"base_menus.h"
#include"input_checks.h"

int get_option(int highest_option) 
{
	//Take a string as the input
	//Check it is only an integer
	//Re try the input, and return to main menu if failed 3 times
	//Return the integer version
	std::string string_option;
	int int_option;
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	getline(std::cin, string_option);
	bool valid_entry{ false };
	int attempts{ 0 };
	while (valid_entry == false) {
		try {
			int_option = std::stoi(string_option);
			if (int_option <= highest_option) {
				return int_option;
			}
			else {
				std::cout << "Error, invalid input is not a valid option. Please try again; after 3 attempts, the program will exit." << 2 - attempts << " attempts remain." << std::endl;
				attempts += 1;
			}
		}
		catch (...) {
			std::cout << "Error, invalid input. Please enter a valid integer input; after 3 attempts, the program will exit. " << 2 - attempts << " attempts remain." << std::endl;
			attempts += 1;
		}
		if (attempts == 3) {
			terminate_program();
		}
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << "Please try again: " << std::endl;
		getline(std::cin, string_option);
	}
	std::cout << "ERROR: Failed to get integer input." << std::endl;
	terminate_program();
	return 0;
}
