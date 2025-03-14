FFT_FLAGS = -I/opt/homebrew/opt/fftw/include -L/opt/homebrew/opt/fftw/lib -lfftw3 -lm
MPG123_FLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lmpg123

main: main.cpp
	g++ main.cpp -o main -std=c++14 $(FFT_FLAGS) $(MPG123_FLAGS)

clean:
	rm main

