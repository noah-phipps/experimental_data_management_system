#include"standard_library_includes.h"
#include"data_type_classes.h"
#ifndef useful_functions_h
#define useful_functions_h
std::string time_t_to_date(time_t time);
std::string time_t_to_date_and_time(time_t time);
bool templates_exist();
double vector_average(std::vector<double> vector);
double vector_standard_deviation(std::vector<double> vector);
void terminate_program();
void terminate_program_good();
bool experiments_exist();
int select_experiment();
int create_experiment();
reading_template reading_template_creator();
std::string time_t_to_date_short(time_t time);

#endif