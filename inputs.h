#include"standard_library_includes.h"
#include"data_type_classes.h"
#ifndef inputs_h
#define inputs_h
void input_existing_data(int experiment_index);
void input_existing_data_menu();
void input_new_data(int experiment_index);
void input_new_data_menu();
void input_readings(dataset& input_dataset, bool is_new);
reading create_reading(reading_template template_to_use, bool is_new);
#endif