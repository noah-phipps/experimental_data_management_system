#include<string>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<iostream>
#include<vector>
#include"useful_functions.h"
#include"input_checks.h"
#include"global_lists.h"
#include"data_type_classes.h"

//Text measurement function definitions
text_measurement::text_measurement(const text_measurement& to_copy) 
{
	measurement_name = to_copy.name();
	measurement_value = to_copy.string_value();
	measurement_units = to_copy.units();
}
text_measurement::text_measurement(std::string name, std::string units) 
{
	measurement_name = name;
	measurement_units = units;
}
text_measurement::text_measurement(std::string value, std::shared_ptr<measurement> template_to_use) 
{
	measurement_value = value;
	measurement_name = template_to_use->name();
	measurement_units = template_to_use->units();
}
void text_measurement::edit_value() 
{
	std::cout << "Please enter the new value: " << std::endl;
	std::string new_value;
	getline(std::cin, new_value);
	measurement_value = new_value;
}

//Text comment function definitions
text_comment::text_comment(std::string title) 
{
	comment_title = title;
}
text_comment::text_comment(std::string new_comment, std::shared_ptr<measurement> template_to_use) 
{
	comment = new_comment;
	comment_title = template_to_use->name();
}
text_comment::text_comment(const text_comment& to_copy) 
{
	comment = to_copy.string_value();
	comment_title = to_copy.name_and_units();
}
void text_comment::edit_value() 
{
	std::cout << "Please enter the new comment: " << std::endl;
	std::string new_value;
	getline(std::cin, new_value);
	comment = new_value;
}

//Boolean measurement function definitions
boolean_measurement::boolean_measurement(std::string name) 
{
	measurement_name = name;
}
boolean_measurement::boolean_measurement(std::string value, std::shared_ptr<measurement> template_to_use) 
{
	measurement_name = template_to_use->name();
	if (value == "true") {
		measurement_value = true;
	}
	else {
		measurement_value = false;
	}
}
boolean_measurement::boolean_measurement(bool value, std::shared_ptr<measurement> template_to_use) 
{
	measurement_value = value;
	measurement_name = template_to_use->name();
}
boolean_measurement::boolean_measurement(const boolean_measurement& to_copy) 
{
	measurement_name = to_copy.name();
	measurement_value = to_copy.value();
}
std::string boolean_measurement::string_value() const 
{
	if (measurement_value == true) {
		return("True");
	}
	else {
		return("False");
	}
}
void boolean_measurement::edit_value() 
{
	std::cout << "Would you like the value to be true of false?" << std::endl;
	std::cout << "1.... True" << std::endl;
	std::cout << "2.... False" << std::endl;
	int option{ get_option(2) };
	if (option == 1) {
		measurement_value = true;
	}
	else {
		measurement_value = false;
	}
}

//Numerical measurement function definitions
numerical_measurement::numerical_measurement(std::string name, std::string units, std::string error, std::string systematic) 
{
	measurement_name = name;
	measurement_units = units;
	if (error == "true") {
		standard_error = true;
	}
	else {
		standard_error = false;
	}
	if (systematic == "true") {
		systematic_error = true;
	}
	else {
		systematic_error = false;
	}
}
numerical_measurement::numerical_measurement(std::string name, std::string units, int systematic, bool error_y_or_n) 
{
	measurement_name = name;
	measurement_units = units;
	if (systematic == 1) {
		systematic_error = true;
	}
	else {
		systematic_error = false;
	}
	if (error_y_or_n == true) {
		standard_error = true;
	}
	else {
		standard_error = false;
	}
	number_repeats = 1;
}
numerical_measurement::numerical_measurement(std::string name, std::string units, int systematic, int repeats) 
{
	measurement_name = name;
	measurement_units = units;
	if (systematic == 1) {
		systematic_error = true;
	}
	else {
		systematic_error = false;
	}
	number_repeats = repeats;
}
numerical_measurement::numerical_measurement(double value, double value_error_in, double systematic_error_in, std::shared_ptr<measurement> template_to_use) 
{
	measurement_name = template_to_use->name();
	measurement_units = template_to_use->units();
	systematic_error = template_to_use->systematic();
	standard_error = template_to_use->standard_err();
	number_repeats = template_to_use->repeats();
	measurement_value = value;
	measurement_error = value_error_in;
	systematic_value = systematic_error_in;
	measurement_type = template_to_use->type();
}
numerical_measurement::numerical_measurement(std::vector<double> values, double systematic_error_in, std::shared_ptr<measurement> template_to_use) 
{
	measurement_name = template_to_use->name();
	measurement_units = template_to_use->units();
	measurement_type = template_to_use->type();
	number_repeats = template_to_use->repeats();
	systematic_error = template_to_use->systematic();
	repeat_measurements = values;
	measurement_value = vector_average(values);
	measurement_error = vector_standard_deviation(values);
	systematic_value = systematic_error_in;

}
numerical_measurement::numerical_measurement(const numerical_measurement& to_copy) 
{
	measurement_name = to_copy.name();
	measurement_units = to_copy.units();
	systematic_error = to_copy.systematic();
	standard_error = to_copy.standard_err();
	number_repeats = to_copy.repeats();
	measurement_value = to_copy.value();
	measurement_error = to_copy.error_value();
	systematic_value = to_copy.systematic_error_value();
	measurement_type = to_copy.type();
	repeat_measurements = to_copy.return_repeats();
}
void numerical_measurement::edit_value() 
{
	std::cout << "Please enter the new value:" << std::endl;
	double new_value{ get_value<double>() };
	measurement_value = new_value;
}
void numerical_measurement::edit_error() 
{
	std::cout << "Please enter the new error:" << std::endl;
	double new_value{ get_value<double>() };
	measurement_error = new_value;
}
void numerical_measurement::edit_systematic() 
{
	std::cout << "Please enter the new systematic error:" << std::endl;
	double new_value{ get_value<double>() };
	systematic_value = new_value;
}
std::string numerical_measurement::error_string_value() const 
{
	std::string return_string_1{ std::to_string((measurement_error)) };
	return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
	return return_string_1;
}
std::string numerical_measurement::systematic_string_value()const 
{
	std::string return_string_1{ std::to_string((systematic_value)) };
	return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
	return return_string_1;
}
std::string numerical_measurement::string_value() const 
{ 
	std::string return_string_1{ std::to_string((measurement_value)) };
	return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
	return return_string_1; 
}
