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

int main(){
    AudioIdentifier identifier;

    std::pair<std::vector<double>, int> ret_pair1 = performFT("/Users/ricardosalgadob/Desktop/Project-SongClassifier/AudioFiles/ElVasodePaja.mp3");
    std::vector<double> ft1 = ret_pair1.first;
    int s_rate1 = ret_pair1.second;

    identifier.sculpt(ft1, s_rate1, "El vaso de paja");

    std::pair<std::vector<double>, int> ret_pair2 = performFT("/Users/ricardosalgadob/Desktop/Project-SongClassifier/AudioFiles/audio2.mp3");
    std::vector<double> ft2 = ret_pair2.first;
    int s_rate2 = ret_pair2.second;

    identifier.sculpt(ft2, s_rate2, "Audio 2");

    std::pair<std::vector<double>, int> ret_pair3 = performFT("/Users/ricardosalgadob/Desktop/Project-SongClassifier/AudioFiles/audio.mp3");
    std::vector<double> ft3 = ret_pair3.first;
    int s_rate3 = ret_pair3.second;

    std::string x = identifier.identify(ft3, s_rate3);

    std::cout << x << std::endl;
}
