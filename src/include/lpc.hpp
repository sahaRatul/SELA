#ifndef _LPC_H_
#define _LPC_H_

#include "data/lpc_decoded_data.hpp"
#include "data/lpc_encoded_data.hpp"

constexpr uint8_t MAX_LPC_ORDER = 100;
constexpr uint8_t CORRECTION_FACTOR = 35;
constexpr double SQRT2 = 1.4142135623730950488016887242096;
constexpr static double firstOrderCoefficients[128] = { -1, -0.9998779296875, -0.99951171875, -0.9989013671875,
    -0.998046875, -0.9969482421875, -0.99560546875, -0.9940185546875, -0.9921875, -0.9901123046875,
    -0.98779296875, -0.9852294921875, -0.982421875, -0.9793701171875, -0.97607421875, -0.9725341796875,
    -0.96875, -0.9647216796875, -0.96044921875, -0.9559326171875, -0.951171875, -0.9461669921875,
    -0.94091796875, -0.9354248046875, -0.9296875, -0.9237060546875, -0.91748046875, -0.9110107421875,
    -0.904296875, -0.8973388671875, -0.89013671875, -0.8826904296875, -0.875, -0.8670654296875, -0.85888671875,
    -0.8504638671875, -0.841796875, -0.8328857421875, -0.82373046875, -0.8143310546875, -0.8046875,
    -0.7947998046875, -0.78466796875, -0.7742919921875, -0.763671875, -0.7528076171875, -0.74169921875,
    -0.7303466796875, -0.71875, -0.7069091796875, -0.69482421875000011, -0.6824951171875, -0.669921875,
    -0.6571044921875, -0.64404296875, -0.63073730468750011, -0.6171875, -0.6033935546875, -0.58935546875,
    -0.57507324218750011, -0.56054687500000011, -0.5457763671875, -0.53076171875, -0.5155029296875,
    -0.50000000000000011, -0.48425292968750011, -0.46826171875, -0.4520263671875, -0.43554687500000011,
    -0.41882324218750011, -0.40185546875000011, -0.38464355468750011, -0.3671875, -0.34948730468750011,
    -0.33154296875000011, -0.31335449218750011, -0.29492187500000022, -0.2762451171875, -0.25732421875000011,
    -0.23815917968750011, -0.21875000000000011, -0.19909667968750022, -0.17919921875, -0.15905761718750011,
    -0.13867187500000011, -0.11804199218750011, -0.097167968750000222, -0.076049804687500222,
    -0.054687500000000111, -0.033081054687500111, -0.011230468750000111, 0.010864257812499778, 0.033203125,
    0.055786132812499778, 0.07861328125, 0.1016845703125, 0.12499999999999978, 0.1485595703125,
    0.17236328124999978, 0.1964111328125, 0.22070312499999956, 0.24523925781249978, 0.27001953125,
    0.29504394531249978, 0.3203125, 0.34582519531249956, 0.37158203124999978, 0.3975830078125,
    0.42382812499999978, 0.4503173828125, 0.47705078124999956, 0.50402832031249978, 0.53125,
    0.55871582031249978, 0.58642578125, 0.61437988281249956, 0.64257812499999978, 0.6710205078125,
    0.69970703124999956, 0.7286376953125, 0.75781249999999956, 0.78723144531249978, 0.81689453125,
    0.84680175781249956, 0.876953125, 0.90734863281249956, 0.93798828124999978, 0.9688720703125 };

constexpr static double secondOrderCoefficients[128] = { -0.50000000000000011, 0.9998779296875, 0.99951171875,
    0.9989013671875, 0.998046875, 0.9969482421875, 0.99560546875, 0.9940185546875, 0.9921875, 0.9901123046875,
    0.98779296875, 0.9852294921875, 0.982421875, 0.9793701171875, 0.97607421875, 0.9725341796875, 0.96875,
    0.9647216796875, 0.96044921875, 0.9559326171875, 0.951171875, 0.9461669921875, 0.94091796875,
    0.9354248046875, 0.9296875, 0.9237060546875, 0.91748046875, 0.9110107421875, 0.904296875, 0.8973388671875,
    0.89013671875, 0.8826904296875, 0.875, 0.8670654296875, 0.85888671875, 0.8504638671875, 0.841796875,
    0.8328857421875, 0.82373046875, 0.8143310546875, 0.8046875, 0.7947998046875, 0.78466796875, 0.7742919921875,
    0.763671875, 0.7528076171875, 0.74169921875, 0.7303466796875, 0.71875, 0.7069091796875, 0.69482421875000011,
    0.6824951171875, 0.669921875, 0.6571044921875, 0.64404296875, 0.63073730468750011, 0.6171875,
    0.6033935546875, 0.58935546875, 0.57507324218750011, 0.56054687500000011, 0.5457763671875, 0.53076171875,
    0.5155029296875, 0.50000000000000011, 0.48425292968750011, 0.46826171875, 0.4520263671875,
    0.43554687500000011, 0.41882324218750011, 0.40185546875000011, 0.38464355468750011, 0.3671875,
    0.34948730468750011, 0.33154296875000011, 0.31335449218750011, 0.29492187500000022, 0.2762451171875,
    0.25732421875000011, 0.23815917968750011, 0.21875000000000011, 0.19909667968750022, 0.17919921875,
    0.15905761718750011, 0.13867187500000011, 0.11804199218750011, 0.097167968750000222, 0.076049804687500222,
    0.054687500000000111, 0.033081054687500111, 0.011230468750000111, -0.010864257812499778, -0.033203125,
    -0.055786132812499778, -0.07861328125, -0.1016845703125, -0.12499999999999978, -0.1485595703125,
    -0.17236328124999978, -0.1964111328125, -0.22070312499999956, -0.24523925781249978, -0.27001953125,
    -0.29504394531249978, -0.3203125, -0.34582519531249956, -0.37158203124999978, -0.3975830078125,
    -0.42382812499999978, -0.4503173828125, -0.47705078124999956, -0.50402832031249978, -0.53125,
    -0.55871582031249978, -0.58642578125, -0.61437988281249956, -0.64257812499999978, -0.6710205078125,
    -0.69970703124999956, -0.7286376953125, -0.75781249999999956, -0.78723144531249978, -0.81689453125,
    -0.84680175781249956, -0.876953125, -0.90734863281249956, -0.93798828124999978, -0.9688720703125 };

constexpr static double higherOrderCoefficients[128] = { -1, -0.984375, -0.96875, -0.953125, -0.9375, -0.921875,
    -0.90625, -0.890625, -0.875, -0.859375, -0.84375, -0.828125, -0.8125, -0.796875, -0.78125, -0.765625, -0.75,
    -0.734375, -0.71875, -0.703125, -0.6875, -0.671875, -0.65625, -0.640625, -0.625, -0.609375, -0.59375,
    -0.578125, -0.5625, -0.546875, -0.53125, -0.515625, -0.5, -0.484375, -0.46875, -0.453125, -0.4375,
    -0.421875, -0.40625, -0.390625, -0.375, -0.359375, -0.34375, -0.328125, -0.3125, -0.296875, -0.28125,
    -0.265625, -0.25, -0.234375, -0.21875, -0.203125, -0.1875, -0.171875, -0.15625, -0.140625, -0.125,
    -0.109375, -0.09375, -0.078125, -0.0625, -0.046875, -0.03125, -0.015625, 0, 0.015625, 0.03125, 0.046875,
    0.0625, 0.078125, 0.09375, 0.109375, 0.125, 0.140625, 0.15625, 0.171875, 0.1875, 0.203125, 0.21875,
    0.234375, 0.25, 0.265625, 0.28125, 0.296875, 0.3125, 0.328125, 0.34375, 0.359375, 0.375, 0.390625, 0.40625,
    0.421875, 0.4375, 0.453125, 0.46875, 0.484375, 0.5, 0.515625, 0.53125, 0.546875, 0.5625, 0.578125, 0.59375,
    0.609375, 0.625, 0.640625, 0.65625, 0.671875, 0.6875, 0.703125, 0.71875, 0.734375, 0.75, 0.765625, 0.78125,
    0.796875, 0.8125, 0.828125, 0.84375, 0.859375, 0.875, 0.890625, 0.90625, 0.921875, 0.9375, 0.953125,
    0.96875, 0.984375 };

namespace lpc {
class LinearPredictor {
public:
    uint8_t optimalLpcOrder = 1;
    std::vector<double> reflectionCoefficients;
    std::vector<int64_t> linearPredictionCoefficients;
    std::vector<int32_t> quantizedReflectionCoefficients;
    LinearPredictor();
    LinearPredictor(std::vector<int32_t> quantizedReflectionCoefficients, uint8_t optimalLpcOrder);
    void dequantizeReflectionCoefficients();
    void generatelinearPredictionCoefficients();
};

class ResidueGenerator {
private:
    const std::vector<int32_t>& samples;
    std::vector<double> quantizedSamples;
    std::vector<double> autocorrelationFactors;
    LinearPredictor linearPredictor;
    uint8_t bitsPerSample;
    int32_t quantizationFactor = INT16_MAX; //Warning: This is hardcoded and subject to change later
    inline void quantizeSamples();
    inline void generateAutoCorrelation();
    inline void generateReflectionCoefficients();
    inline void generateoptimalLpcOrder();
    inline void quantizeReflectionCoefficients();
    inline void generateResidues(std::vector<int32_t>& residues);

public:
    explicit ResidueGenerator(const data::LpcDecodedData& data);
    data::LpcEncodedData process();
};

class SampleGenerator {
private:
    const std::vector<int32_t>& residues;
    uint8_t bitsPerSample;
    LinearPredictor linearPredictor;
    inline void generateSamples(std::vector<int32_t>& samples);

public:
    explicit SampleGenerator(const data::LpcEncodedData& encodedData);
    data::LpcDecodedData process();
};
}

#endif