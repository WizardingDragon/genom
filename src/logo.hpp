#define cimg_use_png
#define cimg_display 0
#include "../logo/CImg-1.7.9/CImg.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "user_interaction.hpp"
#include <algorithm>

using namespace cimg_library;

std::vector<std::vector<double>> read_logo_matrix(std::string const& fileName);
double size(std::vector<std::vector<double>> const& PWM, unsigned N, unsigned pos);

typedef struct nuc_prob_pair{
    double prob;
    CImg<unsigned char> icon;
    int index;
    nuc_prob_pair(CImg<unsigned char> icon, double prob) : prob(prob), icon(icon){
		
	}
}nuc_prob_pair;

bool compareByProb(const nuc_prob_pair &a, const nuc_prob_pair &b)
{
    return a.prob > b.prob;
}

void logo() {

	std::string fileName(ask_logo_matrix());
	std::vector<std::vector<double>> PWM(read_logo_matrix(fileName));

	unsigned size_motif(PWM.size());

	CImg<unsigned char> background(1, 1, 1, 3, 0, 0, 0);
	background.resize(size_motif*500, 1000);

	logo_in_process();

	for (unsigned pos(0); pos<size_motif; ++pos)
	{
		position_in_process(pos+1,PWM.size());
		
		CImg<unsigned char> iconA("../logo/icons/A.png");
		CImg<unsigned char> iconC("../logo/icons/C.png");
		CImg<unsigned char> iconG("../logo/icons/G.png");
		CImg<unsigned char> iconT("../logo/icons/T.png");

		CImg<unsigned char> column(1, 1, 1, 3, 255, 255, 255);
		column.resize(500, 1000);

		double height(0);
		
		
		std::vector<nuc_prob_pair> nuc_pairs;
		
		nuc_pairs.push_back(nuc_prob_pair(iconA, size(PWM,0,pos)));
		nuc_pairs.push_back(nuc_prob_pair(iconC, size(PWM,1,pos)));
		nuc_pairs.push_back(nuc_prob_pair(iconG, size(PWM,2,pos)));
		nuc_pairs.push_back(nuc_prob_pair(iconT, size(PWM,3,pos)));

		std::sort(nuc_pairs.begin(), nuc_pairs.end(), compareByProb);
		
		for(unsigned i = 0; i < nuc_pairs.size(); i++){
			nuc_pairs[i].icon.resize(500, nuc_pairs[i].prob, -100, -100, 2);
			column.draw_image(0, height, 0, 0, nuc_pairs[i].icon);
			height += nuc_pairs[i].prob;
		}
		
		background.draw_image(pos*500, 0, 0, 0, column);

	}
	
	background.save_png("../yourlogo.png");
}



std::vector<std::vector<double>> read_logo_matrix(std::string const& fileName)
{
	/*
	 * to be able to make a logo from any kind of matrix:
	 * opens the file
	 * makes a matrix
	 * converts the log matrix to prob matrix
	 * returns the prob matrix 
	 */
	Matrix mat1(fileName);
	std::vector<std::vector<double>> result(mat1.probMatrix_from_logMatrix());
	
	/*std::vector<std::vector<double>> input_matrix;

    std::ifstream PWM;
    PWM.open(fileName);

    if (PWM.fail())
    {
        throw std::string("Error: Cannot read PWM file");
    }
    else
    {
        std::string line;
        double A, T, C, G;
        std::vector< double > rowi(4);

        while (true) {

            getline(PWM, line);

            if (PWM.eof()) break;

            std::istringstream values(line);
            values >> A >> C >> G >> T;

            rowi.clear();
            rowi.push_back(A);
            rowi.push_back(C);
            rowi.push_back(G);
            rowi.push_back(T);

            input_matrix.push_back(rowi);
        }

        PWM.close();

        return input_matrix;
        
    }*/
    return result;
}

double size(std::vector<std::vector<double>> const& PWM, unsigned N, unsigned pos)
{
	return (PWM[pos][N])*1000;
}

/* commented out because formula causes weird logo
double size(std::vector<std::vector<double>> const& PWM, unsigned N, unsigned pos)
{
	double info_content(0);

	for (unsigned i(0); pos<4; ++pos)
	{
		info_content += ((PWM[pos][i])*(log2(4*PWM[pos][i])));
	}


	std::cout<<"info_content: " << info_content << std::endl;

	return (PWM[pos][N])*1000/info_content;
}
*/


/*

	reasons for choices: if the declaration of the icons is in the for loop
		then they don't lose resolution but the program is slower, otherwise they
		become "ruined" but the program is faster

	final things to implement:
		-print with biggest letter on top
		-correct size calculation
		-check matrix has values > 0 and add up to 1
		-add labels and scale

*/
