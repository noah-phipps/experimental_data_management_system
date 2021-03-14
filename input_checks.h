#include"standard_library_includes.h"
#ifndef input_checks_h
#define input_checks_h
int get_option(int highest_option);
//Template class must be included in header, this does input checking on int and doubles
template <class c_type> c_type get_value() 
{
	c_type result;
	bool valid_entry{ false };
	while (valid_entry == false) {
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		try {
			std::cin >> result;
			valid_entry = true;
		}
		catch (...) {
			std::cout << "ERROR: Please enter an integer value." << std::endl;
		}
	}
	return result;
}
#endif