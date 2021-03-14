#include<string>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<iostream>
#include<vector>
#include"useful_functions.h"
#include"input_checks.h"
#include"global_lists.h"

//Dataset class function definitions
dataset::dataset(std::string name, std::string date, std::string state, std::string contributor) 
{
	dataset_name = name;
	if (state == "true") {
		dataset_state = true;
	}
	else {
		dataset_state = false;
	}
	const char* time_string = date.c_str();
	dataset_date = (time_t)atoll(time_string);
	dataset_experimenter = contributor;
}
dataset::dataset(const dataset& to_copy) 
{
	dataset_name = to_copy.name();
	dataset_date = to_copy.date();
	dataset_experimenter = to_copy.contributor();
	dataset_readings = to_copy.return_all_readings();
	dataset_template = to_copy.return_template();
	dataset_state = to_copy.state();
}
dataset::dataset(const dataset& to_copy, std::string new_name) 
{
	dataset_name = new_name;
	dataset_date = to_copy.date();
	dataset_experimenter = to_copy.contributor();
	dataset_readings = to_copy.return_all_readings();
	dataset_template = to_copy.return_template();
	dataset_state = to_copy.state();
}
dataset::dataset(std::string name, std::string contributor) 
{
	dataset_name = name;
	dataset_experimenter = contributor;
	dataset_date = time(0);
	dataset_state = false;
}
void dataset::add_reading(reading new_reading) 
{
	dataset_readings.push_back(new_reading);
}
void dataset::set_template(reading_template template_to_set) 
{
	dataset_template = template_to_set;

}
void dataset::print_info() 
{
	std::cout << "Dataset \"" << dataset_name << "\": ";
	std::cout << "Dated " << time_t_to_date(dataset_date) << ", ";
	std::cout << "with experimenter \"" << dataset_experimenter << "\", ";
	std::cout << "containing " << dataset_readings.size() << " readings.";
}
void dataset::delete_reading(int reading_index) 
{
	dataset_readings.erase(dataset_readings.begin() + reading_index);
}