#include"useful_functions.h"
#include"base_menus.h"
#include<stdlib.h>

reading_template reading_template_creator() 
{
	std::cout << "Creating a new reading template. Please enter a name for the reading template: " << std::endl;
	std::string template_name;
	getline(std::cin, template_name);
	reading_template return_template(template_name);
	std::cout << "Please select an option from the following list: " << std::endl;
	bool not_finished{ true };
	while (not_finished == true) {//Add option of systematic error, timestamp or not
		std::cout << "1.... Add a numerical measurement, with no error" << std::endl;
		std::cout << "2.... Add a numerical measurement, with an error" << std::endl;
		std::cout << "3.... Add a numerical measurement, with repeats to ascertain an error" << std::endl;
		std::cout << "4.... Add a boolean measurement" << std::endl;
		std::cout << "5.... Add a text measurement" << std::endl;
		std::cout << "6.... Add a text comment" << std::endl;
		std::cout << "7.... Complete template" << std::endl;
		int option{ get_option(7) };
		if (option == 1) {
			std::cout << "Enter a title for the measurement: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::cout << "Enter the measurement units: " << std::endl;
			std::string measurement_units{};
			getline(std::cin, measurement_units);
			std::cout << "Will there be a systematic error associated with this measurement?"<<std::endl;
			std::cout << "1.... Yes" << std::endl;
			std::cout << "2.... No" << std::endl;
			int systematic_error{get_option(2)};
			std::shared_ptr< measurement> to_add = std::make_shared < numerical_measurement>(measurement_title, measurement_units, systematic_error, false);
			return_template.add_measurement(to_add);
		}
		else if (option == 2) {
			std::cout << "Enter a title for the measurement: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::cout << "Enter the measurement units: " << std::endl;
			std::string measurement_units{};
			getline(std::cin, measurement_units);
			std::cout << "Will there be a systematic error associated with this measurement?"<<std::endl;
			std::cout << "1.... Yes" << std::endl;
			std::cout << "2.... No" << std::endl;
			int systematic_error{ get_option(2) };
			std::shared_ptr< measurement> to_add = std::make_shared< numerical_measurement>(measurement_title, measurement_units, systematic_error, true);
			return_template.add_measurement(to_add);
		}
		else if (option == 3) {
			std::cout << "Enter a title for the measurement: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::cout << "Enter the measurement units: " << std::endl;
			std::string measurement_units{};
			getline(std::cin, measurement_units);
			std::cout << "Will there be a systematic error associated with this measurement?"<<std::endl;
			std::cout << "1.... Yes" << std::endl;
			std::cout << "2.... No" << std::endl;
			int systematic_error{ get_option(2) };
			std::cout << "How many repeats will there be for this measurement?" << std::endl;
			int repeats{ get_value<int>() };
			std::shared_ptr< measurement> to_add = std::make_shared <numerical_measurement>(measurement_title, measurement_units, systematic_error, repeats);
			return_template.add_measurement(to_add);
		}
		else if (option == 4) {
			//Boolean measurement
			std::cout << "Enter a title for the measurement: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::shared_ptr< measurement> to_add = std::make_shared < boolean_measurement>(measurement_title);
			return_template.add_measurement(to_add);
		}
		else if (option == 5) {
			//Text measurement
			std::cout << "Enter a title for the measurement: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::cout << "Enter the measurement units: " << std::endl;
			std::string measurement_units{};
			getline(std::cin, measurement_units);
			std::shared_ptr< measurement> to_add = std::make_shared <text_measurement>(measurement_title, measurement_units);
			return_template.add_measurement(to_add);
		}
		else if (option == 6) {
			std::cout << "Enter a title for the comment: " << std::endl;
			std::string measurement_title{};
			getline(std::cin, measurement_title);
			std::shared_ptr< measurement> to_add = std::make_shared < text_comment>(measurement_title);
			return_template.add_measurement(to_add);
		}
		else if (option == 7) {
			not_finished = false;
		}
		std::cout << "\n\nSize of template: " << return_template.size() << ".\n\n";

	}
	std::cout << "\n\nSize of template: " << return_template.size() << ".\n\n";
	template_list.push_back(return_template);
	return return_template;
}
bool experiments_exist() 
{
	if (experiment_list.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}
int select_experiment() 
{
	std::cout << "Select an experiment from the following list:" << std::endl;
	for (size_t i{ 1 }; i <= experiment_list.size(); i++) {
		std::cout << i << ".... " << experiment_list[i - 1].name() << std::endl;
	}
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	int option{ get_option(experiment_list.size()) };
	std::cout << "Experiment \"" << experiment_list[option - 1].name() << "\" selected. " << std::endl;
	return option - 1;
}
int select_template() 
{
	std::cout << "Select an template from the following list:" << std::endl;
	for (size_t i{ 1 }; i <= template_list.size(); i++) {
		std::cout << i << ".... " << template_list[i - 1].return_name() << std::endl;
	}
	int option{ get_option(template_list.size()) };
	std::cout << "Template \"" << template_list[option - 1].return_name() << "\" selected. " << std::endl;
	return option - 1;
}
int create_experiment() 
{
	std::cout << "Please enter the experiment name: " << std::endl;
	std::string new_experiment_name;
	getline(std::cin, new_experiment_name);
	experiment_list.push_back(experiment(new_experiment_name));
	std::cout << "New experiment \"" << experiment_list[experiment_list.size() - 1].name() << "\" created." << std::endl;
	return experiment_list.size() - 1;
	//Needs to return index of new experiment
}
std::string time_t_to_date(time_t time) 
{
	if (time == 0) {
		return "N/A";
	}
	time_t temp_time{ time };
	std::string day_text;
	std::string day_date;
	std::string month_date;
	std::string year_date;
	struct tm ltm;
	localtime_s(&ltm, &temp_time);
	day_date = std::to_string((&ltm)->tm_mday);
	month_date = std::to_string(1 + (&ltm)->tm_mon);
	year_date = std::to_string(1900 + (&ltm)->tm_year);
	int weekday{ (&ltm)->tm_wday };
	if ((&ltm)->tm_wday == 1) {
		day_text = "Monday";
	}
	if ((&ltm)->tm_wday == 2) {
		day_text = "Tuesday";
	}
	if ((&ltm)->tm_wday == 3) {
		day_text = "Wednesday";
	}
	if ((&ltm)->tm_wday == 4) {
		day_text = "Thursday";
	}
	if ((&ltm)->tm_wday == 5) {
		day_text = "Friday";
	}
	if ((&ltm)->tm_wday == 6) {
		day_text = "Saturday";
	}
	if ((&ltm)->tm_wday == 7) {
		day_text = "Sunday";
	}
	std::string return_text{ day_text + ", " + day_date + "/" + month_date + "/" + year_date };
	return return_text;
}
std::string time_t_to_date_short(time_t time) 
{
	if (time == 0) {
		return"N/A";
	}
	time_t temp_time{ time };
	std::string day_text;
	std::string day_date;
	std::string month_date;
	std::string year_date;
	struct tm ltm;
	localtime_s(&ltm, &temp_time);
	day_date = std::to_string((&ltm)->tm_mday);
	month_date = std::to_string(1 + (&ltm)->tm_mon);
	year_date = std::to_string(1900 + (&ltm)->tm_year);
	int weekday{ (&ltm)->tm_wday };
	if ((&ltm)->tm_wday == 1) {
		day_text = "Monday";
	}
	if ((&ltm)->tm_wday == 2) {
		day_text = "Tuesday";
	}
	if ((&ltm)->tm_wday == 3) {
		day_text = "Wednesday";
	}
	if ((&ltm)->tm_wday == 4) {
		day_text = "Thursday";
	}
	if ((&ltm)->tm_wday == 5) {
		day_text = "Friday";
	}
	if ((&ltm)->tm_wday == 6) {
		day_text = "Saturday";
	}
	if ((&ltm)->tm_wday == 7) {
		day_text = "Sunday";
	}
	std::string return_text{ day_date + "/" + month_date + "/" + year_date };
	return return_text;
}
bool templates_exist() 
{
	if (template_list.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}
std::string time_t_to_date_and_time(time_t time) 
{
	if (time == 0) {
		return"N/A";
	}
	time_t temp_time{ time };
	std::string date_text{ time_t_to_date_short(temp_time) };
	struct tm ltm;
	localtime_s(&ltm, &temp_time);
	std::string time_text{std::to_string((&ltm)->tm_hour)};
	time_text += (":");
	time_text += std::to_string((&ltm)->tm_min);
	time_text += (":");
	time_text += std::to_string((&ltm)->tm_sec);
	std::string return_text{ time_text + ", " + date_text };
	return return_text;
}
double vector_average(std::vector<double> vector) 
{
	double average{};
	for (size_t i{}; i < vector.size(); i++) {
		average += vector[i];
	}
	average /= vector.size();
	return average;
}
double vector_standard_deviation(std::vector<double> vector) 
{
	double standard_deviation{};
	double average{ vector_average(vector) };
	for (size_t i{}; i < vector.size(); i++) {
		standard_deviation += pow((vector[i] - average), 2);
	}
	standard_deviation /= vector.size();
	standard_deviation = sqrt(standard_deviation/(vector.size()-1));
	return standard_deviation;
}
void terminate_program() 
{
	std::cout << "\n\nPROGRAM CLOSING...\n\n";
	exit(EXIT_FAILURE);
}
void terminate_program_good()
{
	exit(EXIT_SUCCESS);
}

