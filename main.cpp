// Project UID db1f506d06d84ab787baf250c265e24e

#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <utility>
#include <math.h>
#include <vector>
#include <set>
#include "csvstream.h"

using namespace std;

class BagOfWordsModel
{

private:
    int numPosts;
    int uniqueWords;
    map<string, int> lexicon;
    map<string, int> wordsToPosts;
    map<string, int> labelToPosts;
    map<pair<string, string>, int> labelWordToPost;

public:
    BagOfWordsModel()
    {
        numPosts = 0;
        uniqueWords = 0;
    }

    int getNumPosts()
    {
        return numPosts;
    }

    map<string, int> getLabelToPosts()
    {
        return labelToPosts;
    }

    map<pair<string, string>, int> getlabelWordToPost()
    {
        return labelWordToPost;
    }

    int getNumUniqueWords()
    {
        return uniqueWords;
    }

    void incremenetPosts()
    {
        numPosts++;
    }

    void addWordsToLexicon(string post)
    {
        istringstream ss(post);
        string word;
        while (ss >> word)
        {
            if (lexicon.find(word) == lexicon.end())
            {
                pair<string, int> tuple;
                tuple.first = word;
                tuple.second = 1;
                lexicon.insert(tuple);
            }
            else
            {
                lexicon[word]++;
            }
        }

        uniqueWords = lexicon.size();
    }

    void wordFrequencyAmongPosts(string post)
    {
        istringstream ss(post);
        string word;
        map<string, int> temp;
        while (ss >> word)
        {
            if (temp.find(word) == temp.end())
            {
                pair<string, int> tuple;
                tuple.first = word;
                tuple.second = 1;
                temp.insert(tuple);
            }
        }

        for (auto &val : temp)
        {
            if (wordsToPosts.find(val.first) == wordsToPosts.end())
            {
                wordsToPosts.insert(val);
            }
            else
            {
                wordsToPosts[val.first]++;
            }
        }
    }

    void printWordsToPosts()
    {
        for (auto &val : wordsToPosts)
        {
            std::cout << val.first << ": " << val.second << endl;
        }
    }

    void LabelFrequencyAmongPosts(string label)
    {
        if (labelToPosts.find(label) == labelToPosts.end())
        {
            pair<string, int> tuple;
            tuple.first = label;
            tuple.second = 1;
            labelToPosts.insert(tuple);
        }
        else
        {
            labelToPosts[label]++;
        }
    }

    void printLabelToPosts()
    {
        for (auto &val : labelToPosts)
        {
            std::cout << val.first << ": " << val.second << endl;
        }
    }

    void LabelWordFrequencyAmongPosts(string label, string post)
    {
        istringstream ss(post);
        string word;
        set<pair<string, string>> wordSet;
        while (getline(ss, word, ' '))
        {
            pair<string, string> wordLabelPair;
            wordLabelPair.first = label;
            wordLabelPair.second = word;

            if (labelWordToPost.find(wordLabelPair) == labelWordToPost.end())
            {
                pair<pair<string, string>, int> tuple;
                tuple.first = wordLabelPair;
                tuple.second = 1;
                labelWordToPost.insert(tuple);
                wordSet.insert(wordLabelPair);
            }
            else
            {
                if (wordSet.find(wordLabelPair) == wordSet.end())
                {
                    labelWordToPost[wordLabelPair]++;
                    wordSet.insert(wordLabelPair);
                }
            }
        }
    }

    void printLabelWordToPosts()
    {
        for (auto &val : labelWordToPost)
        {
            std::cout << "<" << val.first.first << ", " << val.first.second << ">"
                      << ": " << val.second << endl;
        }
    }

    double logPrior(string label)
    {
        // FIGURE OUT WHY LOG ISNT WORKING
        double labeledPosts = labelToPosts[label];
        double ratio = double(labeledPosts) / numPosts;
        double val = log(ratio);
        return val;
    }

    double logLikelihood(string label, string word)
    {
        if (lexicon.find(word) == lexicon.end())
        {
            return log(double(1.0) / numPosts);
        }
        pair<string, string> wordLabel;
        wordLabel.first = label;
        wordLabel.second = word;
        if (labelWordToPost.find(wordLabel) == labelWordToPost.end())
        {
            return log(double(wordsToPosts[word]) / numPosts);
        }

        if (labelWordToPost.find(wordLabel) != labelWordToPost.end())
        {
            return log(double(labelWordToPost[wordLabel]) / labelToPosts[label]);
        }

        return 100;
    }

    string logProbability(string post, double &maxlog)
    {
        maxlog = -10000;
        string maxLabel;
        double currLog = 0;
        for (auto &label : labelToPosts)
        {
            set<string> words = unique_words(post);
            currLog += logPrior(label.first);
            for (auto word : words)
            {
                currLog += logLikelihood(label.first, word);
            }

            if (currLog > maxlog)
            {
                maxlog = currLog;
                maxLabel = label.first;
            }
            currLog = 0;
        }
        return maxLabel;
    }

    set<string> unique_words(const string &str)
    {
        istringstream source(str);
        set<string> words;
        string word;
        while (source >> word)
        {
            words.insert(word);
        }
        return words;
    }

    void callFunctionsInOrder(string label, string post)
    {
        this->incremenetPosts();
        this->addWordsToLexicon(post);
        this->wordFrequencyAmongPosts(post);
        this->LabelFrequencyAmongPosts(label);
        this->LabelWordFrequencyAmongPosts(label, post);
    }

    void printDebug()
    {
        std::cout << "trained on " << getNumPosts() << " examples" << endl;
        std::cout << "vocabulary size = " << getNumUniqueWords() << endl
                  << endl;

        map<string, int> labelsToPosts = getLabelToPosts();
        std::cout << "classes:" << endl;

        for (auto &label : labelsToPosts)
        {
            std::cout.precision(3);
            std::cout << "  " << label.first << ", " << label.second
                      << " examples, log-prior = " << logPrior(label.first) << endl;
        }

        map<pair<string, string>, int> labelWordToPost = getlabelWordToPost();
        std::cout << "classifier parameters:" << endl;
        for (auto &label : labelWordToPost)
        {
            std::cout.precision(3);
            std::cout << "  " << label.first.first << ":" << label.first.second
                      << ", count = " << label.second << ", log-likelihood = "
                      << logLikelihood(label.first.first, label.first.second)
                      << endl;
        }
        std::cout << endl;
    }

    bool checkArgs(int argc, char **argv)
    {
        if ((argc == 4 && strcmp(argv[3], "--debug")) || argc < 3 || argc > 4)
        {
            std::cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]";
            return true;
        }
        return false;
    }

    bool checkDebug(int argc, char **argv)
    {
        if (argc == 4 && !strcmp(argv[3], "--debug"))
        {
            return true;
        }
        return false;
    }

    void correctPredictions(vector<string> predictedLabels, vector<string> actualLabels)
    {
        int correctPredictions = 0;
        for (int i = 0; i < actualLabels.size(); i++)
        {
            if (predictedLabels.at(i) == actualLabels.at(i))
            {
                correctPredictions++;
            }
        }
        std::cout << "performance: " << correctPredictions
                  << " / " << predictedLabels.size()
                  << " posts predicted correctly" << endl;
    }

    void printTrainingData(bool debug)
    {
        if (debug)
        {
            std::cout << "training data:" << endl;
        }
    }

    void printTrainedOn(bool debug)
    {
        if (!debug)
        {
            std::cout << "trained on " << getNumPosts() << " examples" << endl
                      << endl;
        }
        std::cout << "test data:" << endl;
    }
};

int main(int argc, char **argv)
{

    csvstream trainingsStream(argv[1]);
    csvstream testngStream(argv[2]);
    map<string, string> trainingData;
    map<string, string> testingData;
    BagOfWordsModel model;
    bool debug = model.checkDebug(argc, argv);
    if (model.checkArgs(argc, argv))
    {
        return 1;
    }
    string label;
    string post;

    model.printTrainingData(debug);
    try
    {
        while (trainingsStream >> trainingData)
        {
            label = trainingData["tag"];
            post = trainingData["content"];
            if (debug)
            {
                std::cout << "  label = " << label << ", content = " << post << endl;
            }
            model.callFunctionsInOrder(label, post);
        }
    }
    catch (const csvstream_exception &e)
    {
        std::cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    if (debug)
    {
        model.printDebug();
    }

    vector<string> predictedLabels;
    vector<string> actualLabels;
    model.printTrainedOn(debug);

    try
    {
        while (testngStream >> testingData)
        {
            label = testingData["tag"];
            post = testingData["content"];
            double logprob;
            actualLabels.push_back(label);
            predictedLabels.push_back(model.logProbability(post, logprob));
            std::cout.precision(3);
            std::cout << "  correct = " << label
                      << ", predicted = " << model.logProbability(post, logprob)
                      << ", log-probability score = " << logprob << endl
                      << "  content = " << post << endl
                      << endl;
        }
    }
    catch (const csvstream_exception &e)
    {
        std::cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }

    model.correctPredictions(predictedLabels, actualLabels);
}
