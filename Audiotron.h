#ifndef AUDIOTRON_H
#define AUDIOTRON_H

#include "AudiotronBase.h"

namespace Audiotron {

    using std::vector;

    class Wave;
    class Instrument;
    class Note;

    class Wave {
    public:
        enum WaveType { Sine, Triangle, Square, Sawtooth };

        Wave(WaveType, float, unsigned);
        //Wave(Wave&& w) :waveType(w.waveType), harmonic(w.harmonic), amplitude(w.amplitude) {}
        ~Wave() {}

        SampleBuffer generate(Sampler, float, float, float);

        bool isAudible() const { return amplitude > minimumAudibility; }
    private:
        WaveType waveType;
        unsigned harmonic;
        float    amplitude;

        float sampleWave(float);
    };

    class NoteEnvelope : public Envelope {
    public:
        NoteEnvelope();
        NoteEnvelope(float, float, float, float);
        ~NoteEnvelope() {}

        float getDuration(Note);
        unsigned getDuration(Sampler, Note);

        void applyTo(SampleBuffer);

    private:
        float attack;
        float decay;
        float sustain;
        float release;
    };

    class Instrument {
    public:
        Instrument(Wave::WaveType);
        ~Instrument() {}

        void addHarmonic(Wave::WaveType, float, unsigned);
        void removeHarmonic(int);

        void setNoteEnvelope(NoteEnvelope _envelope) { envelope = _envelope; }

        SampleBuffer play(Sampler, Note);
    private:
        NoteEnvelope envelope;
        vector<Wave> waves;
    };

    class Note {
    public:
        Note(float, float, float, float);
        ~Note(){}

        float    getTime()                    const { return time; }
        unsigned getTime(Sampler sampler)     const { return sampler.getSamples(time); }
        float    getDuration()                const { return duration; }
        unsigned getDuration(Sampler sampler) const { return sampler.getSamples(duration); }

        float getPitch    () const { return pitch;     }
        float getAmplitude() const { return amplitude; }

        void setTime(float time) { this->time = time; }
        void setTime(Sampler sampler, int time) { this->time = sampler.getMillis(time); }
        void setDuration(float duration) { this->duration = duration; }
        void setDuration(Sampler sampler, int time) { this->duration = sampler.getMillis(time); }

        void setPitch(float pitch) { this->pitch = pitch; }
        void setAmplitude(float amplitude) { this->amplitude = amplitude; }

        bool isAudible() const { return amplitude > minimumAudibility; }
    private:
        float time;      // Time from start of track at key down, in ms.
        float pitch;     // Frequency, in Hz.
        float duration;  // Time from key down to key up, in ms. Does not include release time.
        float amplitude; // Note volume. -1.0 ... 1.0
    };

    class Track {
    public:
        Track();
        Track(Instrument);
        SampleBuffer play(Sampler);

        void addNote(Note note) { notes.push_back(note); }

    private:
        Instrument instrument;
        vector<Note> notes;

        Envelope envelope;
    };


} // namespace Audiotron

#endif
