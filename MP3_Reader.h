#include <vector>
#include <fstream>
#include <string>
#include <mpg123.h>

class MP3Reader {
public:
    MP3Reader();
    MP3Reader(MP3Reader &&) = default;
    MP3Reader(const MP3Reader &) = default;
    MP3Reader &operator=(MP3Reader &&) = default;
    MP3Reader &operator=(const MP3Reader &) = default;
    ~MP3Reader();
    std::vector<double> readToVector(const std::string &);
    int getSampleRate() const {return rate;}
    int getChannels() const {return channels;}

private:
    mpg123_handle *handle;
    long rate;
    int channels, encoding;
};

MP3Reader::MP3Reader() {
    mpg123_init();
    handle = mpg123_new(NULL, NULL);

    if (!handle) throw std::runtime_error("Unable to create mpg123 handle");
}

MP3Reader::~MP3Reader() {
    if (handle){
        mpg123_close(handle);
        mpg123_delete(handle);
    }
    mpg123_exit();
}

std::vector<double> MP3Reader::readToVector(const std::string &filename){
    std::vector<double> audioData;

    if (mpg123_open(handle, filename.c_str()) != MPG123_OK ||
        mpg123_getformat(handle, &rate, &channels, &encoding) != MPG123_OK)
        throw std::runtime_error("Error opening MP3 file or reading format");

    mpg123_format_none(handle);
    mpg123_format(handle, rate, channels, MPG123_ENC_SIGNED_16);

    const size_t BUFFER_SIZE = 4096;
    size_t bytesRead = 0;
    unsigned char buffer[BUFFER_SIZE];

    while (mpg123_read(handle, buffer, BUFFER_SIZE, &bytesRead) == MPG123_OK && bytesRead > 0){
        short *samples = reinterpret_cast<short*>(buffer);
        size_t numSamples = bytesRead / sizeof(short);

        for (size_t i = 0; i < numSamples; ++i){
            float sample = static_cast<double>(samples[i]) / 32768.0f;
            audioData.push_back(sample);
        }
    }

    return audioData;
}
