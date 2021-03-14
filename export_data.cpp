#include"export_data.h"
#include"base_menus.h"

void load_all_data() 
{
	std::cout << "Please enter the name of the file to open, excluding the .edms extension: " << std::endl;
	std::string filename;
	getline(std::cin, filename);
	filename.append(".edms");
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
			filename.append(".edms");
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
	std::cout << "Loading data..." << std::endl;
	std::string input_string;
	getline(my_input_file, input_string);
	int number_experiments{ std::stoi(input_string) };
	for (int i{}; i < number_experiments; i++) {
		getline(my_input_file, input_string);
		experiment new_experiment(input_string);
		getline(my_input_file, input_string);
		std::istringstream contributors_stream(input_string);
		std::string temp_string;
		while (getline(contributors_stream, temp_string, ',')) {
			new_experiment.add_contributor(temp_string);
		}
		getline(my_input_file, input_string);
		int number_datasets{ std::stoi(input_string) };
		for (int j{}; j < number_datasets; j++) {
			getline(my_input_file, input_string);
			std::istringstream dataset_data_stream(input_string);
			std::string temp_string;
			getline(dataset_data_stream, temp_string, ',');
			std::string dataset_name{ temp_string };
			getline(dataset_data_stream, temp_string, ',');
			std::string dataset_time{ temp_string };
			getline(dataset_data_stream, temp_string, ',');
			std::string dataset_state{ temp_string };
			getline(dataset_data_stream, temp_string, ',');
			std::string dataset_experimenter{ temp_string };
			//Now, create the dataset
			dataset new_dataset(dataset_name, dataset_time, dataset_state, dataset_experimenter);
			//Now the template
			getline(my_input_file, input_string);
			std::istringstream template_data_stream(input_string);
			getline(template_data_stream, temp_string, ',');
			std::string template_name{ temp_string };
			reading_template new_template(template_name);
			getline(template_data_stream, temp_string, ',');
			int number_template_measurements{ std::stoi(temp_string) };
			for (int k{}; k < number_template_measurements; k++) {
				//Looping over template measurements
				getline(my_input_file, input_string);
				std::istringstream template_measurement_stream(input_string);
				std::vector<std::string> template_data;
				while (getline(template_measurement_stream, temp_string, ',')) {
					template_data.push_back(temp_string);
				}
				//template_data now contains all the info for this measurement
				if (template_data[0] == "numerical_measurement") {
					new_template.add_measurement(std::make_shared < numerical_measurement>(template_data[1], template_data[2], template_data[3], template_data[4]));
				}
				else if (template_data[0] == "boolean_measurement") {
					new_template.add_measurement(std::make_shared < boolean_measurement>(template_data[1]));
				}
				else if (template_data[0] == "text_measurement") {
					new_template.add_measurement(std::make_shared < text_measurement>(template_data[1],template_data[2]));

				}
				else if (template_data[0] == "text_comment") {
					new_template.add_measurement(std::make_shared < text_comment>(template_data[1]));
				}

			}
			//Finished looping over template
			template_list.push_back(new_template);
			new_dataset.set_template(new_template);
			getline(my_input_file, input_string);
			int number_readings{ std::stoi(input_string) };
			for (int k{}; k < number_readings; k++) {
				getline(my_input_file, input_string);
				std::vector<std::string> reading_string;
				std::istringstream reading_stream(input_string);
				while (getline(reading_stream, temp_string, ',')) {
					reading_string.push_back(temp_string);
				}
				//Reading string now contains all the data for this reading
				//Just need to parse it
				//So, loop over the template
				reading new_reading(reading_string[0], reading_string[1], reading_string[2]);
				int value_counter{ 3 };
				for (int l{}; l < new_template.size(); l++) {
					if (new_template.get_measurement(l)->type() == "numerical_measurement") {
						if (new_template.get_measurement(l)->standard_err() == true && new_template.get_measurement(l)->systematic() == true) {
							//Has both
							new_reading.add_measurement((std::make_shared< numerical_measurement>(std::stod(reading_string[value_counter]), std::stod(reading_string[value_counter+1]), std::stod(reading_string[value_counter+2]),new_template.get_measurement(l))));
							value_counter += 3;
						}
						else if (new_template.get_measurement(l)->standard_err() == true) {
							//Has standard
							new_reading.add_measurement((std::make_shared< numerical_measurement>(std::stod(reading_string[value_counter]), std::stod(reading_string[value_counter + 1]), 0.0, new_template.get_measurement(l))));

							value_counter += 2;
						}
						else if (new_template.get_measurement(l)->systematic() == true) {
							//Has systematic
							new_reading.add_measurement((std::make_shared< numerical_measurement>(std::stod(reading_string[value_counter]), 0.0, std::stod(reading_string[value_counter + 1]), new_template.get_measurement(l))));

							value_counter += 2;
						}
						else {
							//Has neitherv
							new_reading.add_measurement((std::make_shared< numerical_measurement>(std::stod(reading_string[value_counter]),0.0,0.0, new_template.get_measurement(l))));

							value_counter += 1;
						}
					}
					else if (new_template.get_measurement(l)->type() == "boolean_measurement") {
						new_reading.add_measurement((std::make_shared< boolean_measurement>(reading_string[value_counter], new_template.get_measurement(l))));
						value_counter += 1;
					}
					else if (new_template.get_measurement(l)->type() == "text_measurement") {
						new_reading.add_measurement((std::make_shared< text_measurement>(reading_string[value_counter], new_template.get_measurement(l))));

						value_counter += 1;
					}
					else if (new_template.get_measurement(l)->type() == "text_comment") {
						new_reading.add_measurement((std::make_shared< text_comment>(reading_string[value_counter], new_template.get_measurement(l))));

						value_counter += 1;
					}
					//Reading string contains all the data
					//time, has time, good reading
					//all but numerical just have string value
					//numerical has string value, error, systematic, only if they exist; specified by the template
				}
				new_dataset.add_reading(new_reading);
			}
			new_experiment.add_dataset(new_dataset);
		}
		experiment_list.push_back(new_experiment);
	}
	std::cout << "Finished loading!" << std::endl;
}
void save_all_data() 
{
	std::cout << "Please enter a filename to create a file to save the data to:" << std::endl;
	std::string filename;
	getline(std::cin, filename);
	filename += ".edms";
	std::ofstream output_file{ filename };
	if (!output_file.good()) {
		output_file.close();
		std::cout << "ERROR: File could not be opened for writing." << std::endl;
		main_menu();
	}
	std::cout << "File " << filename << " has been created. Writing data to file..." << std::endl;
	output_file << experiment_list.size() << std::endl;
	for (size_t i{}; i < experiment_list.size(); i++) {
		//Looping over all experiments
		output_file << experiment_list[i].name() << std::endl;
		for (int j{}; j < experiment_list[i].number_contributors(); j++) {
			//Loop over contributors
			output_file << experiment_list[i].return_contributor(j);
			if (j == experiment_list[i].number_contributors() - 1) {
				output_file << std::endl;
			}
			else {
				output_file << ",";
			}
		}
		output_file << experiment_list[i].number_datasets() << std::endl;
		for (int j{}; j < experiment_list[i].number_datasets(); j++) {
			//Loop over datasets
			dataset this_set{ experiment_list[i].return_dataset(j) };
			output_file << this_set.name() << "," << this_set.date() << "," << this_set.string_state() << "," << this_set.contributor() << std::endl;
			output_file << this_set.return_template().return_name() << "," << this_set.return_template().size() << std::endl;
			for (int k{}; k < this_set.return_template().size(); k++) {
				//Loop over measurements in template
				if (this_set.return_template().get_measurement(k)->type() == "numerical_measurement") {
					output_file << this_set.return_template().get_measurement(k)->type() << "," << this_set.return_template().get_measurement(k)->name() << "," << this_set.return_template().get_measurement(k)->units() << "," << this_set.return_template().get_measurement(k)->print_error_state() << "," << this_set.return_template().get_measurement(k)->print_systematic_state() << std::endl;
				}
				else if (this_set.return_template().get_measurement(k)->type() == "boolean_measurement"){
					output_file << this_set.return_template().get_measurement(k)->type() << "," << this_set.return_template().get_measurement(k)->name() << std::endl;
				}
				else if (this_set.return_template().get_measurement(k)->type() == "text_measurement") {
					output_file << this_set.return_template().get_measurement(k)->type() << "," << this_set.return_template().get_measurement(k)->name() << "," << this_set.return_template().get_measurement(k)->units() << std::endl;
				}
				else if (this_set.return_template().get_measurement(k)->type() == "text_comment") {
					output_file << this_set.return_template().get_measurement(k)->type() << "," << this_set.return_template().get_measurement(k)->name() << std::endl;
				}

			}
			output_file << this_set.number_readings() << std::endl;//time, has time, good reading too
			for (int k{}; k < this_set.number_readings(); k++) {
				//Loop over all readings now
				reading this_reading{ this_set.return_reading(k) };
				output_file << this_reading.return_time() << "," << this_reading.print_has_time() << "," << this_reading.print_good_reading() << ",";
				for (int l{}; l < this_reading.size(); l++) {
					//Loop over each measurement
					//Lots of if conditions
					if (this_reading.return_measurement(l)->type() == "numerical_measurement") {
						if (this_reading.return_measurement(l)->standard_err() == true && this_reading.return_measurement(l)->systematic() == true) {
							//Has both errors
							output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->error_string_value() << "," << this_reading.return_measurement(l)->systematic_string_value();
						}
						else if (this_reading.return_measurement(l)->standard_err() == true) {
							//Only standard
							output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->error_string_value();
						}
						else if (this_reading.return_measurement(l)->systematic() == true) {
							//Only systematic
							output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->systematic_string_value();
						}
						else {
							//No errors
							output_file << this_reading.return_measurement(l)->string_value();
						}
						if (l < this_reading.size() - 1) {
							output_file << ",";
						}
					}
					else if (this_reading.return_measurement(l)->type() == "boolean_measurement") {
						output_file << this_reading.return_measurement(l)->string_value();
						if (l < this_reading.size() - 1) {
							output_file << ",";
						}
					}
					else if (this_reading.return_measurement(l)->type() == "text_measurement") {
						output_file << this_reading.return_measurement(l)->string_value();
						if (l < this_reading.size() - 1) {
							output_file << ",";
						}
					}
					else if (this_reading.return_measurement(l)->type() == "text_comment") {
						output_file << this_reading.return_measurement(l)->string_value();
						if (l < this_reading.size() - 1) {
							output_file << ",";
						}
					}
				}
			output_file << std::endl;
			}
		}
	}
	output_file << "FILE_END";
	output_file.close();
	std::cout << "File save complete..." << std::endl;
}
void save_dataset(int experiment_index, int dataset_index) 
{
	std::cout << "Dataset will be saved to a new .csv file." << std::endl;
	std::cout << "Please enter a filename to create a file to save the dataset to, excluding the .csv extension:" << std::endl;
	std::string filename;
	getline(std::cin, filename);
	filename += ".csv";
	std::ofstream output_file{ filename };
	if (!output_file.good()) {
		output_file.close();
		std::cout << "ERROR: File could not be opened for writing." << std::endl;
		main_menu();
	}
	std::cout << "File " << filename << " has been created. Writing data to file..." << std::endl;
	std::cout << "Include header in file?" << std::endl;
	std::cout << "1.... Yes" << std::endl;
	std::cout << "2.... No" << std::endl;
	int header_option{ get_option(2) };
	dataset this_set{ experiment_list[experiment_index].return_dataset(dataset_index) };
	if (header_option == 1) {
		//Print headers
		for (int k{}; k < this_set.return_template().size(); k++) {
			//Loop over measurements in template
			if (k != 0) {
				output_file << ",";
			}
			if (this_set.return_template().get_measurement(k)->type() == "numerical_measurement") {
				//Different options dependent on error
				if (this_set.return_template().get_measurement(k)->standard_err() == true && this_set.return_template().get_measurement(k)->systematic() == true) {
					//Has both
					output_file << this_set.return_template().get_measurement(k)->name() << "(" << this_set.return_template().get_measurement(k)->units() << "), Error, Systematic Error";

				}
				else if (this_set.return_template().get_measurement(k)->standard_err() == true) {
					//Has standard
					output_file << this_set.return_template().get_measurement(k)->name() << "(" << this_set.return_template().get_measurement(k)->units() << "), Error";

				}
				else if (this_set.return_template().get_measurement(k)->systematic() == true) {
					//Has systematic
					output_file << this_set.return_template().get_measurement(k)->name() << "(" << this_set.return_template().get_measurement(k)->units() << "), Systematic Error";

				}
				else {
					//Has no errors
					output_file << this_set.return_template().get_measurement(k)->name() << "(" << this_set.return_template().get_measurement(k)->units() << ")";

				}
			}
			else if (this_set.return_template().get_measurement(k)->type() == "boolean_measurement") {
				output_file << this_set.return_template().get_measurement(k)->name();
			}
			else if (this_set.return_template().get_measurement(k)->type() == "text_measurement") {
				output_file << this_set.return_template().get_measurement(k)->name() << ",(" << this_set.return_template().get_measurement(k)->units() << ")";
			}
			else if (this_set.return_template().get_measurement(k)->type() == "text_comment") {
				output_file << this_set.return_template().get_measurement(k)->name();
			}
		}
		output_file << std::endl;
	}
	//Loop over datasets
	for (int k{}; k < this_set.number_readings(); k++) {
		if (k != 0) {
			output_file << std::endl;
		}
		//Loop over all readings now
		reading this_reading{ this_set.return_reading(k) };
		for (int l{}; l < this_reading.size(); l++) {
			//Loop over each measurement
			//Lots of if conditions
			if (this_reading.return_measurement(l)->type() == "numerical_measurement") {
				if (this_reading.return_measurement(l)->standard_err() == true && this_reading.return_measurement(l)->systematic() == true) {
					//Has both errors
					output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->error_string_value() << "," << this_reading.return_measurement(l)->systematic_string_value();
				}
				else if (this_reading.return_measurement(l)->standard_err() == true) {
					//Only standard
					output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->error_string_value();
				}
				else if (this_reading.return_measurement(l)->systematic() == true) {
					//Only systematic
					output_file << this_reading.return_measurement(l)->string_value() << "," << this_reading.return_measurement(l)->systematic_string_value();
				}
				else {
					//No errors
					output_file << this_reading.return_measurement(l)->string_value();
				}
				if (l < this_reading.size() - 1) {
					output_file << ",";
				}
			}
			else if (this_reading.return_measurement(l)->type() == "boolean_measurement") {
				output_file << this_reading.return_measurement(l)->string_value();
				if (l < this_reading.size() - 1) {
					output_file << ",";
				}
			}
			else if (this_reading.return_measurement(l)->type() == "text_measurement") {
				output_file << this_reading.return_measurement(l)->string_value();
				if (l < this_reading.size() - 1) {
					output_file << ",";
				}
			}
			else if (this_reading.return_measurement(l)->type() == "text_comment") {
				output_file << this_reading.return_measurement(l)->string_value();
				if (l < this_reading.size() - 1) {
					output_file << ",";
				}
			}
		}
	}
	output_file.close();
	std::cout << "Dataset successfully written to file!" << std::endl;
}