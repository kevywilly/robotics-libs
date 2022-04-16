/*
 * NeuralNetwork.h
 *
 *  Created on: Jul 5, 2017
 *      Author: kevywilly
 */

#ifndef NEURALNETWORK_H_
#define NEURALNETWORK_H_

#include "networkconfig.h"
#include "networklayer.h"

struct TestResult {
	double Value;
	double Error;
	double Desired;
};

struct TrainingData {
	int numColumns;
	int numRows;
	double ** data;
};

class Network {
public:
	NetworkLayer	InputLayer;
	NetworkLayer	HiddenLayer;
	NetworkLayer	OutputLayer;



	void CleanUp();
	void SetInput(int i, double value);
	double GetOutput(int i);
	double GetInput(int i);
	void SetDesiredOutput(int i, double value);
	void FeedForward(void);
	void BackPropagate(void);
	int GetMaxOutputID(void);
	int * GetTopTwoOutputIDs(void);
	double CalculateError(void);
	double * CalculateOutputErrors(void);
	void SetLearningRate(double rate);
	void SetLinearOutput(bool useLinear);
	void SetMomentum(bool useMomentum, double factor);
	void DumpData(char* filename);
	NetworkConfig GetConfiguration();
	void Initialize(NetworkConfig config);

	double * GetResult(double * inputs);
	void Train(TrainingData & trainingData, double maxError, int maxEpochs, void(*callbackFn)(int,double));
	TestResult * Test(double * input);

	Network();



};

#endif /* NEURALNETWORK_H_ */
