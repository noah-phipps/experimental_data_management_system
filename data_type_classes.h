//Simple return functions are defined and declared at the same time for brevity
#include"standard_library_includes.h"
#ifndef experiment_h
#define experiment_h

//Classes
//Abstract base measurement class, with all of it's derived classes
class measurement
{
public:
	virtual ~measurement() {};
	//Functions to return info on the measurement
	virtual std::string type()const=0;
	virtual std::string name()const=0;
	virtual std::string name_and_units()const = 0;
	virtual std::string units()const=0;
	//Functions only relevant to numerical measurements
	//Numerical, info about repeats
	virtual int repeats()const=0;//Returns number of repeats
	//Numerical, info about whether or not has a certain type of error
	virtual bool systematic()const=0;
	virtual std::string print_systematic_state() const = 0;
	virtual bool standard_err()const = 0;
	virtual std::string print_error_state() const = 0;
	//Numerical, returns an actual value
	virtual double num_value() = 0;
	virtual double num_error() = 0;
	virtual std::string error_string_value() const= 0;
	virtual std::string systematic_string_value() const = 0;
	//Numerical, allows editing
	virtual void edit_error() = 0;
	virtual void edit_systematic() = 0;
	//General functions for getting/editing a measurement value
	virtual std::string string_value() const = 0;
	virtual void edit_value() = 0;
	virtual std::shared_ptr<measurement> clone() const = 0;//Deep copy cloning function
};

class text_measurement : public measurement
{
private:
	std::string measurement_name;
	std::string measurement_value;
	std::string measurement_units;
public:
	text_measurement(std::string name, std::string units);
	text_measurement(std::string value, std::shared_ptr<measurement> template_to_use);
	text_measurement(const text_measurement& to_copy);
	//Derived functions for basic info
	std::string type() const {return "text_measurement";};
	std::string name() const {return measurement_name;};
	std::string name_and_units() const { return (measurement_name + " (" + measurement_units + ")"); };
	std::string units() const { return measurement_units; };
	//Derived functions to get/edit the vaue
	std::string string_value() const { return measurement_value; };
	std::shared_ptr<measurement> clone() const { return std::make_shared<text_measurement>(*this); };
	void edit_value();
	//Derived functions without a good return or method, needed for numerical
	int repeats()const { return 0; };
	bool systematic() const { return false; };
	std::string print_systematic_state() const { return"ERROR, incorrect measurement type."; };
	bool standard_err() const { return false; };
	std::string print_error_state() const { return"ERROR, incorrect measurement type."; };
	double num_value() { return 0.0; };
	double num_error() { return 0.0; };
	std::string error_string_value() const { return "ERROR, incorrect measurement type."; };
	std::string systematic_string_value()const { return "ERROR, incorrect measurement type."; };
	void edit_error() { std::cout << "ERROR, incorrect measurement type." << std::endl; };
	void edit_systematic() { std::cout << "ERROR, incorrect measurement type." << std::endl; };
};

class text_comment : public measurement 
{
private:
	std::string comment_title;
	std::string comment;
public:
	text_comment(const text_comment& to_copy);
	text_comment(std::string to_set);
	text_comment(std::string new_comment, std::shared_ptr<measurement> template_to_use);
	//Derived functions for basic info
	std::string type()const { return "text_comment"; };
	std::string name()const { return comment_title; };
	std::string name_and_units() const { return comment_title; };
	std::string units() const { return "ERROR, incorrect measurement type."; };
	//Derived functions to get/edit value
	std::string string_value() const { return comment; };
	std::shared_ptr<measurement> clone() const { return std::make_shared<text_comment>(*this); };
	void edit_value();
	//Non-derived function, just to set the comment
	void set_comment(std::string to_set) {comment = to_set;};
	//Derived functions with no good return or method, needed for numerical
	int repeats() const { return 0; };
	bool systematic()const { return false; };
	std::string print_systematic_state() const { return"ERROR, incorrect measurement type."; };
	bool standard_err() const { return false; };
	std::string print_error_state() const { return"ERROR, incorrect measurement type."; };
	double num_value() { return 0.0; };
	double num_error() { return 0.0; };
	std::string error_string_value()const { return "ERROR, incorrect measurement type."; };
	std::string systematic_string_value()const { return "ERROR, incorrect measurement type."; };
	void edit_error() { std::cout << "ERROR, incorrect measurement type." << std::endl; };
	void edit_systematic() { std::cout << "ERROR, incorrect measurement type." << std::endl; };
};

class boolean_measurement : public measurement
{
private:
	std::string measurement_name;
	bool measurement_value;
public:
	boolean_measurement(const boolean_measurement& to_copy);
	boolean_measurement(std::string name);
	boolean_measurement(bool value, std::shared_ptr<measurement> template_to_use);
	boolean_measurement(std::string value, std::shared_ptr<measurement> template_to_use);
	//Derived functions to return basic info
	std::string type()const {return "boolean_measurement";};
	std::string name()const {return measurement_name;};
	std::string name_and_units() const { return measurement_name; };
	std::string units() const { return ""; };
	//Derived functions to get/edit data
	std::shared_ptr<measurement> clone() const { return std::make_shared<boolean_measurement>(*this); };
	void edit_value();
	std::string string_value()const;
	//Non-derived functions specific to boolean
	void set_boolean(bool to_set) {measurement_value = to_set;};
	bool value() const { return measurement_value; };
	//Derived functions with no good return/method, needed for numerical
	int repeats() const { return 0; };
	bool systematic()const { return false; };
	std::string print_systematic_state() const { return""; };
	bool standard_err()const { return false; };
	std::string print_error_state() const { return""; };
	double num_value() { return 0.0; };
	double num_error() { return 0.0; };
	std::string error_string_value() const { return ""; };
	std::string systematic_string_value() const { return ""; };
	void edit_error() {};
	void edit_systematic() {};
};

class numerical_measurement : public measurement
{
private:
	std::string measurement_name;
	std::string measurement_units;
	bool systematic_error;
	bool standard_error;
	int number_repeats;
	double measurement_value;
	double measurement_error;
	double systematic_value;
	std::string measurement_type;
	std::vector<double> repeat_measurements;
public:
	numerical_measurement(const numerical_measurement& to_copy);
	numerical_measurement(std::string name, std::string units, int systematic, bool error_y_or_n);
	numerical_measurement(std::string name, std::string units, int systematic, int repeats);
	numerical_measurement(std::string name, std::string units, std::string error, std::string systematic);
	numerical_measurement(double value, double value_error, double systematic_error, std::shared_ptr<measurement> template_to_use);
	numerical_measurement(std::vector<double> values, double systematic_error_in, std::shared_ptr<measurement> template_to_use);
	//Derived functions to return basic info
	std::string type() const {	return "numerical_measurement";};
	std::string name() const {	return measurement_name;};
	std::string units() const {	return measurement_units;};
	std::string name_and_units() const { return (measurement_name + " (" + measurement_units + ")"); };
	//Derived functions specific to numerical type
	int repeats() const {return number_repeats;};
	bool systematic() const {return systematic_error;};
	std::string print_systematic_state() const { if (systematic_error == true) { return "true"; } else { return "false"; } }
	bool standard_err()const {return standard_error;};
	std::string print_error_state() const { if (standard_error == true) { return "true"; } else { return "false"; } }
	double num_value() { return measurement_value; };
	double num_error() { return measurement_error + systematic_value; };//Need to combine systematic and individual error
	std::string error_string_value() const;
	std::string systematic_string_value() const;
	//Derived functions to allow editing of values
	void edit_value();
	void edit_error();
	void edit_systematic();
	//Derived functions to edit/get
	std::shared_ptr<measurement> clone() const { return std::make_shared<numerical_measurement>(*this); };
	std::string string_value() const;
	//Non-derived functions
	std::vector<double> return_repeats() const { return repeat_measurements; };
	double value() const { return measurement_value; };
	double error_value() const { return measurement_error; };
	double systematic_error_value() const { return systematic_value; };
};

class reading
{//Reading class; a reading contains mulitple measurements. Reading template is used to create new readings
private:
	std::vector<std::shared_ptr<measurement>> reading_measurements;
	time_t reading_time{};//Timestamp for the reading
	bool has_time{};//Can be set to false if a reading is inputted that is not new, such as from an old lab book
	bool good_reading{ true };//Will be false if there was an error in reading
public:
	reading() {};
	reading(bool is_good) { good_reading = is_good; };
	reading(const reading& to_copy);
	reading(std::string time, std::string string_has_time, std::string string_good);
	//Functions to get info on the reading
	std::string print_has_time() { if (has_time == true) { return "true"; } else { return "false"; } }
	std::string print_good_reading() { if (good_reading == true) { return "true"; } else { return "false"; } }
	bool is_good() const { return good_reading; };
	int size()const {return reading_measurements.size();};
	std::shared_ptr<measurement> return_measurement(int index)const {return reading_measurements[index];};
	time_t return_time() const { return reading_time; };
	bool does_have_time() const { return has_time; };
	std::shared_ptr<measurement> get_measurement(int index) const { return reading_measurements[index]; };
	std::vector<std::shared_ptr<measurement>> return_all_measurements() const { return reading_measurements; };
	//Functions to add info to the reading
	void add_measurement(std::shared_ptr<measurement> new_measurement);
	//Functions to set/edit info of the reading
	void set_if_new(bool is_new);
	void set_time(time_t to_set);
};

class reading_template 
{
	//Used to construct readings, very similar to the reading class because of this
private:
	std::string name;
	std::vector<std::shared_ptr<measurement>> measurements;
public:
	reading_template(const reading_template& to_copy);
	reading_template() {};
	reading_template(std::string template_name);
	//Overloaded operators
	reading_template operator=(reading_template to_assign);
	//Functions to get info
	std::string return_name() const {return name;};
	int size() { return measurements.size(); };
	std::shared_ptr<measurement> get_measurement(int index) {return measurements[index];};
	std::vector<std::shared_ptr<measurement>> return_all_measurements() const { return measurements; };
	int size()const {return measurements.size();};
	//Functions to add to template
	void add_measurement(std::shared_ptr<measurement> measurement_to_add);
};

class dataset
{
private:
	std::string dataset_name;
	time_t dataset_date;
	bool dataset_state;
	std::string dataset_experimenter;
	std::vector<reading> dataset_readings;
	reading_template dataset_template;

public:
	dataset(const dataset& to_copy);
	dataset(const dataset& to_copy,std::string new_name);
	dataset(std::string name, std::string contributor);
	dataset(std::string name, std::string date, std::string state, std::string contributor);
	//Functions to get info on a dataset
	std::string name() const {return dataset_name;};
	std::string contributor() const {return dataset_experimenter;};
	time_t date() const {return dataset_date;};
	reading_template return_template() const {return dataset_template;};
	void print_info();
	int number_readings() {return dataset_readings.size();};
	reading& return_reading(int index) {return dataset_readings[index];};
	bool state() const { return dataset_state; };
	std::string string_state() { if (dataset_state == true) { return "true"; } else { return "false"; } }
	std::vector<reading> return_all_readings() const { return dataset_readings; };
	//Functions to add to or edit the dataset
	void set_name(std::string new_name) { dataset_name = new_name; };
	void add_reading(reading new_reading);
	void delete_reading(int reading_index);
	void set_state(bool new_state) { dataset_state = new_state; };
	void set_template(reading_template template_to_set);
};

class experiment
{//An experiment contains multiple datasets
private:
	std::vector<dataset> experiment_datasets;
	std::string experiment_name;
	std::vector<std::string> contributors;
public:
	experiment() {};
	experiment(const experiment& to_copy, std::string new_name);
	experiment(std::string name);
	//Overloaded operators
	experiment operator=(experiment to_assign_from);
	//Functions to get experiment info
	dataset& return_dataset(int index) {return experiment_datasets[index];};
	int number_datasets() {return experiment_datasets.size();};
	int number_contributors() {	return contributors.size();};
	std::string return_contributor(int index) {return contributors[index];};
	void print_name() {	std::cout << experiment_name;};
	const std::string name() const { return experiment_name; };
	bool datasets_exist();
	int select_dataset();
	std::vector<std::string> return_all_contributors() const { return contributors; };
	std::vector<dataset> return_all_sets() const { return experiment_datasets; };
	//Functions to add to/edit experimenter
	void delete_set(int dataset_index);
	void rename(std::string new_name) { experiment_name = new_name; };
	void add_contributor(std::string to_add) { contributors.push_back(to_add); };
	void add_dataset(dataset new_dataset);
	int create_dataset(bool is_new_data);
};
#endif