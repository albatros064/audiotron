#include "AudiotronBase.h"

#define  _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace Audiotron {

    SampleBuffer mixBuffers(vector<SampleBuffer> buffers) {
        unsigned start = 0;
        unsigned end = 0;

        vector<SampleBuffer>::iterator it;

        // Determine the size of the final buffer, to save a lot of reallocation
        for (it = buffers.begin(); it != buffers.end(); ++it) {
            unsigned testEnd = (*it).getEnd();
            unsigned testStart = (*it).getStart();
            if (testEnd > end) {
                end = testEnd;
            }
            if (testStart < start) {
                start = testStart;
            }
        }

        // Allocate a new buffer
        SampleBuffer waveForm(start, end - start);

        // And begin the mix-in
        for (it = buffers.begin(); it != buffers.end(); ++it) {
            /*for (unsigned i = 0; i < (*it).getLength(); ++i) {
                std::cout << (*it)[i] << std::endl;
            }*/
            (*it).mixInto(waveForm);
        }

        return waveForm;
    }

    SampleBuffer::SampleBuffer(unsigned _start = 0) :start(_start) {}
    SampleBuffer::SampleBuffer(unsigned _start, unsigned length) : start(_start), samples(length, 0.0) {}
    SampleBuffer::SampleBuffer(Sampler sampler, float _start = 0.0) {
        start = sampler.getSamples(_start);
    }
    SampleBuffer::SampleBuffer(Sampler sampler, float _start, float _length) {
        start = sampler.getSamples(_start);
        samples.resize(sampler.getSamples(_length), 0.0);
    }

    /**
    */
    void SampleBuffer::mixInto(SampleBuffer &buffer) {
        if (getStart() < buffer.getStart()) {
            // throw an exception?
        }
        if (getEnd() > buffer.getEnd()) {
            buffer.resize(getEnd() - buffer.getStart());
        }

        unsigned sourceIndex;
        unsigned targetIndex = getStart() - buffer.getStart();

        for (sourceIndex = 0; sourceIndex < getLength(); ++sourceIndex, ++targetIndex) {
            buffer[targetIndex] += samples[sourceIndex];
        }
    }

    void SampleBuffer::scaleAmplitude(float scale) {
        for (unsigned i = 0; i < samples.size(); ++i) {
            samples[i] *= scale;
        }
    }

    void SampleBuffer::normalize() {
        float maxScale = 0.0f;
        for (unsigned i = 0; i < samples.size(); ++i) {
            float sample = samples[i];
            if (std::fabs(sample) > maxScale) {
                maxScale = std::fabs(sample);
            }
        }

        scaleAmplitude(1.0f / maxScale);
    }

    void SampleBuffer::apply(Envelope envelope) {
        // Do nothing for now...
    }

    void SampleBuffer::drawWaveform(unsigned width, bool normalize = false) {
        if (normalize) {
            this->normalize();
        }
        
        for (unsigned i = 0; i < getLength(); ++i) {
            int spaces = (samples[i] + 1.0) * width;
            for (int i = 0; i < spaces; ++i) {
                std::cout << " ";
            }
            std::cout << "o" << std::endl;
        }
        std::cout << std::endl;
    }

} // namespace Audiotron

