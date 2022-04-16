#include <iostream>
#include "SimpleNN/nn.h"
#include "inputs.h"

double denormalize(double v, double min, double max) {
    return (v*(max-min))-abs(min);
}

double normalize(double v, double min, double max) {
    return (v+abs(min))/(max-min);
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    const int numInputs = 3;
    const int numHiddenNodes = 100;
    const int numOutputs = 3;
    const double tr = 0.5;

    int numDataPoints = 2160;
    int numTrainingPoints = numDataPoints*.7;
    int numTestPoints = numDataPoints*.3;

    SimpleNN nn = SimpleNN(numInputs, numHiddenNodes, numOutputs, tr);
    nn.momentum = 0.1;
    nn.learningRate = 0.4;

    //double ** training_input = new double*[numTrainingPoints];
    //double ** training_output = new double*[numTrainingPoints];

    TrainingData data(numTrainingPoints, numInputs, numOutputs);

    double maxIn = 0;
    double minIn = 0;
    double maxOut = 0;
    double minOut = 0;
    // setup training inputs
    for(int i=0; i < numTrainingPoints; i++) {
        for(int j=0; j < numInputs; j++) {
            double v = dataSet[i][j];
            data.inputs[i][j] = v;
            if(v < minIn)
                minIn = v;
            if(v > maxIn)
                maxIn = v;
        }
        for(int k=0; k < numOutputs; k++) {
            double v = dataSet[i][k+numInputs];
            data.outputs[i][k] = v;
            if(v < minOut)
                minOut = v;
            if(v > maxOut)
                maxOut = v;
        }
    }


    for(int i=0; i < numTrainingPoints; i++) {
        for(int j=0; j < numInputs; j++) {
            data.inputs[i][j] = normalize(data.inputs[i][j], minIn, maxIn);
        }
        for(int k=0; k < numOutputs; k++) {
            data.outputs[i][k] = normalize(data.outputs[i][k], minOut, maxOut);
        }
    }
    // create network

    nn.train(numTrainingPoints, data, 100000);

    return 0;
}
