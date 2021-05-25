#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "ansi_escapes.h"


std::vector<std::string> load_file(std::vector<std::string> &descriptions){
	std::vector<std::string> word_list;
	std::string filename;
	std::cout << "Enter the filename:";
	std::cin >> filename;
	std::ifstream file(filename);
	std::string raw_line;
	if(file.is_open()){
		while(getline(file, raw_line)){
			int pos = raw_line.find(":");
			std::string word = raw_line.substr(0, pos);
			std::string description = raw_line.substr(pos + 1, raw_line.size() -1);
			word_list.push_back(word);
			descriptions.push_back(description);
			
		}
		file.close();
	}
	else{
		std::cout << filename << " was not loaded properly"<< std::endl;
	}
	return word_list;	
}
std::string pick_word(std::vector<std::string> &word_list, int &pos){
	 pos = (rand() % word_list.size());
	 std::string picked_word = word_list[pos];
	 word_list.erase(word_list.begin() + pos);
	 return picked_word; 
}
std::string pick_description(std::vector<std::string> &descriptions, int &pos){
	 std::string picked_description = descriptions[pos];
	 descriptions.erase(descriptions.begin() + pos);
	 return picked_description; 
}
void input_handle(std::string picked_word, std::string description, std::vector<std::string> &word_list, std::vector<std::string> &miss_words, int &correct_words){
	std::string in_word;
	std::cout << "description:"<< description<< std::endl;
	std::cout << ">";
	std::cin >> in_word;
	std::string out_word;
	if(in_word == "exit" or in_word == "q"){
		word_list.clear();
	
	}
	else if(!picked_word.compare(in_word)){
		std::cout << "\x1b[1;32mCorrect!\x1b[0m" << std::endl;
		correct_words++;
	}
	else{
		for(int i = 0; i < picked_word.length(); i++){
			if(in_word[i] == picked_word[i]){
				// append green ansi escape char
				out_word = out_word + "\x1b[1;32m" + in_word[i];
			}
			else if(in_word.length() - 1 < i){
				break;
			}
			else{
				// append red ansi escape char
				out_word = out_word + "\x1b[9;31m" + in_word[i];
				
			}
		}
		for(int i = picked_word.length() - 1; i < in_word.length() - 1; i++){
			// append ansi escape red
			out_word = out_word + "\x1b[9;31m" + in_word[i];
		}
		std::cout << "Incorrect:" << out_word << " \x1b[0mcorrect:" << picked_word << std::endl;
		out_word = picked_word + '\0' + out_word;
		miss_words.push_back(out_word);
	}
}

// debug function
void print(std::vector <std::string> const &a) {
   std::cout << "The vector elements are : ";

   for(int i=0; i < a.size(); i++)
   std::cout << a.at(i) << ' ';
}

void display_stats(std::vector<std::string> &miss_words, int correct_words){
	std::cout << "Stats:\nAmount of correct words:" << correct_words << "\n";
	std::cout << "Percent of correct words:" <<  (float)correct_words / (float)(correct_words + miss_words.size()) * 100 << "%\n";
	std::cout << "missing words include:" << "\n";
	for( int i = 0; i < miss_words.size(); i++){
		std::string CorrectTempWord = "";
		std::string IncorrectTempWord = "";
		bool Incorrect_word = false;
		for(int a = 0; a < miss_words[i].length(); a++){
			if( miss_words[i][a] == '\0'){
				Incorrect_word = true;
				continue;
			}
			else if(Incorrect_word){
				IncorrectTempWord += miss_words[i][a];
			}
			else{
				CorrectTempWord += miss_words[i][a];
			}
		}
		std::cout << "\x1b[0mCorrect:\x1b[1;32m " << CorrectTempWord << " \x1b[0mUser: " << IncorrectTempWord << "\n";
	}
	
}
int main(){
	setupConsole();
	std::vector<std::string> descriptions;
	std::vector<std::string> miss_words;
	int pos;
	srand((unsigned) time(0));
	std::cout << "welcome to spelling exe"<< std::endl;
	std::vector<std::string> word_list = load_file(descriptions);
	int correct_words = 0;
	while( !word_list.empty()){
		std::string picked_word = pick_word(word_list, pos);
		std::string picked_description = pick_description(descriptions, pos);
		input_handle(picked_word, picked_description, word_list, miss_words, correct_words);
	}
	display_stats(miss_words, correct_words);
	restoreConsole();
	system("PAUSE");
	
	return 0;
}