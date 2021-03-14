#include"standard_library_includes.h"
#include"data_type_classes.h"
#ifndef import_data_h
#define import_data_h
dataset import_csv();
dataset import_txt();
dataset import_any();
reading import_reading(reading_template template_to_use, std::string import_line, char delimiter);
#endif