#include "AudiotronOut.h"

#include <iostream>

#include <stdio.h>
#include <sndfile.h>

namespace Audiotron {

    using std::cout;
    using std::endl;

    void AudioFile::in(string fileName) {
        SF_INFO *info = new SF_INFO;
        info->format = 0;

        SNDFILE *file = sf_open(fileName.c_str(), SFM_READ, info);

        if (sf_error(file) != SF_ERR_NO_ERROR) {
            std::cout << sf_strerror(file) << std::endl;
        }

        float *buffer = new float[info->frames * info->channels];

        unsigned count = sf_readf_float(file, buffer, info->frames);

        SampleBuffer buff(0, info->frames);

        buffers.assign(info->channels, buff);

        for (int i = 0; i < count * info->channels; i += info->channels) {
            for (int j = 0; j < info->channels; j++) {
                buffers[j][i] = buffer[i + j];
            }
        }


        sampler.setFrequency(info->samplerate);

        sf_close(file);
        delete info;
        delete [] buffer;
    }

} // namespace Audiotron

