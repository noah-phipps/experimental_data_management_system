#include"manage_data.h"
#include"base_menus.h"

void manage_measurement(int experiment_index, int dataset_index, int reading_index, int measurement_index) 
{
	experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->type();
	std::cout << "This measurement has type \"" << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->type() << "\"." << std::endl;
	if ((experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->standard_err() == true) && (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->systematic()==true)) {
		//Has a standard error AND a systematic
		std::cout << "The measurement value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int value_option{ get_option(2) };
		if (value_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_value();
		}
		std::cout << "The standard error value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->error_string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int error_option{ get_option(2) };
		if (error_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_error();
		}
		std::cout << "The systematic error value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->systematic_string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int systematic_option{ get_option(2) };
		if (systematic_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_systematic();
		}
		std::cout << "Measurement edit complete." << std::endl;
	}
	else if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->standard_err() == true) {
		//Only has a standard error
		std::cout << "The measurement value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int value_option{ get_option(2) };
		if (value_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_value();
		}
		std::cout << "The standard error value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->error_string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int error_option{ get_option(2) };
		if (error_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_error();
		}

		std::cout << "Measurement edit complete." << std::endl;
	}
	else if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->systematic() == true) {
		//Only has a systematic error
		std::cout << "The measurement value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int value_option{ get_option(2) };
		if (value_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_value();
		}
		std::cout << "The systematic error value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->systematic_string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int systematic_option{ get_option(2) };
		if (systematic_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_systematic();
		}
		std::cout << "Measurement edit complete." << std::endl;
	}
	else {
		//Has no error
		std::cout << "The measurement value is " << experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->string_value() << ", would you like to change it?" << std::endl;
		std::cout << "1.... Yes" << std::endl;
		std::cout << "2.... No" << std::endl;
		int value_option{ get_option(2) };
		if (value_option == 1) {
			experiment_list[experiment_index].return_dataset(dataset_index).return_reading(reading_index).return_measurement(measurement_index)->edit_value();
		}
		std::cout << "Measurement edit complete." << std::endl;
	}
}
void manage_readings(int experiment_index, int dataset_index) 
{
	std::cout << "Please select a reading to edit from the table below: " << std::endl;
	//Print a table of readings to allow easy selection
	bprinter::TablePrinter reading_table(&std::cout);
	reading_table.AddColumn("Number", 7);
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); i++) {
		//Add headers
		std::shared_ptr<measurement> current{ experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(i) };
		//Now loop over the measurements in this one and find the longest string
		size_t longest_string{ static_cast<size_t>(current->name_and_units().length()) };//Will always be an int, so perform a static cast
		for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {
			if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->string_value().length() > longest_string) {
				longest_string = experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->string_value().length();
			}
		}
		reading_table.AddColumn(current->name_and_units(), longest_string + 3);
		size_t longest_standard_error{ static_cast<size_t>(8 + current->units().length()) };//8 is length of Error()
		if (current->standard_err() == true) {
			for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {//Loop over all readings, and find the longest error
				if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->error_string_value().length() > longest_standard_error) {
					longest_standard_error = experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->error_string_value().length();
				}
			}
			//Is associated with an error, so need to include this in the table.
			reading_table.AddColumn(("Error (" + current->units() + ")"), longest_standard_error + 3);
		}
		//If has a systematic
		size_t longest_systematic_error{ static_cast<size_t>(18 + current->units().length()) };//18 is length of Systematic Error()
		if (current->systematic() == true) {
			for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {//Loop over all readings, and find the longest error
				if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->systematic_string_value().length() > longest_systematic_error) {
					longest_systematic_error = experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->systematic_string_value().length();
				}
			}
			//Is associated with a systematic error, so need to include this in the table.
			reading_table.AddColumn(("Systematic Error (" + current->units() + ")"), longest_systematic_error + 3);
		}

	}
	reading_table.PrintHeader();
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); i++) {
		reading_table << i + 1;
		//Add each reading to the table
		reading current_reading{ experiment_list[experiment_index].return_dataset(dataset_index).return_reading(i) };
		for (int j{}; j < current_reading.size(); j++) {
			
			reading_table << current_reading.return_measurement(j)->string_value();
			if (current_reading.return_measurement(j)->standard_err() == true) {
				//Has an error
				reading_table << current_reading.return_measurement(j)->error_string_value();
			}
			if (current_reading.return_measurement(j)->systematic() == true) {
				//Has a systematic error
				reading_table << current_reading.return_measurement(j)->systematic_string_value();
			}

		}
	}
	//Need to ascertain the column widths by finding the longest string printed; do this by looping over within the loop. Also need to add the systematic and random errors
	reading_table.PrintFooter();
	int reading_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).number_readings())-1 };
	std::cout << "Please select an option from the list below: " << std::endl;
	std::cout << "1.... Edit a measurement in this reading" << std::endl;
	std::cout << "2.... Delete this reading" << std::endl;
	std::cout << "3.... Return to the main menu" << std::endl;
	int option{ get_option(3) };
	if (option == 1) {
		std::cout << "Please select which measurement you would like to edit: " << std::endl;
		for (int i{ 1 }; i <= experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); i++) {
			std::cout << i << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(i - 1)->name_and_units() << std::endl;
		}
		int measurement_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size())-1 };
		manage_measurement(experiment_index, dataset_index,reading_index, measurement_index);
	}
	else if (option == 2) {
		std::cout << "Please confirm you wish to delete this reading: " << std::endl;
		std::cout << "1.... Confirm Delete" << std::endl;
		std::cout << "2.... Return to main menu" << std::endl;
		int delete_option{ get_option(2) };
		if (delete_option == 1) {
			std::cout << "Deleting reading." << std::endl;
			try {
				experiment_list[experiment_index].return_dataset(dataset_index).delete_reading(reading_index);
				std::cout << "Reading successfully deleted." << std::endl;
			}
			catch (...) {
				std::cout << "ERROR: Unexpected error, reading could not be deleted." << std::endl;
			}
			}
		}
	else if (option == 3) {
		main_menu();
	}
}
void manage_dataset(int experiment_index, int dataset_index) 
{
	std::cout << "Managing dataset \"" << experiment_list[experiment_index].return_dataset(dataset_index).name() << "\". " << std::endl;
	std::cout << "Each reading contains " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().size() << " measurements." << std::endl;
	std::cout << "Please select an option from the list below:" << std::endl;
	std::cout << "1.... Manage a reading within this dataset" << std::endl;
	std::cout << "2.... Rename this dataset" << std::endl;
	std::cout << "3.... Change the dataset state" << std::endl;
	std::cout << "4.... Create a copy of the dataset" << std::endl;
	std::cout << "5.... Delete this dataset" << std::endl;
	std::cout << "6.... Return to main menu" << std::endl;
	int option{ get_option(6) };
	if (option == 1) {
		manage_readings(experiment_index, dataset_index);
	}
	if (option == 2) {
		std::cout << "Enter the new name for the dataset: " << std::endl;
		std::string new_name;
		getline(std::cin, new_name);
		experiment_list[experiment_index].return_dataset(dataset_index).set_name(new_name);
	}
	if (option == 3) {
		if (experiment_list[experiment_index].return_dataset(dataset_index).state() == true) {
			std::cout << "The dataset state is currently set to complete. Would you like to change it?" << std::endl;
			std::cout << "1.... Change state to incomplete" << std::endl;
			std::cout << "2.... Leave state as complete" << std::endl;
			int state_option{ get_option(2) };
			if (state_option == 1) {
				experiment_list[experiment_index].return_dataset(dataset_index).set_state(false);
			}
		}
		else {
			std::cout << "The dataset state is currently set to incomplete. Would you like to change it?" << std::endl;
			std::cout << "1.... Change state to complete" << std::endl;
			std::cout << "2.... Leave state as incomplete" << std::endl;
			int state_option{ get_option(2) };
			if (state_option == 1) {
				experiment_list[experiment_index].return_dataset(dataset_index).set_state(true);
			}
		}
	}
	if (option == 4) {
		std::string new_name;
		std::cout << "Enter the name for the new (copy) dataset:" << std::endl;
		getline(std::cin, new_name);
		experiment_list[experiment_index].add_dataset(dataset(experiment_list[experiment_index].return_dataset(dataset_index), new_name));
	}
	if (option == 5) {
		std::cout << "Please confirm you wish to delete this dataset: " << std::endl;
		std::cout << "1.... Confirm Delete" << std::endl;
		std::cout << "2.... Return to main menu" << std::endl;
		int delete_option{ get_option(2) };
		if (delete_option == 1) {
			std::cout << "Deleting dataset." << std::endl;
			try {
				experiment_list[experiment_index].delete_set(dataset_index);
				std::cout << "Dataset successfully deleted." << std::endl;
			}
			catch (...) {
				std::cout << "ERROR: Unexpected error, dataset could not be deleted." << std::endl;
			}
		}
		main_menu();
	}
	if (option == 6) {
		main_menu();
	}
}
void manage_experiment(int experiment_index) 
{
	if (experiment_list[experiment_index].number_datasets() == 0) {
		std::cout << "This experiment contains no data." << std::endl;
	}
	else {
		std::cout << "This experiment contains " << experiment_list[experiment_index].number_datasets() << " datasets: " << std::endl;
		for (int i{1}; i <= experiment_list[experiment_index].number_datasets(); i++) {
			std::cout << i << ".... ";
			experiment_list[experiment_index].return_dataset(i - 1).print_info();
			std::cout << std::endl;
		}
	}
	std::cout << "There are " << experiment_list[experiment_index].number_contributors() << " contributors:" << std::endl;
	for (int i{}; i < experiment_list[experiment_index].number_contributors(); i++) {
		std::cout << "~" << experiment_list[experiment_index].return_contributor(i) << std::endl;
	}
	std::cout << "Please select an option from the following list: " << std::endl;
	std::cout << "1.... Manage a dataset within this experiment" << std::endl;
	std::cout << "2.... Rename this experiment" << std::endl;
	std::cout << "3.... Create a copy of this experiment" << std::endl;
	std::cout << "4.... Delete this experiment" << std::endl;
	std::cout << "5.... Return to main menu" << std::endl;
	int option{ get_option(5) };
	if (option == 1) {
		std::cout << "Please select a dataset to manage: " << std::endl;
		for (int i{ 1 }; i <= experiment_list[experiment_index].number_datasets(); i++) {
			std::cout << i << ".... ";
			experiment_list[experiment_index].return_dataset(i - 1).print_info();
			std::cout << std::endl;
		}
		int dataset_index{ get_option(experiment_list[experiment_index].number_datasets()) - 1 };
		manage_dataset(experiment_index, dataset_index);
	}
	if (option == 2) {
		std::cout << "Enter the new name for the experiment: " << std::endl;
		std::string new_name;
		getline(std::cin, new_name);
		experiment_list[experiment_index].rename(new_name);
		std::cout << "Experiment renamed!." << std::endl;
	}
	if (option == 3) {
		//Create a new experiment, that is a copy of this experiment
		std::string new_name;
		std::cout << "Enter the name for the new (copy) experiment:" << std::endl;
		getline(std::cin, new_name);
		experiment_list.push_back(experiment(experiment_list[experiment_index],new_name));
	}
	if (option == 4) {
		std::cout << "Please confirm you wish to delete this experiment: " << std::endl;
		std::cout << "1.... Confirm Delete" << std::endl;
		std::cout << "2.... Return to main menu" << std::endl;
		int delete_option{ get_option(2) };
		if (delete_option == 1) {
			std::cout << "Deleting experiment." << std::endl;
			try {
				experiment_list.erase(experiment_list.begin() + experiment_index);
				std::cout << "Experiment successfully deleted." << std::endl;
			}
			catch (...) {
				std::cout << "ERROR: Unexpected error, experiment could not be deleted." << std::endl;
			}
		}
		main_menu();
	}
	if (option == 5) {
		main_menu();
	}
	main_menu();
}

