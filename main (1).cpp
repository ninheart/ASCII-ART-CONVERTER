
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <istream>


// Helper function to read the provided font from a file.  The format
// of the font file is described in comments below.  The width,
// height, and bitmap_letters variables are set by this function.
void ReadFont(const std::string &font_file,
              int &width,
              int &height,
              std::vector<std::vector<std::string> > &bitmap_letters) {

    // open the font file for reading
    std::ifstream istr(font_file.c_str());
    if (!istr) {
        std::cerr << "ERROR: cannot open font file " << font_file << std::endl;
        exit(0);
    }

    // read in the width & height for every character in the file
    istr >> width >> height;
    assert (width >= 1);
    assert (height >= 1);

    // Create a vector to store all 256 ASCII characters of the
    // characters.  Each character is represented as a vector of
    // <height> strings that are each <width> wide.  Initially the
    // characters are unknown (represented with the '?' character).
    bitmap_letters = std::vector<std::vector<std::string> >
            ( 256, std::vector<std::string> ( height, std::string(width, '?')));

    // read in all the characters
    // first is the ascii integer representation of the character
    int ascii;
    while (istr >> ascii) {
        assert (ascii >= 0 && ascii < 256);
        // next the character is printed in single quotes
        char c;
        istr >> c;
        assert (c == '\'');
        // use std::noskipws to make sure we can read the space character correctly
        istr >> std::noskipws >> c;
        // verify that the ascii code matches the character
        assert (c == (char)ascii);
        // switch back to std::skipws mode
        istr >> std::skipws >> c;
        assert (c == '\'');
        // read in the letter
        std::vector<std::string> bitmap;
        std::string tmp;
        for (int i = 0; i < height; i++) {
            istr >> tmp;
            assert ((int)tmp.size() == width);
            // make sure the letter uses only '#' and '.' characters
            for (unsigned int j = 0; j < tmp.size(); j++) {
                assert (tmp[j] == '.' || tmp[j] == '#');
            }
            bitmap.push_back(tmp);
        }
        // overwrite the initially unknown letter in the vector
        bitmap_letters[ascii] = bitmap;
    }
}
//separate function for read function
std::string read_opt(std::string bitmap_file, char* input_file){
    std::string output = "";
    std::vector<std::vector<std::string> > bitmap_letters;
    //set the vector for storing bitmap letters;
    std::vector<std::string>str = std::vector<std::string>(256 , "");
    char background;
    int width=6;
    int height=7;
    ReadFont(bitmap_file, width, height, bitmap_letters);
    std::ifstream infile(input_file);
    //infile>>std::noskipws;
    std::string r;
    //use getline to read in whitespaces;
    std::getline(infile,r);
    //set the background character as the character between each letter since they all are the background of the input file
    background = r[width];
    infile.close();
    infile.open(input_file);
    int line_length = r.size();
    //read input file letters by letters and store into read_letter vector;
    std::vector<std::string>read_letter = std::vector<std::string> (line_length/(width+1), "");
    while(std::getline(infile,r)){
        int count = 0;
        int dex = 0;
        std::string s = "";
        for(int i = 0; i<line_length; i++){
            if(count != width){ //swap the foreground and background of each letter to pound and dot, which matches the ascii art file;
                if(r[i] == background) s += '.';
                else s += '#';
                count++;
            }else{
                read_letter[dex] += s; // when the count equals the width, add the ascii art to read_letter vector index;
                count = 0; //reset count to 0 to read the next characer;
                dex++; //index of the vector increases;
                s = "";//ready to add the new ascii art character;
            }
        }
    }
    //add bitmap ascii art in the font file into a vector by the character's height(7);
    for(int i = 0; i<95; i++){//i<95 because the ascii number starts at 32 instead of 0;
        for(int j = 0; j < height; j++){
            str[i] += bitmap_letters[i+32][j];
        }
    }
    //two strings; first one is the input file string, and the second one is the font file string
    std::string compare1;
    std::string compare2;

    //loop over every string in the read_letter vector;
    for(std::string::size_type i = 0; i<read_letter.size(); i++){
        //set the first string
        compare1 = read_letter[i];
        // loop over the entire ascii font strings in str vector;
        for(std::string::size_type j = 0; j < str.size(); j++){
            compare2 = str[j];
            //std::cout<<compare2<<std::endl;
            //if the string match; output string gets the character based on the ascii number of the char
            if(compare1.compare(compare2) == 0){
                output+=char(j+32);
            }
        }
    }
    //output the final string;
    return output;
}

int main(int argc, char* argv[]) {
    //if the user type in display;
    if(argc != 6 && std::string(argv[1]) == "display"){
        std::cerr<<"invalid input"<<std::endl;
        std::exit(1);
    }else if(std::string(argv[1]) == "read" && argc != 4){
        std::cerr<<"invalid input"<<std::endl;
        std::exit(1);
    }
    std::vector<std::vector<std::string> > bitmap_letters;
    int width;
    int height;
    ReadFont(std::string(argv[2]),width,height,bitmap_letters);
    /*loop over each line of the corresponding ascii character. Replace foreground and background with the
     * user's input and print out the result */
    if(std::string(argv[1]) == "display"){
        for(int i = 0; i < height; i++){
            //obtain the ascii number of each letter from the input string
            for(std::string::size_type  j = 0; j<std::string(argv[3]).size(); j++){
                //change each foreground and background
                //access each character
                for(int k = 0; k < 6; k++){
                    if(bitmap_letters[int(std::string(argv[3])[j])][i][k] == '.'){
                        //change the background to user's option if it's a dot
                        bitmap_letters[int(std::string(argv[3])[j])][i][k] = *argv[5];
                    }else if(bitmap_letters[int(std::string(argv[3])[j])][i][k] == '#'){
                        //change the foreground to user's option if it's a pound
                        bitmap_letters[int(std::string(argv[3])[j])][i][k] = *argv[4];
                    }
                    //display the character
                    std::cout<<bitmap_letters[int(std::string(argv[3])[j])][i][k];
                }
                //separate each character by the background character;
                std::cout<<argv[5];
            }

            std::cout<<std::endl;
        }
    }else if(std::string(argv[1]) == "read"){ //if user chooses to read ascii to strings
        std::string output_string = read_opt(std::string(argv[2]),argv[3]); //call read function above
        std::cout<<output_string;
    }

}

