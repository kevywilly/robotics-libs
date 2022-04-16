#include <iostream>

#include "time.h"
#include "stdlib.h"
#include "neuralnet/network.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include "math.h"
#include "neuralnet/networkconfig.h"
#include "networktools.h"

using namespace std;

int cmdValidate(int argc, char *argv[]) {
    int i, j;
    double * outputs;
    int numCorrect = 0;
    int activations = 0;

    if (argc < 4) {
        cout << "Not enough arguments" << endl;
        cout << "usage: SimpleNeural validate <dataFile> <modelfile>" << endl;
        return -1;
    }

    const string trainingFile = argv[2];
    const string modelFile = argv[3];

    // the network
    Network * net = LoadNetwork(modelFile);

    // attempt to load network from model file
    if (net == NULL) {
        cout << "Data file: " << modelFile << " could not be loaded." << endl;
        return -1;
    }

    Network nn = *net;

    // Training data
    TrainingData data = LoadTrainingData(trainingFile);



    double * inputs = new double[nn.InputLayer.NumberOfNodes];
    double * desired = new double[nn.OutputLayer.NumberOfNodes];

    cout << "num rows " << data.numRows << endl;
    for (i = 0; i < data.numRows; i++) {

        for (j = 0; j < nn.InputLayer.NumberOfNodes; j++) {
            inputs[j] = data.data[i][j];
        }
        // desired
        for (j = 0; j < nn.OutputLayer.NumberOfNodes; j++) {
            desired[j] =
                    (data.data[i][j + nn.InputLayer.NumberOfNodes] > 0.5) ?
                    1.0 : 0.0;
            cout << desired[j] << " ";
        }

        outputs = nn.GetResult(inputs);

        int correct = 1;
        for (j = 0; j < nn.OutputLayer.NumberOfNodes; j++) {
            double o = (outputs[j] > 0.5) ? 1.0 : 0.0;
            cout << o << " ";
            if (o != desired[j])
                correct = 0;
            if(o > 0)
                activations +=1;
        }

        numCorrect += correct;
    }

    cout << endl << endl;
    double pct = 100.0f * numCorrect / double(data.numRows);
    cout << fixed << setprecision(2);
    cout << numCorrect << " out of " << data.numRows << " rows were correct. (" << pct << "%)" << endl;;
    cout << activations << " activations " << 100.0f*activations/data.numRows << " %" << endl;;
    cout << endl;

    return 1;
}
int cmdTrain(int argc, char *argv[]) {

    if (argc < 9) {
        cout << "Not enough arguments" << endl;
        cout
                << "usage: SimpleNeural train <trainingfile> <modelfile> <numInputs> <numHidden> <numOutputs> <maxError> <maxEpochs>"
                << endl;
    }

    const string trainingFile = argv[2];
    const string modelFile = argv[3];
    int numInputs = atoi(argv[4]);
    int numHidden = atoi(argv[5]);
    int numOutputs = atoi(argv[6]);
    double maxError = atof(argv[7]);
    int maxEpochs = atof(argv[8]);

    double learningRate = 0.25;
    double momentumFactor = 0.4;
    bool useMomentum = true;

    if(argc > 9) {
        learningRate = atof(argv[9]);
    }

    if(argc > 10) {
        momentumFactor = atof(argv[10]);
        useMomentum = (momentumFactor > 0.0);
    }


// the network
    Network * net = LoadNetwork(modelFile);

    Network nn;
// attempt to load network from model file
    if (net == NULL) {

        cout << "building new network" << endl;
        NetworkConfig config;

        config.learningRate = learningRate;
        config.useMomentum = useMomentum;
        config.momentumFactor = momentumFactor;
        config.numInput = numInputs;
        config.numHidden = numHidden;
        config.numOutput = numOutputs;
        config.linearOutput = false;

        nn.Initialize(config);

    } else {
        nn = *net;
    }


// Training data
    TrainingData data = LoadTrainingData(trainingFile);

    if(data.numColumns != (nn.InputLayer.NumberOfNodes + nn.OutputLayer.NumberOfNodes)) {
        cout << "Training set does not match network configuration: " << data.numColumns << " vs " << nn.InputLayer.NumberOfNodes + nn.OutputLayer.NumberOfNodes << " columns" << endl;
    }

    TrainNetwork(nn, data, maxError, maxEpochs);

    SaveNetwork(nn, modelFile);


    return 0;
}

bool YesOrNoPrompt(const string & prompt) {

    string s = "z";

    while (1) {
        cout << prompt << ": ";
        cin >> s;
        if ((s == "Y") || (s == "y"))
            return true;
        if ((s == "N") || (s == "n"))
            return false;
    }

    cin.clear();
    cout << endl;

}
int cmdInteractive(int argc, char *argv[]) {

    if (argc < 3) {
        cout << "Not enough arguments" << endl;
        cout << "usage: SimpleNeural interactive <modelfile>" << endl;
    }

    const string modelFile = argv[2];

    Network * net = LoadNetwork(modelFile);

    Network nn;

    if ( net == NULL) {
        cout << "Model file: " << modelFile << " could not be loaded." << endl;
        exit(0);
    } else {
        nn = *net;
    }

    bool changed = false;
    bool stop = false;
    while (!stop) {

        // Ask for input
        cout << "Provide " << nn.InputLayer.NumberOfNodes
             << " input values or -1 to quit? " << endl;
        for (int i = 0; i < nn.InputLayer.NumberOfNodes; i++) {
            string input;
            double inputVal = nn.GetInput(i);
            cout << "Input " << i << ": (" << inputVal << ") ";
            cin.clear();
            std::getline(std::cin, input);
            if (!input.empty()) {
                istringstream stream(input);
                stream >> inputVal;
            }
            cout << "got " << inputVal << endl;
            if (inputVal == -1) {
                stop = true;
                break;
            }
            nn.SetInput(i, inputVal);
        }

        // If not stop, feed forward and show results
        if (!stop) {
            nn.FeedForward();
            cout << "=====================" << endl;
            PrintOutputs(nn);
            cout << endl;
        } else {
            cout << "done";
            if (changed) {
                SaveNetwork(nn, modelFile);
                cout << "...changes saved";
            }
            cout << endl;
            return 0;
        }

        // Get confirmation - if bad then get correction
        if (!YesOrNoPrompt("Am I Correct? (Y/N) ")) {
            cout << "Sorry I failed you, please teach me..." << endl << endl;
            string in;
            cin.clear();
            std::getline(std::cin, in);
            for (int i = 0; i < nn.OutputLayer.NumberOfNodes; i++) {
                string input;
                double desired = round(nn.GetOutput(i));
                cout << "Desired Output " << i << ": (" << desired << ")";
                cin.clear();
                std::getline(std::cin, input);
                if (!input.empty()) {
                    istringstream stream(input);
                    stream >> desired;
                }
                cout << "got " << desired << endl;
                nn.SetDesiredOutput(i, desired);
            }

            cout << "Training network ...";
            nn.BackPropagate();
            cout << "done" << endl << endl;

            changed = true;

        } else {
            cout << "That is good news! ..." << endl << endl;
            string in;
            cin.clear();
            std::getline(std::cin, in);
        }

    }

    return 0;
}

// Usage notes
void Usage() {
    cout << "Simple Neural version 1.0" << endl;
    cout
            << "======================================================================================="
            << endl;
    cout << "Usage:" << endl;
    cout
            << "SimpleNeural train <trainingfile> <modelfile> <numInputs> <numHidden> <numOutputs>"
            << endl;
    cout << "SimpleNeural interactive <modelfile>" << endl;
    cout << "SimpleNeural validate <dataFile> <modelfile>" << endl;
}

int main() {

    return 0;
}
// Main Function
int main2(int argc, char *argv[]) {

    if (argc < 2) {
        Usage();
        return -1;
    }

// get command
    string command = argv[1];

    if (command == "train") {
        return cmdTrain(argc, argv);
    } else if (command == "interactive") {
        return cmdInteractive(argc, argv);
    } else if (command == "validate") {
        return cmdValidate(argc, argv);
    } else {
        Usage();
        return -1;
    }

    return 0;

}
