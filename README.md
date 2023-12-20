<p align="center">
  <img width=15% src="https://raw.githubusercontent.com/electro-smith/daisysp/master/resources/assets/banner.png">
</p>
<h1 align="center"> DaisySP </h11>
<h3 align="center">A Powerful, Open Source DSP Library in C++</h3>
<br>

<!--CI Badges-->
<p align="center">
    <a href="https://github.com/electro-smith/DaisySP/actions?query=workflow%3ABuild">
      <img src="https://github.com/electro-smith/DaisySP/workflows/Build/badge.svg">
    </a>
    <a href="https://github.com/electro-smith/DaisySP/actions?query=workflow%3AStyle">
      <img src="https://github.com/electro-smith/DaisySP/workflows/Style/badge.svg">
    </a>
    <a href="https://electro-smith.github.io/DaisySP/index.html">
      <img src="https://github.com/electro-smith/DaisySP/workflows/Documentation/badge.svg">
    </a>
</p>

<!-- Non-CI Badges -->
<p align="center">
  <a href="https://opensource.org/licenses/MIT">
    <img src="https://img.shields.io/badge/license-MIT-yellow">
  </a>
  <a href="https://discord.gg/ByHBnMtQTR">
      <img src="https://img.shields.io/badge/join-us%20on%20discord-gray.svg?longCache=true&logo=discord&colorB=purple">
  </a>
  <a href="https://forum.electro-smith.com/">
    <img src="https://img.shields.io/badge/chat-daisy%20forum-orange">
  </a>
</p>

<p align="center">
  <a href="#applications">Applications</a> •
  <a href="#features">Features</a> •
  <a href="https://github.com/electro-smith/DaisyExamples">Examples</a> •
  <a href="#getting-started">Getting Started</a> •
  <a href="#community">Community</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a>
</p>

<br/>

## Applications

- Embedded hardware using the [Daisy Audio Platform](https://www.electro-smith.com/daisy)
- Audio plug-ins (VST, AU, [JUCE](https://github.com/electro-smith/Daisy-Juce-Example))
- Mobile apps (iOS, Android)
- [VCV Rack](https://vcvrack.com/) modules

## Features

- [**Control Signal Generators**](https://github.com/electro-smith/DaisySP/tree/master/Control/) AD and ADSR Envelopes, Phasor
- [**Drum Synthesis**](https://github.com/electro-smith/DaisySP/tree/master/Drums/) Analog/Synth Bass/Snare Drum Models, HiHat
- [**Dynamics Processors**](https://github.com/electro-smith/DaisySP/tree/master/Source/Dynamics) Crossfade, Compressor, Limiter
- [**Effects Processors:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Effects) Phaser, Wavefolder, Decimate, Overdrive
- [**Filters:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Filters) One pole Lowpass and Highpass, FIR, SOAP
- [**Noise Generators**](https://github.com/electro-smith/DaisySP/tree/master/Source/Noise/) Clocked Noise, Dust, Fractal Noise, Particle Noise, Whitenoise
- [**Physical Modeling Synthesis**](https://github.com/electro-smith/DaisySP/tree/master/Source/PhysicalModeling/) Karplus Strong, Resonators, Modal Synthesis
- [**Sampling Engines**](https://github.com/electro-smith/DaisySP/tree/master/Source/Sampling/) Granular Player
- [**Synthesis Methods:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Synthesis) Subtractive, Physical Modeling, FM
- [**Utilities:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Utility) Math Functions, Signal Conditioning, Aleatoric Generators, Looper, DCBlocker

## Code Example

```cpp
#include "daisysp.h"

static daisysp::MoogLadder flt;
static daisysp::Oscillator osc, lfo;
float saw, freq, output;

for(size_t i = 0; i < size; i++)
{
  freq = lfo.Process();
  saw = osc.Process();

  flt.SetFreq(freq);
  output = flt.Process(saw);

  out[i] = output;
}
```

## Getting Started

- Get the source: `git clone https://github.com/electro-smith/DaisySP`
- Navigate to the DaisySP repo: `cd DaisySP`
- Build the library: `make`
- Make some noise with the [example programs!](https://github.com/electro-smith/DaisyExamples)

## Community

Connect with other users and developers:

- Chat on the [Daisy Discord Server](https://discord.gg/ByHBnMtQTR)
- Join the [Daisy Forum](https://forum.electro-smith.com/)

## Contributing

Here are some ways that you can get involved:

- Proof read the [documentation](https://electro-smith.github.io/DaisySP/index.html) and suggest improvements
- Test existing functionality and make [issues](https://github.com/electro-smith/DaisySP/issues)
- Make new DSP modules. See issues labeled "feature"
- Port existing DSP modules from other open source projects (MIT). See issues labeled "port"
- Fix problems with existing modules. See issues labeled "bug" and/or "polish"

Before working on code, please check out our [Contribution Guidelines](https://github.com/electro-smith/DaisyWiki/wiki/6.-Contribution-Guidelines) and [Style Guide.](https://github.com/electro-smith/DaisySP/blob/master/doc/style_guide.pdf)

## License

DaisySP uses the MIT license.

It can be used in both closed source and commercial projects, and does not provide a warranty of any kind.

For the full license, read the [LICENSE](https://github.com/electro-smith/DaisySP/blob/master/LICENSE) file in the root directory.
