/*
 * NeuralNetwork.cpp
 *
 *  Created on: Jul 5, 2017
 *      Author: kevywilly
 */

#include "network.h"
#include "limits.h"
#include "math.h"

Network::Network() {

}

void Network::Initialize(NetworkConfig config) {

	InputLayer.NumberOfNodes = config.numInput;
	InputLayer.NumberOfChildNodes = config.numHidden;
	InputLayer.NumberOfParentNodes = 0;
	InputLayer.Initialize(config.numInput, NULL, &HiddenLayer);
	InputLayer.RandomizeWeights();

	HiddenLayer.NumberOfNodes = config.numHidden;
	HiddenLayer.NumberOfChildNodes = config.numOutput;
	HiddenLayer.NumberOfParentNodes = config.numInput;
	HiddenLayer.Initialize(config.numHidden, &InputLayer, &OutputLayer);
	HiddenLayer.RandomizeWeights();

	OutputLayer.NumberOfNodes = config.numOutput;
	OutputLayer.NumberOfChildNodes = 0;
	OutputLayer.NumberOfParentNodes = config.numHidden;
	OutputLayer.Initialize(config.numOutput, &HiddenLayer, NULL);

	SetLearningRate(config.learningRate);
	SetMomentum(config.useMomentum, config.momentumFactor);
	SetLinearOutput(config.linearOutput);

}

void Network::CleanUp(void) {
	InputLayer.CleanUp();
	HiddenLayer.CleanUp();
	OutputLayer.CleanUp();
}

void Network::SetInput(int i, double value) {
	if ((i >= 0) && (i < InputLayer.NumberOfNodes)) {
		InputLayer.NeuronValues[i] = value;
	}
}

double Network::GetOutput(int i) {
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
		return OutputLayer.NeuronValues[i];
	}
	return (double) INT_MAX; // to indicate an error
}

double Network::GetInput(int i) {
	if ((i >= 0) && (i < InputLayer.NumberOfNodes)) {
		return InputLayer.NeuronValues[i];
	}
	return (double) INT_MAX; // to indicate an error
}

void Network::SetDesiredOutput(int i, double value) {
	if ((i >= 0) && (i < OutputLayer.NumberOfNodes)) {
		OutputLayer.DesiredValues[i] = value;
	}
}

double * Network::GetResult(double * inputs) {
	for(int i=0; i < InputLayer.NumberOfNodes; i++) {
		SetInput(i, inputs[i]);
	}

	FeedForward();
	return OutputLayer.NeuronValues;

}
void Network::FeedForward(void) {
	InputLayer.CalculateNeuronValues();
	HiddenLayer.CalculateNeuronValues();
	OutputLayer.CalculateNeuronValues();
}

void Network::BackPropagate(void) {
	OutputLayer.CalculateErrors();
	HiddenLayer.CalculateErrors();

	HiddenLayer.AdjustWeights();
	InputLayer.AdjustWeights();
}

int Network::GetMaxOutputID() {
	int i, id;
	double maxval;

	maxval = OutputLayer.NeuronValues[0];
	id = 0;
	for (i = 1; i < OutputLayer.NumberOfNodes; i++) {
		if (OutputLayer.NeuronValues[i] > maxval) {
			maxval = OutputLayer.NeuronValues[i];
			id = i;
		}
	}
	return id;
}

int * Network::GetTopTwoOutputIDs() {
	int i;
	double maxval;

	int * result = new int[2];
	result[0] = GetMaxOutputID();
	result[1] = result[0];

	maxval = -1;

	for (i = 0; i < OutputLayer.NumberOfNodes; i++) {
		if ((i != result[0]) && (OutputLayer.NeuronValues[i] > maxval)) {
			maxval = OutputLayer.NeuronValues[i];
			result[1] = i;
		}
	}

	return result;
}
double * Network::CalculateOutputErrors() {
	double * errors = new double[OutputLayer.NumberOfNodes];

	for (int i = 0; i < OutputLayer.NumberOfNodes; i++) {
		errors[i] = pow(
				OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i], 2);
	}
	return errors;
}
double Network::CalculateError(void) {
	int i;
	double error = 0.0;
	for (i = 0; i < OutputLayer.NumberOfNodes; i++) {
		error += pow(OutputLayer.NeuronValues[i] - OutputLayer.DesiredValues[i],
				2);
	}
	error = error / OutputLayer.NumberOfNodes;
	return error;
}

void Network::SetLearningRate(double rate) {

	InputLayer.LearningRate = rate;
	HiddenLayer.LearningRate = rate;
	OutputLayer.LearningRate = rate;
}

void Network::SetLinearOutput(bool useLinear) {

	InputLayer.LinearOutput = useLinear;
	HiddenLayer.LinearOutput = useLinear;
	OutputLayer.LinearOutput = useLinear;
}

void Network::SetMomentum(bool useMomentum, double factor) {

	InputLayer.UseMomentum = useMomentum;
	HiddenLayer.UseMomentum = useMomentum;
	OutputLayer.UseMomentum = useMomentum;

	InputLayer.MomentumFactor = factor;
	HiddenLayer.MomentumFactor = factor;
	OutputLayer.MomentumFactor = factor;
}

NetworkConfig Network::GetConfiguration() {
	NetworkConfig config;

	config.numInput = InputLayer.NumberOfNodes;
	config.numHidden = HiddenLayer.NumberOfNodes;
	config.numOutput = OutputLayer.NumberOfNodes;
	config.learningRate = InputLayer.LearningRate;
	config.useMomentum = InputLayer.UseMomentum;
	config.momentumFactor = InputLayer.MomentumFactor;
	config.linearOutput = InputLayer.LinearOutput;

	return config;
}

void Network::Train(TrainingData & trainingData, double maxError,
		int maxEpochs, void (*callbackFn)(int, double)) {

	int batchsize = trainingData.numRows;
	int numbatches = 1;
	int remainder = 0;

	if(trainingData.numRows > 10000) {
		batchsize = 5000;
		numbatches = trainingData.numRows / batchsize;
		remainder = trainingData.numRows % batchsize;
	}

	if(numbatches < 0) {

		numbatches = 1;
	}


	int i, j;
	int c = 0;
	double error = 1.0;
	int numInputs = InputLayer.NumberOfNodes;
	int numOutputs = OutputLayer.NumberOfNodes;

	while ((c < maxEpochs * 1000) && (error > maxError)) {
		c++;
		for(int k=0; k < numbatches+1; k++) {
			int start = k*batchsize;
			int maxcount = batchsize;
			if((maxcount + start) > trainingData.numRows) {
				maxcount = trainingData.numRows - start;
			}

			error = 0.0;
			for (i = start; i < (start+maxcount); i++) {
				for (j = 0; j < numInputs; j++) {
					SetInput(j, trainingData.data[i][j]);
				}
				for (j = 0; j < numOutputs; j++) {
					SetDesiredOutput(j, trainingData.data[i][j + numInputs]);

				}

				FeedForward();
				BackPropagate();
				error += CalculateError();

			}

			error = error / maxcount;
			if(callbackFn != NULL) {
				if((c < 1000) || ((c % 200) == 0)) {
					callbackFn(c, error);
				}
			}
		}
	}
}

TestResult * Network::Test(double * input) {
		int i;
		int nInput = InputLayer.NumberOfNodes;
		int nOutput = OutputLayer.NumberOfNodes;
		TestResult * result = new TestResult[nOutput];


		for(i=0; i < nInput; i++) {
			SetInput(i, input[i]);
		}
		for(i=0; i < nOutput; i++) {
			SetDesiredOutput(i, input[i+nInput]);
		}

		FeedForward();

		double * outputs = OutputLayer.NeuronValues;
		double * desired = OutputLayer.DesiredValues;
		double * errors = CalculateOutputErrors();

		for(i=0; i < nOutput; i++) {
			result[i].Desired = desired[i];
			result[i].Value = outputs[i];
			result[i].Error = errors[i];
		}

		return result;
	}
