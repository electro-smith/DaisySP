<div align=center>
<img width=15% src="https://raw.githubusercontent.com/electro-smith/daisysp/master/resources/assets/banner.png">

# DaisySP • A Powerful DSP Library in C++

[![Build Badge](https://github.com/electro-smith/DaisySP/workflows/Build/badge.svg)](https://github.com/electro-smith/DaisySP/actions?query=workflow%3ABuild)
[![Style Badge](https://github.com/electro-smith/DaisySP/workflows/Style/badge.svg)](https://github.com/electro-smith/DaisySP/actions?query=workflow%3AStyle)
[![Documentation Badge](https://github.com/electro-smith/DaisySP/workflows/Documentation/badge.svg)](https://electro-smith.github.io/DaisySP/index.html)
[![Discord](https://img.shields.io/discord/1037767234803740694?logo=discord&label=Discord)](https://discord.gg/ByHBnMtQTR)
[![Forum Badge](https://img.shields.io/badge/chat-daisy%20forum-orange)](https://forum.electro-smith.com/)
[![License Badge](https://img.shields.io/badge/license-MIT-yellow)](https://opensource.org/licenses/MIT)

> DaisySP is an open-source DSP (Digital Signal Processing) library that provides a comprehensive collection of modular components for creating audio software across various contexts, facilitating the development of high-quality and customizable audio applications.

</div>

## 📱 Applications 

- Embedded hardware using the [Daisy Audio Platform](https://www.electro-smith.com/daisy)
- Audio plug-ins (VST, AU, [JUCE](https://github.com/electro-smith/Daisy-Juce-Example))
- Mobile apps (iOS, Android)
- [VCV Rack](https://vcvrack.com/) modules

## ✨ Features

- [**Control Signal Generators**](https://github.com/electro-smith/DaisySP/tree/master/Source/Control/) AD and ADSR Envelopes, Phasor
- [**Drum Synthesis**](https://github.com/electro-smith/DaisySP/tree/master/Source/Drums/) Analog/Synth Bass/Snare Drum Models, HiHat
- [**Dynamics Processors**](https://github.com/electro-smith/DaisySP/tree/master/Source/Dynamics) Crossfade, Limiter
- [**Effects Processors:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Effects) Phaser, Wavefolder, Decimate, Overdrive
- [**Filters:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Filters) One pole Lowpass and Highpass, FIR, SOAP
- [**Noise Generators**](https://github.com/electro-smith/DaisySP/tree/master/Source/Noise/) Clocked Noise, Dust, Fractal Noise, Particle Noise, Whitenoise
- [**Physical Modeling Synthesis**](https://github.com/electro-smith/DaisySP/tree/master/Source/PhysicalModeling/) Karplus Strong, Resonators, Modal Synthesis
- [**Sampling Engines**](https://github.com/electro-smith/DaisySP/tree/master/Source/Sampling/) Granular Player
- [**Synthesis Methods:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Synthesis) Subtractive, Physical Modeling, FM
- [**Utilities:**](https://github.com/electro-smith/DaisySP/tree/master/Source/Utility) Math Functions, Signal Conditioning, Aleatoric Generators, Looper, DCBlocker

## 🚀 Getting Started

- Get the source: `git clone https://github.com/electro-smith/DaisySP`
- Navigate to the DaisySP repo: `cd DaisySP`
- Build the library: `make`
- Make some noise with the [example programs!](https://github.com/electro-smith/DaisyExamples)

## 👨‍💻 Code Example

```cpp
#include "daisysp.h"

static daisysp::OnePole flt;
static daisysp::Oscillator osc, lfo;
float saw, freq, output;

for(size_t i = 0; i < size; i++)
{
  freq = lfo.Process();
  saw = osc.Process();

  flt.SetFrequency(freq);
  output = flt.Process(saw);

  out[i] = output;
}
```

## ❤️ Community

Connect with other users and developers:

- Chat on the [Daisy Discord Server](https://discord.gg/ByHBnMtQTR)
- Join the [Daisy Forum](https://forum.electro-smith.com/)

## 🏆 Contributors

Thank you to all of the awesome people who have given their time and effort to this project!

<br>
<a href="https://github.com/electro-smith/DaisySP/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=electro-smith/DaisySP" />
</a>

Made with [contrib.rocks](https://contrib.rocks).

## ✍️ Contributing

Here are some ways that you can get involved:

- Proof read the [documentation](https://electro-smith.github.io/DaisySP/index.html) and suggest improvements
- Test existing functionality and make [issues](https://github.com/electro-smith/DaisySP/issues)
- Make new DSP modules. See issues labeled "feature"
- Port existing DSP modules from other open source projects (MIT). See issues labeled "port"
- Fix problems with existing modules. See issues labeled "bug" and/or "polish"

Before working on code, please check out our [Style Guide.](https://github.com/electro-smith/DaisySP/blob/master/doc/style_guide.pdf)

## ⚠️ License

DaisySP uses the MIT license.

It can be used in both closed source and commercial projects, and does not provide a warranty of any kind.

For the full license, read the [LICENSE](https://github.com/electro-smith/DaisySP/blob/master/LICENSE) file in the root directory.
