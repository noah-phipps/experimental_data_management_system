#include "import_data.h"
#include"base_menus.h"

dataset import_csv() 
{
	std::cout << "Importing data from a csv file." << std::endl;
	std::cout << "Please confirm that all values within the file are separated by a comma(, ) and that each new set of values is on a new line: " << std::endl;
	std::cout << "1.... Confirm" << std::endl;
	std::cout << "2.... Return to Main Menu" << std::endl;
	int option{ get_option(2) };
	if (option == 2) {
		main_menu();
	}
	else {
		std::cout << "File type confirmed as csv." << std::endl;
	}
	std::cout << "Does a matching template exist, or is a new template required for the imported data: " << std::endl;
	std::cout << "1.... New template required" << std::endl;
	std::cout << "2.... Template already exists" << std::endl;
	int template_option{ get_option(2) };
	reading_template import_template;
	if (template_option == 1) {
		std::cout << "Please create the template that will be assigned to the imported dataset." << std::endl;
		std::cout << "Each measurement can have an error, as well as a systematic error assigned." << std::endl;
		std::cout << "If these have already been combined, simply select a standard error." << std::endl;
		std::cout << "Please ensure that text measurements are treated as such." << std::endl;
		std::cout << "Please remove any headers from the file prior to import." << std::endl;
		import_template=reading_template_creator();
	}
	else {
		import_template=template_list[select_experiment()];
	}
	std::cout << "Template assigned. Please enter the filename, excluding the .csv extension. Please ensure the file has been placed in the correct directory." << std::endl;
	std::string filename;
	getline(std::cin, filename);
	filename.append(".csv");
	std::ifstream my_test_file{ filename };
	int attempts{ 4 };
	if (!my_test_file.good()) {
		bool good_file{ false };
		while (good_file == false) {
			//Data file not successfully opened
			if (attempts == 1) {
				main_menu();
			}
			my_test_file.close();
			attempts -= 1;
			std::cout << "\nERROR: File could not be opened successfully. After 3 attempts, you will be returned to the main menu (" << attempts << "remain).\n" << std::endl;
			std::cout << "Please enter the filename again: " << std::endl;
			getline(std::cin, filename);
			filename.append(".csv");
			std::ifstream new_test_file{ filename };
			if (new_test_file.good()) {
				good_file = true;
			}
			new_test_file.close();
		}
	}
	my_test_file.close();
	std::ifstream my_input_file{ filename };
	if (!my_input_file.good()) {
		std::cout << "ERROR: Unexpected error. Returning to main menu...";
		main_menu();
	}
	std::cout << "File \""<<filename<<"\" opened successfully." << std::endl;
	std::cout << "Please input the name of the new dataset: " << std::endl;
	std::string new_dataset_title;
	getline(std::cin, new_dataset_title);
	std::cout << "Please input the experimenter for the new dataset: " << std::endl;
	std::string new_contributor;
	getline(std::cin, new_contributor);
	dataset new_dataset(new_dataset_title, new_contributor);
	while (!my_input_file.eof()) {
		std::string new_line;
		try {
			
			getline(my_input_file, new_line);
			reading new_reading{ import_reading(import_template,new_line,',') };
			if (new_reading.is_good() == true) {
				new_dataset.add_reading(new_reading);
			}
			else {
				std::cout << "ERROR: Failed line detected. The line was: " << new_line << std::endl;
				std::cout << "This line has been discarded, continuing to next line..." << std::endl;
			}
		}
		catch (...) {
			std::cout << "ERROR: Unexpected error in file reading. The line was: " << new_line << std::endl;
			std::cout << "This line has been discarded, continuing to next line..." << std::endl;
		}
	}

	std::cout << "End of file. " << new_dataset.number_readings() << " readings have been created in the dataset." << std::endl;
	new_dataset.set_template(import_template);
	return new_dataset;
}
dataset import_txt() 
{
	std::cout << "Importing data from a .txt file." << std::endl;
	std::cout << "Please confirm that each new set of values is on a new line, and the same delimiter is used throughout the file: " << std::endl;
	std::cout << "1.... Confirm" << std::endl;
	std::cout << "2.... Return to Main Menu" << std::endl;
	int option{ get_option(2) };
	if (option == 2) {
		main_menu();
	}
	else {
		std::cout << "File type confirmed." << std::endl;
	}
	std::cout << "Does a matching template exist, or is a new template required for the imported data: " << std::endl;
	std::cout << "1.... New template required" << std::endl;
	std::cout << "2.... Template already exists" << std::endl;
	int template_option{ get_option(2) };
	reading_template import_template;
	if (template_option == 1) {
		std::cout << "Please create the template that will be assigned to the imported dataset." << std::endl;
		std::cout << "Each measurement can have an error, as well as a systematic error assigned." << std::endl;
		std::cout << "If these have already been combined, simply select a standard error." << std::endl;
		std::cout << "Please ensure that text measurements are treated as such." << std::endl;
		std::cout << "Please remove any headers from the file prior to import." << std::endl;
		import_template = reading_template_creator();
	}
	else {
		import_template = template_list[select_experiment()];
	}
	std::cout << "Template assigned. Please enter the filename, excluding the .txt extension. Please ensure the file has been placed in the correct directory." << std::endl;
	std::string filename;
	getline(std::cin, filename);
	filename.append(".txt");
	std::ifstream my_test_file{ filename };
	int attempts{ 4 };
	if (!my_test_file.good()) {
		bool good_file{ false };
		while (good_file == false) {
			//Data file not successfully opened
			if (attempts == 1) {
				main_menu();
			}
			my_test_file.close();
			attempts -= 1;
			std::cout << "\nERROR: File could not be opened successfully. After 3 attempts, you will be returned to the main menu (" << attempts << "remain).\n" << std::endl;
			std::cout << "Please enter the filename again: " << std::endl;
			getline(std::cin, filename);
			filename.append(".txt");
			std::ifstream new_test_file{ filename };
			if (new_test_file.good()) {
				good_file = true;
			}
			new_test_file.close();
		}
	}
	my_test_file.close();
	std::ifstream my_input_file{ filename };
	if (!my_input_file.good()) {
		std::cout << "ERROR: Unexpected error. Returning to main menu...";
		main_menu();
	}
	std::cout << "File \"" << filename << "\" opened successfully." << std::endl;
	std::cout << "Please input the name of the new dataset: " << std::endl;
	std::string new_dataset_title;
	getline(std::cin, new_dataset_title);
	std::cout << "Please input the experimenter for the new dataset: " << std::endl;
	std::string new_contributor;
	getline(std::cin, new_contributor);
	dataset new_dataset(new_dataset_title, new_contributor);
	std::cout << "Is a 'space', or 'tab' used as the delimiter, or is something else required:" << std::endl;
	std::cout << "1.... Space" << std::endl;
	std::cout << "2.... Tab";
	std::cout << "3.... Enter another delimiter" << std::endl;
	int delim_option{ get_option(3) };
	char delimiter{' '};
	if (delim_option == 2) {
		delimiter = '\t';
	}
	if (delim_option == 3) {
		std::cout << "Please enter the delimiter character, with no other characters, and no spaces:";
		std::cin >> delimiter;
	}
	while (!my_input_file.eof()) {
		std::string new_line;
		try {

			getline(my_input_file, new_line);
			reading new_reading{ import_reading(import_template,new_line,delimiter) };
			if (new_reading.is_good() == true) {

				new_dataset.add_reading(&new_reading);
			}
			else {
				std::cout << "ERROR: Failed line detected. The line was: " << new_line << std::endl;
				std::cout << "This line has been discarded, continuing to next line..." << std::endl;
			}
		}
		catch (...) {
			std::cout << "ERROR: Unexpected error in file reading. The line was: " << new_line << std::endl;
			std::cout << "This line has been discarded, continuing to next line..." << std::endl;
		}
	}
	std::cout << "End of file. " << new_dataset.number_readings() << " readings have been created in the dataset." << std::endl;
	new_dataset.set_template(import_template);
	return new_dataset;
}
dataset import_any() 
{
	std::cout << "Importing data from a file of unknown type." << std::endl;
	std::cout << "Please confirm that each new set of values is on a new line, and the same delimiter is used throughout the file: " << std::endl;
	std::cout << "1.... Confirm" << std::endl;
	std::cout << "2.... Return to Main Menu" << std::endl;
	int option{ get_option(2) };
	if (option == 2) {
		main_menu();
	}
	else {
		std::cout << "File type confirmed." << std::endl;
	}
	std::cout << "Does a matching template exist, or is a new template required for the imported data: " << std::endl;
	std::cout << "1.... New template required" << std::endl;
	std::cout << "2.... Template already exists" << std::endl;
	int template_option{ get_option(2) };
	reading_template import_template;
	if (template_option == 1) {
		std::cout << "Please create the template that will be assigned to the imported dataset." << std::endl;
		std::cout << "Each measurement can have an error, as well as a systematic error assigned." << std::endl;
		std::cout << "If these have already been combined, simply select a standard error." << std::endl;
		std::cout << "Please ensure that text measurements are treated as such." << std::endl;
		std::cout << "Please remove any headers from the file prior to import." << std::endl;
		import_template = reading_template_creator();
	}
	else {
		import_template = template_list[select_experiment()];
	}
	std::cout << "Template assigned. Please enter the filename, including the extension. Please ensure the file has been placed in the correct directory." << std::endl;
	std::string filename;
	getline(std::cin, filename);
	std::ifstream my_test_file{ filename };
	int attempts{ 4 };
	if (!my_test_file.good()) {
		bool good_file{ false };
		while (good_file == false) {
			//Data file not successfully opened
			if (attempts == 1) {
				main_menu();
			}
			my_test_file.close();
			attempts -= 1;
			std::cout << "\nERROR: File could not be opened successfully. After 3 attempts, you will be returned to the main menu (" << attempts << "remain).\n" << std::endl;
			std::cout << "Please enter the filename again: " << std::endl;
			getline(std::cin, filename);
			std::ifstream new_test_file{ filename };
			if (new_test_file.good()) {
				good_file = true;
			}
			new_test_file.close();
		}
	}
	my_test_file.close();
	std::ifstream my_input_file{ filename };
	if (!my_input_file.good()) {
		std::cout << "ERROR: Unexpected error. Returning to main menu...";
		main_menu();
	}
	std::cout << "File \"" << filename << "\" opened successfully." << std::endl;
	std::cout << "Please input the name of the new dataset: " << std::endl;
	std::string new_dataset_title;
	getline(std::cin, new_dataset_title);
	std::cout << "Please input the experimenter for the new dataset: " << std::endl;
	std::string new_contributor;
	getline(std::cin, new_contributor);
	dataset new_dataset(new_dataset_title, new_contributor);
	std::cout << "Is a 'space' used as the delimiter, or is something else required:" << std::endl;
	std::cout << "1.... Space" << std::endl;
	std::cout << "2.... Tab" << std::endl;
	std::cout << "3.... Enter another delimiter" << std::endl;
	int delim_option{ get_option(3) };
	char delimiter{ ' ' };
	if (delim_option == 2) {
		delimiter = '\t';
	}
	if (delim_option == 3) {
		std::cout << "Please enter the delimiter character, with no other characters, and no spaces:";
		std::cin >> delimiter;
	}
	while (!my_input_file.eof()) {
		std::string new_line;
		try {

			getline(my_input_file, new_line);
			reading new_reading{ import_reading(import_template,new_line,delimiter) };
			if (new_reading.is_good() == true) {
				new_dataset.add_reading(new_reading);
			}
			else {
				std::cout << "ERROR: Failed line detected. The line was: " << new_line << std::endl;
				std::cout << "This line has been discarded, continuing to next line..." << std::endl;
			}
		}
		catch (...) {
			std::cout << "ERROR: Unexpected error in file reading. The line was: " << new_line << std::endl;
			std::cout << "This line has been discarded, continuing to next line..." << std::endl;
		}
	}
	std::cout << "End of file. " << new_dataset.number_readings() << " readings have been created in the dataset." << std::endl;
	new_dataset.set_template(import_template);
	return new_dataset;
}
reading import_reading(reading_template template_to_use, std::string import_line, char delimiter) 
{
	reading new_reading{};
	reading fail_reading{false};
	new_reading.set_if_new(false);//It's imported, so cannot be new
	std::vector<std::string> new_string_vector;
	std::string new_string;
	std::istringstream new_string_stream(import_line);
	while (getline(new_string_stream, new_string, delimiter)) {
		new_string_vector.push_back(new_string);
	}
	size_t string_counter{};
	//Now, loop over the template
	for (int i{}; i < template_to_use.size(); i++) {
		std::shared_ptr<measurement> current{ template_to_use.get_measurement(i) };
		if (current->type() == "numerical_measurement") {
			double new_value{};
			try {
				new_value = std::stod(new_string_vector[string_counter]);
			}
			catch (...) {
				std::cout << "ERROR: " << new_string_vector[string_counter] << " could not be converted to double type. This line will be discared." << std::endl;
				return fail_reading;
			}
			double new_error{};
			double new_systematic{};
			string_counter += 1;
			if (current->standard_err() == true) {
				try {
					new_error = std::stod(new_string_vector[string_counter]);
					string_counter += 1;
				}
				catch (...) {
					std::cout << "ERROR: " << new_string_vector[string_counter] << " could not be converted to double type. This line will be discared." << std::endl;
					return fail_reading;
				}
			}
			if (current->systematic() == true) {
				try {
					new_systematic = std::stod(new_string_vector[string_counter]);
					string_counter += 1;
				}
				catch (...) {
					std::cout << "ERROR: " << new_string_vector[string_counter] << " could not be converted to double type. This line will be discared." << std::endl;
					return fail_reading;
				}
			}
			new_reading.add_measurement(std::make_shared< numerical_measurement>(new_value, new_error, new_systematic, current));
		}
		else if (current->type() == "text_measurement") {
			try {
				new_reading.add_measurement(std::make_shared < text_measurement>(new_string_vector[string_counter], current));
				string_counter += 1;
			}
			catch (...) {
				std::cout << "ERROR: Text measurement could not be created. This line will be discared." << std::endl;
				return fail_reading;
			}
		}
		else if (current->type() == "boolean_measurement") {
			try {
				if (new_string_vector[string_counter] == "true") {
					new_reading.add_measurement(std::make_shared < boolean_measurement>(true, current));//Add other options such as True, 0, 1 etc
				}
				else if (new_string_vector[string_counter] == "false") {
					new_reading.add_measurement(std::make_shared < boolean_measurement>(false, current));//Same as above
				}
				else {
					std::cout << "ERROR: Boolean measurement could not be created. This line will be discared." << std::endl;
					return fail_reading;
				}
				string_counter += 1;
			}
			catch (...) {
				std::cout << "ERROR: Boolean measurement could not be created. This line will be discared." << std::endl;
				return fail_reading;
			}

		}
		else if (current->type() == "text_comment") {
			try {
				new_reading.add_measurement(std::make_shared <text_comment>(new_string_vector[string_counter], current));
				string_counter += 1;
			}
			catch (...) {
				std::cout << "ERROR: Comment could not be created. This line will be discared." << std::endl;
				return fail_reading;
			}

		}
		else {
			std::cout << "ERROR: Invalid measurement type detected. This line will be discared." << std::endl;
			return fail_reading;
		}
	}
	//Now return the reading
	if (string_counter < new_string_vector.size()) {
		std::cout << "WARNING: Fewer measurements have been assigned than present in the file. Please check the template used is correct." << std::endl;
		std::cout << "This reading will be added to the dataset, but checking before analysis is strongly recommeneded." << std::endl;
	}
	return new_reading;
}

