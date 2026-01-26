\ *************************************
\ Datas and structures for ESP32 Synth.
\    Filename:      datasAndStructs.fs
\    Date:          26 jan 2026
\    Updated:       26 jan 2026
\    File Version:  1.0
\    MCU:           ESP32-S3 - ESP32 WROOM
\    Forth:         ESP32forth all versions 7.0.7.21+
\    Copyright:     Marc PETREMANN
\    Author:        Marc PETREMANN
\    GNU General Public License
\ **************************************


RECORDFILE /spiffs/datasAndStructs.fs

\ enum SynthOutputMode used by synt.begin
0 constant SMODE_PDM
1 constant SMODE_I2S
2 constant SMODE_DAC

\ enum WaveType
0 constant WAVE_SINE
1 constant WAVE_TRIANGLE
2 constant WAVE_SAW
3 constant WAVE_PULSE
4 constant WAVE_WAVETABLE
5 constant WAVE_NOISE
6 constant WAVE_SAMPLE

\ enum FilterType
0 constant FILTER_NONE
1 constant FILTER_LP
2 constant FILTER_HP
3 constant FILTER_BP

\ enum BitDepth
0 constant BITS_4
1 constant BITS_8
2 constant BITS_16

\ enum EnvState
0 constant ENV_IDLE
1 constant ENV_ATTACK
2 constant ENV_DECAY
3 constant ENV_SUSTAIN
4 constant ENV_RELEASE

\ Definitions for basic waves (using negative numbers)
-1 constant W_SINE
-2 constant W_TRI
-3 constant W_SAW
-4 constant W_PULSE
-5 constant W_NOISE

\ enum I2S_Depth
0 constant I2S_8BIT     \ Lo-Fi Bitcrusher (Hardware 16b, Software Mask)
1 constant I2S_16BIT    \ Standard
2 constant I2S_32BIT    \ Hi-Fi (Hardware 32b)

<EOF>






// Modos de Loop
// Loop Modes
enum LoopMode {
    LOOP_OFF,
    LOOP_FORWARD,   // Esquerda -> Direita (Padrão)
                    // Left -> Right (Default)
    LOOP_PINGPONG,  // Vai e Volta
                    // Back and Forth
    LOOP_REVERSE    // Direita -> Esquerda
                    // Right -> Left
};


struct Instrument {
    const uint8_t* seqVolumes;
    const int16_t* seqWaves;
    uint8_t        seqLen;
    uint16_t       seqSpeedMs;

    uint8_t        susVol;
    int16_t        susWave;

    const uint8_t* relVolumes;
    const int16_t* relWaves;
    uint8_t        relLen;
    uint16_t       relSpeedMs;

    bool           smoothMorph;
};

#define MAX_SAMPLES 100

struct SampleData {
    const int16_t* data;
    uint32_t length;
    uint32_t sampleRate;
    uint32_t rootFreqCentiHz;
};

struct SampleZone {
    uint32_t lowFreq;
    uint32_t highFreq;
    uint16_t sampleId;
    uint32_t rootOverride;
};

// Instrumento baseado em Samples
// Sample-based Instrument
struct Instrument_Sample {
    const SampleZone* zones;
    uint8_t           numZones;
    LoopMode          loopMode;    // Modo de loop / Loop mode
    uint32_t          loopStart;   // Início do loop / Loop start
    uint32_t          loopEnd;     // Fim do loop (0 = fim do arquivo) / Loop end (0 = end of file)
};

struct Voice {
    bool active = false;
    uint32_t freqVal = 0;
    uint32_t phase = 0;
    uint32_t phaseInc = 0;
    uint8_t vol = 127;
    uint32_t pulseWidth = 0x80000000;
    uint32_t rngState = 12345;
    int16_t noiseSample = 0;

    // Vibrato
    uint32_t vibPhase = 0;
    uint32_t vibRateInc = 0;
    uint32_t vibDepthInc = 0;
    int32_t  vibOffset = 0;

    // Tremolo
    uint32_t trmPhase = 0;
    uint32_t trmRateInc = 0;
    uint16_t trmDepth = 0;
    uint16_t trmModGain = 256;

    EnvState envState = ENV_IDLE;
    uint32_t currEnvVal = 0;
    uint32_t rateAttack = 0;
    uint32_t rateDecay = 0;
    uint32_t rateRelease = 0;
    uint32_t levelSustain = 0;

    // Parâmetros de síntese
    // Synthesis parameters
    WaveType type = WAVE_SINE;
    const void* wtData = nullptr;
    uint32_t wtSize = 0;
    BitDepth depth = BITS_8;

    // Estado do instrumento
    // Instrument state
    Instrument* inst = nullptr;
    uint8_t stageIdx = 0;
    uint32_t controlTick = 0;
    uint16_t currWaveId = 0;
    uint16_t nextWaveId = 0;
    uint8_t currWaveIsBasic = 0;
    uint8_t nextWaveIsBasic = 0;
    uint8_t currWaveType = 0;
    uint8_t nextWaveType = 0;
    uint8_t morph = 0;

    uint16_t attackMs = 0;
    uint16_t decayMs = 0;
    uint16_t releaseMs = 0;

    // Estado do portamento (slide)
    // Portamento (slide) state
    bool slideActive = false;
    uint32_t slideTicksRemaining = 0;
    uint32_t slideTicksTotal = 0;
    int32_t slideDeltaInc = 0;
    uint32_t slideTargetInc = 0;
    uint32_t slideTargetFreqCenti = 0;
    int32_t slideRem = 0;
    int32_t slideRemAcc = 0;

    // Estado do arpejador
    // Arpeggiator state
    bool     arpActive = false;
    uint32_t arpNotes[MAX_ARP_NOTES];
    uint8_t  arpLen = 0;
    uint8_t  arpIdx = 0;
    uint16_t arpSpeedMs = 100;
    uint32_t arpTickCounter = 0;

    // Motor de Sample
    // Sample Engine
    Instrument_Sample* instSample = nullptr;
    uint16_t curSampleId = 0;

    // Posição 64-bit para evitar overflow
    // 64-bit position to avoid overflow
    uint64_t samplePos1616 = 0;
    uint32_t sampleInc1616 = 0;

    // Controle de Loop
    // Loop Control
    LoopMode sampleLoopMode = LOOP_OFF;
    bool     sampleDirection = true; // true: forward, false: backward
    uint32_t sampleLoopStart = 0;
    uint32_t sampleLoopEnd = 0;
    bool     sampleFinished = false;
};
