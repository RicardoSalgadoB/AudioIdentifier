# Audio Identifier
A software for ... wonder what? Identifying audios!!!

## Discrete Fourier Transform
This was implemented through the [FFTW library](https://fftw.org/fftw3.pdf) and a real to complex one dimensional transform. Then the modulus of the complex numbers was extracted.

The MP3 were preprocessed into a vector with the MPG123 library and a little help from *Claudius*.

## Hopfield Network
This Physics Nobel Award winning concept is used to give the model some flexibility.

The Network tends to "minimize the energy in the system" and at these minimum points lie the audios that the user is trying to identify.

Most of the intuition behind this model can be learned from this [video](https://youtu.be/1WPJdAW-sFo?feature=shared).

Each neuron is mapped to a certain threshold (arbitrary for now) in a given range.

**NOTES**
* There is a little tiny error in the implementation of this, but discovering it is left as an exercise to the reader.

## Hash Table
Only Hash for now... I need to implement a proper Hash Table when I return.

The capacity of the list is the number of neurons divided by 10.

I got the hash function for [here](https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector).

## Other Considerations
When typing the file_name don't use spaces. And of course don't try to overflow the buffer.

Put the audios to be analyzed in a Folder named "Audios" within this repo.

As I said, I will be further updating this when I return (I won't elaborate further on the meaning of this). I am to adjust this to be able to identify songs through the microphone and I have a rough idea of how to do this.

Either way, farewell.
