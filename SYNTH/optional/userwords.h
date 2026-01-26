
# include "ESP32Synth.h"

  // Notes A
  #undef a0
  #undef a1
  #undef a2
  #undef a3
  #undef a4
  #undef a5
  #undef a6
  #undef a7
  #undef a8

  // Notes B
  #undef b0
  #undef b1
  #undef b2
  #undef b3
  #undef b4
  #undef b5
  #undef b6
  #undef b7
  #undef b8

  // Notes C
  #undef c0
  #undef c1
  #undef c2
  #undef c3
  #undef c4
  #undef c5
  #undef c6
  #undef c7
  #undef c8

  // Notes D (conflit avec SD_MMC)
  #undef d0
  #undef d1
  #undef d2
  #undef d3
  #undef d4
  #undef d5
  #undef d6
  #undef d7
  #undef d8

  // Notes E
  #undef e0
  #undef e1
  #undef e2
  #undef e3
  #undef e4
  #undef e5
  #undef e6
  #undef e7
  #undef e8

  // Notes F
  #undef f0
  #undef f1
  #undef f2
  #undef f3
  #undef f4
  #undef f5
  #undef f6
  #undef f7
  #undef f8

  // Notes G
  #undef g0
  #undef g1
  #undef g2
  #undef g3
  #undef g4
  #undef g5
  #undef g6
  #undef g7
  #undef g8

  // Notes dièses (sharp)
  #undef cs0
  #undef cs1
  #undef cs2
  #undef cs3
  #undef cs4
  #undef cs5
  #undef cs6
  #undef cs7
  #undef cs8

  #undef ds0
  #undef ds1
  #undef ds2
  #undef ds3
  #undef ds4
  #undef ds5
  #undef ds6
  #undef ds7
  #undef ds8

  #undef fs0
  #undef fs1
  #undef fs2
  #undef fs3
  #undef fs4
  #undef fs5
  #undef fs6
  #undef fs7
  #undef fs8

  #undef gs0
  #undef gs1
  #undef gs2
  #undef gs3
  #undef gs4
  #undef gs5
  #undef gs6
  #undef gs7
  #undef gs8

  #undef as0
  #undef as1
  #undef as2
  #undef as3
  #undef as4
  #undef as5
  #undef as6
  #undef as7
  #undef as8


  ESP32Synth synth;

//   X("synth.noteOn", SYNTH_NOTEON, synth.noteOn(uint8_t n2, uint32_t n1, uint8_t n0); DROPn(3))
//  X("synth.noteOff", SYNTH_NOTEOFF, synth.noteOff(n0); DROP)
//  X("synth.setWaveform", SYNTH_SETWAVEFORM, synth.setWaveform(n1, n0); DROPn(2))
//  X("synth.setVolume", SYNTH_SETVOLUME, synth.setVolume(n1, n0); DROPn(2))
//  X("synth.setPan", SYNTH_SETPAN, synth.setPan(n1, n0); DROPn(2))
//  X("synth.setFrequency", SYNTH_SETFREQUENCY, synth.setFrequency(n1, n0); DROPn(2))
//  X("synth.setEnvelope", SYNTH_SETENVELOPE, synth.setEnvelope(n4, n3, n2, n1, n0);
//    DROPn(5))
//  X("synth.setFilter", SYNTH_SETFILTER, synth.setFilter(n2, n1, n0); DROPn(3))
//  X("synth.enableReverb", SYNTH_ENABLEREVERB, synth.enableReverb(n1, n0); DROPn(2))
//  X("synth.enableChorus", SYNTH_ENABLECHORUS, synth.enableChorus(n1, n0); DROPn(2))
//  X("synth.mute", SYNTH_MUTE, synth.mute(n0); DROP)
//  X("synth.unmute", SYNTH_UNMUTE, synth.unmute(n0); DROP)
//  X("synth.stop", SYNTH_STOP, synth.stop(n0); DROP)



#define USER_WORDS \
  X("synth.begin", SYNTH_BEGIN, n0 = synth.begin(static_cast<int>(n0), static_cast<SynthOutputMode>(n1), \
      static_cast<int>(n2), static_cast<int>(n3), static_cast<I2S_Depth>(n4)); DROPn(4)) \
  X("synth.noteOff", SYNTH_NOTEOFF, synth.noteOff(static_cast<uint8_t>(n0)); DROP)



