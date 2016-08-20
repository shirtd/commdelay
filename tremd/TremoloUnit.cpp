/*
<samplecode>
<abstract>Tremolo Effect AU</abstract>
</samplecode>
*/

// This file defines the TremoloUnit class, as well as the TremoloUnitEffectKernel helper class.

#include "TremoloUnit.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
AUDIOCOMPONENT_ENTRY(AUBaseFactory, TremoloUnit)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The COMPONENT_ENTRY macro is required for the Mac OS X Component Manager to recognize and 
// use the audio unit

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The constructor for new TremoloUnit audio units
TremoloUnit::TremoloUnit (AudioUnit component) : AUEffectBase (component) {

	// This method, defined in the AUBase superclass, ensures that the required audio unit
	//  elements are created and initialized.
	CreateElements ();
	
	// Invokes the use of an STL vector for parameter access.  
	//  See AUBase/AUScopeElement.cpp
	Globals () -> UseIndexedParameters (kNumberOfParameters);

	// During instantiation, sets up the parameters according to their defaults.
	//	The parameter defaults should correspond to the settings for the default preset.
	SetParameter (
		kParameter_Frequency, 
		kDefaultValue_Tremolo_Freq 
	);
    
    SetParameter (
                  kParameter_Signature,
                  kDefaultValue_Signature
                  );
    
    SetParameter (
                  kParameter_Speed,
                  kDefaultValue_Speed
                  );
    
    SetParameter (
                  kParameter_Depth,
                  kDefaultValue_Tremolo_Depth
                  );
    
    SetParameter (
                  kParameter_Waveform,
                  kDefaultValue_Tremolo_Waveform
                  );
    SetParameter (
                  kParameter_Ring,
                  kDefaultValue_Ring
                  );
    
    SetParameter (
                  kParameter_Ring_Signature,
                  kDefaultValue_Ring_Signature
                  );
    
    SetParameter (
                  kParameter_Ring_Speed,
                  kDefaultValue_Ring_Speed
                  );
    
    SetParameter (
                  kParameter_Ring_Depth,
                  kDefaultValue_Ring_Depth
                  );
                  
                  
    SetParameter (
                  kParameter_Ring_Waveform,
                  kDefaultValue_Ring_Waveform
                  );
    
    SetParameter (
                  kParameter_Signal_Power,
                  kDefaultValue_Signal_Power
                  );
    
    SetParameter (
                  kParameter_Delay_Power,
                  kDefaultValue_Delay_Power
                  );

	// Also during instantiation, sets the preset menu to indicate the default preset,
	//	which corresponds to the default parameters. It's possible to set this so a
	//	fresh audio unit indicates the wrong preset, so be careful to get it right.
	SetAFactoryPresetAsCurrent (
		kPresets [kPreset_Default]
	);
        
	#if AU_DEBUG_DISPATCHER
		mDebugDispatcher = new AUDebugDispatcher (this);
	#endif
}


#pragma mark ____Parameters

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Called by the audio unit's view; provides information needed for the view to display the
//  audio unit's parameters
ComponentResult TremoloUnit::GetParameterInfo (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		AudioUnitParameterInfo	&outParameterInfo
) {
	ComponentResult result = noErr;

	// Adds two flags to all parameters for the audio unit, indicating to the host application 
	// that it should consider all the audio unit’s parameters to be readable and writable.
	outParameterInfo.flags = 	  
		kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
    
    // All three parameters for this audio unit are in the "global" scope.
	if (inScope == kAudioUnitScope_Global) {
        switch (inParameterID) {
		
            case kParameter_Frequency:
			// Invoked when the view needs information for the kTremoloParam_Frequency 
			// parameter; defines how to represent this parameter in the user interface.
				AUBase::FillInParameterName (
					outParameterInfo,
					kParamName_Tremolo_Freq,
					false
				);
				outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
					// Sets the unit of measurement for the Frequency parameter to Hertz.
				outParameterInfo.minValue		= kMinimumValue_Tremolo_Freq;
					// Sets the minimum value for the Frequency parameter.
				outParameterInfo.maxValue		= kMaximumValue_Tremolo_Freq;
					// Sets the maximum value for the Frequency parameter.
				outParameterInfo.defaultValue	= kDefaultValue_Tremolo_Freq;
					// Sets the default value for the Frequency parameter.
				outParameterInfo.flags			|= kAudioUnitParameterFlag_DisplayLogarithmic;
					// Adds a flag to indicate to the host that it should use a logarithmic 
					// control for the Frequency parameter.
				break;
                
            case kParameter_Signature:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Signature,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signature;
                outParameterInfo.maxValue		= kMaximumValue_Signature;
                outParameterInfo.defaultValue	= kDefaultValue_Signature;
                break;
                
            case kParameter_Speed:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Speed,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Speed;
                outParameterInfo.maxValue		= kMaximumValue_Speed;
                outParameterInfo.defaultValue	= kDefaultValue_Speed;
                break;
                
            case kParameter_Depth:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Tremolo_Depth,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Tremolo_Depth;
                outParameterInfo.maxValue		= kMaximumValue_Tremolo_Depth;
                outParameterInfo.defaultValue	= kDefaultValue_Tremolo_Depth;
                break;
                
            case kParameter_Waveform:
                // Invoked when the view needs information for the kTremoloParam_Waveform parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Tremolo_Waveform,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                // Sets the unit of measurement for the Waveform parameter to "indexed," allowing
                // it to be displayed as a pop-up menu in the generic view. The following three
                // statements set the minimum, maximum, and default values for the depth parameter.
                // All three are required for proper functioning of the parameter's user interface.
                outParameterInfo.minValue		= kSineWave_Tremolo_Waveform;
                outParameterInfo.maxValue		= kSquareWave_Tremolo_Waveform;
                outParameterInfo.defaultValue	= kDefaultValue_Tremolo_Waveform;
                break;
                
            case kParameter_Ring:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Ring,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Ring;
                outParameterInfo.maxValue		= kMaximumValue_Ring;
                outParameterInfo.defaultValue	= kDefaultValue_Ring;
                break;
            case kParameter_Ring_Signature:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Signature,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signature;
                outParameterInfo.maxValue		= kMaximumValue_Signature;
                outParameterInfo.defaultValue	= kDefaultValue_Signature;
                break;
                
            case kParameter_Ring_Speed:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Speed,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Speed;
                outParameterInfo.maxValue		= kMaximumValue_Speed;
                outParameterInfo.defaultValue	= kDefaultValue_Speed;
                break;
                
            case kParameter_Ring_Depth:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Tremolo_Depth,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Tremolo_Depth;
                outParameterInfo.maxValue		= kMaximumValue_Tremolo_Depth;
                outParameterInfo.defaultValue	= kDefaultValue_Tremolo_Depth;
                break;
                
            case kParameter_Ring_Waveform:
                // Invoked when the view needs information for the kTremoloParam_Waveform parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Tremolo_Waveform,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                // Sets the unit of measurement for the Waveform parameter to "indexed," allowing
                // it to be displayed as a pop-up menu in the generic view. The following three
                // statements set the minimum, maximum, and default values for the depth parameter.
                // All three are required for proper functioning of the parameter's user interface.
                outParameterInfo.minValue		= kSineWave_Tremolo_Waveform;
                outParameterInfo.maxValue		= kSquareWave_Tremolo_Waveform;
                outParameterInfo.defaultValue	= kDefaultValue_Tremolo_Waveform;
                break;
                
            case kParameter_Signal_Power:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Signal_Power,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power;
                break;
                
            case kParameter_Delay_Power:
                // Invoked when the view needs information for the kTremoloParam_Depth parameter.
                AUBase::FillInParameterName (
                                             outParameterInfo,
                                             kParamName_Delay_Power,
                                             false
                                             );
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Delay_Power;
                outParameterInfo.maxValue		= kMaximumValue_Delay_Power;
                outParameterInfo.defaultValue	= kDefaultValue_Delay_Power;
                break;

			default:
				result = kAudioUnitErr_InvalidParameter;
				break;
		}
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Provides the strings for the Waveform popup menu in the generic view
ComponentResult TremoloUnit::GetParameterValueStrings (
	AudioUnitScope			inScope,
	AudioUnitParameterID	inParameterID,
	CFArrayRef				*outStrings
) {
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Waveform)) {
	// This method applies only to the waveform parameter, which is in the global scope.
	
		// When this method gets called by the AUBase::DispatchGetPropertyInfo method, which 
		// provides a null value for the outStrings parameter, just return without error.
		if (outStrings == NULL) return noErr;
		
		// Defines an array that contains the pop-up menu item names.
		CFStringRef	strings [] = {
			kMenuItem_Tremolo_Sine,
			kMenuItem_Tremolo_Square
		};
   
		// Creates a new immutable array containing the menu item names, and places the array 
		// in the outStrings output parameter.
		*outStrings = CFArrayCreate (
			NULL,
			(const void **) strings,
			(sizeof (strings) / sizeof (strings [0])),
			NULL
		);
		return noErr;
    }
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Ring_Waveform)) {
        // This method applies only to the waveform parameter, which is in the global scope.
        
        // When this method gets called by the AUBase::DispatchGetPropertyInfo method, which
        // provides a null value for the outStrings parameter, just return without error.
        if (outStrings == NULL) return noErr;
        
        // Defines an array that contains the pop-up menu item names.
        CFStringRef	strings [] = {
            kMenuItem_Ring_Sine,
            kMenuItem_Ring_Square
        };
        
        // Creates a new immutable array containing the menu item names, and places the array
        // in the outStrings output parameter.
        *outStrings = CFArrayCreate (
                                     NULL,
                                     (const void **) strings,
                                     (sizeof (strings) / sizeof (strings [0])),
                                     NULL
                                     );
        return noErr;
    }
    return kAudioUnitErr_InvalidParameter;
}

#pragma mark ____Properties

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult TremoloUnit::GetPropertyInfo (
	// This audio unit doesn't define any custom properties, so it uses this generic code for
	// this method.
	AudioUnitPropertyID	inID,
	AudioUnitScope		inScope,
	AudioUnitElement	inElement,
	UInt32				&outDataSize,
	Boolean				&outWritable
) {
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult TremoloUnit::GetProperty (
	// This audio unit doesn't define any custom properties, so it uses this generic code for
	// this method.
	AudioUnitPropertyID inID,
	AudioUnitScope 		inScope,
	AudioUnitElement 	inElement,
	void				*outData
) {
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}

#pragma mark ____Factory Presets

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::GetPresets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// For users to be able to use the factory presets you define, you must add a generic 
// implementation of the GetPresets method. The code here works for any audio unit that can 
// support factory presets.

// The GetPresets method accepts a single parameter, a pointer to a CFArrayRef object. This 
// object holds the factory presets array generated by this method. The array contains just 
// factory preset numbers and names. The host application uses this array to set up its 
// factory presets menu and when calling the NewFactoryPresetSet method.

ComponentResult TremoloUnit::GetPresets (
	CFArrayRef	*outData
) const {

	// Checks whether factory presets are implemented for this audio unit.
	if (outData == NULL) return noErr;
	
	// Instantiates a mutable Core Foundation array to hold the factory presets.
	CFMutableArrayRef presetsArray = CFArrayCreateMutable (
		NULL,
		kNumberPresets,
		NULL
	);
	
	// Fills the factory presets array with values from the definitions in the TremoloUnit.h 
	// file.
	for (int i = 0; i < kNumberPresets; ++i) {
		CFArrayAppendValue (
			presetsArray,
			&kPresets [i]
		);
    }
    
	// Stores the factory presets array at the outData location.
	*outData = (CFArrayRef) presetsArray;
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::NewFactoryPresetSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// The NewFactoryPresetSet method defines all the factory presets for an audio unit. Basically, 
// for each preset, it invokes a series of SetParameter calls.

// This method takes a single argument of type AUPreset, a structure containing a factory
//  preset name and number.
OSStatus TremoloUnit::NewFactoryPresetSet (
	const AUPreset &inNewFactoryPreset
) {
	// Gets the number of the desired factory preset.
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	if (
		// Tests whether the desired factory preset is defined.
		chosenPreset == kPreset_Slow ||
		chosenPreset == kPreset_Fast
	) {
		// This 'for' loop, and the 'if' statement that follows it, allow for noncontiguous preset 
		// numbers.
		for (int i = 0; i < kNumberPresets; ++i) {
			if (chosenPreset == kPresets[i].presetNumber) {

				//Selects the appropriate case statement based on the factory preset number.
				switch (chosenPreset) {

					// The settings for the "Slow & Gentle" factory preset.
					case kPreset_Slow:
						SetParameter (
							kParameter_Frequency,
							kParameter_Preset_Frequency_Slow
						);
						SetParameter (
							kParameter_Depth,
							kParameter_Preset_Depth_Slow
						);
						SetParameter (
							kParameter_Waveform,
							kParameter_Preset_Waveform_Slow
						);
						break;
					
					// The settings for the "Fast & Hard" factory preset.
					case kPreset_Fast:
						SetParameter (
							kParameter_Frequency,
							kParameter_Preset_Frequency_Fast
						);
						SetParameter (
							kParameter_Depth,
							kParameter_Preset_Depth_Fast
						);
						SetParameter (
							kParameter_Waveform,
							kParameter_Preset_Waveform_Fast
						);
						break;
				}
				
				// Updates the preset menu in the generic view to display the new factory preset.
				SetAFactoryPresetAsCurrent (
					kPresets [i]
				);
				return noErr;
			}
		}
	}
	return kAudioUnitErr_InvalidProperty;
}



#pragma mark ____TremoloUnitEffectKernel

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::TremoloUnitKernel()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This is the constructor for the TremoloUnitKernel helper class, which holds the DSP code 
//  for the audio unit. TremoloUnit is an n-to-n audio unit; one kernel object gets built for 
//  each channel in the audio unit.
//
// The first line of the method consists of the constructor method declarator and constructor-
//  initializer. In addition to calling the appropriate superclasses, this code initializes two 
//  member variables:
//
// (In the Xcode template, the header file contains the call to the superclass constructor.)
TremoloUnit::TremoloUnitKernel::TremoloUnitKernel (AUEffectBase *inAudioUnit ) : AUKernelBase (inAudioUnit),
	mSamplesProcessed (0)//, mCurrentScale (0)
{	
	mSampleFrequency = GetSampleRate ();
    
    memset(lastDelay,0,sizeof(float)*maxDelaySamples);
    memset(delay,0,sizeof(float)*maxDelaySamples);
//    for (int i = 0; i < maxDelaySamples; i++) {
//        lastDelay[i] = 0;
//    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Because we're calculating each output sample based on a unique input sample, there's no 
// need to clear any buffers. We simply reinitialize the variables that were initialized on
// instantiation of the kernel object.
void TremoloUnit::TremoloUnitKernel::Reset() {
	mSamplesProcessed	= 0;
    head = 0;
    memset(lastDelay,0,sizeof(float)*maxDelaySamples);
    memset(delay,0,sizeof(float)*maxDelaySamples);
//    for (int i = 0; i < maxDelaySamples; i++) {
//        lastDelay[i] = 0;
//    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This method contains the DSP code. 
void TremoloUnit::TremoloUnitKernel::Process (
	const Float32 	*inSourceP,			// The audio sample input buffer.
	Float32		 	*inDestP,			// The audio sample output buffer.
	UInt32 			inSamplesToProcess,	// The number of samples in the input buffer.
	UInt32			inNumChannels,		// The number of input channels. This is always equal to 1 
										//   because there is always one kernel object instantiated
										//   per channel of audio.
	bool			&ioSilence			// A Boolean flag indicating whether the input to the audio 
										//   unit consists of silence, with a TRUE value indicating 
										//   silence.
) {
	if (!ioSilence) {
		// Assigns a pointer variable to the start of the audio sample input buffer.
		const Float32 *sourceP = inSourceP;
		// Assigns a pointer variable to the start of the audio sample output buffer.
		Float32	*destP = inDestP,
				ring,
                ringDepth,
                tremoloDepth,
                signalPower,
                delayPower;
        int     tremoloFrequency,
                samplesPerTremoloCycle;
				
		int		tremoloWaveform, speed, signature;
        int		ringWaveform, ringSpeed, ringSignature;
        
        tremoloFrequency = GetParameter (kParameter_Frequency);
        if (tremoloFrequency != lastLength) {
            if (tremoloFrequency	< kMinimumValue_Tremolo_Freq)
                tremoloFrequency	= kMinimumValue_Tremolo_Freq;
            if (tremoloFrequency	> kMaximumValue_Tremolo_Freq)
                tremoloFrequency	= kMaximumValue_Tremolo_Freq;
            lastLength = tremoloFrequency;
        }
        signature = (int) GetParameter(kParameter_Signature);
        if (signature != lastSignature) {
            if (signature	< kMinimumValue_Signature)
                signature	= kMinimumValue_Signature;
            if (signature	> kMaximumValue_Signature)
                signature	= kMaximumValue_Signature;
            lastSignature = signature;
        }
        speed = (int) GetParameter(kParameter_Speed);
        if (speed != lastSpeed) {
            if (speed		< kMinimumValue_Speed)
                speed		= kMinimumValue_Speed;
            if (speed		> kMaximumValue_Speed)
                speed		= kMaximumValue_Speed;
            lastSpeed = speed;
        }
		tremoloDepth = GetParameter (kParameter_Depth);
        if (tremoloDepth != lastDepth) {
            if (tremoloDepth		< kMinimumValue_Tremolo_Depth)
                tremoloDepth		= kMinimumValue_Tremolo_Depth;
            if (tremoloDepth		> kMaximumValue_Tremolo_Depth)
                tremoloDepth		= kMaximumValue_Tremolo_Depth;
        }
		tremoloWaveform =  (int) GetParameter (kParameter_Waveform);
        if (tremoloWaveform != lastDirection) {
            if (tremoloWaveform == kSineWave_Tremolo_Waveform)  {
                direction = 1;
            } else {
                direction = -1;
            }
        }
        ring = GetParameter(kParameter_Ring);
        if (ring		< kMinimumValue_Ring)
            ring		= kMinimumValue_Ring;
        if (ring		> kMaximumValue_Ring)
            ring		= kMaximumValue_Ring;
        if (ring != lastRing) {
            lastRing = ring;
        }
        ringSignature = (int) GetParameter(kParameter_Signature);
        if (signature != lastSignature) {
            if (ringSignature	< kMinimumValue_Signature)
                ringSignature	= kMinimumValue_Signature;
            if (ringSignature	> kMaximumValue_Signature)
                ringSignature	= kMaximumValue_Signature;
            lastRingSignature = ringSignature;
        }
        ringSpeed = (int) GetParameter(kParameter_Speed);
        if (speed != lastSpeed) {
            if (ringSpeed		< kMinimumValue_Speed)
                ringSpeed		= kMinimumValue_Speed;
            if (ringSpeed		> kMaximumValue_Speed)
                ringSpeed		= kMaximumValue_Speed;
            lastRingSpeed = ringSpeed;
        }
        ringDepth = GetParameter (kParameter_Ring_Depth);
        if (ringDepth != lastRingDepth) {
            if (ringDepth		< kMinimumValue_Tremolo_Depth)
                ringDepth		= kMinimumValue_Tremolo_Depth;
            if (ringDepth		> kMaximumValue_Tremolo_Depth)
                ringDepth		= kMaximumValue_Tremolo_Depth;
            lastRingDepth = ringDepth;
        }
        ringWaveform =  (int) GetParameter (kParameter_Waveform);
        if (ringWaveform != lastRingDirection) {
            if (ringWaveform == kSineWave_Ring_Waveform)  {
                ringDirection = 1;
            } else {
                ringDirection = -1;
            }
        }
        signalPower = GetParameter (kParameter_Signal_Power);
        if (signalPower != lastSignalPower) {
            if (signalPower		< kMinimumValue_Signal_Power)
                signalPower		= kMinimumValue_Signal_Power;
            if (signalPower		> kMaximumValue_Signal_Power)
                signalPower		= kMaximumValue_Signal_Power;
            lastSignalPower = signalPower;
        }
        delayPower = GetParameter (kParameter_Delay_Power);
        if (delayPower != lastDelayPower) {
            if (delayPower		< kMinimumValue_Delay_Power)
                delayPower		= kMinimumValue_Delay_Power;
            if (delayPower		> kMaximumValue_Delay_Power)
                delayPower		= kMaximumValue_Delay_Power;
            lastDelayPower = delayPower;
        }
        
        Float64		bpm;
        OSStatus	err	= mAudioUnit->CallHostBeatAndTempo(NULL, &bpm);
        if (err == noErr) { bps = bpm/60; }
        
        samplesPerTremoloCycle = (int) (mSampleFrequency*tremoloFrequency/(2*bps*signature));
        samplesPerTremoloCycle = samplesPerTremoloCycle*2;
        
        int n = inSamplesToProcess;
        last = *sourceP;
        while(n--) {
            if (head == 0) {
                if (samplesPerTremoloCycle != lastRate) {
                    lastRate = samplesPerTremoloCycle;
                    memset(lastDelay,0,sizeof(float)*lastRate);
                    memset(delay,0,sizeof(float)*lastRate);
                }
                if (tremoloDepth != lastDepth) {
                    lastDepth = tremoloDepth;
                }
                if (direction != lastDirection) {
                    lastDirection = direction;
                }
                if (ringDirection != lastRingDirection) {
                    lastRingDirection = ringDirection;
                }
            }

            prev = last;
            last = *sourceP++;
            
            float mod = 6*sinf(pi*head/(lastRate));
            if (mod < 0) {
                mod = 0;
            } else if (mod > 1) {
                mod = 1;
            }
            
            if (head == 0) {
                delay[0] = last;
            } else if (head == lastRate-1) {
                delay[lastRate-1] = mod*(delay[lastRate-2] + delay[0] + last)/3;
                lastDelay[lastRate-1] = delay[lastRate-1] + lastDepth*lastDelay[lastRate-1];
                lastDelay[0] = 0;
            } else {
                delay[head] = mod*last;
                lastDelay[head] = delay[head] + lastDepth*lastDelay[head];
            }
            
            head = (head+1)%(lastRate);
            
            if (lastDirection > 0) {
                dhead = head;
            } else {
                dhead = (lastRate-head)%(lastRate);
            }
            if (ringDirection) {
                rhead = head;
            } else {
                rhead = (lastRate-head)%(lastRate);
            }
            
            *destP++ = (1-lastRingDepth)*last + lastRingDepth*lastDelay[dhead]
                        + ring*(last*lastDelay[rhead]) + signalPower*pow(last,ringSignature)
                        + delayPower*pow(lastDelay[dhead],ringSpeed);
            
        }
	}
}
