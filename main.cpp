#include "csvstream.hpp"
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <set>

using namespace std;

class Data
{
private:
    int post_count;
    set<string> unique_words;
    map<pair<string, string>, double> post_label_word_frequency;
    map<string, double> post_label_frequency;
    map<string, double> post_word_frequency;

public:
    // constructor
    Data(const vector<pair<string, set<string>>> &data_vector)
    : post_count(data_vector.size())
    {
        set_unique_words(data_vector);
        count_label_word_frequency(data_vector);
    }

    // data mutators
    void set_unique_words(const vector<pair<string, set<string>>> &data_vector)
    {
        for (const auto &item : data_vector)
        {
            for (const auto &word : item.second)
            {
                unique_words.insert(word);
            }
        }
    }

    void count_label_word_frequency(const vector<pair<string, set<string>>> &data_vector)
    {
        for (const auto &post : data_vector) {
            post_label_frequency[post.first] += 1;

            for (const auto &word : post.second) {
                post_label_word_frequency[pair(post.first, word)] += 1;
                post_word_frequency[word] += 1;
            }
        }
    }

    // accessors
    int get_post_count() const
    {
        return post_count;
    }

    set<string> & get_unique_words()
    {
        return unique_words;
    }

    map<pair<string, string>, double> & get_label_word_frequency()
    {
        return post_label_word_frequency;
    }

    map<string, double> & get_label_frequency()
    {
        return post_label_frequency;
    }

    map<string, double> & get_word_frequency()
    {
        return post_word_frequency;
    }
};

class Classifier {
private:
    vector<pair<string, string>> unadjusted_vector;
    vector<pair<string, set<string>>> adjusted_vector;

    // probability vars
    double current_log_probability;
    double max_log_probability;
    string predicated_label;
    int correct_predictions;

public:
    // constructor
    Classifier (const vector<pair<string, string>> &data_vector, 
                const vector<pair<string, string>> &test_data_vector, int mode) 
    : unadjusted_vector(data_vector), current_log_probability(0), 
      max_log_probability(log(0)), correct_predictions(0)
    {
        adjust_vector(data_vector);
        Data *data = new Data(adjusted_vector);

        if (mode == 4) {
            print_training_data();
            print_training_post_count(data);
            print_vocab_size(data);
            print_classes(data);
            print_classifier_params(data);
        }
        else if (mode == 3) {
            print_training_post_count(data);
        }
        cout << "\ntest data:" << endl;

        for (const auto &post_X : test_data_vector) {
            max_log_probability = log(0);
            for (const auto &C : data->get_label_frequency()) {
                Calculate_C_Prob(data, C.first, get_parsed_string(post_X.second));
                if (current_log_probability > max_log_probability) {
                    max_log_probability = current_log_probability;
                    predicated_label = C.first;
                }
            }
            if (predicated_label == post_X.first) {
                correct_predictions += 1;
            }
            cout << "  correct = " << post_X.first << ", predicted = " 
                 << predicated_label << ", log-probability score = " 
                 << max_log_probability << endl; 
            cout << "  content = " << post_X.second << "\n" << endl;
        }
        cout << "performance: " << correct_predictions << " / " 
             << test_data_vector.size()
             << " posts predicted correctly" << endl;

        delete data;
    }

    // probability functions
    void Calculate_C_Prob(Data *data, const string &label, const set<string> &content) {
        current_log_probability = get_log_prior(data, label);
        for (const string &word : content) {
            current_log_probability += get_log_likelihood(data, label, word);
        }
    }

    double get_log_likelihood(Data *data, const string &label, const string &word) {
        if (data->get_word_frequency().find(word) == data->get_word_frequency().end()) {
            return log(1.0/data->get_post_count());
        }
        else if (data->get_label_word_frequency().find(pair(label, word)) 
                 == data->get_label_word_frequency().end()) {
            return log(data->get_word_frequency()[word]/data->get_post_count());
        }
        else {
            return log(data->get_label_word_frequency()[pair(label, word)]
                   / data->get_label_frequency()[label]);
        }
    }

    double get_log_prior(Data *data, string label) {
        return log(data->get_label_frequency()[label]/data->get_post_count());
    }

    // helper data functions
    void adjust_vector(const vector<pair<string, string>> &vec) {
        set<string> words_in_content;

        for (const auto &post : vec) {
            words_in_content.clear();

            string tag = post.first;
            string content = post.second;
            istringstream source(content);
            string word;

            while (source >> word) {
                words_in_content.insert(word);
            }

            pair<string, set<string>> value(tag, words_in_content);
            adjusted_vector.push_back(pair(tag, words_in_content));
        }
    }

    // EFFECTS: Return a set of unique whitespace delimited words.x
    set<string> get_parsed_string(const string &str) {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        return words;
    }

    void print_training_data() {
        cout << "training data:" << endl;
        for (const auto &post : unadjusted_vector) {
            cout << "  label = " << post.first << ", content = " << post.second << endl;
        }
    }

    void print_training_post_count(Data *data) {
        cout << "trained on " << data->get_post_count() << " examples" << endl;
    }

    void print_vocab_size(Data *data) {
        cout << "vocabulary size = " << data->get_unique_words().size() << "\n" << endl;
    }

    void print_classes(Data *data) {
        cout << "classes:" << endl;
        for (const auto &label : data->get_label_frequency())
        cout << "  " << label.first << ", " << label.second 
             << " examples, log-prior = " << get_log_prior(data, label.first) << endl;
    }

    void print_classifier_params(Data *data) {
        cout << "classifier parameters:" << endl;
        for (const auto &lw : data->get_label_word_frequency()) {
            cout << "  " <<lw.first.first << ":" << lw.first.second
                 << ", count = " << lw.second << ", log-likelihood = "
                 << get_log_likelihood(data, lw.first.first, lw.first.second) << endl;
        }
    }
};


int main(int argc, char * argv[]) {
    cout.precision(3);
    string debug = "--debug";

    if (argc == 4) {
        if (argv[3] != debug) {
            cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
            return -1;
        }
    }

    if (argc != 3 && argc != 4) { //checks for correct # of arguments
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
    }

    string train_file_name = argv[1];
    string test_file_name = argv[2];

    vector<pair<string, string>> train_rows;
    vector<pair<string, string>> test_rows;
    
    try {
        csvstream train_csv(train_file_name);
        map<string, string> train_row;
        while (train_csv >> train_row) {
            //cout << row["tag"] << ": " << row["content"] << endl;
            pair<string, string> new_row(train_row["tag"], train_row["content"]);
            train_rows.push_back(new_row);
        }
    } 
    catch (const csvstream_exception &e) {
        cout << "Error opening file: " << train_file_name << endl;
        return -1;
    }

    try {
        csvstream test_csv(test_file_name);
        map<string, string> test_row;
        while (test_csv >> test_row) {
            pair<string, string> new_row(test_row["tag"], test_row["content"]);
            test_rows.push_back(new_row);
        }
    } 
    catch (const csvstream_exception &e) {
        cout << "Error opening file: " << test_file_name << endl;
        return -1;
    }

    Classifier classy(train_rows, test_rows, argc);
}