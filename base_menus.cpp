#include"base_menus.h"

void input_data_menu() 
{
	std::cout << "Input Data Menu" << std::endl;
	std::cout << "Please select an option from the following list:" << std::endl;
	std::cout << "1.... Input Existing Data, omitting timestamps" << std::endl;
	std::cout << "2.... Input New Data, with timestamps" << std::endl;
	std::cout << "3.... Return to Main Menu" << std::endl;
	int option{ get_option(3) };
	if (option == 1) {
		input_existing_data_menu();
	}
	else if (option == 2) {
		input_new_data_menu();
	}
	else if (option == 3) {
		main_menu();
	}
}
void import_data_menu() 
{
	std::cout << "Import Data Menu" << std::endl;
	std::cout << "Please select an option from the following list:" << std::endl;
	std::cout << "1.... Import Dataset from .csv file" << std::endl;
	std::cout << "2.... Import Dataset from .txt file" << std::endl;
	std::cout << "3.... Import Dataset from another file type" << std::endl;
	std::cout << "4.... Main Menu" << std::endl;
	int option{ get_option(4) };
	if (option == 1) {
		std::cout << "Would you like to input data for an existing experiment, or create a new experiment?" << std::endl;
		std::cout << "1.... Use an existing experiment" << std::endl;
		std::cout << "2.... Create a new experiment" << std::endl;
		std::cout << "3.... Return to Main Menu" << std::endl;
		int exp_option{ get_option(3) };
		int experiment_index;
		if (exp_option == 1) {
			if (experiments_exist() == true) {
				experiment_index = select_experiment();
			}
			else {
				std::cout << "No experiments found! Please create an experiment." << std::endl;
				experiment_index = create_experiment();
			}
			experiment_list[experiment_index].add_dataset(import_csv());
			//Now go to input
		}
		else if (exp_option == 2) {
			experiment_index = create_experiment();
			//Go to input
			experiment_list[experiment_index].add_dataset(import_csv());
		}
	}
	else if (option == 2) {
		std::cout << "Would you like to input data for an existing experiment, or create a new experiment?" << std::endl;
		std::cout << "1.... Use an existing experiment" << std::endl;
		std::cout << "2.... Create a new experiment" << std::endl;
		std::cout << "3.... Return to Main Menu" << std::endl;
		int exp_option{ get_option(3) };
		int experiment_index;
		if (exp_option == 1) {
			if (experiments_exist() == true) {
				experiment_index = select_experiment();
			}
			else {
				std::cout << "No experiments found! Please create an experiment." << std::endl;
				experiment_index = create_experiment();
			}
			experiment_list[experiment_index].add_dataset(import_txt());
			//Now go to input
		}
		else if (exp_option == 2) {
			experiment_index = create_experiment();
			//Go to input
			experiment_list[experiment_index].add_dataset(import_txt());
		}
	}
	else if (option == 3) {
		std::cout << "Would you like to input data for an existing experiment, or create a new experiment?" << std::endl;
		std::cout << "1.... Use an existing experiment" << std::endl;
		std::cout << "2.... Create a new experiment" << std::endl;
		std::cout << "3.... Return to Main Menu" << std::endl;
		int exp_option{ get_option(3) };
		int experiment_index;
		if (exp_option == 1) {
			if (experiments_exist() == true) {
				experiment_index = select_experiment();
			}
			else {
				std::cout << "No experiments found! Please create an experiment." << std::endl;
				experiment_index = create_experiment();
			}
			experiment_list[experiment_index].add_dataset(import_any());
			//Now go to input
		}
		else if (exp_option == 2) {
			experiment_index = create_experiment();
			//Go to input
			experiment_list[experiment_index].add_dataset(import_any());
		}	
	}
	else if (option == 4) {
		main_menu();
	}
}
void view_data_menu() 
{
	std::cout << "View Data Menu" << std::endl;
	if (experiments_exist() == true) {
		std::cout << "The following experiments exist. Select one to view more details:" << std::endl;
		for (size_t i{ 1 }; i <= experiment_list.size(); i++) {
			std::cout << i << ".... " << experiment_list[i - 1].name() << std::endl;
		}
		std::cout << experiment_list.size() + 1 << ".... Return to main menu" << std::endl;
		int option{ get_option(experiment_list.size() + 1) };
		if (option == experiment_list.size() + 1) {
			main_menu();
		}
		else {
			view_experiment(option - 1);
		}
	}
	else {
		std::cout << "No experiments exist." << std::endl;
		main_menu();
	}
}
void manage_data_menu() 
{
	std::cout << "Manage Data Menu" << std::endl;
	if (experiments_exist() == false) {
		std::cout << "No experiments exist!" << std::endl;
		main_menu();
	}
	std::cout << "Please select an experiment to manage." << std::endl;
	manage_experiment(select_experiment());

}
void analyse_data_menu() 
{
	std::cout << "Analyse Menu" << std::endl;
	if (experiments_exist() == false) {
		std::cout << "No experiments exist!" << std::endl;
		main_menu();
	}
	std::cout << "Please select an experiment to analyse:" << std::endl;
	analyse_experiment(select_experiment());
}
void export_data_menu() 
{
	std::cout << "Export Data Menu" << std::endl;
	if (experiments_exist() == false) {
		std::cout << "No experiments exist!" << std::endl;
		std::cout << "Please select an option:" << std::endl;
		std::cout << "1.... Load data from a .edms file" << std::endl;
		std::cout << "2.... Main Menu" << std::endl;
		int no_experiment_option{ get_option(2) };
		if (no_experiment_option == 2) {
			main_menu();
		}
		else {
			load_all_data();
		}
		main_menu();
	}
	std::cout << "Please select an option from the following list:" << std::endl;
	std::cout << "1.... Save all data to a .edms file" << std::endl;
	std::cout << "2.... Load data in from a .edms file" << std::endl;
	std::cout << "3.... Export an individual dataset to a .csv file" << std::endl;
	std::cout << "4.... Main menu" << std::endl;
	int option{ get_option(4) };
	if (option == 1) {
		save_all_data();
	}
	else if (option == 2) {
		load_all_data();
	}
	else if (option == 3) {
		std::cout << "Please select an experiment to save a dataset from:" << std::endl;
		int experiment_index{ select_experiment() };
		std::cout << "Please select a dataset to export:" << std::endl;
		int dataset_index{ experiment_list[experiment_index].select_dataset() };
		save_dataset(experiment_index, dataset_index);
	}
	else {
		main_menu();
	}
	
}
void main_menu() 
{
	std::cout << "Main Menu" << std::endl;
	std::cout << "Please select an option from the following list:" << std::endl;
	std::cout << "1.... Input Data" << std::endl;
	std::cout << "2.... Import Data from File" << std::endl;
	std::cout << "3.... View Data" << std::endl;
	std::cout << "4.... Manage Data" << std::endl;
	std::cout << "5.... Analyse Data" << std::endl;
	std::cout << "6.... Export/Load Data" << std::endl;
	std::cout << "7.... Exit Program" << std::endl;
	int option{ get_option(7) };
	if (option == 1) {
		input_data_menu();
	}
	else if (option == 2) {
		import_data_menu();
	}
	else  if (option == 3) {
		view_data_menu();
	}
	else if (option == 4) {
		manage_data_menu();
	}
	else if (option == 5) {
		analyse_data_menu();
	}
	else if (option == 6) {
		export_data_menu();
	}
	else {
		std::cout << "Leaving program!";
		terminate_program_good();
	}
	return;
}