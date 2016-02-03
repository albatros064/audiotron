#ifndef AUDIOTRON_BASE_H
#define AUDIOTRON_BASE_H

#include <vector>

namespace Audiotron {

    using std::vector;

    const float minimumAudibility = 0.001f;

    class Sampler {
    public:
        Sampler() :frequency(0) {}
        Sampler(unsigned);
        ~Sampler() {}

        unsigned getSamples(float duration  ) const { return (unsigned) (duration / 1000.0f * frequency); }
        float    getMillis (unsigned samples) const { return (float) samples / (float) frequency * 1000.0f; }
        unsigned getFrequency() const { return frequency; }

        void setFrequency(unsigned _frequency) { frequency = _frequency; }

    private:
        unsigned frequency;
    };

    class Envelope {
    };


    class SampleBuffer {
    public:
        SampleBuffer(unsigned);
        SampleBuffer(unsigned, unsigned);
        SampleBuffer(Sampler, float);
        SampleBuffer(Sampler, float, float);
        ~SampleBuffer() {}

        void setStart(unsigned _start) { start = _start;  }
        void setStart(Sampler sampler, float _start) { start = sampler.getSamples(_start); }
        unsigned getStart () const { return start;               }
        unsigned getEnd   () const { return start + getLength(); }
        unsigned getLength() const { return samples.size();      }

        void resize(unsigned size) { samples.resize(size, 0.0); }

        vector<float>::reference       operator[](unsigned i)       { return samples[i]; }
        vector<float>::const_reference operator[](unsigned i) const { return samples[i]; }

        void scaleAmplitude(float);
        void normalize();

        void apply(Envelope);

        void mixInto(SampleBuffer&);


        void drawWaveform(unsigned, bool);

    private:
        unsigned start;
        vector<float> samples;
    };

    SampleBuffer mixBuffers(vector<SampleBuffer>);

} // namespace Audiotron

#endif

