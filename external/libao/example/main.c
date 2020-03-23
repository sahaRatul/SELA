#include <ao/ao.h>
#define _USE_MATH_DEFINES
#include <math.h>

// C major scale
const int notes[] = {
        0, 2, 4, 5, 7, 9, 11, 12,
        12, 11, 9, 7, 5, 4, 2, 0
};
const double volume = 0.3;
#define BPM 220
#define SMOOTH 0.1

#define NOTES_COUNT (sizeof(notes) / sizeof(int))

#define SAMPLE_RATE 44100
#define BUFFER_TIME 2
#define BUFFER_SIZE (BUFFER_TIME * SAMPLE_RATE)

const double smoothStep = SMOOTH * SAMPLE_RATE;

#define SAMPLE_PER_BEAT ((int) (SAMPLE_RATE / (BPM / 60.)))

int main() {
    printf("Initializing\n");
    ao_initialize();

    int driver = ao_default_driver_id();
    printf("  + driver id: %i\n", driver);

    ao_sample_format sformat;
    sformat.bits = 16;
    sformat.channels = 1;
    sformat.rate = SAMPLE_RATE;
    sformat.byte_format = AO_FMT_NATIVE;
    sformat.matrix = 0;

    ao_device *adevice = ao_open_live(driver, &sformat, NULL);

    int i = 0;
    double cycle = 0;
    short buffer[BUFFER_SIZE];

    const int lastSample = NOTES_COUNT * SAMPLE_PER_BEAT;
    while (i < lastSample) {
        uint32_t j = 0;
        for (; j < BUFFER_SIZE && i < lastSample; ++j) {
            int nota = i / SAMPLE_PER_BEAT;

            double freq = 440 * pow(2, notes[nota] / 12.);
            cycle += sin(1 / 44100. * freq);

            double sample = sin(cycle * 2 * M_PI);

            // smooth step
            if (i < smoothStep) {
                sample *= sin(i / smoothStep * M_PI_2);
            }
            if (i > lastSample - smoothStep) {
                sample *= sin((lastSample - i) / smoothStep * M_PI_2);
            }

            buffer[j] = (short) ((sample * 0x7fff) * volume);
            i++;
        }

        printf("Writed %i samples to device\n", j * 2);
        if (!ao_play(adevice, (char *) buffer, j * 2)) break;
    }

    printf("Flushing device\n");
    ao_close(adevice);

    printf("Closing\n");
    ao_shutdown();
    return 0;
}
