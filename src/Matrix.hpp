

// Created by Erica Geneletti and Katia Schalk on 5/10/16

#include <iostream>

Using namespace std;

class Matrix
{
	private:
	vector<vector<double>> logMatrix;
	vector<vector<double>> probMatrix;
	Matrix(string fileName); // cobstructor
	void probToLog();
	// add an atribute sequence ???
	public :
	
	/* Returns the probability for a nucleotide in a specific position.
	The first argument corresponds to the column: the nucleotide who interest us.
	The second argument corresponds to the lign: the place of this nucleotide in the sequence of 7 nucleotides.*/
	static double getProbability (char const N, int const l); // Remove static when everything works
	
};