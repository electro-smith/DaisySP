<p align="center"><img width=15% src="https://github.com/electro-smith/daisysp/blob/master/resources/assets/banner.png"></p>
<p align="center"><img width=23% src="https://github.com/electro-smith/daisysp/blob/master/resources/assets/daisysp.png"></p>

<h3 align="center">A Powerful, Open Source DSP Library in C++</h3>
<br>

<p align="center">
  <a href="https://travis-ci.com/electro-smith/DaisySP">
    <img src="https://travis-ci.com/electro-smith/DaisySP.svg?branch=master">
         </a>
    <a href="https://opensource.org/licenses/MIT">
      <img src="https://img.shields.io/badge/license-MIT-yellow"
           </a>
      <a href="https://join.slack.com/t/es-daisy/shared_invite/zt-f9cfm1g4-DgdCok1h1Rj4fpX90~IOww">
        <img src="https://img.shields.io/badge/join-us%20on%20slack-gray.svg?longCache=true&logo=slack&colorB=purple"
             </a>
      <a href="https://electro-smith.github.io/DaisySP/index.html">
        <img src="https://img.shields.io/badge/documentation-online-blue"
             </a>
      <a href="https://forum.electro-smith.com/">
        <img src="https://img.shields.io/badge/chat-daisy%20forum-orange"
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

---

## Applications
- Embedded hardware using the [Daisy Audio Platform](https://www.electro-smith.com/daisy)
- Audio plug-ins (VST, AU, [JUCE](https://juce.com/))
- Mobile apps (iOS, Android)
- [VCV Rack](https://vcvrack.com/) modules

## Features

- **Synthesis Methods:** [Subtractive](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_moog_ladder.html), [Physical Modeling](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_pluck.html), [FM](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_fm2.html)
- **Filters:** [Biquad](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_biquad.html), [State-Variable](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_svf.html), [Modal](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_mode.html), [Comb](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_comb.html)
- **Effects Processors:** [Reverb](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_reverb_sc.html), [Delay](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_delay_line.html), [Decimate](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_decimator.html), [Compressor](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_compressor.html)
- **Utilities:** [Math Functions](https://github.com/electro-smith/DaisySP/blob/master/modules/dsp.h), [Signal Conditioning](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_port.html), [Aleatoric Generators](https://electro-smith.github.io/DaisySP/classdaisysp_1_1_maytrig.html)  

## Code Example
![](https://github.com/electro-smith/daisysp/blob/master/resources/assets/code_example.PNG)

## Getting Started
- Get the source: `git clone https://github.com/electro-smith/DaisySP`
- Navigate to the DaisySP repo: `cd DaisySP`
- Build the library: `make`
- Make some noise with the [example programs!](https://github.com/electro-smith/DaisyExamples)

## Community

Connect with other users and developers:

- Join the [Daisy Forum](https://forum.electro-smith.com/)
- Chat on the [Daisy Slack Workspace](https://join.slack.com/t/es-daisy/shared_invite/zt-f9cfm1g4-DgdCok1h1Rj4fpX90~IOww)

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
