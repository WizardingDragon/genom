//
//  procedures.cpp
//
//
//  Created by Mättu on 15.11.16.
//
//

#include "procedures.hpp"


void enzyme_on_sequence() {

    system("Clear");

    std::string file_name_matrix;

    Matrix enzyme(ask_name_matrix());

	std::string file_name(Ask_Outputfile_Name());
    // DEBUG_
    enzyme.print_log_matrix();

    std::vector<SearchResults> enzyme_matches;
    enzyme_matches = analyze_sequence_opt2(ask_name_fasta(), enzyme, ask_cutoff());

    for (unsigned index(0); index < enzyme_matches.size(); index++) {
        //print_results(enzyme_matches[index]);
        print_results(enzyme_matches[index],file_name);
    }

}


void enzyme_from_sequences() {
    if(ask_binding_length_known())
        binding_length_known();
    else
        binding_length_unknown();
}


void binding_length_known() {
    std::vector <Sequence> sequence_list;
    sequence_list = Initialization();
    Matrix enzyme(generate_PWM_from_Seq_list(sequence_list, false),absoluteMatrix);
    //enzyme.save_matrix_loop();
}

void binding_length_unknown() {
  //EM implementation

}
