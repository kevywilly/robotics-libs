/*
 * NeuralNetworkLayer.h
 *
 *  Created on: Jul 5, 2017
 *      Author: kevywilly
 */

#ifndef NEURALNETWORKLAYER_H_
#define NEURALNETWORKLAYER_H_

#include "stdlib.h"
#include "time.h"

class NetworkLayer {
public:
	int NumberOfNodes;
	int NumberOfChildNodes;
	int NumberOfParentNodes;
	double** Weights;
	double** WeightChanges;
	double* NeuronValues;
	double* DesiredValues;
	double* Errors;
	double* BiasWeights;
	double* BiasValues;
	double LearningRate;
	bool LinearOutput;
	bool UseMomentum;
	double MomentumFactor;

	NetworkLayer * ParentLayer;
	NetworkLayer * ChildLayer;

	NetworkLayer();

	void Initialize(int NumNodes, NetworkLayer* parent, NetworkLayer* child);
	void CleanUp(void);
	void RandomizeWeights(void);
	void CalculateErrors(void);
	void CalculateNeuronValues(void);
	void AdjustWeights(void);

	static int getRand(int min, int max) {
		int number;
		number = (((abs(rand())%(max-min+1))+min));
		if(number > max) number = max;
		if(number < min) number = min;
		return number;
	}

};

#endif /* NEURALNETWORKLAYER_H_ */
