#include"analysis.h"
#include"base_menus.h"

//Evaluation Functions
double polynomial_evaluator(double x_i, int degree, std::vector<double> coefficients) 
{
	//Returns f(x_i)
	double f_x_i{ 0 };
	for (int i{}; i <= degree; i++) {
		f_x_i += (coefficients[i] * pow(x_i, i));
	}
	return f_x_i;
}
double chi_squared_cost(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, std::vector<double> coefficients, double step) 
{
	//Returns the chi-squared
	double chi_squared{ 0 };
	for (size_t i{}; i < x_data.size(); i++) {
		chi_squared += pow(((polynomial_evaluator(x_data[i], degree, coefficients) - y_data[i]) / y_errors[i]), 2);
	}
	return chi_squared;
}

//Fitting/Optimisation Functions
std::vector<double> polynomial_fitting(std::vector<double> x_data, std::vector<double> y_data, int degree) 
{
	//Fits a polynomial of any degree to the provided data, and returns the coefficients for that polynomial; uses no errors, and a least squares fitting procedure involving a matrix equation
	//Create a vector of doubles, length 2*degree + 1
	std::vector<double> sigma_xi_2n;
	for (int i{}; i < (2 * degree + 1); i++) {
		sigma_xi_2n.push_back(0.0);
	}
	//Initialise these values, using user dara
	for (size_t i{}; i < sigma_xi_2n.size(); i++) {
		sigma_xi_2n[i] = 0;
		for (size_t j{}; j < x_data.size(); j++) {
			sigma_xi_2n[i] += pow(x_data[j], i);
		}
	}
	//Create a vector to store the final coefficients of the polynomial in
	std::vector<double> final_coefficients;
	for (int i{}; i < (degree + 1); i++) {
		final_coefficients.push_back(0.0);
	}
	//Create the augmented matrix
	std::vector<std::vector<double>> augmented_matrix;
	for (int i{}; i < (degree + 1); i++) {
		std::vector<double> temp;
		for (int i{}; i < (degree + 2); i++) {
			temp.push_back(0);
		}
		augmented_matrix.push_back(temp);
	}
	//Populate the augmented matrix
	for (int i{}; i <= degree; i++) {
		for (int j{}; j <= degree; j++) {
			augmented_matrix[i][j] = sigma_xi_2n[i + j];
		}
	}
	//Create another vector of doubles, length degree+1
	std::vector<double> sigma_xi_n_yi;
	for (int i{}; i < degree + 1; i++) {
		sigma_xi_n_yi.push_back(0.0);
	}
	//Initialise these values, using user data
	for (int i{}; i < degree + 1; i++) {
		sigma_xi_n_yi[i] = 0.0;
		for (size_t j{}; j < x_data.size(); j++) {
			sigma_xi_n_yi[i] += pow(x_data[j], i) * y_data[j];
		}
	}
	//Set the last column of the augmented matrix to the values of sigma_xi_n_yi
	for (int i{}; i <= degree; i++) {
		augmented_matrix[i][degree + 1] = sigma_xi_n_yi[i];
	}
	//temporary double
	double tmp;
	//Pivot the augmented matrix augmented
	for (int i{}; i < degree + 1; i++) {
		for (int k{ i + 1 }; k < degree + 1; k++) {
			if (augmented_matrix[i][i] < augmented_matrix[k][i]) {
				for (int j{}; j <= degree + 1; j++) {
					tmp = augmented_matrix[i][j];
					augmented_matrix[i][j] = augmented_matrix[k][j];
					augmented_matrix[k][j] = tmp;
				}
			}
		}
	}
	//Perform gaussian elimination, making elements below pivot = 0 or eliminating variable
	for (int i{}; i < degree; i++) {
		for (int k{ i + 1 }; k < degree + 1; k++) {
			double t{ augmented_matrix[k][i] / augmented_matrix[i][i] };
			for (int j{}; j <= degree + 1; j++) {
				augmented_matrix[k][j] -= t * augmented_matrix[i][j];
			}
		}
	}
	//back substitution, setting variable as rhs of previous equation, subtracting lhs values except desired coefficient, and divide rhs by coefficient of calculated variable
	for (int i{ degree }; i >= 0; i--) {
		final_coefficients[i] = augmented_matrix[i][degree + 1];
		for (int j{}; j < degree + 1; j++) {
			if (j != i) {
				final_coefficients[i] -= augmented_matrix[i][j] * final_coefficients[j];
			}
		}
		final_coefficients[i] /= augmented_matrix[i][i];
	}
	std::cout << "Matrix Methods Polynomial fitting complete." << std::endl;
	std::cout << "The polynomial fit is: " << std::endl << "y = ";
	for (size_t i{}; i < final_coefficients.size(); i++) {
		if (i != 0) { std::cout << " + "; };
		std::cout << final_coefficients[i] << "*x^" << i;
	}
	return final_coefficients;
}
std::vector<double> optimise_coefficients(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, std::vector<double> coefficients, int coefficient_index, double step) 
{
	std::vector<double> best_coefficients;
	best_coefficients = coefficients;
	double best_chi_squared{ chi_squared_cost(x_data,y_data,y_errors,degree,best_coefficients,step) };
	double chi_squared{};
	bool stuck{ false };
	while (chi_squared <= best_chi_squared && stuck == false) {
		int stuck_counter{};
		std::vector<double> coefficients_up{ best_coefficients };
		std::vector<double> coefficients_down{ best_coefficients };
		coefficients_up[coefficient_index] += step;
		coefficients_down[coefficient_index] -= step;
		double up_step{ chi_squared_cost(x_data,y_data,y_errors,degree,coefficients_up,step) };
		double down_step{ chi_squared_cost(x_data,y_data,y_errors,degree,coefficients_down,step) };
		if (up_step < down_step) {
			//Upstep is better
			chi_squared = chi_squared_cost(x_data, y_data, y_errors, degree, coefficients_down, step);
			if (up_step < best_chi_squared) {
				//Upstep is best
				best_chi_squared = up_step;
				best_coefficients = coefficients_up;
			}
		}
		else {
			chi_squared = chi_squared_cost(x_data, y_data, y_errors, degree, coefficients_down, step);
			if (down_step < best_chi_squared) {
				//Downstep is best
				best_chi_squared = down_step;
				best_coefficients = coefficients_down;
			}
		}
		if (chi_squared == best_chi_squared) {
			stuck_counter += 1;
		}
		if (stuck_counter >= 3) {
			//Error if it keeps being stuck, but not fatal; just not necessarily optimised
			std::cout << "\n\nSTUCK 3 TIMES! FORCED EXIT FROM OPTIMISATION LOOP.\n\n";
			stuck = true;
		}
	}
	return best_coefficients;
}

//Analysis Functions
std::string unweighted_mean(dataset analysed_set, int measurement_index) 
{
	double mean{};
	int number{ analysed_set.number_readings() };
	for (int i{}; i < number; i++) {
		mean += analysed_set.return_reading(i).return_measurement(measurement_index)->num_value();
	}
	std::string return_string{ std::to_string((mean/number)) };
	return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
	return return_string;
}
std::string weighted_mean(dataset analysed_set, int measurement_index) 
{
	if (analysed_set.return_template().get_measurement(measurement_index)->standard_err() == false && analysed_set.return_template().get_measurement(measurement_index)->systematic() == false) {
		return "N/A";
	}
	double mean{};
	double weights{};
	int number{ analysed_set.number_readings() };
	for (int i{}; i < number; i++) {
		mean += analysed_set.return_reading(i).return_measurement(measurement_index)->num_value() * analysed_set.return_reading(i).return_measurement(measurement_index)->num_error();
		weights += analysed_set.return_reading(i).return_measurement(measurement_index)->num_error();
	}
	std::string return_string{ std::to_string(mean / weights) };
	return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
	return return_string;
}
std::string weighted_error(dataset analysed_set, int measurement_index) 
{
	if (analysed_set.return_template().get_measurement(measurement_index)->standard_err() == false && analysed_set.return_template().get_measurement(measurement_index)->systematic() == false) {
		return "N/A";
	}
	double weights{};
	int number{ analysed_set.number_readings() };
	for (int i{}; i < number; i++) {
		weights += analysed_set.return_reading(i).return_measurement(measurement_index)->num_error();
	}
	std::string return_string{ std::to_string((1/sqrt(weights))) };
	return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
	return return_string;
}
std::string standard_error(dataset analysed_set, int measurement_index) 
{
	double standard_error{};
	double mean{ stod(unweighted_mean(analysed_set,measurement_index)) };
	int number{ analysed_set.number_readings() };
	for (int i{}; i < number; i++) {
		standard_error += pow((analysed_set.return_reading(i).return_measurement(measurement_index)->num_value() - mean), 2);
	}
	std::string return_string{ std::to_string((sqrt(standard_error / number))) };
	return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
	return return_string;
}
std::string standard_error_percent(dataset analysed_set, int measurement_index) 
{
	double mean{ stod(unweighted_mean(analysed_set,measurement_index)) };
	double error{ stod(standard_error(analysed_set,measurement_index)) };
	double percentage{ (error / mean) * 100 };
	std::string return_string{ std::to_string((percentage)) };
	return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
	return return_string+"%";
}
std::string mode(dataset analysed_set, int measurement_index) 
{
	int number{ analysed_set.number_readings() };
	std::vector<double> values;
	std::vector<double> modes;
	std::map<int, double> frequency;
	for (int i{}; i < number; i++) {
		values.push_back(analysed_set.return_reading(i).return_measurement(measurement_index)->num_value());
	}
	std::sort(values.begin(), values.end());
	for (int i{}; i < number; i++) {
		frequency[values[i]]++;//The error C4244 generated on this line appears to be a visual studio bug as far as I can tell
	}
	double current_max{};
	for (auto iterator{ frequency.cbegin() }; iterator != frequency.cend(); iterator++) {
		if (iterator->second > current_max) {
			modes.push_back(iterator->first);
			current_max = iterator->second;
		}
	}
	if (current_max == 1) {
		return "N/A";
	}
	else {
		std::string return_string{ std::to_string((modes[modes.size() - 1])) };
		return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
		return return_string;
	}
}
std::string median(dataset analysed_set, int measurement_index) 
{
	int number{ analysed_set.number_readings() };
	std::vector<double> values;
	for (int i{}; i < number; i++) {
		values.push_back(analysed_set.return_reading(i).return_measurement(measurement_index)->num_value());
	}
	std::sort(values.begin(), values.end());
	if (number % 2) {
		//It's odd
		std::string return_string{ std::to_string(((values[(number + 1 / 2) - 1]))) };
		return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
		return return_string;
	}
	else {
		//It's even
		double result{ values[number / 2] + values[(number / 2) + 1] };
		std::string return_string{ std::to_string((result/2)) };
		return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
		return return_string;
	}
}
std::string boolean_true_analysis(dataset analysed_set, int measurement_index) 
{
	int true_values{};
	for (int i{}; i < analysed_set.number_readings(); i++) {
		if (analysed_set.return_reading(i).return_measurement(measurement_index)->string_value() == "True") {
			true_values += 1;
		}
	}

	std::string return_string_1{ std::to_string((true_values)) };
	return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
	std::string return_string_2{ std::to_string((100*true_values/analysed_set.number_readings())) };
	return_string_2.erase(return_string_2.find_last_not_of('0') + 1, std::string::npos);
	return "(" + return_string_1 + ", " +return_string_2 + "%)";
}
std::string boolean_false_analysis(dataset analysed_set, int measurement_index) 
{
	int false_values{};
	for (int i{}; i < analysed_set.number_readings(); i++) {
		if (analysed_set.return_reading(i).return_measurement(measurement_index)->string_value() == "False") {
			false_values += 1;
		}
	}

	std::string return_string_1{ std::to_string((false_values)) };
	return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
	std::string return_string_2{ std::to_string((100 * false_values / analysed_set.number_readings())) };
	return_string_2.erase(return_string_2.find_last_not_of('0') + 1, std::string::npos);
	return "(" + return_string_1 + ", " + return_string_2 + "%)";
}
std::vector<std::vector<std::string>> simple_boolean_analysis_results(int experiment_index, int dataset_index) 
{
	dataset current{ experiment_list[experiment_index].return_dataset(dataset_index) };
	std::vector<std::vector<std::string>> results;
	for (int i{}; i < current.number_readings(); i++) {//Loop over all readings
		reading this_reading{ current.return_reading(i) };
		for (int j{}; j < this_reading.size(); j++) {//Loop over all measurements
			if (this_reading.return_measurement(j)->type() == "boolean_measurement") {
				std::vector<std::string> temp;
				temp.push_back(this_reading.return_measurement(j)->name());
				temp.push_back(boolean_true_analysis(current, j));
				temp.push_back(boolean_false_analysis(current, j));
				results.push_back(temp);
			}
		}
	}
	return results;
}
std::vector<std::vector<std::string>> simple_numerical_analysis_results(int experiment_index, int dataset_index) 
{
	dataset current{ experiment_list[experiment_index].return_dataset(dataset_index) };
	std::vector<std::vector<std::string>> results;
	std::vector<bool> has_expected;
	std::vector<double> expected;
	reading this_reading{ current.return_reading(0) };
	for (int j{}; j < this_reading.size(); j++) {//Loop over all measurements
		has_expected.push_back(false);
		expected.push_back(0.0);
		if (this_reading.return_measurement(j)->type() == "numerical_measurement") {
			std::cout << "Does the measurement " << this_reading.return_measurement(j)->name_and_units() << " have an expected value?" << std::endl;
			std::cout << "1.... Yes" << std::endl;
			std::cout << "2.... No" << std::endl;
			int option{ get_option(2) };
			if (option == 1) {
				has_expected[j] = true;
				std::cout << "Please enter the expected value:" << std::endl;
				expected[j] = get_value<double>();
			}
			std::vector<std::string> temp;
			temp.push_back(this_reading.return_measurement(j)->name_and_units());
			temp.push_back(weighted_mean(current, j));
			temp.push_back(weighted_error(current, j));
			temp.push_back(unweighted_mean(current, j));
			temp.push_back(standard_error(current, j));
			temp.push_back(standard_error_percent(current, j));
			temp.push_back(mode(current, j));
			temp.push_back(median(current, j));
			if (has_expected[j] == true) {
				double difference{ abs(expected[j] - stod(unweighted_mean(current, j))) / abs(expected[j]) };
				std::string return_string_1{ std::to_string((expected[j])) };
				return_string_1.erase(return_string_1.find_last_not_of('0') + 1, std::string::npos);
				std::string return_string_2{ std::to_string((difference)) };
				return_string_2.erase(return_string_2.find_last_not_of('0') + 1, std::string::npos);
				temp.push_back(return_string_1);
				temp.push_back(return_string_2 + " %");
			}
			else {
				temp.push_back("N/A");
				temp.push_back("N/A");
			}
			results.push_back(temp);
		}
	}
	return results;
}

//Base Functions
void search_measurement(int experiment_index, int dataset_index) 
{
	std::cout << "Please select a measurement to search through the values of:" << std::endl;
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); i++) {
		std::cout << i + 1 << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(i)->name_and_units() << std::endl;
	}
	int measurement_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size()) - 1 };
	std::vector<std::string> measurements;
	for (int i{}; i < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); i++) {
		measurements.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(i).get_measurement(measurement_index)->string_value());
	}
	//measurements now contains all the data to search through
	std::cout << "Enter the value you want to search for:" << std::endl;
	std::string search_term;
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	getline(std::cin, search_term);
	std::vector<int> indexes;
	int index{};
	auto iterator = std::find_if(measurements.begin(), measurements.end(), [search_term,&indexes,&index](std::string value) {
		if (value == search_term) {
			indexes.push_back(index);
			index += 1;
			return true;
		}
		index += 1;
		return false;
	});
	for (size_t i{}; i < indexes.size(); i++) {
		std::cout << "Value found at measurement " << indexes[i] + 1 << std::endl;
	}
	if (indexes.size() == 0) {
		std::cout << "No values found." << std::endl;
	}
}
void simple_analysis(int experiment_index, int dataset_index) 
{
	//Simple analysis on numerical measurements
	//Mean, weighted mean, standard deviation, mode, median, comparison with expected, percentages
	//On text measurements give the most repeated word, and the least
	//On boolean measurements, give % true % false
	//Analyses whole dataset
	//Initialise the tables
	bprinter::TablePrinter numerical_table(&std::cout);
	bprinter::TablePrinter text_table(&std::cout);
	bprinter::TablePrinter boolean_table(&std::cout);
	numerical_table.AddColumn("Measurement", 25);//Need to determine the length by looping over
	numerical_table.AddColumn("Weighted Mean", 14);
	numerical_table.AddColumn("Weighted Error", 15);
	numerical_table.AddColumn("Unweighted Mean", 20);
	numerical_table.AddColumn("Standard Error", 20);
	numerical_table.AddColumn("Standard Error (%)", 20);
	numerical_table.AddColumn("Mode", 10);
	numerical_table.AddColumn("Median", 10);
	numerical_table.AddColumn("Expected Value", 15);
	numerical_table.AddColumn("Difference from Expected", 30);
	boolean_table.AddColumn("Measurement", 30);
	boolean_table.AddColumn("True (Number, %)",15);
	boolean_table.AddColumn("False (Number, %)",15);
	//Now print tables
	std::vector<std::vector<std::string>> numerical_results{ simple_numerical_analysis_results(experiment_index,dataset_index) };
	std::vector<std::vector<std::string>> boolean_results{ simple_boolean_analysis_results(experiment_index,dataset_index) };
	if (numerical_results.size() > 0) {
		std::cout << "The numerical results are: " << std::endl;
		numerical_table.PrintHeader();
		for (size_t i{}; i < numerical_results.size(); i++) {
			for (size_t j{}; j < numerical_results[i].size(); j++) {
				numerical_table << numerical_results[i][j];
			}
		}
		numerical_table.PrintFooter();
	}
	else {
		std::cout << "No numerical measurements." << std::endl;
	}
	if (boolean_results.size() > 0) {
		std::cout << "\n\nThe boolean results are:" << std::endl;
		boolean_table.PrintHeader();
		for (size_t i{}; i < boolean_results.size(); i++) {
			for (size_t j{}; j < boolean_results[i].size(); j++) {
				boolean_table << boolean_results[i][j];
			}
		}
		boolean_table.PrintFooter();
	}
	else {
		std::cout << "No boolean measurements." << std::endl;
	}
	std::cout << "\n\nFinished simple analysis.";
}
void analyse_experiment(int experiment_index) 
{
	//Base function called from main menu
	std::cout << "Please select a dataset for analysis: " << std::endl;
	int dataset_index{ experiment_list[experiment_index].select_dataset() };
	std::cout << "Please select an option from the following list: " << std::endl;
	std::cout << "1.... Simple Analysis" << std::endl;
	std::cout << "	~Mean, standard deviation, comparison to expected value, etc for numerical data" << std::endl;
	std::cout << "	~Comparison of true and false results for boolean data" << std::endl;
	std::cout << "2.... Polynomial Fitting" << std::endl;
	std::cout << "	~Attempted fitting to an n-th degree polynomial, including 0 degrees, through matrix methods or iterative fitting" << std::endl;
	std::cout << "3.... Search for a value in the dataset" << std::endl;
	int option{ get_option(3) };
	if (option == 1) {
		//Simple analysis is performed on whole dataset rather than individual measurements since it is so quick and easy
		simple_analysis(experiment_index, dataset_index);
	}
	else if (option == 2) {
		//Complex analysis
		std::cout << "Polynomial fitting can be achieved through two methods, please select an option:" << std::endl;
		std::cout << "1.... Matrix methods" << std::endl;
		std::cout << "	~Ignores errors when fitting, works very quickly for any n-th degree polynomial." << std::endl;
		std::cout << "2.... Chi-Squared Minimisation" << std::endl;
		std::cout << "	~Attempted fitting to an n-th degree polynomial, including 0 degrees, through iterative fitting by minimising the chi-squared. Slower, depending on input parameters." << std::endl;
		int option{ get_option(2) };
		if (option == 1) {
			//Do the polyfit
			std::cout << "Please select the measurement to use for the x_data; this will have no errors: " << std::endl;
			for (int j{ 1 }; j <= experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); j++) {
				if (experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->standard_err() == true || experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->systematic() == true) {
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << " WARNING: This measurement has an associated error that will NOT be analysed" << std::endl;
				}
				else {
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << std::endl;
				}
			}
			int x_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size()) - 1 };
			std::cout << "Please select the measurement to use for the y_data; this will have errors: " << std::endl;
			std::vector<int> bad_y_sets;//Vector to store indexes of unsuitable measurements for the y data
			for (int j{ 1 }; j <= experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); j++) {
				if (j == x_index + 1) {//Check for duplicate x data
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << "WARNING: This data has already been selected for the x-data, no useful relation will be found." << std::endl;
					bad_y_sets.push_back(j - 1);
				}
				else {
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << std::endl;
				}
			}
			int y_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size()) - 1 };
			//Now check that bad y-data wasn't selected
			for (size_t i{}; i < bad_y_sets.size(); i++) {
				if (bad_y_sets[i] == y_index) {
					std::cout << "ERROR: Unsuitable data selected for the y-data. Terminating fitting procedure." << std::endl;
					main_menu();
				}
			}
			std::vector<double> x_data;
			std::vector<double> y_data;
			for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {
				x_data.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).get_measurement(x_index)->num_value());
				y_data.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).get_measurement(y_index)->num_value());
			}
			std::cout << "What degree polynomial would you like to fit the data to? Enter 0 for a straight line, 1 for (y=ax+b), etc" << std::endl;
			int degree{ get_value<int>() };
			std::vector<double> polyfit_coefficients{ polynomial_fitting(x_data,y_data,degree) };
		}
		else if (option == 2) {
			//Do the iterative fit
			std::cout << "Please select the measurement to use for the x_data; this will have no errors: " << std::endl;
			for (int j{ 1 }; j <= experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); j++) {
				if (experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->standard_err() == true || experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->systematic() == true) {
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << " WARNING: This measurement has an associated error that will NOT be analysed" << std::endl;
				}
				else {
					std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << std::endl;
				}
			}
			int x_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size()) - 1 };
			std::cout << "Please select the measurement to use for the y_data; this will have errors: " << std::endl;
			std::vector<int> bad_y_sets;//Vector to store indexes of unsuitable measurements for the y data
			for (int j{ 1 }; j <= experiment_list[experiment_index].return_dataset(dataset_index).return_template().size(); j++) {
				if (j == x_index + 1) {//Check for duplicate x data
					if (experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->standard_err() == false && experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->systematic() == false) {
						std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << "WARNING: This data has no errors, do not select for analysis as the y-data." << std::endl;
						bad_y_sets.push_back(j - 1);

					}
					else {
						std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << "WARNING: This data has already been selected for the x-data, no useful relation will be found." << std::endl;
					}
				}
				else {
					if (experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->standard_err() == false && experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->systematic() == false) {
						std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << "WARNING: This data has no errors, do not select for analysis as the y-data." << std::endl;
						bad_y_sets.push_back(j - 1);
					}
					else {
						std::cout << j << ".... " << experiment_list[experiment_index].return_dataset(dataset_index).return_template().get_measurement(j - 1)->name_and_units() << std::endl;

					}
				}
			}
			int y_index{ get_option(experiment_list[experiment_index].return_dataset(dataset_index).return_template().size()) - 1 };
			//Now check that bad y-data wasn't selected
			for (size_t i{}; i < bad_y_sets.size(); i++) {
				if (bad_y_sets[i] == y_index) {
					std::cout << "ERROR: Unsuitable data selected for the y-data. Terminating fitting procedure." << std::endl;
					main_menu();
				}
			}
			std::vector<double> x_data;
			std::vector<double> y_data;
			std::vector<double> y_errors;
			for (int j{}; j < experiment_list[experiment_index].return_dataset(dataset_index).number_readings(); j++) {
				x_data.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).get_measurement(x_index)->num_value());
				y_data.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).get_measurement(y_index)->num_value());
				y_errors.push_back(experiment_list[experiment_index].return_dataset(dataset_index).return_reading(j).get_measurement(y_index)->num_error());
			}
			std::cout << "What degree polynomial would you like to fit the data to? Enter 0 for a straight line, 1 for (y=ax+b), etc" << std::endl;
			int degree{ get_value<int>() };
			std::cout << "What step size would you like to use for the fitting? A smaller step will take longer, but is more accurate. 0.1 is recommended as an initial attempt." << std::endl;//Give recommendation
			double step{ get_value<double>() };
			chi_squared_polynomial(x_data, y_data, y_errors, degree, step);
		}
	}
	else {
		search_measurement(experiment_index, dataset_index);
	}
}
void chi_squared_polynomial(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, double step) 
{
	//Minimises the chi squared
	srand((unsigned)time(0));//Seed the rand
	double high_limit{2};
	double low_limit{-2};
	std::cout << "Polynomial of degree " << degree << " will be fitted to the data." << std::endl;
	std::cout << "Running basic polynomial fit to approximate initial values for iterative fitting..." << std::endl;
	std::vector<double> polyfit_coefficients{ polynomial_fitting(x_data,y_data,degree) };
	std::vector<double> coefficients{ polyfit_coefficients };
	std::vector<double> best_coefficients{ coefficients };
	std::cout << "\nInitial values assigned..." << std::endl;
	double best_chi_squared{ chi_squared_cost(x_data,y_data,y_errors,degree,best_coefficients,step) };//Initialises a best value to compare others to
	double chi_squared{};
	bool finished{ false };
	int bad_attempts{};
	while (finished == false) {
		bool optimised{ false };
		double last_val{ chi_squared_cost(x_data,y_data,y_errors,degree,coefficients,step) };
		while (optimised==false){
			for (size_t i{}; i < coefficients.size(); i++) {
				coefficients = optimise_coefficients(x_data, y_data, y_errors, degree, coefficients, i, step);
			}
			if (chi_squared_cost(x_data, y_data, y_errors, degree, coefficients, step) == last_val) {
				optimised = true;
			}
			last_val = chi_squared_cost(x_data, y_data, y_errors, degree, coefficients, step);
		}
		//Finished this attempt at optimisation
		//Is it better?
		chi_squared = (chi_squared_cost(x_data, y_data, y_errors, degree, coefficients, step));
		std::cout << "Fitting still running... Approximately " << bad_attempts * 10 << "% complete..." << std::endl;
		if(chi_squared < best_chi_squared) {
			best_chi_squared = chi_squared;
			best_coefficients = coefficients;
		}
		else {
			bad_attempts += 1;
		}
		//Reset the coefficients to a new set of values
		for (int k{};k <= degree; k++) {
			coefficients[k]=polyfit_coefficients[k]+(low_limit + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (high_limit - low_limit))));
		}
		if (bad_attempts == 10) {
			finished = true;
		}
	}
	std::cout << "\n\n\nIterative Fitting complete." << std::endl;
	std::cout << "The polynomial fit is: " << std::endl << "y = ";
	for (size_t i{}; i < best_coefficients.size(); i++) {
		if (i != 0) { std::cout << " + "; };
		std::cout << best_coefficients[i] << "*x^" << i;
	}
	std::cout << "\nThe chi-squared is " << best_chi_squared << std::endl;
	std::cout << "The reduced chi-squared is " << best_chi_squared / (y_data.size() - static_cast<double>(degree) - 1) << std::endl;
	if (best_chi_squared / (y_data.size() - static_cast<double>(degree) - 1) < 0.5) {
		std::cout << "Fit appears to be too accurate. Errors may have been overestimated, please review the data." << std::endl;
	}
	else if (best_chi_squared / (y_data.size() - static_cast<double>(degree) - 1) > 2) {
		std::cout << "Fit appears to be inaccurate. Errors may have been underestimated, please review the data." << std::endl;

	}
	else {
		std::cout << "This is a good chi-squared value!" << std::endl;
	}
	if (x_data.size() < 5) {
		std::cout << "Warning: Fitting using a chi-squared method can be inaccurate with so few data points." << std::endl;
	}
}