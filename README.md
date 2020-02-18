# Audio-Ingest-Outgest


This is not the full program, as the required JUCE libraries exceed Github's upload size.

Please download the full program files here: https://drive.google.com/open?id=1HpebHgtAlRsbzmxgtazwGgqjdU0EVJxO

Core goals: to utilize existing frameworks in order to best showcase an understanding of DSP code.

This program is a simple audio in/out station with a few additions.
It takes in .wav and .aiff format files and uses a lossless converter to read the metadata.
The user is able to not only select their input and output, but by opening the advanced settings, able to select different sample rates.
This is a routine setting to limit on older machines in order to achieve smooth playback.

There is also a small visualizer tied to audio input. Again, to demonstrate knowledge of DSP code, this is weighted towards an even-tempered brown noise spectrum. This means that mid-range frequencies will excite the visualizer more than high or low frequencies.

Placeholder for demonstration video.
