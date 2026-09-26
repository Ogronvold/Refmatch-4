#pragma once
#include <JuceHeader.h>
#include <array>
#include <atomic>
#include <cmath>

// Lightweight BS.1770-inspired loudness tracker for short A/B level matching.
// It applies an approximate K-weighting curve (low-cut + 4 dB high shelf) and
// exposes a 400 ms exponentially averaged loudness estimate. The absolute
// number is less important than the A/B delta; both paths use the exact same
// detector.
class LoudnessTracker
{
public:
    void prepare(double newSampleRate, int channels)
    {
        sampleRate = juce::jmax(8000.0, newSampleRate);
        numChannels = juce::jlimit(1, 2, channels);
        designFilters();
        reset();
    }

    void reset()
    {
        for (auto& s : states)
            s = {};
        meanSquare = 0.0;
        currentLufs.store(-100.0f);
        lastUpdateMs.store(0.0);
        primed = false;
    }

    void processBlock(const juce::AudioBuffer<float>& buffer)
    {
        const int channels = juce::jmin(numChannels, buffer.getNumChannels());
        const int samples = buffer.getNumSamples();
        if (channels <= 0 || samples <= 0)
            return;

        double energy = 0.0;
        for (int ch = 0; ch < channels; ++ch)
        {
            auto& state = states[(size_t) ch];
            const float* in = buffer.getReadPointer(ch);
            double channelEnergy = 0.0;
            for (int i = 0; i < samples; ++i)
            {
                const double hp = processBiquad((double) in[i], highPass, state.hp);
                const double weighted = processBiquad(hp, highShelf, state.shelf);
                channelEnergy += weighted * weighted;
            }
            energy += channelEnergy / (double) samples;
        }
        energy /= (double) channels;

        // 400 ms is the BS.1770/EBU momentary window scale and is stable enough
        // for a responsive five-second level-match measurement.
        const double blockSeconds = (double) samples / sampleRate;
        const double alpha = std::exp(-blockSeconds / 0.400);
        if (!primed)
        {
            meanSquare = energy;
            primed = true;
        }
        else
        {
            meanSquare = alpha * meanSquare + (1.0 - alpha) * energy;
        }

        // BS.1770 loudness uses -0.691 dB as its calibration constant. Because
        // both A and B use the same detector, the delta remains meaningful even
        // though this is a compact K-weighting approximation rather than a
        // certification meter.
        const float lufs = meanSquare > 1.0e-12
            ? (float) (-0.691 + 10.0 * std::log10(meanSquare))
            : -100.0f;
        currentLufs.store(juce::jlimit(-100.0f, 20.0f, lufs));
        lastUpdateMs.store(juce::Time::getMillisecondCounterHiRes());
    }

    float getLufs() const noexcept { return currentLufs.load(); }
    double getLastUpdateMs() const noexcept { return lastUpdateMs.load(); }

private:
    struct Coeffs { double b0=1, b1=0, b2=0, a1=0, a2=0; };
    struct BiquadState { double z1=0, z2=0; };
    struct ChannelState { BiquadState hp, shelf; };

    static double processBiquad(double x, const Coeffs& c, BiquadState& s) noexcept
    {
        const double y = c.b0 * x + s.z1;
        s.z1 = c.b1 * x - c.a1 * y + s.z2;
        s.z2 = c.b2 * x - c.a2 * y;
        return y;
    }

    void designFilters()
    {
        highPass = makeHighPass(sampleRate, 38.13547087602444, 0.5);
        highShelf = makeHighShelf(sampleRate, 1681.974450955533, 4.0, 1.0);
    }

    static Coeffs makeHighPass(double sr, double frequency, double q)
    {
        const double w0 = juce::MathConstants<double>::twoPi * frequency / sr;
        const double cosw = std::cos(w0), sinw = std::sin(w0);
        const double alpha = sinw / (2.0 * q);
        const double a0 = 1.0 + alpha;
        Coeffs c;
        c.b0 = ((1.0 + cosw) * 0.5) / a0;
        c.b1 = (-(1.0 + cosw)) / a0;
        c.b2 = c.b0;
        c.a1 = (-2.0 * cosw) / a0;
        c.a2 = (1.0 - alpha) / a0;
        return c;
    }

    static Coeffs makeHighShelf(double sr, double frequency, double gainDb, double slope)
    {
        const double A = std::pow(10.0, gainDb / 40.0);
        const double w0 = juce::MathConstants<double>::twoPi * frequency / sr;
        const double cosw = std::cos(w0), sinw = std::sin(w0);
        const double alpha = sinw * 0.5 * std::sqrt((A + 1.0 / A) * (1.0 / slope - 1.0) + 2.0);
        const double twoSqrtAAlpha = 2.0 * std::sqrt(A) * alpha;

        const double b0 = A * ((A + 1.0) + (A - 1.0) * cosw + twoSqrtAAlpha);
        const double b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosw);
        const double b2 = A * ((A + 1.0) + (A - 1.0) * cosw - twoSqrtAAlpha);
        const double a0 = (A + 1.0) - (A - 1.0) * cosw + twoSqrtAAlpha;
        const double a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosw);
        const double a2 = (A + 1.0) - (A - 1.0) * cosw - twoSqrtAAlpha;

        Coeffs c;
        c.b0 = b0 / a0; c.b1 = b1 / a0; c.b2 = b2 / a0;
        c.a1 = a1 / a0; c.a2 = a2 / a0;
        return c;
    }

    double sampleRate = 48000.0;
    int numChannels = 2;
    double meanSquare = 0.0;
    bool primed = false;
    Coeffs highPass, highShelf;
    std::array<ChannelState, 2> states{};
    std::atomic<float> currentLufs {-100.0f};
    std::atomic<double> lastUpdateMs {0.0};
};
