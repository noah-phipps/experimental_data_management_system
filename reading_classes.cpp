#include"base_menus.h"
#include"data_type_classes.h"
//Reading class function definitions
reading::reading(const reading& to_copy) 
{
	reading_measurements = to_copy.return_all_measurements();
	reading_time = to_copy.return_time();
	has_time = to_copy.does_have_time();
	good_reading = to_copy.is_good();
}
reading::reading(std::string time, std::string string_has_time, std::string string_good) 
{
	if (string_has_time == "true") {
		has_time = true;
	}
	else {
		has_time = false;
	}
	if (string_good == "true") {
		good_reading = true;
	}
	else {
		good_reading = false;
	}
	const char* time_string = time.c_str();
	reading_time = (time_t)atoll(time_string);
}
void reading::add_measurement(std::shared_ptr<measurement> new_measurement) 
{
	reading_measurements.push_back(new_measurement);
}
void reading::set_if_new(bool is_new) 
{
	if (is_new == true) {
		has_time = true;
	}
	else {
		has_time = false;
	}
}
void reading::set_time(time_t to_set) 
{
	reading_time = to_set;
}

//Reading template class function defintions
reading_template::reading_template(std::string template_name) 
{
	name = template_name;
}
reading_template::reading_template(const reading_template& to_copy) 
{
	name = to_copy.return_name();
	measurements = to_copy.return_all_measurements();
}
void reading_template::add_measurement(std::shared_ptr<measurement> measurement_to_add) 
{
	measurements.push_back(measurement_to_add);
}
reading_template reading_template::operator=(reading_template to_assign) 
{
	name = to_assign.return_name();
	measurements = to_assign.return_all_measurements();
	return *this;
}