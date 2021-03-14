#include"inputs.h"
#include"base_menus.h"

reading create_reading(reading_template template_to_use, bool is_new) 
{
	reading new_reading{};
	time_t now = time(0);
	if (is_new == true) {
		//Timestamp it.
		std::cout << "New reading created with timestamp " << time_t_to_date_and_time(now) << std::endl;
		new_reading.set_if_new(true);
		new_reading.set_time(now);
	}
	else {
		std::cout << "New reading created with no timestamp." << std::endl;
		new_reading.set_if_new(false);
	}
	//Now loop over the template an get the required values
	for (int i{}; i < template_to_use.size(); i++) {
		std::shared_ptr<measurement> current{ template_to_use.get_measurement(i) };
		if (current->type() == "numerical_measurement") {
			if (current->repeats() == 1) {//only a single measurement
				double new_value{};
				double new_standard_error{};
				double new_systematic_error{};
				std::cout << "Enter a value for the " << current->name() << ", with units " << current->units() << ":" << std::endl;
				std::cin.ignore(std::cin.rdbuf()->in_avail());
				new_value = get_value<double>();
				if (current->standard_err() == true) {
					//Has an error
					std::cout << "Enter the error associated with this measurement, with units " << current->units() << ": " << std::endl;
					std::cin.ignore(std::cin.rdbuf()->in_avail());
					new_standard_error = get_value<double>();
				}
				if (current->systematic() == true) {
					//Has an error
					std::cout << "Enter the systematic error associated with this measurement, with units " << current->units() << ": " << std::endl;
					std::cin.ignore(std::cin.rdbuf()->in_avail());
					new_systematic_error = get_value<double>();
				}
				new_reading.add_measurement(std::make_shared<numerical_measurement>(new_value, new_standard_error, new_systematic_error, current));
			}
			else {
				//repeats
				std::vector<double> new_data;
				double new_systematic_error{};
				for (int i{}; i < current->repeats(); i++) {//This might not be the right limits
					std::cout << "Please enter measurement repeat " << i + 1 << " of " << current->repeats() << ", with units " << current->units() << ": " << std::endl;
					double new_value;
					std::cin.ignore(std::cin.rdbuf()->in_avail());
					new_value = get_value<double>();
					new_data.push_back(new_value);
				}
				if (current->systematic() == true) {
					//Has an error
					std::cout << "Enter the systematic error associated with this measurement, with units " << current->units() << ": " << std::endl;
					std::cin.ignore(std::cin.rdbuf()->in_avail());
					new_systematic_error = get_value<double>();
				}

				std::shared_ptr< measurement> to_add = std::make_shared< numerical_measurement>(new_data, new_systematic_error, current);

				new_reading.add_measurement(to_add);
			}
		}
		else if (current->type() == "text_measurement") {
			std::string new_value;
			std::cout << "Enter a value for the " << current->name() << ", with units " << current->units() << ":" << std::endl;
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			getline(std::cin, new_value);
			std::shared_ptr< measurement> to_add = std::make_shared < text_measurement>(new_value, current);
			new_reading.add_measurement(to_add);
		}
		else if (current->type() == "boolean_measurement") {
			bool new_value;
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			std::cout << "Please select an option for the measurement " << current->name() << ": " << std::endl;
			std::cout << "1.... True" << std::endl;
			std::cout << "2.... False" << std::endl;
			int option{ get_option(2) };
			if (option == 1) {
				new_value = true;
			}
			else {
				new_value = false;
			}

			new_reading.add_measurement(std::make_shared < boolean_measurement>(new_value, current));
		}
		else if (current->type() == "text_comment") {
			std::string new_value;
			std::cout << "Please enter the comment " << current->name() << ": " << std::endl;
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			getline(std::cin, new_value);
			new_reading.add_measurement(std::make_shared <text_comment>(new_value, current));
		}
		else {
			std::cout << "ERROR: Invalid measurement type detected." << std::endl;
		}
	}
	return new_reading;
}
void input_readings(dataset& input_dataset, bool is_new) 
{
	std::cout << "Creating readings for dataset " << input_dataset.name() << std::endl;
	bool finished_input{ false };
	while (finished_input == false) {
		//Take in new readings
		input_dataset.add_reading(create_reading(input_dataset.return_template(), is_new));
		std::cout << "Please select an option: " << std::endl;
		std::cout << "1.... Input another reading" << std::endl;
		std::cout << "2.... Finish inputting readings" << std::endl;
		int option{ get_option(2) };
		if (option == 2) {
			finished_input = true;
		}
	}
	main_menu();
}
void input_existing_data(int experiment_index) 
{
	//Check for datasets
	std::cout << "Would you like to use an existing dataset or create a new one?" << std::endl;
	std::cout << "1.... Use an existing dataset" << std::endl;
	std::cout << "2.... Create a new dataset" << std::endl;
	std::cout << "3.... Return to Main Menu" << std::endl;
	int option{ get_option(3) };
	if (option == 1) {
		if (experiment_list[experiment_index].datasets_exist() == true) {
			//Datasets exist, select one
			int dataset_index{ experiment_list[experiment_index].select_dataset() };
			if (experiment_list[experiment_index].return_dataset(dataset_index).state() == true) {
				std::cout << "This dataset is complete, please change its state in Manage Data to edit it." << std::endl;
				main_menu();
			}
			input_readings(experiment_list[experiment_index].return_dataset(dataset_index), false);
			main_menu();
			//Now create a new reading in this dataset
		}
		else {
			int dataset_index{ experiment_list[experiment_index].create_dataset(false) };
			input_readings(experiment_list[experiment_index].return_dataset(dataset_index), false);
			main_menu();
		}
	}
	if (option == 2) {
		int dataset_index{ experiment_list[experiment_index].create_dataset(false) };
		input_readings(experiment_list[experiment_index].return_dataset(dataset_index), false);
		main_menu();
	}
	if (option == 3) {
		main_menu();
	}
}
void input_existing_data_menu() 
{
	std::cout << "Would you like to input data for an existing experiment, or create a new experiment?" << std::endl;
	std::cout << "1.... Use an existing experiment" << std::endl;
	std::cout << "2.... Create a new experiment" << std::endl;
	std::cout << "3.... Return to Main Menu" << std::endl;
	int option{ get_option(3) };
	int experiment_index;
	if (option == 1) {
		if (experiments_exist() == true) {
			experiment_index = select_experiment();
		}
		else {
			std::cout << "No experiments found! Please create an experiment." << std::endl;
			experiment_index = create_experiment();
		}
		input_existing_data(experiment_index);
		//Now go to input
	}
	if (option == 2) {
		experiment_index = create_experiment();
		//Go to input
		input_existing_data(experiment_index);
	}
	if (option == 3) {
		main_menu();
	}
}
void input_new_data(int experiment_index) 
{
	//Check for datasets
	std::cout << "Would you like to use an existing dataset or create a new one?" << std::endl;
	std::cout << "1.... Use an existing dataset" << std::endl;
	std::cout << "2.... Create a new dataset" << std::endl;
	std::cout << "3.... Return to Main Menu" << std::endl;
	int option{ get_option(3) };
	if (option == 1) {
		if (experiment_list[experiment_index].datasets_exist() == true) {
			//Datasets exist, select one
			int dataset_index{ experiment_list[experiment_index].select_dataset() };
			if (experiment_list[experiment_index].return_dataset(dataset_index).state() == true) {
				std::cout << "This dataset is complete, please change its state in Manage Data to edit it." << std::endl;
				main_menu();
			}
			input_readings(experiment_list[experiment_index].return_dataset(dataset_index), true);
			//Now create a new reading in this dataset
		}
		else {
			int dataset_index{ experiment_list[experiment_index].create_dataset(true) };
			input_readings(experiment_list[experiment_index].return_dataset(dataset_index), true);
			//Now create a new reading in this dataset
		}
	}
	if (option == 2) {
		int dataset_index{ experiment_list[experiment_index].create_dataset(true) };
		input_readings(experiment_list[experiment_index].return_dataset(dataset_index), true);
		//Now create a new reading in this dataset
	}
	if (option == 3) {
		main_menu();
	}
}
void input_new_data_menu() 
{
	std::cout << "Would you like to input data for an existing experiment, or create a new experiment?" << std::endl;
	std::cout << "1.... Use an existing experiment" << std::endl;
	std::cout << "2.... Create a new experiment" << std::endl;
	std::cout << "3.... Return to Main Menu" << std::endl;
	int option{ get_option(3) };
	int experiment_index;
	if (option == 1) {
		if (experiments_exist() == true) {
			experiment_index = select_experiment();
		}
		else {
			std::cout << "No experiments found! Please create an experiment." << std::endl;
			experiment_index = create_experiment();
		}
		input_new_data(experiment_index);
		//Now go to input
	}
	if (option == 2) {
		experiment_index = create_experiment();
		//Go to input
		input_new_data(experiment_index);
	}
	if (option ==3) {
		main_menu();
	}
}