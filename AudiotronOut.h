#ifndef AUDIOTRON_OUT_H
#define AUDIOTRON_OUT_H

#include "AudiotronBase.h"

#include <string>

namespace Audiotron {

    using std::vector;
    using std::string;

    class AudioFile {
    public:
        void in(string fileName);
        void out(string fileName);

        void setSampler(Sampler);
        void setBuffer (SampleBuffer, unsigned);
        Sampler      getSampler();
        SampleBuffer getBuffer(unsigned channel) { if (channel < getChannels()) { return buffers[channel]; } return buffers[0]; }

        unsigned getChannels() const { return buffers.size(); }
    private:
        Sampler sampler;
        
        vector<SampleBuffer> buffers;
    };

} // namespace Audiotron

#endif

