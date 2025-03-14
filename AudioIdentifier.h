#include <vector>
#include <string>
#include <iostream>

class SongClassifier{
private:
    std::vector<bool> weights;
    std::vector<bool> neurons;

    std::vector<std::string> song_names_table;

    void songToNeurons(const std::vector<double> &ft, int s_rate);
    void sculpt();
    void updateNeurons();

    void embedName(const std::string &song_name);
    std::string recoverName() const;

    size_t hashName() const;

public:
    SongClassifier(int num_neurons = 100);
    
    void sculpt(const std::vector<double> &ft, int s_rate, const std::string &song_name);
    void sculpt(const std::vector<double> &ft, int s_rate, std::string &&song_name);

    std::string identify(const std::vector<double> &ft, int s_rate);
};

SongClassifier::SongClassifier(int num_neurons){
    song_names_table.resize(num_neurons/10);
    neurons.resize(num_neurons);
    weights.resize(num_neurons*num_neurons);
}

void SongClassifier::sculpt(const std::vector<double> &ft, int s_rate, const std::string &song_name){
    songToNeurons(ft, s_rate);
    sculpt();
    embedName(song_name);
}

void SongClassifier::sculpt(const std::vector<double> &ft, int s_rate, std::string &&song_name){
    songToNeurons(ft, s_rate);
    sculpt();
    embedName(song_name);
}

std::string SongClassifier::identify(const std::vector<double> &ft, int s_rate){
    songToNeurons(ft, s_rate);
    updateNeurons();
    return recoverName();
}

void SongClassifier::songToNeurons(const std::vector<double> &ft, int s_rate){
    double sum = 0;
    int mid_cut = 1000*ft.size()/s_rate;
    for (int i = 0; mid_cut > i; ++i){
        sum += ft[i];

        if ( i % ( mid_cut/ (neurons.size()/2) ) == 0){
            int pos = i / (mid_cut / (neurons.size()/2) );
            double avg = sum/ (mid_cut / (neurons.size()/2) );
            neurons[pos] = (avg > 400)? true: false;
            sum = 0;
        }
    }

    sum = 0;
    int spacing = (ft.size() - mid_cut)/(neurons.size()/2);
    for (int j = 0; j < ft.size() - mid_cut; ++j){
        sum += ft[j + mid_cut];

        if ( j % spacing == 0){
            int pos = (j / spacing) + mid_cut;
            double avg = sum / spacing;
            neurons[pos] = (avg > 200)? true : false;
            sum = 0;
        }
    }
}

void SongClassifier::sculpt(){
    for (int i = 0; i < neurons.size(); ++i){
        for (int j = 0; j < neurons.size(); ++j){
            if (i != j)
                weights[j + i*neurons.size()] = (neurons[i] == neurons[j]);
        }
    }
}

void SongClassifier::updateNeurons(){
    std::vector<bool> prev;
    
    int last_resource = 0;
    do{
        prev = neurons;
        for (int i = 0; i < neurons.size(); ++i){
            int sum = 0;
            for (int j = 0; j < neurons.size(); ++j)
                if (i != j){
                    sum += (neurons[j] == weights[j + i*neurons.size()])? 1 : -1;
                }
            
            if (sum > 0)
                neurons[i] = true;
            else
                neurons[i] = false;
        }

        last_resource++;
    } while (neurons != prev && last_resource < 100000);
}

void SongClassifier::embedName(const std::string &song_name){
    size_t pos = hashName();
    song_names_table[pos] = song_name;
}

std::string SongClassifier::recoverName() const{
    size_t pos = hashName();
    return song_names_table[pos];
}

size_t SongClassifier::hashName() const{
    size_t seed = neurons.size();
    for (auto n : neurons)
        seed ^= n + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    return seed % song_names_table.size();
}
