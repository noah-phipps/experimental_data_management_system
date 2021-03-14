#include"viewing.h"
#include"base_menus.h"

void view_experiment(int experiment_index) 
{
	std::cout << "Viewing data for experiment " << experiment_list[experiment_index].name() << ":" << std::endl;
	if (experiment_list[experiment_index].number_datasets() == 0) {
		std::cout << "This experiment contains no data." << std::endl;
		main_menu();
	}
	std::cout << "There are " << experiment_list[experiment_index].number_contributors() << " contributors:" << std::endl;
	for (int i{}; i < experiment_list[experiment_index].number_contributors(); i++) {
		std::cout << "~" << experiment_list[experiment_index].return_contributor(i) << std::endl;
	}
	std::cout << "Select a dataset to view more info: " << std::endl;
	for (int i{ 1 }; i <= experiment_list[experiment_index].number_datasets();i++) {
		std::cout << i << ".... ";
		experiment_list[experiment_index].return_dataset(i - 1).print_info();
		std::cout << std::endl;
	}
	std::cout << experiment_list[experiment_index].number_datasets() + 1 << ".... Main Menu" << std::endl;
	int option{ get_option(experiment_list[experiment_index].number_datasets() + 1) };
	if (option == experiment_list[experiment_index].number_datasets() + 1) {
		main_menu();
	}
	else {
		view_dataset(experiment_index,option - 1);
		main_menu();
	}
}
void view_dataset(int experiment_index, int dataset_index) 
{
	std::cout << "Viewing data for dataset " << experiment_list[experiment_index].return_dataset(dataset_index).name() << ", in experiment " << experiment_list[experiment_index].name() << ": " << std::endl;
	std::cout << "Each reading contains " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().size() << " measurements." << std::endl;
	std::cout << "Dataset is displayed below: \n" << std::endl;
	bprinter::TablePrinter reading_table(&std::cout);
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); i++) {
		//Add headers
		std::shared_ptr<measurement> current{ experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(i) };
		//Now loop over the measurements in this one and find the longest string
		size_t longest_string{static_cast<size_t>(current->name_and_units().length())};//Will always be an int, so perform a static cast
		for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {
			if (experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->string_value().length() > longest_string) {
				longest_string = experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).return_measurement(i)->string_value().length();
			}
		}
		reading_table.AddColumn(current->name_and_units(), longest_string+3);
		size_t longest_standard_error{static_cast<size_t>( 8 + current->units().length() )};//8 is length of Error()
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
		size_t longest_systematic_error{static_cast<size_t>( 18 + current->units().length() )};//18 is length of Systematic Error()
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
	reading_table.AddColumn("Timestamp", 20);
	reading_table.PrintHeader();
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); i++) {
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
		reading_table << time_t_to_date_and_time(current_reading.return_time());
	}
	reading_table.PrintFooter();
}