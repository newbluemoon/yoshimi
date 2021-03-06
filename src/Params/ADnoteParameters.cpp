/*
    ADnoteParameters.cpp - Parameters for ADnote (ADsynth)

    Original ZynAddSubFX author Nasca Octavian Paul
    Copyright (C) 2002-2005 Nasca Octavian Paul
    Copyright 2009-2011, Alan Calvert

    This file is part of yoshimi, which is free software: you can redistribute
    it and/or modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.   See the GNU General Public License (version 2 or
    later) for more details.

    You should have received a copy of the GNU General Public License along with
    yoshimi; if not, write to the Free Software Foundation, Inc., 51 Franklin
    Street, Fifth Floor, Boston, MA  02110-1301, USA.

    This file is derivative of ZynAddSubFX original code, modified May 2017
*/

#include <iostream>

#include <cmath>
#include <stdlib.h>

using namespace std;

#include "Params/ADnoteParameters.h"

int ADnoteParameters::ADnote_unison_sizes[] =
{2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 25, 30, 40, 50, 0};

ADnoteParameters::ADnoteParameters(FFTwrapper *fft_, SynthEngine *_synth) :
    Presets(_synth),
    fft(fft_)
{
    setpresettype("ADnoteParameters");
    GlobalPar.FreqEnvelope = new EnvelopeParams(0, 0, synth);
    GlobalPar.FreqEnvelope->ASRinit(64, 50, 64, 60);
    GlobalPar.FreqLfo = new LFOParams(70, 0, 64, 0, 0, 0, 0, 0, synth);

    GlobalPar.AmpEnvelope = new EnvelopeParams(64, 1, synth);
    GlobalPar.AmpEnvelope->ADSRinit_dB(0, 40, 127, 25);
    GlobalPar.AmpLfo = new LFOParams(80, 0, 64, 0, 0, 0, 0, 1, synth);

    GlobalPar.GlobalFilter = new FilterParams(2, 94, 40, 0, synth);
    GlobalPar.FilterEnvelope = new EnvelopeParams(0, 1, synth);
    GlobalPar.FilterEnvelope->ADSRinit_filter(64, 40, 64, 70, 60, 64);
    GlobalPar.FilterLfo = new LFOParams(80, 0, 64, 0, 0, 0, 0, 2, synth);
    GlobalPar.Reson = new Resonance(synth);

    for (int nvoice = 0; nvoice < NUM_VOICES; ++nvoice)
        enableVoice(nvoice);
    defaults();
}


void ADnoteParameters::defaults(void)
{
    // Frequency Global Parameters
    GlobalPar.PStereo = true; // stereo
    GlobalPar.PDetune = 8192; // zero
    GlobalPar.PCoarseDetune = 0;
    GlobalPar.PDetuneType = 1;
    GlobalPar.FreqEnvelope->defaults();
    GlobalPar.FreqLfo->defaults();
    GlobalPar.PBandwidth = 64;

    // Amplitude Global Parameters
    GlobalPar.PVolume = 90;
    setGlobalPan(GlobalPar.PPanning = 64); // center
    GlobalPar.PAmpVelocityScaleFunction = 64;
    GlobalPar.AmpEnvelope->defaults();
    GlobalPar.AmpLfo->defaults();
    GlobalPar.Fadein_adjustment = FADEIN_ADJUSTMENT_SCALE;
    GlobalPar.PPunchStrength = 0;
    GlobalPar.PPunchTime = 60;
    GlobalPar.PPunchStretch = 64;
    GlobalPar.PPunchVelocitySensing = 72;
    GlobalPar.Hrandgrouping = 0;

    // Filter Global Parameters
    GlobalPar.PFilterVelocityScale = 64;
    GlobalPar.PFilterVelocityScaleFunction = 64;
    GlobalPar.GlobalFilter->defaults();
    GlobalPar.FilterEnvelope->defaults();
    GlobalPar.FilterLfo->defaults();
    GlobalPar.Reson->defaults();

    for (int nvoice = 0; nvoice < NUM_VOICES; ++nvoice)
        defaults(nvoice);
    VoicePar[0].Enabled = 1;
}


// Defaults a voice
void ADnoteParameters::defaults(int n)
{
    int nvoice = n;
    VoicePar[nvoice].Enabled = 0;

    VoicePar[nvoice].Unison_size = 1;
    VoicePar[nvoice].Unison_frequency_spread = 60;
    VoicePar[nvoice].Unison_stereo_spread = 64;
    VoicePar[nvoice].Unison_vibratto = 64;
    VoicePar[nvoice].Unison_vibratto_speed = 64;
    VoicePar[nvoice].Unison_invert_phase = 0;
    VoicePar[nvoice].Unison_phase_randomness = 127;

    VoicePar[nvoice].Type = 0;
    VoicePar[nvoice].Pfixedfreq = 0;
    VoicePar[nvoice].PfixedfreqET = 0;
    VoicePar[nvoice].PBendAdjust = 88; // 64 + 24
    VoicePar[nvoice].POffsetHz     = 64;
    VoicePar[nvoice].Presonance = 1;
    VoicePar[nvoice].Pfilterbypass = 0;
    VoicePar[nvoice].Pextoscil = -1;
    VoicePar[nvoice].PextFMoscil = -1;
    VoicePar[nvoice].Poscilphase = 64;
    VoicePar[nvoice].PFMoscilphase = 64;
    VoicePar[nvoice].PDelay = 0;
    VoicePar[nvoice].PVolume = 100;
    VoicePar[nvoice].PVolumeminus = 0;
    setVoicePan(nvoice, VoicePar[nvoice].PPanning = 64); // center
    VoicePar[nvoice].PDetune = 8192; // 8192 = 0
    VoicePar[nvoice].PCoarseDetune = 0;
    VoicePar[nvoice].PDetuneType = 0;
    VoicePar[nvoice].PFreqLfoEnabled = 0;
    VoicePar[nvoice].PFreqEnvelopeEnabled = 0;
    VoicePar[nvoice].PAmpEnvelopeEnabled = 0;
    VoicePar[nvoice].PAmpLfoEnabled = 0;
    VoicePar[nvoice].PAmpVelocityScaleFunction = 127;
    VoicePar[nvoice].PFilterEnabled = 0;
    VoicePar[nvoice].PFilterEnvelopeEnabled = 0;
    VoicePar[nvoice].PFilterLfoEnabled = 0;
    VoicePar[nvoice].PFilterVelocityScale = 0;
    VoicePar[nvoice].PFilterVelocityScaleFunction = 64;
    VoicePar[nvoice].PFMEnabled = 0;
    VoicePar[nvoice].PFMFixedFreq = false;

    // I use the internal oscillator (-1)
    VoicePar[nvoice].PFMVoice = -1;

    VoicePar[nvoice].PFMVolume = 90;
    VoicePar[nvoice].PFMVolumeDamp = 64;
    VoicePar[nvoice].PFMDetune = 8192;
    VoicePar[nvoice].PFMCoarseDetune = 0;
    VoicePar[nvoice].PFMDetuneType = 0;
    VoicePar[nvoice].PFMFreqEnvelopeEnabled = 0;
    VoicePar[nvoice].PFMAmpEnvelopeEnabled = 0;
    VoicePar[nvoice].PFMVelocityScaleFunction = 64;

    VoicePar[nvoice].OscilSmp->defaults();
    VoicePar[nvoice].FMSmp->defaults();

    VoicePar[nvoice].AmpEnvelope->defaults();
    VoicePar[nvoice].AmpLfo->defaults();

    VoicePar[nvoice].FreqEnvelope->defaults();
    VoicePar[nvoice].FreqLfo->defaults();

    VoicePar[nvoice].VoiceFilter->defaults();
    VoicePar[nvoice].FilterEnvelope->defaults();
    VoicePar[nvoice].FilterLfo->defaults();

    VoicePar[nvoice].FMFreqEnvelope->defaults();
    VoicePar[nvoice].FMAmpEnvelope->defaults();
}


// Init the voice parameters
void ADnoteParameters::enableVoice(int nvoice)
{
    VoicePar[nvoice].OscilSmp = new OscilGen(fft, GlobalPar.Reson, synth);
    VoicePar[nvoice].FMSmp = new OscilGen(fft, NULL, synth);

    VoicePar[nvoice].AmpEnvelope = new EnvelopeParams(64, 1, synth);
    VoicePar[nvoice].AmpEnvelope->ADSRinit_dB(0, 100, 127, 100);
    VoicePar[nvoice].AmpLfo = new LFOParams(90, 32, 64, 0, 0, 30, 0, 1, synth);

    VoicePar[nvoice].FreqEnvelope = new EnvelopeParams(0, 0, synth);
    VoicePar[nvoice].FreqEnvelope->ASRinit(30, 40, 64, 60);
    VoicePar[nvoice].FreqLfo = new LFOParams(50, 40, 0, 0, 0, 0, 0, 0, synth);

    VoicePar[nvoice].VoiceFilter = new FilterParams(2, 50, 60, 0, synth);
    VoicePar[nvoice].FilterEnvelope = new EnvelopeParams(0, 0, synth);
    VoicePar[nvoice].FilterEnvelope->ADSRinit_filter(90, 70, 40, 70, 10, 40);
    VoicePar[nvoice].FilterLfo = new LFOParams(50, 20, 64, 0, 0, 0, 0, 2, synth);

    VoicePar[nvoice].FMFreqEnvelope = new EnvelopeParams(0, 0, synth);
    VoicePar[nvoice].FMFreqEnvelope->ASRinit(20, 90, 40, 80);
    VoicePar[nvoice].FMAmpEnvelope = new EnvelopeParams(64, 1, synth);
    VoicePar[nvoice].FMAmpEnvelope->ADSRinit(80, 90, 127, 100);
}


// Get the Multiplier of the fine detunes of the voices
float ADnoteParameters::getBandwidthDetuneMultiplier(void)
{
    float bw = (GlobalPar.PBandwidth - 64.0f) / 64.0f;
    bw = powf(2.0f, bw * pow(fabs(bw), 0.2f) * 5.0f);
    return bw;
}


// Get the unison spread in cents for a voice
float ADnoteParameters::getUnisonFrequencySpreadCents(int nvoice)
{
    float unison_spread = VoicePar[nvoice].Unison_frequency_spread / 127.0f;
    unison_spread = powf(unison_spread * 2.0f, 2.0f) * 50.0f; // cents
    return unison_spread;
}


// Kill the voice
void ADnoteParameters::killVoice(int nvoice)
{
    delete VoicePar[nvoice].OscilSmp;
    delete VoicePar[nvoice].FMSmp;

    delete VoicePar[nvoice].AmpEnvelope;
    delete VoicePar[nvoice].AmpLfo;

    delete VoicePar[nvoice].FreqEnvelope;
    delete VoicePar[nvoice].FreqLfo;

    delete VoicePar[nvoice].VoiceFilter;
    delete VoicePar[nvoice].FilterEnvelope;
    delete VoicePar[nvoice].FilterLfo;

    delete VoicePar[nvoice].FMFreqEnvelope;
    delete VoicePar[nvoice].FMAmpEnvelope;
}


ADnoteParameters::~ADnoteParameters()
{
    delete GlobalPar.FreqEnvelope;
    delete GlobalPar.FreqLfo;
    delete GlobalPar.AmpEnvelope;
    delete GlobalPar.AmpLfo;
    delete GlobalPar.GlobalFilter;
    delete GlobalPar.FilterEnvelope;
    delete GlobalPar.FilterLfo;
    delete GlobalPar.Reson;

    for (int nvoice = 0; nvoice < NUM_VOICES; ++nvoice)
        killVoice(nvoice);
}


void ADnoteParameters::setGlobalPan(char pan)
{
    GlobalPar.PPanning = pan;
    if (!randomGlobalPan())
    {
        float t = (float)(GlobalPar.PPanning - 1) / 126.0f;
        GlobalPar.pangainL = cosf(t * HALFPI);
        GlobalPar.pangainR = cosf((1.0f - t) * HALFPI);
    }
    else
        GlobalPar.pangainL = GlobalPar.pangainR = 0.7f;
}


void ADnoteParameters::setVoicePan(int nvoice, char pan)
{
    VoicePar[nvoice].PPanning = pan;
    if (!randomVoicePan(nvoice))
    {
        float t = (float)(VoicePar[nvoice].PPanning - 1) / 126.0f;
        VoicePar[nvoice].pangainL = cosf(t * HALFPI);
        VoicePar[nvoice].pangainR = cosf((1.0f - t) * HALFPI);
    }
    else
        VoicePar[nvoice].pangainL = VoicePar[nvoice].pangainR = 0.7f;
}


void ADnoteParameters::add2XMLsection(XMLwrapper *xml, int n)
{
    int nvoice = n;
    if (nvoice >= NUM_VOICES)
        return;

    int oscilused = 0, fmoscilused = 0; // if the oscil or fmoscil are used by another voice

    for (int i = 0; i < NUM_VOICES; ++i)
    {
        if (VoicePar[i].Pextoscil == nvoice)
            oscilused = 1;
        if (VoicePar[i].PextFMoscil == nvoice)
            fmoscilused = 1;
    }

    xml->addparbool("enabled", VoicePar[nvoice].Enabled);
    if (((VoicePar[nvoice].Enabled == 0) && (oscilused==0) && (fmoscilused==0)) && (xml->minimal)) return;

    xml->addpar("type", VoicePar[nvoice].Type);

    xml->addpar("unison_size", VoicePar[nvoice].Unison_size);
    xml->addpar("unison_frequency_spread",
                VoicePar[nvoice].Unison_frequency_spread);
    xml->addpar("unison_stereo_spread", VoicePar[nvoice].Unison_stereo_spread);
    xml->addpar("unison_vibratto", VoicePar[nvoice].Unison_vibratto);
    xml->addpar("unison_vibratto_speed", VoicePar[nvoice].Unison_vibratto_speed);
    xml->addpar("unison_invert_phase", VoicePar[nvoice].Unison_invert_phase);
    xml->addpar("unison_phase_randomness", VoicePar[nvoice].Unison_phase_randomness);

    xml->addpar("delay", VoicePar[nvoice].PDelay);
    xml->addparbool("resonance", VoicePar[nvoice].Presonance);

    xml->addpar("ext_oscil", VoicePar[nvoice].Pextoscil);
    xml->addpar("ext_fm_oscil", VoicePar[nvoice].PextFMoscil);

    xml->addpar("oscil_phase", VoicePar[nvoice].Poscilphase);
    xml->addpar("oscil_fm_phase", VoicePar[nvoice].PFMoscilphase);

    xml->addparbool("filter_enabled", VoicePar[nvoice].PFilterEnabled);
    xml->addparbool("filter_bypass", VoicePar[nvoice].Pfilterbypass);

    xml->addpar("fm_enabled", VoicePar[nvoice].PFMEnabled);

    xml->beginbranch("OSCIL");
        VoicePar[nvoice].OscilSmp->add2XML(xml);
    xml->endbranch();


    xml->beginbranch("AMPLITUDE_PARAMETERS");
        xml->addpar("panning", VoicePar[nvoice].PPanning);
        xml->addpar("volume", VoicePar[nvoice].PVolume);
        xml->addparbool("volume_minus", VoicePar[nvoice].PVolumeminus);
        xml->addpar("velocity_sensing", VoicePar[nvoice].PAmpVelocityScaleFunction);
        xml->addparbool("amp_envelope_enabled", VoicePar[nvoice].PAmpEnvelopeEnabled);
        if ((VoicePar[nvoice].PAmpEnvelopeEnabled!=0) || (!xml->minimal))
        {
            xml->beginbranch("AMPLITUDE_ENVELOPE");
                VoicePar[nvoice].AmpEnvelope->add2XML(xml);
            xml->endbranch();
        }
        xml->addparbool("amp_lfo_enabled", VoicePar[nvoice].PAmpLfoEnabled);
        if ((VoicePar[nvoice].PAmpLfoEnabled != 0) || (!xml->minimal))
        {
            xml->beginbranch("AMPLITUDE_LFO");
                VoicePar[nvoice].AmpLfo->add2XML(xml);
            xml->endbranch();
        }
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");
        xml->addparbool("fixed_freq", VoicePar[nvoice].Pfixedfreq);
        xml->addpar("fixed_freq_et", VoicePar[nvoice].PfixedfreqET);
        xml->addpar("bend_adjust", VoicePar[nvoice].PBendAdjust);
        xml->addpar("offset_hz", VoicePar[nvoice].POffsetHz);
        xml->addpar("detune", VoicePar[nvoice].PDetune);
        xml->addpar("coarse_detune", VoicePar[nvoice].PCoarseDetune);
        xml->addpar("detune_type", VoicePar[nvoice].PDetuneType);

        xml->addparbool("freq_envelope_enabled", VoicePar[nvoice].PFreqEnvelopeEnabled);
        if ((VoicePar[nvoice].PFreqEnvelopeEnabled != 0) || (!xml->minimal))
        {
            xml->beginbranch("FREQUENCY_ENVELOPE");
                VoicePar[nvoice].FreqEnvelope->add2XML(xml);
            xml->endbranch();
        }
        xml->addparbool("freq_lfo_enabled", VoicePar[nvoice].PFreqLfoEnabled);
        if ((VoicePar[nvoice].PFreqLfoEnabled != 0) || (!xml->minimal))
        {
            xml->beginbranch("FREQUENCY_LFO");
                VoicePar[nvoice].FreqLfo->add2XML(xml);
            xml->endbranch();
        }
    xml->endbranch();

    if ((VoicePar[nvoice].PFilterEnabled != 0) || (!xml->minimal))
    {
        xml->beginbranch("FILTER_PARAMETERS");
            xml->addpar("velocity_sensing_amplitude", VoicePar[nvoice].PFilterVelocityScale);
            xml->addpar("velocity_sensing", VoicePar[nvoice].PFilterVelocityScaleFunction);
            xml->beginbranch("FILTER");
                VoicePar[nvoice].VoiceFilter->add2XML(xml);
            xml->endbranch();

            xml->addparbool("filter_envelope_enabled", VoicePar[nvoice].PFilterEnvelopeEnabled);
            if ((VoicePar[nvoice].PFilterEnvelopeEnabled != 0) || (!xml->minimal))
            {
                xml->beginbranch("FILTER_ENVELOPE");
                    VoicePar[nvoice].FilterEnvelope->add2XML(xml);
                xml->endbranch();
            }

            xml->addparbool("filter_lfo_enabled", VoicePar[nvoice].PFilterLfoEnabled);
            if ((VoicePar[nvoice].PFilterLfoEnabled !=0) || (!xml->minimal))
            {
                xml->beginbranch("FILTER_LFO");
                    VoicePar[nvoice].FilterLfo->add2XML(xml);
                xml->endbranch();
            }
            xml->endbranch();
    }

    if ((VoicePar[nvoice].PFMEnabled != 0) || (fmoscilused !=0 ) || (!xml->minimal))
    {
        xml->beginbranch("FM_PARAMETERS");
            xml->addpar("input_voice", VoicePar[nvoice].PFMVoice);

            xml->addpar("volume", VoicePar[nvoice].PFMVolume);
            xml->addpar("volume_damp", VoicePar[nvoice].PFMVolumeDamp);
            xml->addpar("velocity_sensing", VoicePar[nvoice].PFMVelocityScaleFunction);

            xml->addparbool("amp_envelope_enabled", VoicePar[nvoice].PFMAmpEnvelopeEnabled);
            if ((VoicePar[nvoice].PFMAmpEnvelopeEnabled != 0) || (!xml->minimal))
            {
                xml->beginbranch("AMPLITUDE_ENVELOPE");
                    VoicePar[nvoice].FMAmpEnvelope->add2XML(xml);
                xml->endbranch();
            }
            xml->beginbranch("MODULATOR");
                xml->addpar("detune", VoicePar[nvoice].PFMDetune);
                xml->addpar("coarse_detune", VoicePar[nvoice].PFMCoarseDetune);
                xml->addpar("detune_type", VoicePar[nvoice].PFMDetuneType);

                xml->addparbool("freq_envelope_enabled", VoicePar[nvoice].PFMFreqEnvelopeEnabled);
                xml->addparbool("fixed_freq", VoicePar[nvoice].PFMFixedFreq);
                if ((VoicePar[nvoice].PFMFreqEnvelopeEnabled != 0) || (!xml->minimal))
                {
                    xml->beginbranch("FREQUENCY_ENVELOPE");
                        VoicePar[nvoice].FMFreqEnvelope->add2XML(xml);
                    xml->endbranch();
                }

                xml->beginbranch("OSCIL");
                    VoicePar[nvoice].FMSmp->add2XML(xml);
                xml->endbranch();

            xml->endbranch();
        xml->endbranch();
    }
}


void ADnoteParameters::add2XML(XMLwrapper *xml)
{
    xml->information.ADDsynth_used = 1;

    xml->addparbool("stereo", GlobalPar.PStereo);

    xml->beginbranch("AMPLITUDE_PARAMETERS");
        xml->addpar("volume", GlobalPar.PVolume);
        xml->addpar("panning", GlobalPar.PPanning);
        xml->addpar("velocity_sensing", GlobalPar.PAmpVelocityScaleFunction);
        xml->addpar("fadein_adjustment", GlobalPar.Fadein_adjustment);
        xml->addpar("punch_strength", GlobalPar.PPunchStrength);
        xml->addpar("punch_time", GlobalPar.PPunchTime);
        xml->addpar("punch_stretch", GlobalPar.PPunchStretch);
        xml->addpar("punch_velocity_sensing", GlobalPar.PPunchVelocitySensing);
        xml->addpar("harmonic_randomness_grouping", GlobalPar.Hrandgrouping);

        xml->beginbranch("AMPLITUDE_ENVELOPE");
            GlobalPar.AmpEnvelope->add2XML(xml);
        xml->endbranch();

        xml->beginbranch("AMPLITUDE_LFO");
            GlobalPar.AmpLfo->add2XML(xml);
        xml->endbranch();
    xml->endbranch();

    xml->beginbranch("FREQUENCY_PARAMETERS");
        xml->addpar("detune", GlobalPar.PDetune);

        xml->addpar("coarse_detune", GlobalPar.PCoarseDetune);
        xml->addpar("detune_type", GlobalPar.PDetuneType);

        xml->addpar("bandwidth", GlobalPar.PBandwidth);

        xml->beginbranch("FREQUENCY_ENVELOPE");
            GlobalPar.FreqEnvelope->add2XML(xml);
        xml->endbranch();

        xml->beginbranch("FREQUENCY_LFO");
            GlobalPar.FreqLfo->add2XML(xml);
        xml->endbranch();
    xml->endbranch();


    xml->beginbranch("FILTER_PARAMETERS");
        xml->addpar("velocity_sensing_amplitude", GlobalPar.PFilterVelocityScale);
        xml->addpar("velocity_sensing", GlobalPar.PFilterVelocityScaleFunction);

        xml->beginbranch("FILTER");
            GlobalPar.GlobalFilter->add2XML(xml);
        xml->endbranch();

        xml->beginbranch("FILTER_ENVELOPE");
            GlobalPar.FilterEnvelope->add2XML(xml);
        xml->endbranch();

        xml->beginbranch("FILTER_LFO");
            GlobalPar.FilterLfo->add2XML(xml);
        xml->endbranch();
    xml->endbranch();

    xml->beginbranch("RESONANCE");
        GlobalPar.Reson->add2XML(xml);
    xml->endbranch();

    for (int nvoice=0;nvoice<NUM_VOICES;nvoice++)
    {
        xml->beginbranch("VOICE",nvoice);
        add2XMLsection(xml,nvoice);
        xml->endbranch();
    }
}


void ADnoteParameters::getfromXML(XMLwrapper *xml)
{
    GlobalPar.PStereo = (xml->getparbool("stereo", GlobalPar.PStereo)) != 0;

    if (xml->enterbranch("AMPLITUDE_PARAMETERS"))
    {
        GlobalPar.PVolume = xml->getpar127("volume", GlobalPar.PVolume);
        setGlobalPan(xml->getpar127("panning", GlobalPar.PPanning));
        GlobalPar.PAmpVelocityScaleFunction =
            xml->getpar127("velocity_sensing", GlobalPar.PAmpVelocityScaleFunction);
        GlobalPar.Fadein_adjustment = xml->getpar127("fadein_adjustment", GlobalPar.Fadein_adjustment);
        GlobalPar.PPunchStrength =
            xml->getpar127("punch_strength", GlobalPar.PPunchStrength);
        GlobalPar.PPunchTime = xml->getpar127("punch_time", GlobalPar.PPunchTime);
        GlobalPar.PPunchStretch = xml->getpar127("punch_stretch", GlobalPar.PPunchStretch);
        GlobalPar.PPunchVelocitySensing =
            xml->getpar127("punch_velocity_sensing", GlobalPar.PPunchVelocitySensing);
        GlobalPar.Hrandgrouping =
            xml->getpar127("harmonic_randomness_grouping", GlobalPar.Hrandgrouping);

        if (xml->enterbranch("AMPLITUDE_ENVELOPE"))
        {
            GlobalPar.AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if (xml->enterbranch("AMPLITUDE_LFO"))
        {
            GlobalPar.AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }

        xml->exitbranch();
    }

    if (xml->enterbranch("FREQUENCY_PARAMETERS"))
    {
        GlobalPar.PDetune = xml->getpar("detune", GlobalPar.PDetune, 0, 16383);
        GlobalPar.PCoarseDetune =
            xml->getpar("coarse_detune", GlobalPar.PCoarseDetune, 0, 16383);
        GlobalPar.PDetuneType =
            xml->getpar127("detune_type", GlobalPar.PDetuneType);

        GlobalPar.PBandwidth = xml->getpar127("bandwidth", GlobalPar.PBandwidth);

        xml->enterbranch("FREQUENCY_ENVELOPE");
            GlobalPar.FreqEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FREQUENCY_LFO");
            GlobalPar.FreqLfo->getfromXML(xml);
        xml->exitbranch();

        xml->exitbranch();
    }


    if (xml->enterbranch("FILTER_PARAMETERS"))
    {
        GlobalPar.PFilterVelocityScale =
            xml->getpar127("velocity_sensing_amplitude", GlobalPar.PFilterVelocityScale);
        GlobalPar.PFilterVelocityScaleFunction =
            xml->getpar127("velocity_sensing", GlobalPar.PFilterVelocityScaleFunction);

        xml->enterbranch("FILTER");
            GlobalPar.GlobalFilter->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_ENVELOPE");
            GlobalPar.FilterEnvelope->getfromXML(xml);
        xml->exitbranch();

        xml->enterbranch("FILTER_LFO");
            GlobalPar.FilterLfo->getfromXML(xml);
        xml->exitbranch();

        xml->exitbranch();
    }

    if (xml->enterbranch("RESONANCE"))
    {
        GlobalPar.Reson->getfromXML(xml);
        xml->exitbranch();
    }

    for (int nvoice = 0; nvoice < NUM_VOICES; nvoice++)
    {
        VoicePar[nvoice].Enabled=0;
        if (xml->enterbranch("VOICE", nvoice) == 0)
            continue;
        getfromXMLsection(xml, nvoice);
        xml->exitbranch();
    }
}

void ADnoteParameters::getfromXMLsection(XMLwrapper *xml, int n)
{
    int nvoice=n;
    if (nvoice>=NUM_VOICES) return;

    VoicePar[nvoice].Enabled = xml->getparbool("enabled", 0);

    VoicePar[nvoice].Unison_size =
        xml->getpar127("unison_size", VoicePar[nvoice].Unison_size);
    VoicePar[nvoice].Unison_frequency_spread =
        xml->getpar127("unison_frequency_spread", VoicePar[nvoice].Unison_frequency_spread);
    VoicePar[nvoice].Unison_stereo_spread =
        xml->getpar127("unison_stereo_spread", VoicePar[nvoice].Unison_stereo_spread);
    VoicePar[nvoice].Unison_vibratto =
        xml->getpar127("unison_vibratto", VoicePar[nvoice].Unison_vibratto);
    VoicePar[nvoice].Unison_vibratto_speed =
        xml->getpar127("unison_vibratto_speed", VoicePar[nvoice].Unison_vibratto_speed);
    VoicePar[nvoice].Unison_invert_phase =
        xml->getpar127("unison_invert_phase", VoicePar[nvoice].Unison_invert_phase);
    VoicePar[nvoice].Unison_phase_randomness =
        xml->getpar127("unison_phase_randomness", VoicePar[nvoice].Unison_phase_randomness);

    VoicePar[nvoice].Type = xml->getpar127("type", VoicePar[nvoice].Type);
    VoicePar[nvoice].PDelay = xml->getpar127("delay", VoicePar[nvoice].PDelay);
    VoicePar[nvoice].Presonance = xml->getparbool("resonance", VoicePar[nvoice].Presonance);

    VoicePar[nvoice].Pextoscil = xml->getpar("ext_oscil", -1, -1, nvoice - 1);
    VoicePar[nvoice].PextFMoscil = xml->getpar("ext_fm_oscil", -1, -1,nvoice - 1);

    VoicePar[nvoice].Poscilphase =
        xml->getpar127("oscil_phase", VoicePar[nvoice].Poscilphase);
    VoicePar[nvoice].PFMoscilphase =
        xml->getpar127("oscil_fm_phase", VoicePar[nvoice].PFMoscilphase);

    VoicePar[nvoice].PFilterEnabled =
        xml->getparbool("filter_enabled",VoicePar[nvoice].PFilterEnabled);
    VoicePar[nvoice].Pfilterbypass =
        xml->getparbool("filter_bypass",VoicePar[nvoice].Pfilterbypass);

    VoicePar[nvoice].PFMEnabled = xml->getpar127("fm_enabled",VoicePar[nvoice].PFMEnabled);

    if (xml->enterbranch("OSCIL"))
    {
        VoicePar[nvoice].OscilSmp->getfromXML(xml);
        xml->exitbranch();
    }

    if (xml->enterbranch("AMPLITUDE_PARAMETERS"))
    {
        setVoicePan(nvoice, xml->getpar127("panning", VoicePar[nvoice].PPanning));
        VoicePar[nvoice].PVolume = xml->getpar127("volume", VoicePar[nvoice].PVolume);
        VoicePar[nvoice].PVolumeminus =
            xml->getparbool("volume_minus", VoicePar[nvoice].PVolumeminus);
        VoicePar[nvoice].PAmpVelocityScaleFunction =
            xml->getpar127("velocity_sensing", VoicePar[nvoice].PAmpVelocityScaleFunction);

        VoicePar[nvoice].PAmpEnvelopeEnabled =
            xml->getparbool("amp_envelope_enabled",VoicePar[nvoice].PAmpEnvelopeEnabled);
        if (xml->enterbranch("AMPLITUDE_ENVELOPE"))
        {
            VoicePar[nvoice].AmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice].PAmpLfoEnabled =
            xml->getparbool("amp_lfo_enabled",VoicePar[nvoice].PAmpLfoEnabled);
        if (xml->enterbranch("AMPLITUDE_LFO"))
        {
            VoicePar[nvoice].AmpLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if (xml->enterbranch("FREQUENCY_PARAMETERS"))
    {
        VoicePar[nvoice].Pfixedfreq =
            xml->getparbool("fixed_freq", VoicePar[nvoice].Pfixedfreq);
        VoicePar[nvoice].PfixedfreqET =
            xml->getpar127("fixed_freq_et", VoicePar[nvoice].PfixedfreqET);
        VoicePar[nvoice].PBendAdjust =
            xml->getpar127("bend_adjust", VoicePar[nvoice].PBendAdjust);
        VoicePar[nvoice].POffsetHz =
            xml->getpar127("offset_hz", VoicePar[nvoice].POffsetHz);


        VoicePar[nvoice].PDetune =
            xml->getpar("detune", VoicePar[nvoice].PDetune, 0, 16383);

        VoicePar[nvoice].PCoarseDetune =
            xml->getpar("coarse_detune", VoicePar[nvoice].PCoarseDetune, 0, 16383);
        VoicePar[nvoice].PDetuneType =
            xml->getpar127("detune_type", VoicePar[nvoice].PDetuneType);

        VoicePar[nvoice].PFreqEnvelopeEnabled =
            xml->getparbool("freq_envelope_enabled", VoicePar[nvoice].PFreqEnvelopeEnabled);
        if (xml->enterbranch("FREQUENCY_ENVELOPE"))
        {
            VoicePar[nvoice].FreqEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice].PFreqLfoEnabled =
            xml->getparbool("freq_lfo_enabled", VoicePar[nvoice].PFreqLfoEnabled);
        if (xml->enterbranch("FREQUENCY_LFO"))
        {
            VoicePar[nvoice].FreqLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if (xml->enterbranch("FILTER_PARAMETERS"))
    {
        VoicePar[nvoice].PFilterVelocityScale =
            xml->getpar127("velocity_sensing_amplitude",
            VoicePar[nvoice].PFilterVelocityScale);
        VoicePar[nvoice].PFilterVelocityScaleFunction =
            xml->getpar127("velocity_sensing",
            VoicePar[nvoice].PFilterVelocityScaleFunction);

        if (xml->enterbranch("FILTER"))
        {
            VoicePar[nvoice].VoiceFilter->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice].PFilterEnvelopeEnabled =
            xml->getparbool("filter_envelope_enabled", VoicePar[nvoice].PFilterEnvelopeEnabled);
        if (xml->enterbranch("FILTER_ENVELOPE"))
        {
            VoicePar[nvoice].FilterEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        VoicePar[nvoice].PFilterLfoEnabled =
            xml->getparbool("filter_lfo_enabled", VoicePar[nvoice].PFilterLfoEnabled);
        if (xml->enterbranch("FILTER_LFO"))
        {
            VoicePar[nvoice].FilterLfo->getfromXML(xml);
            xml->exitbranch();
        }
        xml->exitbranch();
    }

    if (xml->enterbranch("FM_PARAMETERS"))
    {
        VoicePar[nvoice].PFMVoice =
            xml->getpar("input_voice", VoicePar[nvoice].PFMVoice, -1, nvoice - 1);

        VoicePar[nvoice].PFMVolume = xml->getpar127("volume", VoicePar[nvoice].PFMVolume);
        VoicePar[nvoice].PFMVolumeDamp =
            xml->getpar127("volume_damp", VoicePar[nvoice].PFMVolumeDamp);
        VoicePar[nvoice].PFMVelocityScaleFunction =
            xml->getpar127("velocity_sensing", VoicePar[nvoice].PFMVelocityScaleFunction);

        VoicePar[nvoice].PFMAmpEnvelopeEnabled =
            xml->getparbool("amp_envelope_enabled", VoicePar[nvoice].PFMAmpEnvelopeEnabled);
        if (xml->enterbranch("AMPLITUDE_ENVELOPE"))
        {
            VoicePar[nvoice].FMAmpEnvelope->getfromXML(xml);
            xml->exitbranch();
        }

        if (xml->enterbranch("MODULATOR"))
        {
            VoicePar[nvoice].PFMDetune =
                xml->getpar("detune",VoicePar[nvoice].PFMDetune, 0, 16383);
            VoicePar[nvoice].PFMCoarseDetune =
                xml->getpar("coarse_detune", VoicePar[nvoice].PFMCoarseDetune, 0, 16383);
            VoicePar[nvoice].PFMDetuneType =
                xml->getpar127("detune_type", VoicePar[nvoice].PFMDetuneType);

            VoicePar[nvoice].PFMFreqEnvelopeEnabled =
                xml->getparbool("freq_envelope_enabled",
                                VoicePar[nvoice].PFMFreqEnvelopeEnabled);
            VoicePar[nvoice].PFMFixedFreq = xml->getparbool("fixed_freq", VoicePar[nvoice].PFMFixedFreq);
            if (xml->enterbranch("FREQUENCY_ENVELOPE"))
            {
                VoicePar[nvoice].FMFreqEnvelope->getfromXML(xml);
                xml->exitbranch();
            }

            if (xml->enterbranch("OSCIL"))
            {
                VoicePar[nvoice].FMSmp->getfromXML(xml);
                xml->exitbranch();
            }

            xml->exitbranch();
        }
        xml->exitbranch();
    }
}


void ADnoteParameters::getLimits(CommandBlock *getData)
{
    int control = getData->data.control;
    int engine = getData->data.engine;

    // defaults
    int type = 0;
    int min = 0;
    int def = 0;
    int max = 127;

    if (engine < 0x80)
    {
        switch (control)
        {
            case 0:
                type |= 0x40;
                def = 900;
                break;

            case 1:
                type |= 0x40;
                def = 640;
                break;

            case 2:
                type |= 0x40;
                def = 640;
                break;

            case 8:
                type |= 0x40;
                def = 10;
                max = 1;
                break;

            case 32:
                type |= 0x40;
                min = -8192;
                max = 8191;
                break;

            case 35:
                type |= 0x40;
                min = -8;
                max = 7;
                break;

            case 36:
                max = 3;
                break;

            case 37:
                min = -64;
                max = 63;
                break;

            case 39:
                type |= 0x40;
                def = 640;
                break;

            case 112:
                type |= 0x40;
                def = 10;
                max = 1;
                break;

            case 113:
                max = 1;
                break;

            case 120:
                def = FADEIN_ADJUSTMENT_SCALE * 10;

            case 121: // just ensures it doesn't get caught by default
                break;

            case 122:
                def = 600;
                break;

            case 123:
                def = 640;
                break;

            case 124:
                def = 720;
                break;

            default:
                min = -1;
                def = -10;
                max = -1;
                break;
        }
        getData->data.type |= type;
        getData->limits.min = min;
        getData->limits.def = def;
        getData->limits.max = max;
        return;
    }

    switch (control)
    {
        case 0:
            type |= 0x40;
            def = 1000;
            break;

        case 1:
            type |= 0x40;
            def = 1270;
            break;

        case 2:
            type |= 0x40;
            def = 640;
            break;

        case 4:
            max = 1;
            break;

        case 8:
        case 9:
        case 88:
            type |= 0x40;
            max = 1;
            break;

        case 16:
            type |= 0x40;
            max = 5;
            break;

        case 17:
            min = -1;
            def = -10;
            max = 6;
            break;

        case 32:
        case 96:
            type |= 0x40;
            min = -8192;
            max = 8191;
            break;

        case 33:
            type |= 0x40;
            break;

        case 34:
        case 98:
            max = 1;
            break;

        case 35:
        case 99:
            type |= 0x40;
            min = -8;
            max = 7;
            break;

        case 36:
        case 100:
            max = 4;
            break;

        case 37:
        case 101:
            min = -64;
            max = 63;
            break;

        case 38:
            type |= 0x40;
            def = 880;
            break;

        case 39:
            type |= 0x40;
            def = 640;
            break;

        case 40:
        case 41:
        case 104:
            type |= 0x40;
            max = 1;
            break;

        case 48:
            type |= 0x40;
            def = 600;
            break;

        case 49:
            type |= 0x40;
            def = 1270;
            break;

        case 50:
            type |= 0x40;
            def = 640;
            break;

        case 51:
            type |= 0x40;
            def = 640;
            break;

        case 52:
            type |= 0x40;
            def = 640;
            break;

        case 53:
            min = 2;
            def = 20;
            max = 50;
            break;

        case 54:
            max = 5;
            break;

        case 56:
            max = 1;
            break;

        case 64:
            max = 1;
            break;

        case 68:
        case 72:
        case 73:
            type |= 0x40;
            max = 1;
            break;

        case 80:
            type |= 0x40;
            def = 900;
            break;

        case 81:
            type |= 0x40;
            def = 640;
            break;

        case 82:
        case 112:
        case 136:
            type |= 0x40;
            min = -64;
            max = 63;
            break;

        case 113:
            min = -1;
            def = -10;
            max = 6;
            break;

        case 128:
            type |= 0x40;
            break;

        case 129:
            type |= 0x40;
            if (engine == 0x80)
                def = 10;
            max = 1;
            break;

        case 130:
            def = 10;
            max = 1;
            break;

        case 137:
            min = -1;
            def = -10;
            max = 6;
            break;

        case 138:
            max = 2;
            break;

        default:
            min = -1;
            def = -10;
            max = -1;
            break;
    }
    getData->data.type |= type;
    getData->limits.min = min;
    getData->limits.def = def;
    getData->limits.max = max;
}

void ADnoteParameters::postrender(void)
{
    // loop over our gathered dirty flags and unset them for the next period
      GlobalPar.AmpLfo->updated
    = GlobalPar.FilterLfo->updated
    = GlobalPar.FreqLfo->updated
    = false;
    for (int i = 0; i < NUM_VOICES; ++i)
    {
        if (VoicePar[i].Enabled)
              VoicePar[i].AmpLfo->updated
            = VoicePar[i].FilterLfo->updated
            = VoicePar[i].FreqLfo->updated
            = false;

    }
}
