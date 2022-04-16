/*
 * NeuralNetworkLayer.cpp
 *
 *  Created on: Jul 5, 2017
 *      Author: kevywilly
 */

#include "networklayer.h"

#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "neuralmath.h"


NetworkLayer::NetworkLayer() {
	ParentLayer = NULL;
	ChildLayer = NULL;
	LearningRate = 0.5;
	LinearOutput = false;
	UseMomentum = false;
	MomentumFactor = 0.9;
	NeuronValues = new double[0];
	DesiredValues = new double[0];
	Errors = new double[0];
	BiasValues = NULL;
	BiasWeights = NULL;
	WeightChanges = NULL;
	Weights = NULL;
	NumberOfParentNodes = 0;
	NumberOfChildNodes = 0;
	NumberOfNodes = 0;

}

void NetworkLayer::Initialize(int NumNodes, NetworkLayer* parent,
		NetworkLayer * child) {
	int i, j;

	NeuronValues = new double[NumberOfNodes];
	DesiredValues = new double[NumberOfNodes];
	Errors = new double[NumberOfNodes];

	// parent layer
	if (parent != NULL) {
		ParentLayer = parent;
	}

	// child layer
	if (child != NULL) {
		ChildLayer = child;
		Weights = new double*[NumberOfNodes];
		WeightChanges = new double*[NumberOfNodes];
		for (i = 0; i < NumberOfNodes; i++) {
			Weights[i] = new double[NumberOfChildNodes];
			WeightChanges[i] = new double[NumberOfChildNodes];
		}
		BiasValues = new double[NumberOfChildNodes];
		BiasWeights = new double[NumberOfChildNodes];
	} else {
		Weights = NULL;
		BiasValues = NULL;
		BiasWeights = NULL;
		WeightChanges = NULL;
	}

	// Make sure 0s everywhere

	for (i = 0; i < NumberOfNodes; i++) {
		NeuronValues[i] = 0;
		DesiredValues[i] = 0;
		Errors[i] = 0;

		if (ChildLayer != NULL)
			for (j = 0; j < NumberOfChildNodes; j++) {
				Weights[i][j] = 0;
				WeightChanges[i][j] = 0;
			}

	}

	// Initialize Bias values and weights
	if (ChildLayer != NULL)
		for (j = 0; j < NumberOfChildNodes; j++) {
			BiasValues[j] = 1;
			BiasWeights[j] = 0;
		}

}

void NetworkLayer::CleanUp(void) {
	int i;
	free(NeuronValues);
	free(DesiredValues);
	free(Errors);
	if (Weights != NULL) {
		for (i = 0; i < NumberOfNodes; i++) {
			free(Weights[i]);
			free(WeightChanges[i]);
		}
	}

	if (BiasValues != NULL)
		free(BiasValues);
	if (BiasWeights != NULL)
		free(BiasWeights);
}

void NetworkLayer::RandomizeWeights(void) {
	int i, j;
	int min = 0;
	int max = 200;
	int number;
	srand(time(NULL));

	for (i = 0; i < NumberOfNodes; i++) {
		for (j = 0; j < NumberOfChildNodes; j++) {
			number = (((abs(rand())%(max-min+1))+min));
			if(number>max)
				number = max;
			if(number<min)
				number = min;

			Weights[i][j] = number / 100.0f - 1;
		}
	}

	for (j = 0; j < NumberOfChildNodes; j++) {
		number = (((abs(rand())%(max-min+1))+min));
		if(number>max)
			number = max;
		if(number<min)
			number = min;

		BiasWeights[j] = number / 100.0f - 1;
	}
}

void NetworkLayer::CalculateNeuronValues(void) {
	int i, j;
	double sum;
	double x0,x1,x2,x3;



	if (ParentLayer != NULL) {
			for (j = 0; j < NumberOfNodes; j++) {
				sum = 0;

				for (i = 0; i < NumberOfParentNodes; i++)
				{
					sum += ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
				}

				sum += ParentLayer->BiasValues[j] * ParentLayer->BiasWeights[j];

				if (ChildLayer != NULL && LinearOutput)
					NeuronValues[j] = sum;
				else
					NeuronValues[j] = 1.0f / (1 + exp(-sum));

			}
		}
	/*
	if (ParentLayer != NULL) {
		for (j = 0; j < NumberOfNodes; j++) {
			sum = 0;

			for (i = 0; i < (NumberOfParentNodes-3); i+=4)
			{
				x0 = ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
				x1 = ParentLayer->NeuronValues[i+1] * ParentLayer->Weights[i+1][j];
				x2 = ParentLayer->NeuronValues[i+2] * ParentLayer->Weights[i+2][j];
				x3 = ParentLayer->NeuronValues[i+3] * ParentLayer->Weights[i+3][j];
				sum += x0 + x1 + x2 + x3;
			}

			for(i=i+0; i < NumberOfParentNodes; i++) {
				sum += ParentLayer->NeuronValues[i] * ParentLayer->Weights[i][j];
			}

			sum += ParentLayer->BiasValues[j] * ParentLayer->BiasWeights[j];

			if (ChildLayer != NULL && LinearOutput)
				NeuronValues[j] = sum;
			else
				NeuronValues[j] = 1.0f / (1 + exp(-sum));

		}
	}*/
}

void NetworkLayer::CalculateErrors(void) {
	int i, j;
	double sum;

	if (ChildLayer == NULL) { //output layer
		for (i = 0; i < NumberOfNodes; i++) {
			Errors[i] = (DesiredValues[i] - NeuronValues[i]) * NeuronValues[i]
					* (1.0f - NeuronValues[i]); // mean square
			//Errors[i] = DesiredValues[i] - NeuronValues[i]; //cross entropy
		}
	} else if (ParentLayer == NULL) { //input layer
		//for (i = 0; i < NumberOfNodes; i++) {
		//	Errors[i] = 0.0f;
		//}
	} else { //hidden layer

		for (i = 0; i < NumberOfNodes; i++) {
			sum = 0;
			for (j = 0; j < NumberOfChildNodes; j++) {
				sum += ChildLayer->Errors[j] * Weights[i][j];
			}
			Errors[i] = sum * NeuronValues[i] * (1.0f - NeuronValues[i]); // mean square
			//Errors[i] = sum; // cross-entropy
		}
	}
}

void NetworkLayer::AdjustWeights(void) {
	int i, j;
	double dw0,dw1,dw2,dw3;

	if (ChildLayer != NULL) {
		for (i = 0; i < NumberOfNodes; i++) {
			for (j = 0; j < NumberOfChildNodes-3; j+=4) {
				dw0 = LearningRate * ChildLayer->Errors[j] * NeuronValues[i];
				dw1 = LearningRate * ChildLayer->Errors[j+1] * NeuronValues[i];
				dw2 = LearningRate * ChildLayer->Errors[j+2] * NeuronValues[i];
				dw3 = LearningRate * ChildLayer->Errors[j+3] * NeuronValues[i];
				if (UseMomentum) {
					Weights[i][j] += dw0 + MomentumFactor * WeightChanges[i][j];
					Weights[i][j+1] += dw1 + MomentumFactor * WeightChanges[i][j+1];
					Weights[i][j+2] += dw2 + MomentumFactor * WeightChanges[i][j+2];
					Weights[i][j+3] += dw3 + MomentumFactor * WeightChanges[i][j+3];
					WeightChanges[i][j] = dw0;
					WeightChanges[i][j+1] = dw1;
					WeightChanges[i][j+2] = dw2;
					WeightChanges[i][j+3] = dw3;
				} else {
					Weights[i][j] += dw0;
					Weights[i][j+1] += dw1;
					Weights[i][j+2] += dw2;
					Weights[i][j+3] += dw3;
				}
			}
			for (j = j+0; j < NumberOfChildNodes; j++) {
				dw0 = LearningRate * ChildLayer->Errors[j] * NeuronValues[i];
				if (UseMomentum) {
					Weights[i][j] += dw0 + MomentumFactor * WeightChanges[i][j];
					WeightChanges[i][j] = dw0;
				} else {
					Weights[i][j] += dw0;
				}
			}
		}

		for (j = 0; j < NumberOfChildNodes-3; j+=4) {
			BiasWeights[j] += LearningRate * ChildLayer->Errors[j] * BiasValues[j];
			BiasWeights[j+1] += LearningRate * ChildLayer->Errors[j+1] * BiasValues[j+1];
			BiasWeights[j+2] += LearningRate * ChildLayer->Errors[j+2] * BiasValues[j+2];
			BiasWeights[j+3] += LearningRate * ChildLayer->Errors[j+3] * BiasValues[j+3];
		}
		for (j = j+0; j < NumberOfChildNodes-3; j+=4) {
					BiasWeights[j] += LearningRate * ChildLayer->Errors[j] * BiasValues[j];
		}
	}
}

