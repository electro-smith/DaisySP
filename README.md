![](https://github.com/electro-smith/DaisySP/blob/master/resources/assets/banner.png)

[![Build Status](https://travis-ci.com/electro-smith/DaisySP.svg?branch=master)](https://travis-ci.com/electro-smith/DaisySP)

**DaisySP is an open source DSP library written in C++ and specifically tailored to embedded audio applications.**

It was originally created for use with the [Daisy Embedded Audio Platform](https://www.electro-smith.com/daisy), however it can easily be used elsewhere. 

## Features

- Synthesis blocks such as oscillators, filters, envelopes, etc.

- DSP helper functions optimized for embedded applications

- Effects algorithms including reverb, delay, pitch-shift

## Code Example
```c++
static void AudioCallback(float *in, float *out, size_t size)
{
    float saw, freq, output;
    for (size_t i = 0; i < size; i += 2)
    {
        freq = 2500 + ( lfo.Process()*2500 );
    	saw = osc.Process();

        flt.SetFreq(freq);
        output = flt.Process(saw);

    	// left out
        out[i] = output;

        // right out
        out[i+1] = output;
    }
}
```
## Getting Started
- Browse the [online reference documentation,](https://electro-smith.github.io/DaisySP/index.html) or take it with you in [PDF form](https://github.com/electro-smith/DaisySP/blob/master/doc/daisysp_reference.pdf)
- Check out our [Getting Started Wiki Page](https://github.com/electro-smith/DaisyWiki/wiki)
- Make some sound!

## Contributing

Here are some ways that you can get involved:
- Proof read the documentation and suggest improvements
- Test existing functionality and make [issues](https://github.com/electro-smith/DaisySP/issues) 
- Make new DSP modules. See issues labeled "feature"
- Port existing DSP modules from other open source projects (MIT). See issues labeled "port"
- Fix problems with existing modules. See issues labeled "bug" and/or "polish"

Before working on code, please check out our [Contribution Guidelines](https://github.com/electro-smith/DaisyWiki/wiki/6.-Contribution-Guidelines) and [Style Guide.](https://github.com/electro-smith/DaisySP/blob/master/doc/style_guide.pdf)

## Support

Here are some ways to get support and connect with other users and developers:

- Join the [Daisy Forum](https://forum.electro-smith.com/)

- Make a [GitHub Issue](https://github.com/electro-smith/DaisySP/issues) 

- Join the [Daisy Slack Workspace](https://join.slack.com/t/es-daisy/shared_invite/zt-f9cfm1g4-DgdCok1h1Rj4fpX90~IOww)

## License
DaisySP is licensed with the permissive MIT open source license. 

This allows for modification and reuse in both commercial and personal projects. 
It does not provide a warranty of any kind. 

For the full license, read the [LICENSE](https://github.com/electro-smith/DaisySP/blob/master/LICENSE) file in the root directory. 
