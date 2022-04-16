/*
 * network_config.h
 *
 *  Created on: Jul 14, 2017
 *      Author: kevywilly
 */

#ifndef NETWORKCONFIG_H_
#define NETWORKCONFIG_H_

struct NetworkConfig {
	int    numInput;
	int	   numHidden;
	int	   numOutput;
	bool 	useMomentum;
	bool 	linearOutput;
	double learningRate;
	double momentumFactor;

	NetworkConfig() {
			numInput = 0;
			numHidden = 0;
			numOutput = 0;
			useMomentum = true;
			linearOutput = false;
			learningRate = 0.2;
			momentumFactor = 0.5;
	}

};

#endif /* NETWORKCONFIG_H_ */
