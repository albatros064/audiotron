#include "Audiotron.h"

#include <iostream>

int main(int argc, char **argv) {

    Audiotron::Instrument instrument(Audiotron::Wave::Sine);
    Audiotron::NoteEnvelope envelope;
    instrument.setNoteEnvelope(envelope);

    instrument.addHarmonic(Audiotron::Wave::Sine, 0.3f, 2);
    instrument.addHarmonic(Audiotron::Wave::Sine, 0.25f, 3);

    Audiotron::Track track(instrument);
    Audiotron::Note note(0.0, 440.0, 1000.0, 1.0);
    track.addNote(note);

    Audiotron::Sampler sampler(48000);
    Audiotron::SampleBuffer buffer = track.play(sampler);
    buffer.normalize();

    for (unsigned i = 0; i < buffer.getLength(); ++i) {
        //std::cout << buffer[i] << " ";
        int spaces = (buffer[i] + 1.0) * 60;
        for (int i = 0; i < spaces; ++i) {
            std::cout << " ";
        }
        std::cout << "o" << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
