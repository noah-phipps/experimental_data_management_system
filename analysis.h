#include"data_type_classes.h"
#include"input_checks.h"
#include"standard_library_includes.h"
#include"global_lists.h"
#include"bprinter.h"
#include"bprinter.tpp.h"
#ifndef analysis_h
#define analysis_h

//Evaluation functions, evaluates a certain quantity and returns it
double polynomial_evaluator(double x_i, int degree, std::vector<double> coefficients);
double chi_squared_cost(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, std::vector<double> coefficients, double step);

//Fitting/Optimisation functions, take some data and fit/optimise it
std::vector<double> polynomial_fitting(std::vector<double> x_data, std::vector<double> y_data, int degree);
std::vector<double> optimise_coefficients(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, std::vector<double> coefficients, int coefficient_index, double step);

//Analysis functions, takes a set a performs analysis on it, return results
std::string unweighted_mean(dataset analysed_set, int measurement_index);
std::string weighted_mean(dataset analysed_set, int measurement_index);
std::string weighted_error(dataset analysed_set, int measurement_index);
std::string standard_error(dataset analysed_set, int measurement_index);
std::string standard_error_percent(dataset analysed_set, int measurement_index);
std::string mode(dataset analysed_set, int measurement_index);
std::string median(dataset analysed_set, int measurement_index);
std::string boolean_true_analysis(dataset analysed_set, int measurement_index);
std::string boolean_false_analysis(dataset analysed_set, int measurement_index);
std::vector<std::vector<std::string>> simple_boolean_analysis_results(int experiment_index, int dataset_index);
std::vector<std::vector<std::string>> simple_numerical_analysis_results(int experiment_index, int dataset_index);

//Base Functions, call the above functions to perform the analysis
void search_measurement(int experiment_index, int dataset_index);
void simple_analysis(int experiment_index, int dataset_index);
void analyse_experiment(int experiment_index);
void chi_squared_polynomial(std::vector<double> x_data, std::vector<double> y_data, std::vector<double> y_errors, int degree, double step);
#endif // !