//
// Created by Kevin Williams on 10/2/21.
//

#ifndef SIMPLENEURAL_NETWORKTOOLS_H
#define SIMPLENEURAL_NETWORKTOOLS_H

#include "stdlib.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <vector>

using namespace std;



// Methods Declarations

void PrintConfiguration(Network& network);

TrainingData LoadTrainingData(const std::string & filename) {

    TrainingData data;

    int i,j;
    data.numColumns = 0;
    data.numRows = 0;

    std::vector<std::vector<double> > values;
    std::vector<double> row;


    std::ifstream csv(filename);

    if(!csv) {
        std::cout << "Could not open file: " << filename << std::endl;
        csv.close();
        return data;
    }

    std::string item;
    int rowNum = 0;
    for(std::string line; getline(csv,line);) {
        std::istringstream in(line);
        while(std::getline(in, item, ',')) {
            row.push_back((double)atof(item.c_str()));
        }

        // Make sure row length is consistent
        if(data.numColumns == 0) {
            data.numColumns = row.size();
        } else if (row.size() != data.numColumns) {
            std::cout << "Inconsistent number of columns in row " << rowNum << std::endl;
            csv.close();
            return data;
        }
        values.push_back(row);
        row.clear();
    }
    data.numRows = values.size();

    data.data = new double*[data.numRows];

    for(i=0; i < data.numRows; i++) {
        data.data[i] = new double[data.numColumns];
        for(j=0; j < data.numColumns; j++) {
            data.data[i][j] = values[i][j];
            values[i].clear();
        }
    }

    csv.close();
    cout << data.numRows << " rows, " << data.numColumns << " columns read" << endl;
    return data;
}

void PrintWeights(Network & nn) {
    NetworkConfig config = nn.GetConfiguration();

    //cout << fixled << setprecision(6);
    for (int i = 0; i < config.numInput; i++) {
        cout << nn.InputLayer.BiasWeights[i] << ", ";
    }
    cout << endl;
    for (int i = 0; i < config.numInput; i++) {
        for (int j = 0; j < config.numHidden; j++) {
            cout << nn.InputLayer.Weights[i][j] << ", ";
        }
        cout << endl;
    }
    cout << endl;

    for (int i = 0; i < config.numHidden; i++) {

        cout << nn.HiddenLayer.BiasWeights[i] << ", ";

    }
    cout << endl;
    for (int i = 0; i < config.numHidden; i++) {
        for (int j = 0; j < config.numOutput; j++) {
            cout << nn.HiddenLayer.Weights[i][j] << ", ";
        }
        cout << endl;
    }
    cout << endl;

}
bool SaveNetwork(Network network, const string filename) {
    int i, j;

    NetworkConfig config = network.GetConfiguration();

    //remove(filename);

    FILE *ptr_myfile;

    ptr_myfile = fopen(filename.c_str(), "wb");
    if (ptr_myfile) {

        fwrite(&config, sizeof(NetworkConfig), 1, ptr_myfile);

        for (i = 0; i < network.InputLayer.NumberOfNodes; i++) {
            double d = network.InputLayer.BiasWeights[i];
            fwrite(&d, sizeof(double), 1, ptr_myfile);
        }

        for (i = 0; i < network.InputLayer.NumberOfNodes; i++) {
            for (j = 0; j < network.InputLayer.NumberOfChildNodes; j++) {
                double d = network.InputLayer.Weights[i][j];
                fwrite(&d, sizeof(double), 1, ptr_myfile);
            }
        }

        for (i = 0; i < network.HiddenLayer.NumberOfNodes; i++) {
            double d = network.HiddenLayer.BiasWeights[i];
            fwrite(&d, sizeof(double), 1, ptr_myfile);
        }

        for (int i = 0; i < network.HiddenLayer.NumberOfNodes; i++) {
            for (j = 0; j < network.HiddenLayer.NumberOfChildNodes; j++) {
                double d = network.HiddenLayer.Weights[i][j];
                fwrite(&d, sizeof(double), 1, ptr_myfile);
            }
        }

        int r = fclose(ptr_myfile);
        cout << "saved file: " << r << endl;
    }

    //outfile.close();
    cout << "Saved network" << endl;
    PrintConfiguration(network);
    PrintWeights(network);

    return true;
}

Network * LoadNetwork(const string & filename) {

    int i, j, r;

    Network * network = new Network;
    NetworkConfig config;

    FILE *ptr_myfile;

    ptr_myfile = fopen(filename.c_str(), "rb");

    if (!ptr_myfile) {
        cout << "Unable to open file: " << filename << endl;
        return NULL;
    }
    // read config
    r = fread(&config, sizeof(NetworkConfig), 1, ptr_myfile);
    cout << "read" << r << endl;
    // initialize network
    network->Initialize(config);

    // input bias weights
    for (i = 0; i < network->InputLayer.NumberOfChildNodes; i++) {
        //double d =
        r=fread(&network->InputLayer.BiasWeights[i], sizeof(double), 1,
                ptr_myfile);
        cout << "read" << r << endl;
    }

    // input weights
    for (i = 0; i < network->InputLayer.NumberOfNodes; i++) {
        for (j = 0; j < network->InputLayer.NumberOfChildNodes; j++) {
            r=fread(&network->InputLayer.Weights[i][j], sizeof(double), 1,
                    ptr_myfile);
            cout << "read" << r << endl;
        }
    }

    // hidden bias weights
    for (i = 0; i < network->HiddenLayer.NumberOfChildNodes; i++) {
        r=fread(&network->HiddenLayer.BiasWeights[i], sizeof(double), 1,
                ptr_myfile);
        cout << "read" << r << endl;
    }


    // hidden weights
    for (i = 0; i < network->HiddenLayer.NumberOfNodes; i++) {
        for (j = 0; j < network->HiddenLayer.NumberOfChildNodes; j++) {
            r=fread(&network->HiddenLayer.Weights[i][j], sizeof(double), 1,
                    ptr_myfile);
            cout << "read" << r << endl;
        }
    }


    r = fclose(ptr_myfile);
    cout << "closed file" << r << endl;
    PrintConfiguration(*network);
    PrintWeights(*network);



    //free(ptr_myfile);

    return network;


}

void PrintConfiguration(Network & nn) {
    NetworkConfig config = nn.GetConfiguration();
    cout << "======== Neural Network Configuration ============" << endl;
    cout << "Inputs: " << config.numInput << endl;
    cout << "Hidden: " << config.numHidden << endl;
    cout << "Outputs: " << config.numOutput << endl;
    cout << "LearningRate: " << config.learningRate << endl;
    cout << "UseMomentum: " << config.useMomentum << endl;
    cout << "MomentumFactor: " << config.momentumFactor << endl;
    cout << "LinearOutput: " << config.linearOutput << endl;
    cout << endl;
}
void PrintTestResults(TestResult * results, int len) {

    //cout << fixled << setprecision(6);
    for (int i = 0; i < len; i++) {
        cout << results[i].Value << " -> " << results[i].Desired << " -> "
             << results[i].Error << endl;
    }
}
void PrintTrainingResults(int count, double error) {
    //cout << fixled << setprecision(6);
    cout << "iteration: " << count << " error: " << error << endl;
}

void PrintOutputs(Network & nn) {
    cout << fixed << setprecision(6);
    int j;

    cout << "Inputs: " << endl;
    for(j=0; j < nn.InputLayer.NumberOfNodes; j++) {
        cout << nn.InputLayer.NeuronValues[j] << " ";
    }

    cout << endl << "Output:" << endl;

    for (j = 0; j < nn.OutputLayer.NumberOfNodes; j++) {
        cout << j << ": " << nn.OutputLayer.NeuronValues[j] << endl;
    }
}

// Train the network
void TrainNetwork(Network & nn, TrainingData & trainingData, double maxError, int maxEpochs) {
    nn.Train(trainingData, maxError, maxEpochs, &PrintTrainingResults);
}

// Test the network
void TestNetwork(Network& network, TrainingData& trainingData) {
    int i, j;

    NetworkConfig config = network.GetConfiguration();

    // ============ Test the network ====================

    for (i = 0; i < trainingData.numRows / 4; i++) {
        TestResult * r = network.Test(trainingData.data[i]);
        cout << "In: ";
        for(j=0; j < network.InputLayer.NumberOfNodes; j++) {
            cout << network.InputLayer.NeuronValues[j] << " ";
        }
        cout << endl << "------------------------------------------" << endl;
        cout << "Result: " << endl;
        PrintTestResults(r, config.numOutput);
        cout << "==========================================" << endl;

    }
}

#endif //SIMPLENEURAL_NETWORKTOOLS_H
