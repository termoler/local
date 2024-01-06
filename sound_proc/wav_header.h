#ifndef SOUND_PROCESSOR_WAV_HEADER_H
#define SOUND_PROCESSOR_WAV_HEADER_H

struct wav_header{
    char riff[4];
    unsigned long chunk_size;
    char format[4];
    char subchunk_1_id[4];
    unsigned long subchunk_1_size;
    unsigned short audio_format;
    unsigned short num_channels;
    unsigned long sample_rate;
    unsigned long byte_rate;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char subchunk_2_id[4];
    unsigned long subchunk_2_size;
};
#endif //SOUND_PROCESSOR_WAV_HEADER_H
