#include "BinarySearchTree.h"
#include "csvstream.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <map> //using standard library's map, not Map.h

using namespace std;

class Classifier
{
private:
    int num_posts;
    int num_unique_words_in_training_set;
    map<string, int> posts_containing_word; //word as key, num posts as value
    map<string, int> posts_containing_label; //label as key, num posts as value
    map<string, map<string, int>> posts_containing_label_and_word;
    set<string> unique_labels_in_training_set;
    set<string> unique_words_in_training_set;

    // EFFECTS: Return a set of unique whitespace delimited words.
    //this function is given in spec, do not modify
    set<string> unique_words(const string &str) 
    {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word) {
            words.insert(word);
        }
        return words;
    }

    set<string> unique_labels(csvstream &csvin) 
    {
        string label;
        map<string, string> row;
        set<string> labels;
        while (csvin >> row)
        {
            label = row["tag"];
            labels.insert(label);
        }
        return labels;
    }

    double log_prior(const string &label) {
        return log((1.0 * posts_containing_label[label])/num_posts);
    }

    double log_likelihood(string label) {
        return log((1.0 * posts_containing_label[label])/num_posts);
    }

public:
//the public member functions should provide an interface that allows you to 
//train the classifier and make predictions for new piazza posts.

    // need to customize output if --debug arguement is included @emilywal
    void print_training_data(string &filename, bool debug)
    {

        csvstream csvin(filename);

        string label;
        string content;
        int num_lines = 0;

        map<string, string> junk;
        csvin >> junk;

        if (debug)
            cout << "training data:" << endl;

        map<string, string> row;
        while (csvin >> row)
        {
            if (debug) {
                label = row["tag"];
                content = row["content"];
                cout << "  label = " << label << ", " << "content = " << content << endl;
            }

            num_lines++;
        }

        cout << "trained on " << num_lines << " examples" << endl;
    }

    //EFFECTS: reading in file, updating all private member variables accordingly
    void train_classifier(const string &filename, bool debug)
    {
        bool contains_word;
        bool contains_label;
        string line;
        string some_word;

        num_posts = 0;

        csvstream csvin1(filename);
        csvstream csvin2(filename);

        unique_labels_in_training_set = unique_labels(csvin1);

        map<string, string> junk;
        csvin2 >> junk;

        map<string, string> row;

        while (csvin2 >> row)
        {
            string content = row["content"]; //takes content of a row
            set<string> new_unique_words = unique_words(content); //finds the unique words in content of this row

            set<string>::iterator it;
            for (it = new_unique_words.begin(); it != new_unique_words.end(); it++) {
                unique_words_in_training_set.insert(*it); //adds previously found unique words to unique_words_in_training_set member variable
            }

            // using range-based for loop to iterate through labels, ...
            //checking whether each label is the label for the the current posting, then adding to count if necessary
            set<string>::iterator itt;
            for (itt = unique_labels_in_training_set.begin(); itt != unique_labels_in_training_set.end(); ++itt)
            {
                if (*itt == (row["tag"]))
                {
                    contains_label = true;
                    posts_containing_label[*itt]+=1; //will [] operator cause any issues with const? -- i dont think so since just the filename is const right?
                }

                // using traversal by pointer to iterate through unique_words_in_training_set, ...
                //checking whether each word is present in the current posting, then adding to count if necessary
                set<string>::iterator it2;
                for (it2 = unique_words_in_training_set.begin(); it2 != unique_words_in_training_set.end(); it2++) 
                {
                    if (*it2 == (row["content"]))
                    {
                        contains_word = true;
                        posts_containing_word[*it2]+=1;

                        //adding a count to posts_containing_label_and_word if both are true
                        if(contains_label && contains_word) {
                            posts_containing_label_and_word[*itt][*it2]+=1;
                        }
                    }
                }
            }
            num_posts++;
        }
        if (debug) {
            num_unique_words_in_training_set = unique_words_in_training_set.size();
            cout << "vocabulary size = " << num_unique_words_in_training_set << endl;
        }

        cout << endl;

        if (debug) {
            cout << "classes:" << endl;
            set<string>::iterator itt;
            for (itt = unique_labels_in_training_set.begin(); itt != unique_labels_in_training_set.end(); ++itt)
            {
                cout << "  " << *itt << ", " << posts_containing_label[*itt] << " examples, log-prior = " << log_prior(*itt) << endl;
            }

            cout << "classifier parameters:" << endl;
            for (itt = unique_labels_in_training_set.begin(); itt != unique_labels_in_training_set.end(); ++itt)
            {
                set<string>::iterator it;
                for (it = unique_words_in_training_set.begin(); it != unique_words_in_training_set.end(); it++) 
                {
                    cout << *itt << ":" << *it << ", count = " << posts_containing_label_and_word[*itt][*it] << ", log-likelihood = " 
                    << log(posts_containing_label_and_word[*itt][*it]/posts_containing_label[*itt]) << endl;
                }
            }

            cout << endl;
        }
    }     

    //write predict function, use private member variables and calculation from spec
    pair<string,double> predict_label(const string &post)
    {

        double max_likeliness = -2147483648; //set this to be likeliness of first label
        string most_likely_label = *(unique_labels_in_training_set.begin());
        string word;
        set<string> unique_words_in_post = unique_words(post);

        set<string>::iterator itt;
        for (itt = unique_labels_in_training_set.begin(); itt != unique_labels_in_training_set.end(); ++itt)
        {
            double likeliness = 0;
            string something = *itt;
            likeliness += log_prior(something); //BUG: likeliness set to -infinity
            
            set<string>::iterator it;
            for (it = unique_words_in_post.begin(); it != unique_words_in_post.end(); it++) 
            {
                if (posts_containing_label_and_word[*itt][*it] != 0) //word is found in post with this label
                    likeliness += log((1.0 * posts_containing_label_and_word[*itt][*it])/posts_containing_label[*itt]);
                else if ((posts_containing_label_and_word[*itt][*it] == 0 && posts_containing_word[*it] != 0) 
                && (unique_words_in_training_set.find(*it) != unique_words_in_training_set.end())) //word occurs in training data but not in posts with this label
                    likeliness += log((1.0 * posts_containing_word[*it])/num_posts);
                else //word does not occur anywhere in training set
                    likeliness += log(1.0/num_posts);
            }

            if (likeliness > max_likeliness || 
                (likeliness == max_likeliness && *itt < most_likely_label))
            {
                max_likeliness = likeliness;
                most_likely_label = *itt;
            }
        
            likeliness = 0;
        }

        return {most_likely_label, max_likeliness};;
    }
};

int main(int argc, char **argv)
{
    cout.precision(3); //told to do this in spec

    //1: check number of arguments
    bool cond1 = (argc == 3 || argc ==4);
    bool cond2 = (argc == 3 || !strcmp(argv[3], "--debug"));

    if (!(cond1 && cond2))
    {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }

    //2: read posts from a file and use them to train the classifier
    //argv[1] is train_file

    string train_file = argv[1];
    Classifier classifier;

    //call train_classifier to update private member variables in Classifier class
    classifier.train_classifier(train_file, (argc == 4) && !(strcmp(*(argv+argc-1), "--debug")));

    /*print output either as you're training or write separate function for it
    remember that there is slightly different output with --debug
    */

    classifier.print_training_data(train_file, (argc == 4) && !(strcmp(*(argv+argc), "--debug"))); 
    
    //3: classifier should compute log-probability score of a post given a particular label
    //use training data to predict labels using probability function and member variables

    //4: read posts from a file to use as testing data
    //argv[2] is test_file

    string test_file = argv[2];
    csvstream csvin(test_file);

    cout << "test data:" << endl;

    int num_correct = 0;
    int num_test_posts = 0;
    map<string, string> row;
    while (csvin >> row)
    {
        string correct_label = row["tag"]; //somehow, this is the content? OH
        string content = row["content"]; 

        pair<string, double> pred_label= classifier.predict_label(content);
        string predicted_label = pred_label.first;
        double log_prob = pred_label.second;

        cout << "  correct = " << correct_label << ", predicted = " << predicted_label << ", log-probability score = " << log_prob << endl;
        cout << "  content = " << content << endl << endl;

        num_test_posts++;

        if(correct_label == predicted_label) {
            num_correct++;
        }
    }

    cout << "performance: " << num_correct << "/" << num_test_posts << " posts predicted correctly" << endl;
    cout << endl;

    return 0;
}
