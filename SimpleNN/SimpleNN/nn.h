//
// Created by Kevin Williams on 9/30/21.
// https://towardsdatascience.com/activation-functions-neural-networks-1cbd9f8d91d6

//

#ifndef SIMPLENN_NN_H
#define SIMPLENN_NN_H
#include <stdlib.h>
#include <math.h>

class TrainingData {
public:
    double ** inputs;
    double ** outputs;
    double * index;
    int numEntries;

    TrainingData(int numEntries, int numInputs, int numOutputs) : numEntries(numEntries) {
        inputs = new double*[numEntries];
        outputs = new double*[numEntries];
        index = new double[numEntries];

        for(int i=0; i < numEntries; i++) {
            inputs[i] = new double[numInputs];
            outputs[i] = new double[numOutputs];
            index[i] = i;
        }

    }

    void shuffleIndex()
    {
        if (numEntries > 1)
        {
            size_t i;
            for (i = 0; i < numEntries - 1; i++)
            {
                size_t j = i + rand() / (RAND_MAX / (numEntries - i) + 1);
                int t = index[j];
                index[j] = index[i];
                index[i] = t;
            }
        }
    }


};
class SimpleNN {
public:
    static double sigmoid(double x) { return 1 / (1 + exp(-x)); }
    static double dSigmoid(double x) { return x * (1 - x); }
    static double linear(double x) {return x;}
    static double dlinear(double x) {return 1;}
    static double dtanh(double x) {return 1-tanh(x)* tanh(x);}
    static double init_weight() { return ((double)rand())/((double)RAND_MAX); }

    int numInputs = 2;
    int numHiddenNodes = 2;
    int numOutputs = 1;
    double learningRate;
    double * hiddenLayer;
    double * outputLayer;
    double * hiddenLayerBias;
    double * outputLayerBias;
    double ** hiddenWeights;
    double ** prevHiddenDelta;
    double ** outputWeights;
    double ** prevOutputDelta;
    double momentum = .5;

    SimpleNN(int numInputs, int numHiddenNodes, int numOutputs, double lr) : numInputs(numInputs), numHiddenNodes(numHiddenNodes),
                                                                             numOutputs(numOutputs), learningRate(lr) {
        hiddenLayer = new double[numHiddenNodes];
        outputLayer = new double[numOutputs];
        hiddenLayerBias = new double[numHiddenNodes];
        outputLayerBias = new double[numOutputs];
        hiddenWeights = new double*[numInputs];
        prevHiddenDelta = new double*[numInputs];
        outputWeights = new double*[numHiddenNodes];
        prevOutputDelta = new double*[numHiddenNodes];

        for(int i=0; i < numInputs; i++) {
            outputLayerBias[i] = init_weight();
            hiddenWeights[i] = new double[numHiddenNodes];
            prevHiddenDelta[i] = new double[numHiddenNodes];
            for(int j=0; j < numHiddenNodes; j++) {
                hiddenWeights[i][j] = init_weight();
                prevHiddenDelta[i][j] = 0;
            }
        }
        for(int i=0; i < numHiddenNodes; i++) {
            hiddenLayerBias[i] = init_weight();
            outputWeights[i] = new double[numOutputs];
            prevOutputDelta[i] = new double[numOutputs];
            for(int j=0; j < numOutputs; j++) {
                outputWeights[i][j] = init_weight();
                prevOutputDelta[i][j] = 0;
            }
        }
    }


    void printInputs(double * d) {
        printf("\n in: ");
        for(int i=0; i < numInputs; i++) {
            printf("%f, ", d[i]);
        }
    }

    void printOutputs(double * d) {
        printf(" out: ");
        for(int i=0; i < numOutputs; i++) {
            printf("%f, ", d[i]);
        }
    }
    void train(int numTrainingSets, TrainingData &data,  int epochs) {
        double mse = 1;
        for (int n = 0; n < epochs; n++) {
            data.shuffleIndex();
            mse = 0;
            for (int x = 0; x < numTrainingSets; x++) {
                int i = data.index[x];

                //printInputs(data.inputs[i]);
                //printOutputs(data.outputs[i]);

                // Compute hidden layer activations
                for (int j = 0; j < numHiddenNodes; j++) {
                    double activation = hiddenLayerBias[j];
                    for (int k = 0; k < numInputs; k++) {
                        activation += data.inputs[i][k] * hiddenWeights[k][j];
                    }
                    hiddenLayer[j] = tanh(activation);
                }

                // compute output layer activation
                for (int j = 0; j < numOutputs; j++) {
                    double activation = outputLayerBias[j];
                    for (int k = 0; k < numHiddenNodes; k++) {
                        activation += hiddenLayer[k] * outputWeights[k][j];
                    }
                    outputLayer[j] = tanh(activation);
                }

                double deltaOutput[numOutputs];
                //printOutputs(outputLayer);


                // Calc output layer errors

                for (int j = 0; j < numOutputs; j++) {
                    double dError = (data.outputs[i][j] - outputLayer[j]);
                    double delta = dError * dtanh(outputLayer[j]);
                    deltaOutput[j] = delta;

                    mse += abs(dError)/numOutputs;


                }



                // Calc hidden layer errors
                double deltaHidden[numHiddenNodes];
                for (int j = 0; j < numHiddenNodes; j++) {
                    double dError = 0.0f;
                    for (int k = 0; k < numOutputs; k++) {
                        dError += deltaOutput[k] * outputWeights[j][k];
                    }
                    deltaHidden[j] = dError * dtanh(hiddenLayer[j]);
                }

                // Apply change in output weights
                for (int j = 0; j < numOutputs; j++) {
                    outputLayerBias[j] += deltaOutput[j] * learningRate;
                    for (int k = 0; k < numHiddenNodes; k++) {
                        outputWeights[k][j] += hiddenLayer[k] * deltaOutput[j] * learningRate;
                        if(momentum > 0)
                            outputWeights[k][j] += momentum * prevOutputDelta[k][j];
                        prevOutputDelta[k][j] = deltaOutput[j];
                    }
                }
                // Apply change in hidden weights
                for (int j = 0; j < numHiddenNodes; j++) {
                    hiddenLayerBias[j] += deltaHidden[j] * learningRate;
                    for (int k = 0; k < numInputs; k++) {
                        //double dw = learningRate * ptr_inputs[i].Value * Error;

                        hiddenWeights[k][j] += data.inputs[i][k] * deltaHidden[j] * learningRate;
                        if(momentum > 0)
                            hiddenWeights[k][j] += momentum * prevHiddenDelta[k][j];
                        prevHiddenDelta[k][j] = deltaHidden[j];
                    }
                }
            }

            printf("EPOCH: %d ERROR: %f\n", n, mse/numTrainingSets);
        }
    }
};
#endif //SIMPLENN_NN_H
