#include "Audiotron.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

namespace Audiotron {

    Wave::Wave(Wave::WaveType _type, float _amplitude, unsigned _harmonic) :waveType(_type), amplitude(_amplitude), harmonic(_harmonic) {}

    SampleBuffer Wave::generate(Sampler sampler, float frequency, float duration, float startingPhase = 0.0) {
        unsigned samples = sampler.getSamples(duration);
        SampleBuffer buffer(0, samples);

        frequency *= harmonic; //pow(2, harmonic);

        float sampleFrequency = (float) sampler.getFrequency();
        float dummyIntPart;

        for (unsigned i = 0; i < samples; ++i) {
            // calculate phase of sample i of frequency, at sampler rate. First calculate in the 0.0-1.0 range, then scaled to radians
            float phase = std::modf(startingPhase + (float) i / sampleFrequency * frequency, &dummyIntPart) * M_PI * 2;
            buffer[i] = sampleWave(phase) * amplitude;
        }

        return buffer;
    }

    float Wave::sampleWave(float phase) {
        float sample;
        switch (waveType) {
        case Sine:
        default:
            sample = std::sin(phase);
            break;
        };

        return sample;
    }

    Note::Note(float _start, float _frequency, float _duration, float _amplitude):
      time(_start),
      pitch(_frequency),
      duration(_duration),
      amplitude(_amplitude) {
    }

    Instrument::Instrument(Wave::WaveType waveType) {
        addHarmonic(waveType, 1.0f, 1);
    }

    SampleBuffer Instrument::play(Sampler sampler, Note note) {
        vector<SampleBuffer> harmonics;

        float pitch = note.getPitch();
        float duration = envelope.getDuration(note);

        SampleBuffer waveForm(sampler, 0, duration);

        // Run through each harmonic, and add its waveform to the resulting output
        for (vector<Wave>::iterator it = waves.begin(); it != waves.end(); ++it) {
            if ((*it).isAudible()) {
                (*it).generate(sampler, pitch, duration).mixInto(waveForm);
            }
        }

        waveForm.scaleAmplitude(note.getAmplitude());
        waveForm.apply(envelope);

        waveForm.setStart(note.getTime(sampler));

        return waveForm;
    }

    void Instrument::addHarmonic(Wave::WaveType type, float amplitude, unsigned harmonic = 0) {
        if (harmonic == 0) {
            harmonic = waves.size() + 1;
        }

        Wave newHarmonic(type, amplitude, harmonic);

        waves.push_back(std::move(newHarmonic));
    }

    NoteEnvelope::NoteEnvelope() :attack(0.0), decay(0.0), sustain(0.0), release(0.0) {}
    NoteEnvelope::NoteEnvelope(float _attack, float _decay, float _sustain, float _release) :attack(_attack), decay(_decay), sustain(_sustain), release(_release) {}

    float NoteEnvelope::getDuration(Note note) {
        return note.getDuration() + release;
    }
    unsigned NoteEnvelope::getDuration(Sampler sampler, Note note) {
        return note.getDuration(sampler) + sampler.getSamples(release);
    }

    Track::Track(Instrument _instrument) :instrument(_instrument) {}

    Sampler::Sampler(unsigned _frequency) :frequency(_frequency) {}

    SampleBuffer Track::play(Sampler sampler) {
        vector<SampleBuffer> noteBuffers;
        for (vector<Note>::iterator it = notes.begin(); it != notes.end(); ++it) {
            if ((*it).isAudible()) {
                noteBuffers.push_back(instrument.play(sampler, *it));
            }
        }

        SampleBuffer waveForm = mixBuffers(noteBuffers);
        waveForm.apply(envelope);

        return waveForm;
    }

} // namespace Audiotron

