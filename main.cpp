#include "MP3_Reader.h"
#include "AudioIdentifier.h"
#include <vector>
#include <cmath>
#include <string>
#include <fftw3.h>
#include <iostream>
#include <algorithm>

std::pair<std::vector<double>, int> performFT(std::string &&filename){
    MP3Reader reader;
    std::vector<double> audioData = reader.readToVector(filename);

    int N = audioData.size();
    fftw_complex *out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fftw_plan plan = fftw_plan_dft_r2c_1d(N, audioData.data(), out, FFTW_MEASURE);
    
    fftw_execute(plan);

    std::vector<double> out_dist(N/2 + 1, 0);
    for (int i = 0; i <= N/2; ++i)
        out_dist[i] = std::sqrt(out[i][0]*out[i][0]+out[i][1]*out[i][1]);

    fftw_destroy_plan(plan);
    fftw_free(out);

    std::pair<std::vector<double>, int> ret_pair;
    ret_pair.first = out_dist;
    ret_pair.second = reader.getSampleRate();

    return ret_pair;
}

void sculpt(AudioIdentifier &identifier){
    std::string file_name;
    std::string audio_name;

    std::cout << "Write the path to the MP3 file to be identified: ";
    std::cin >> file_name;
    std::cout << "Write the name of the audio: ";
    std::cin >> audio_name;
    std::cout << "Wait..." << std::endl;

    std::pair<std::vector<double>, int> ret_pair = performFT("Audios/" + file_name);
    std::vector<double> ft = ret_pair.first;
    int s_rate = ret_pair.second;

    identifier.sculpt(ft, s_rate, audio_name);
    std::cout << audio_name << " has been embeded into the Network.\n" << std::endl;
}

void identify(AudioIdentifier &identifier){
    std::string file_name;

    std::cout << "Write the path (absolute or relative) to the MP3 file to be identified: ";
    std::cin >> file_name;
    std::cout << "Wait..." << std::endl;

    std::pair<std::vector<double>, int> ret_pair = performFT("Audios/" + file_name);
    std::vector<double> ft = ret_pair.first;
    int s_rate = ret_pair.second;

    std::string identified = identifier.identify(ft, s_rate);
    std::cout << "The audio corresponding or closest to the given file is " << identified << std::endl;
    std::cout << std::endl;
}

int main(){
    AudioIdentifier identifier;
    bool breaker = true;
    int option;
    while (breaker){
        std::cout << "Choose wisely:" << std::endl;
        std::cout << "\t1) Embed an audio into the identifier" << std::endl;
        std::cout << "\t2) Identify an audio" << std::endl;
        std::cout << "\t3) QUIT" << std::endl;
        std::cin >> option;
        std::cout << "Okey..." << std::endl;
        std::cout << std::endl;

        switch(option){
            case 1:
                sculpt(identifier);
                break;
            case 2:
                identify(identifier);
            break;
            case 3:
                breaker = false;
                break;
        }
    }
}
