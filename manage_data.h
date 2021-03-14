#ifndef manage_h
#define manage_h
void manage_experiment(int experiment_index);
void manage_dataset(int experiment_index, int dataset_index);
void manage_measurement(int experiment_index, int dataset_index, int reading_index, int measurement_index);
void manage_readings(int experiment_index, int dataset_index);
#endif