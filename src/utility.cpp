#include "utility.hpp"


std::map< char, nuc > charmap{
    {'A', A}, {'a', A},
    {'C', C}, {'c', C},
    {'G', G}, {'g', G},
    {'T', T}, {'t', T},
    {'N', N}, {'n', N},
    {'-', N}, {'.', N}
};

std::map< char, nuc > complementmap{
    {'A', T}, {'a', T},
    {'C', G}, {'c', G},
    {'G', C}, {'g', C},
    {'T', A}, {'t', A},
    {'N', N}, {'n', N},
    {'-', N}, {'.', N}
};

std::map< nuc, char > backwardsmap {
    {A, 'A'},
    {C, 'C'},
    {G, 'G'},
    {T, 'T'},
    {N, 'N'}
};




/// Program starting
//-------------------------------------------------------------------------


PROCEDURE whatToDo() {
    // system("Clear");
    PROCEDURE whatToDo;
    unsigned int answer;
    std::cout << "Welcome to our program. What would you like to do? \n";
    std::cout << "Enter 1 to obtain a probability weight matrix from multiple" << std::endl
              << "sequences." << std::endl;
    std::cout << "Enter 2 to obtain all binding positions of a protein from a " << std::endl
              << "probability matrix on a given nucleotide sequence." << std::endl;
    std::cout << "Enter 0 to exit the program." << std::endl;
    
    while (true) {
        std::cin >> answer;
        
        if (answer == 1) {
            whatToDo = MatrixFromSequences;
            break;
        }
        else if (answer == 2) {
            whatToDo = SequencesFromMatrix;
            break;
        }
        else if (answer == 0) {
            whatToDo = Exit;
            break;
        }
        else {
            std::cout << "Unrecognized input. Please try again." << std::endl;
        }
    }
    
    return whatToDo;
    
}


/// Matrix reading
//-------------------------------------------------------------------------


void AskNameMatrix(std::string& entry_name)
{
    std::cout <<"Please give the name of your matrix file: ";
    std::cin >>entry_name;
    
    std::ifstream entry(entry_name.c_str());
    
    if (entry.fail()) {
        std::string error("Impossible to read the file: ");
        error+=entry_name;
        throw error;
    }
    entry.close(); // Don't you have to close it afterwards?

}
//-------------------------------------------------------------------------



///Sequence
//-------------------------------------------------------------------------



std::vector <Sequence> Initialization()
{
    std::string entry_name;		// This string will contain the name of the file we want to open.
    
    try {
        AskNameFasta(entry_name);
    }
    catch(std::string& err) {
        std::cerr <<"Error: " <<err <<std::endl;
        std:: cerr <<"Program stop. ";
        exit(1); // Stops the program.
    }
    
    std::vector <Sequence> sequence_list;
    
    std::vector <std::string> sequences(ExtractSequence(entry_name));
    for(size_t i(0); i<sequences.size(); ++i){
        Sequence seq(sequences[i]);
        sequence_list.push_back(seq);
    }
    
    return sequence_list;
}

void AskNameFasta(std::string& entry_name)
{
    std::cout <<"Please give the name of your sequence file: ";
    std::cin >>entry_name;
    
    std::ifstream entry(entry_name.c_str());
    
    if (entry.fail()) {
        std::string error("Impossible to read the file:");
        error+=entry_name;
        throw error;
    }
    entry.close(); // Don't you have to close it afterwards?
    
    if(InvalidFormat(entry_name)) {
        throw std::string("Unknown format.");
    }
}

std::vector <std::string> ExtractSequence(std::string const& entry_name)
{
    std::ifstream entry(entry_name.c_str());
    std::string line;
    
    std::vector <std::string> sequences; // That is the object that will be returned.
    
    std::string intermediate_value;
    
    // Reads lines
    while(getline(entry, line)){
        if(line.front()=='>'){
            sequences.push_back(intermediate_value);
            intermediate_value.clear(); }
        //else if(line.front()=='\n'){}
        else {
            
            // Makes DNA Sequence on line all uppercase, allowing for easier search afterwards
            for (auto & c: line) c = toupper(c);
            intermediate_value+=line;
        }
    }
    
    
    
    // Allow to register the last value even though there's no >...
    sequences.push_back(intermediate_value);
    
    // Deletes the first vector created, which is a ghost one. This is just a hack cobbled together, it'd probably be best to correct that in a better way at some point.
    sequences.erase(sequences.begin());
    
    
    // Testing the values by showing them
    /*for(size_t i(0); i<sequences.size(); ++i){
     std::cout <<sequences[i] <<'\n';
     }*/
    
    entry.close(); // Don't you have to close it afterwards?
    return sequences;
}

bool InvalidFormat(std::string file_name)
{
    
    // Defines list with known file formats
    static const std::vector<std::string> validValues {".fasta", ".fas", ".fna", ".ffn"};
    
    for(unsigned int i = 0; i < validValues.size(); i++) {
        if(file_name.find(validValues[i])!=std::string::npos)
            return 0;
        // Returns 0 if the file extension can be found
    }
    
    return 1;
}

std::vector <double> nucleotide_probability(
    std::vector <std::string> nucleotide_sequences,
    unsigned int position
) {
    
    std::vector <unsigned int> nucleotide_counter(4, 0);
    std::vector <double> probabilities(4, 0);
    // std::array <unsigned int, 4> nucleotide_counter = {0,0,0,0};
    // std::array <double, 4> probabilities;
    
    unsigned int number_sequences(nucleotide_sequences.size());
    
    
    assert(number_sequences>0);
    
    for (unsigned int i(0); i<number_sequences; i++) {
        switch (nucleotide_sequences[i][position]) {
            case 'A':
                nucleotide_counter[0]++;
                break;
            case 'C':
                nucleotide_counter[1]++;
                break;
            case 'G':
                nucleotide_counter[2]++;
                break;
            case 'T':
                nucleotide_counter[3]++;
                break;
            default:
                std::cout << "WARNING, nucleotide " << nucleotide_sequences[i][position]
                          << " not recognized" << std::endl;
                return {0,0,0,0};
                // Better error handling needed!!
        }
    }
    
    for (unsigned int i(0); i < 4; i++) {
        probabilities[i] = (double)nucleotide_counter[i] / (double)number_sequences;
    }
    return probabilities;
}


Matrix_Neo generate_PWM_from_Seq_list(std::vector <Sequence> sequence_list, bool entire_sequence) {
    Matrix_Neo probability_matrix;
    std::vector <std::string> binding_sequences;
    
    unsigned int position;
    unsigned int length;
    unsigned int number_sequences(sequence_list.size());
    
    if(!entire_sequence) {
        // todo if only a part of the sequence is relevant
        while (true) {
            std::cout << "At what position is the binding site (starts at 1)?" << std::endl;
            std::cin >> position;
            std::cout << "How long is the binding site?" << std::endl;
            std::cin >> length;
        
            if(position <= 0) {
                std::cout << "Error, binding position must be positive" << std::endl;
            }
            else if(length <= 0) {
                std::cout << "Error, binding site length must be positive" << std::endl;
            }
            else {
                break;
            }
        }
    }
    
    else {
        position = 1;
        length = sequence_list[0].length();
    }
    
    for (unsigned int i(0); i < number_sequences; i++) {
        binding_sequences.push_back(
                                    sequence_list[i].access_sequence_pos(
                                                                         position-1,
                                                                         length
                                                                         )
                                    );
    }
    // Goes through all sequences, extracts binding sites and
    // stores them in vector binding_sequences
    

    for(unsigned int i(0); i < length; i++) {
        probability_matrix.push_back(nucleotide_probability(binding_sequences, i));
    }
    // Goes through every position and calculates probabilities at that position
    
    
    /* FOR TESTING
    for(int i(0); i < probability_matrix.size(); i++) {
        for(int j(0); j < 4; j++) {
            std::cout << probability_matrix[i][j] << "\t";
        }
        std::cout << "\n";
    }
     */
    
    return probability_matrix;
}


Matrix_Neo matrix_from_sequence_results(std::string filename) {
    
    std::ifstream entry(filename);
    std::string line;
    
    std::vector <Sequence> sequences; // That is the object that will be returned.
    std::string nucleotides;
    
    
    // Reads lines, converts line to stream, reads the first word and creates a sequence out of it
    while(getline(entry, line)){
        
        std::istringstream stringstream(line);
        stringstream >> nucleotides;
        
        Sequence sequence(nucleotides);
        sequences.push_back(sequence);
    }
    
    entry.close();
    return generate_PWM_from_Seq_list(sequences, true);
}




//==========================================================================================
// I am very sorry for the uglyness of this function

std::vector<SearchResults> analyze_sequence(std::string filename, Matrix matrix, double cutoff) {
    
    // File to read
    std::ifstream entry_file(filename);
    
    // Search results of all sequences
    std::vector <SearchResults> output;
    
    // Search results of one sequence
    SearchResults sequence_matches;
    
    SearchResult sequence_match;
    
    std::list<nuc> forwardSequence;
    std::list<nuc> backwardSequence;
    
    bool fill(true);
    
    char character;
    
    unsigned int char_counter(0);
    unsigned int length(matrix.get_length());
    unsigned int sequence_counter(0);
    unsigned int position_counter(0);
    
    double score;
    
    
    
    
    while(entry_file.get(character)) {
        // Skip if endline is found
        if(character == '\n')
            break;
        
        // What to do if current line is description
        if(character == '>' or character == ';') {
            if (sequence_counter) {
                output.push_back(sequence_matches);
                sequence_matches = SearchResults(); // Reset to 0
                position_counter = 0;
            }
            
            getline(entry_file, sequence_matches.description);
            sequence_matches.description += "\n";
            break;
        }
        
        // What to do if the sequence hasn't been filled yet with sufficient characters
        if(fill) {
            forwardSequence.push_back(charmap[character]);
            backwardSequence.push_front(complementmap[character]);
            char_counter++;
            
            if(char_counter >= length)
                fill = false;
            break;
        }
        
        // Checks if character is valid
        auto it = charmap.find(character);
        if (it == charmap.end())
        {
            std::cout << "Unknown character. Character is skipped and ignored\n";
            break;
        }
        position_counter++;
        
        // Updates sequence with the new character
        forwardSequence.pop_front();
        forwardSequence.push_back(charmap[character]);
        backwardSequence.pop_back();
        backwardSequence.push_front(charmap[character]);
        
        // What to do if forward is binding
        score = matrix.sequence_score(forwardSequence);
        if(score >= cutoff) {
            std::list<nuc>::iterator iterator;
            
            for (iterator = forwardSequence.begin(); iterator != forwardSequence.end(); iterator++)
                sequence_match.sequence += backwardsmap[*iterator];
            
            sequence_match.position = position_counter;
            sequence_match.score = score;
            sequence_match.direction = '+';
            
            sequence_matches.searchResults.push_back(sequence_match);
            sequence_match = SearchResult(); // Empty match
        }
        
        // What to do if backward is binding
        score = matrix.sequence_score(backwardSequence);
        if(score >= cutoff) {
            std::list<nuc>::iterator iterator;
            
            for (iterator = backwardSequence.begin(); iterator != backwardSequence.end(); iterator++)
                sequence_match.sequence += backwardsmap[*iterator];
            
            sequence_match.position = position_counter;
            sequence_match.score = score;
            sequence_match.direction = '-';
            
            sequence_matches.searchResults.push_back(sequence_match);
            sequence_match = SearchResult(); // Empty match
        }
    }
    
    entry_file.close();
    return output;
}


//==========================================================================================
void print_results(SearchResults results, std::string filename) {
    std::ofstream outputfile;
    outputfile.open(filename);
    unsigned int size = results.searchResults.size();
    
    outputfile << results.description << std::endl;
    
    
    for (unsigned int i(0); i < size; i++) {
        outputfile << results.searchResults[i].sequence << " found at position "
                   << results.searchResults[i].position << " in "
                   << results.searchResults[i].direction << " direction with the score "
                   << results.searchResults[i].score << std::endl;
    }
    outputfile.close();
}


//==========================================================================================
void print_results(SearchResults results) {
    unsigned int size = results.searchResults.size();
    
    std::cout << results.description << std::endl;
    
    
    for (unsigned int i(0); i < size; i++) {
        std::cout << results.searchResults[i].sequence << " found at position "
                << results.searchResults[i].position << " in "
                << results.searchResults[i].direction << " direction with the score "
                << results.searchResults[i].score << std::endl;
    }
}




