#include<string>
#include<vector>
#include<stdlib.h>
#include<ctime>
#include<iostream>
#include<vector>
#include"useful_functions.h"
#include"input_checks.h"
#include"global_lists.h"

#include"data_type_classes.h"

//Experiment class function definitions
experiment::experiment(std::string name) 
{
	experiment_name = name;
}
experiment::experiment(const experiment& to_copy, std::string new_name) 
{
	//Experiment deep copy constructor
	if (&to_copy == this) {
		std::cout << "ERROR: ATTEMPTED SELF ASSIGNMENT";
	}
	experiment_datasets = to_copy.return_all_sets();
	experiment_name = new_name;
	contributors = to_copy.return_all_contributors();
}
experiment experiment::operator=(experiment to_assign_from) 
{
	if (&to_assign_from == this) {
		std::cout << "ERROR: ATTEMPTED SELF ASSIGNMENT";
	}
	experiment_datasets = to_assign_from.return_all_sets();
	experiment_name = to_assign_from.name();
	contributors = to_assign_from.return_all_contributors();
	return *this;
}
void experiment::delete_set(int dataset_index) 
{
	experiment_datasets.erase(experiment_datasets.begin() + dataset_index);
}
void experiment::add_dataset(dataset new_dataset) 
{
	experiment_datasets.push_back(new_dataset);
	bool duplicate{ false };
	for (size_t i{}; i < contributors.size(); i++) {
		if (contributors[i] == new_dataset.contributor()) {
			duplicate = true;
		}
	}
	if (duplicate == false) {
		contributors.push_back(new_dataset.contributor());
	}
}
bool experiment::datasets_exist() 
{
	if (experiment_datasets.size() > 0) {
		return true;
	}
	else {
		return false;
	}
}
int experiment::select_dataset() 
{
	//Select a dataset, return its index literal
	std::cout << "Please select a dataset from the following list: " << std::endl;
	for (size_t i{ 1 }; i <= experiment_datasets.size(); i++) {
		std::cout << i << ".... " << experiment_datasets[i - 1].name() << std::endl;
	}
	int option{ get_option(experiment_datasets.size()) };
	std::cout << "Dataset \"" << experiment_datasets[option - 1].name() << "\" selected. " << std::endl;
	return option - 1;
}
int experiment::create_dataset(bool is_new_data)
{
	std::cout << "Please enter the new dataset name: " << std::endl;
	std::string new_dataset_name;
	std::string contributor;
	getline(std::cin, new_dataset_name);
	if (contributors.size() > 0) {
		std::cout << "Please select a previous contributor to author this dataset, or choose to create a new contributor: " << std::endl;
		for (size_t i{ 1 }; i <= contributors.size(); i++) {
			std::cout << i << ".... " << contributors[i - 1] << std::endl;
		}
		std::cout << contributors.size() + 1 << ".... Create a new contributor" << std::endl;
		int option{ get_option(contributors.size() + 1) };

		if (option == contributors.size() + 1) {
			std::cout << "Enter new contributor name: " << std::endl;
			getline(std::cin, contributor);
		}
		else {
			contributor = contributors[option - 1];
		}
	}
	else {
		std::cout << "No contributors exist. Enter new contributor name: " << std::endl;
		getline(std::cin, contributor);
	}
	if (is_new_data == true) {
		experiment_datasets.push_back(dataset(new_dataset_name, contributor));
		contributors.push_back(contributor);
	}
	else {
		experiment_datasets.push_back(dataset(new_dataset_name, contributor));//Not sure if it should be dated or not
		contributors.push_back(contributor);
	}
	std::cout << "Would you like to use an existing reading template or create a new one for this dataset? " << std::endl;
	std::cout << "1.... Create a new reading template" << std::endl;
	std::cout << "2.... Use an existing reading template" << std::endl;
	int option{ get_option(2) };
	if (option == 1) {
		//Create new template
		reading_template new_template{ reading_template_creator() };
		template_list.push_back(new_template);//Add it to the list of templates
		experiment_datasets[experiment_datasets.size() - 1].set_template(new_template);
	}
	if (option == 2) {
		//Check templates exist, loop over, select OR allow to choose to create a new one
		if (templates_exist() == true) {
			std::cout << "Please select a template from the following list:" << std::endl;
			for (size_t i{ 1 }; i <= template_list.size(); i++) {
				std::cout << i << ".... " << template_list[i].return_name() << std::endl;
			}
			std::cout << template_list.size() + 1 << ".... Create a new template" << std::endl;
			int option{ get_option(template_list.size() + 1) };
			if (option == template_list.size() + 1) {
				reading_template new_template{ reading_template_creator() };
				template_list.push_back(new_template);//Add it to the list of templates
				experiment_datasets[experiment_datasets.size() - 1].set_template(new_template);
			}
			else {
				experiment_datasets[experiment_datasets.size() - 1].set_template(template_list[option - 1]);
			}
		}
		else {
			std::cout << "No templates exist. Please create a new template." << std::endl;
			reading_template new_template{ reading_template_creator() };
			template_list.push_back(new_template);//Add it to the list of templates
			experiment_datasets[experiment_datasets.size() - 1].set_template(new_template);
		}
	}
	std::cout << "New dataset \"" << experiment_datasets[experiment_datasets.size() - 1].name() << "\" created, with contributor " << experiment_datasets[experiment_datasets.size() - 1].contributor() << ", dated " << time_t_to_date(experiment_datasets[experiment_datasets.size() - 1].date()) << std::endl;
	return experiment_datasets.size() - 1;
	//Create a new dataset and return its index
}