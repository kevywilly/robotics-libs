/*
 * neuralmath.h
 *
 *  Created on: Jul 18, 2017
 *      Author: kevywilly
 */

#ifndef NEURALMATH_H_
#define NEURALMATH_H_

double dotprod(int n, double * vec1, double * vec2);
void fillArray(double * array, int size, double value);

double * doublePointer(int size, double value) {
	double * ar = new double[size];
	for(int i=0; i < size; i++) {
		ar[i] = value;
	}
	return ar;
}
void fillArray(double * array, int size, double value) {

	for(int i=0; i < size; i++) {
		array[i] = value;
	}

}


double sumprod(int n, double * vec1, double mult) {
	int k,m;
	double sum;

	sum = 0.0;
	k = n/4;
	m = n % 4;

	while(k--) {
		sum+= *(vec1) * mult;
		sum+= *(vec1+1) * mult;
		sum+= *(vec1+2) * mult;
		sum+= *(vec1+3) * mult;
		vec1 +=4;
	}
	while(m--) {
		sum+= *vec1++ * mult;
	}

	return sum;
}
double dotprod(int n, double * vec1, double * vec2) {
	int k,m;
	double sum;

	sum = 0.0;
	k = n/4;
	m = n % 4;

	while(k--) { // Do each group of 4
		sum += *(vec1) * *(vec2);
		sum += *(vec1 + 1) * *(vec2 + 1);
		sum += *(vec1 + 2) * *(vec2 + 2);
		sum += *(vec1 + 3) * *(vec2 + 3);

		vec1 +=4;
		vec2 +=4;
	}

	while(m--)
		sum += *vec1++ * *vec2++;


	return sum;
}

#endif /* NEURALMATH_H_ */
