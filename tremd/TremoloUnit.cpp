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
    
    SetParameter (kParameter_Length, kDefaultValue_Length);
    
    SetParameter (kParameter_Direction, kDefaultValue_Direction);
    SetParameter (kParameter_Mix, kDefaultValue_Mix);
    SetParameter (kParameter_Signature, kDefaultValue_Signature);
    SetParameter (kParameter_Speed, kDefaultValue_Speed);
    SetParameter (kParameter_Depth, kDefaultValue_Depth);
    SetParameter (kParameter_Delay_Power, kDefaultValue_Delay_Power);
    SetParameter (kParameter_Delay_Power_Scale, kDefaultValue_Delay_Power_Scale);
    
    SetParameter (kParameter_Ring_Direction, kDefaultValue_Ring_Direction);
    SetParameter (kParameter_Ring_Mix, kDefaultValue_Ring_Mix);
    SetParameter (kParameter_Ring, kDefaultValue_Ring);
    SetParameter (kParameter_Ring_Signature, kDefaultValue_Ring_Signature);
    SetParameter (kParameter_Ring_Speed, kDefaultValue_Ring_Speed);
    SetParameter (kParameter_Ring_Depth, kDefaultValue_Ring_Depth);
    
    SetParameter (kParameter_Signal_Power_Direction, kDefaultValue_Signal_Power_Direction);
    SetParameter (kParameter_Signal_Power, kDefaultValue_Signal_Power);
    SetParameter (kParameter_Signal_Power_Scale, kDefaultValue_Signal_Power_Scale);
    SetParameter (kParameter_Signal_Power_Signature, kDefaultValue_Signal_Power_Signature);
    SetParameter (kParameter_Signal_Power_Speed, kDefaultValue_Signal_Power_Speed);
    SetParameter (kParameter_Signal_Power_Depth, kDefaultValue_Signal_Power_Depth);
    
    SetParameter (kParameter_Fade, kDefaultValue_Fade);
    SetParameter (kParameter_Duck, kDefaultValue_Duck);

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
                
            case kParameter_Length:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Length, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Length;
                outParameterInfo.maxValue		= kMaximumValue_Length;
                outParameterInfo.defaultValue	= kDefaultValue_Length;
                // outParameterInfo.flags			|= kAudioUnitParameterFlag_DisplayLogarithmic;
                // Adds a flag to indicate to the host that it should use a logarithmic control for the Frequency parameter.
                break;
                
            case kParameter_Direction:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Direction, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kForward_Direction;
                outParameterInfo.maxValue		= kBackward_Direction;
                outParameterInfo.defaultValue	= kDefaultValue_Direction;
                break;
                
            case kParameter_Mix:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Mix, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Mix;
                outParameterInfo.maxValue		= kMaximumValue_Mix;
                outParameterInfo.defaultValue	= kDefaultValue_Mix;
                break;
                
            case kParameter_Signature:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signature, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signature;
                outParameterInfo.maxValue		= kMaximumValue_Signature;
                outParameterInfo.defaultValue	= kDefaultValue_Signature;
                break;
                
            case kParameter_Speed:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Speed, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Speed;
                outParameterInfo.maxValue		= kMaximumValue_Speed;
                outParameterInfo.defaultValue	= kDefaultValue_Speed;
                break;
                
            case kParameter_Delay_Power:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Delay_Power, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Delay_Power;
                outParameterInfo.maxValue		= kMaximumValue_Delay_Power;
                outParameterInfo.defaultValue	= kDefaultValue_Delay_Power;
                break;
                
            case kParameter_Delay_Power_Scale:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Delay_Power_Scale, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Delay_Power_Scale;
                outParameterInfo.maxValue		= kMaximumValue_Delay_Power_Scale;
                outParameterInfo.defaultValue	= kDefaultValue_Delay_Power_Scale;
                break;
                
            case kParameter_Depth:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Depth, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Depth;
                outParameterInfo.maxValue		= kMaximumValue_Depth;
                outParameterInfo.defaultValue	= kDefaultValue_Depth;
                break;
                
            case kParameter_Ring_Direction:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring_Direction, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kForward_Ring_Direction;
                outParameterInfo.maxValue		= kBackward_Ring_Direction;
                outParameterInfo.defaultValue	= kDefaultValue_Ring_Direction;
                break;
                
            case kParameter_Ring_Mix:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring_Mix, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Ring_Mix;
                outParameterInfo.maxValue		= kMaximumValue_Ring_Mix;
                outParameterInfo.defaultValue	= kDefaultValue_Ring_Mix;
                break;
                
            case kParameter_Ring:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Ring;
                outParameterInfo.maxValue		= kMaximumValue_Ring;
                outParameterInfo.defaultValue	= kDefaultValue_Ring;
                break;
            case kParameter_Ring_Signature:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring_Signature, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Ring_Signature;
                outParameterInfo.maxValue		= kMaximumValue_Ring_Signature;
                outParameterInfo.defaultValue	= kDefaultValue_Ring_Signature;
                break;
                
            case kParameter_Ring_Speed:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring_Speed, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Ring_Speed;
                outParameterInfo.maxValue		= kMaximumValue_Ring_Speed;
                outParameterInfo.defaultValue	= kDefaultValue_Ring_Speed;
                break;
                
            case kParameter_Ring_Depth:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Ring_Depth, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Ring_Depth;
                outParameterInfo.maxValue		= kMaximumValue_Ring_Depth;
                outParameterInfo.defaultValue	= kDefaultValue_Ring_Depth;
                break;
                
            case kParameter_Signal_Power_Direction:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power_Direction, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kForward_Signal_Power_Direction;
                outParameterInfo.maxValue		= kBackward_Signal_Power_Direction;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power_Direction;
                break;
                
            case kParameter_Signal_Power:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power;
                break;
                
            case kParameter_Signal_Power_Scale:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power_Scale, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power_Scale;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power_Scale;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power_Scale;
                break;
                
            case kParameter_Signal_Power_Signature:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power_Signature, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power_Signature;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power_Signature;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power_Signature;
                break;
                
            case kParameter_Signal_Power_Speed:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power_Speed, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power_Speed;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power_Speed;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power_Speed;
                break;
                
            case kParameter_Signal_Power_Depth:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Signal_Power_Depth, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Percent;
                outParameterInfo.minValue		= kMinimumValue_Signal_Power_Depth;
                outParameterInfo.maxValue		= kMaximumValue_Signal_Power_Depth;
                outParameterInfo.defaultValue	= kDefaultValue_Signal_Power_Depth;
                break;
            
            case kParameter_Fade:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Fade, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Fade;
                outParameterInfo.maxValue		= kMaximumValue_Fade;
                outParameterInfo.defaultValue	= kDefaultValue_Fade;
                break;
                
            case kParameter_Duck:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Duck, false);
                outParameterInfo.unit			= kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue		= kMinimumValue_Duck;
                outParameterInfo.maxValue		= kMaximumValue_Duck;
                outParameterInfo.defaultValue	= kDefaultValue_Duck;
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
// Provides the strings for the Direction popup menu in the generic view
ComponentResult TremoloUnit::GetParameterValueStrings (
	AudioUnitScope			inScope,
	AudioUnitParameterID	inParameterID,
	CFArrayRef				*outStrings
) {
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Direction)) {
	// This method applies only to the Direction parameter, which is in the global scope.
	
		// When this method gets called by the AUBase::DispatchGetPropertyInfo method, which 
		// provides a null value for the outStrings parameter, just return without error.
		if (outStrings == NULL) return noErr;
		
		// Defines an array that contains the pop-up menu item names.
        CFStringRef	strings [] = {
            kMenuItem_Forward,
			kMenuItem_Backward
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
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Ring_Direction)) {
        // This method applies only to the Direction parameter, which is in the global scope.
        
        // When this method gets called by the AUBase::DispatchGetPropertyInfo method, which
        // provides a null value for the outStrings parameter, just return without error.
        if (outStrings == NULL) return noErr;
        
        // Defines an array that contains the pop-up menu item names.
        CFStringRef	strings [] = {
            kMenuItem_Ring_Forward,
            kMenuItem_Ring_Backward
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
    if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Signal_Power_Direction)) {
        // This method applies only to the Direction parameter, which is in the global scope.
        
        // When this method gets called by the AUBase::DispatchGetPropertyInfo method, which
        // provides a null value for the outStrings parameter, just return without error.
        if (outStrings == NULL) return noErr;
        
        // Defines an array that contains the pop-up menu item names.
        CFStringRef	strings [] = {
            kMenuItem_Signal_Power_Forward,
            kMenuItem_Signal_Power_Backward
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
	
	// Fills the factory presets array with values from the definitions in the TremoloUnit.h file.
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

// This method takes a single argument of type AUPreset, a structure containing a factory preset name and number.
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
							kParameter_Length,
							kParameter_Preset_Frequency_Slow
						);
						SetParameter (
							kParameter_Depth,
							kParameter_Preset_Depth_Slow
						);
						SetParameter (
							kParameter_Direction,
							kParameter_Preset_Forward
						);
						break;
					
					// The settings for the "Fast & Hard" factory preset.
					case kPreset_Fast:
						SetParameter (
							kParameter_Length,
							kParameter_Preset_Frequency_Fast
						);
						SetParameter (
							kParameter_Depth,
							kParameter_Preset_Depth_Fast
						);
						SetParameter (
							kParameter_Direction,
							kParameter_Preset_Backward
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

    memset(lastRingDelay,0,sizeof(float)*maxDelaySamples);
    memset(ringDelay,0,sizeof(float)*maxDelaySamples);
    
    memset(lastSignalPowerDelay,0,sizeof(float)*maxDelaySamples);
    memset(signalPowerDelay,0,sizeof(float)*maxDelaySamples);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void TremoloUnit::TremoloUnitKernel::Reset() {
	mSamplesProcessed	= 0;
    
    head = 0;
    rhead = 0;
    phead = 0;
    
//    first = true;
//    firstRing = true;
    
    memset(lastDelay,0,sizeof(float)*maxDelaySamples);
    memset(delay,0,sizeof(float)*maxDelaySamples);
    
    memset(lastRingDelay,0,sizeof(float)*maxDelaySamples);
    memset(ringDelay,0,sizeof(float)*maxDelaySamples);
    
    memset(lastSignalPowerDelay,0,sizeof(float)*maxDelaySamples);
    memset(signalPowerDelay,0,sizeof(float)*maxDelaySamples);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	TremoloUnit::TremoloUnitKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// This method contains the DSP code. 
void TremoloUnit::TremoloUnitKernel::Process (
	const Float32 	*inSourceP,			// The audio sample input buffer.
	Float32		 	*inDestP,			// The audio sample output buffer.
	UInt32 			inSamplesToProcess,	// The number of samples in the input buffer.
	UInt32			inNumChannels,		// The number of input channels. This is always equal to 1 because there
                                        //  is always one kernel object instantiated per channel of audio.
	bool			&ioSilence			// A Boolean flag indicating whether the input to the audio unit
                                        //  consists of silence, with a TRUE value indicating silence.
) {
	if (!ioSilence) {
		// Assigns a pointer variable to the start of the audio sample input buffer.
		const Float32 *sourceP = inSourceP;
		// Assigns a pointer variable to the start of the audio sample output buffer.
		Float32	*destP = inDestP,
                mix,
                ringMix,
				ring,
                ringDepth,
                signalPowerDepth,
                depth,
                signalPower,
                signalPowerScale,
                delayPower,
                delayPowerScale;
        int     length,
                samplesPerDelay,
                samplesPerRingDelay,
                samplesPerSignalPowerDelay,
                direction,
                speed,
                signature,
                ringDirection,
                ringSpeed,
                ringSignature,
                fade,
                duck,
                signalPowerDirection,
                signalPowerSignature,
                signalPowerSpeed;
        
        // ---- UPDATE PARAMETERS ---- //
        length = GetParameter (kParameter_Length);
        if (length < kMinimumValue_Length) length = kMinimumValue_Length;
        if (length > kMaximumValue_Length) length = kMaximumValue_Length;
        if (length != lastLength) lastLength = length;
        
        direction =  (int) GetParameter (kParameter_Direction);
        if (direction == kForward_Direction) direction = 1;
        else direction = -1;
        
        mix = GetParameter (kParameter_Mix);
        if (mix < kMinimumValue_Mix) mix = kMinimumValue_Mix;
        if (mix > kMaximumValue_Mix) mix = kMaximumValue_Mix;
        if (mix != lastMix) lastMix= mix;
        
        signature = (int) GetParameter(kParameter_Signature);
        if (signature < kMinimumValue_Signature) signature = kMinimumValue_Signature;
        if (signature > kMaximumValue_Signature) signature = kMaximumValue_Signature;
        if (signature != lastSignature) lastSignature = signature;
        
        speed = (int) GetParameter(kParameter_Speed);
        if (speed < kMinimumValue_Speed) speed = kMinimumValue_Speed;
        if (speed > kMaximumValue_Speed) speed = kMaximumValue_Speed;
        if (speed != lastSpeed) lastSpeed = speed;
        
		depth = GetParameter (kParameter_Depth);
        if (depth < kMinimumValue_Depth) depth = kMinimumValue_Depth;
        if (depth > kMaximumValue_Depth) depth = kMaximumValue_Depth;
        if (depth != lastDepth) lastDepth = depth;
        
        delayPower = GetParameter (kParameter_Delay_Power);
        if (delayPower < kMinimumValue_Delay_Power) delayPower = kMinimumValue_Delay_Power;
        if (delayPower > kMaximumValue_Delay_Power) delayPower = kMaximumValue_Delay_Power;
        if (delayPower != lastDelayPower) lastDelayPower = delayPower;
        
        delayPowerScale = GetParameter (kParameter_Delay_Power_Scale);
        if (delayPowerScale < kMinimumValue_Delay_Power_Scale) delayPower = kMinimumValue_Delay_Power_Scale;
        if (delayPowerScale > kMaximumValue_Delay_Power_Scale) delayPower = kMaximumValue_Delay_Power_Scale;
        if (delayPowerScale != lastDelayPowerScale) lastDelayPowerScale = delayPowerScale;
        
        ringDirection =  (int) GetParameter (kParameter_Ring_Direction);
        if (ringDirection == kForward_Ring_Direction) ringDirection = 1;
        else ringDirection = -1;
        
        ringMix = GetParameter(kParameter_Ring_Mix);
        if (ringMix < kMinimumValue_Ring_Mix) ringMix = kMinimumValue_Ring_Mix;
        if (ringMix > kMaximumValue_Ring_Mix) ringMix = kMaximumValue_Ring_Mix;
        if (ringMix != lastRingMix) lastRingMix = ringMix;
        
        ring = GetParameter(kParameter_Ring);
        if (ring < kMinimumValue_Ring) ring = kMinimumValue_Ring;
        if (ring > kMaximumValue_Ring) ring = kMaximumValue_Ring;
        if (ring != lastRing) lastRing = ring;
        
        ringSignature = (int) GetParameter(kParameter_Ring_Signature);
        if (ringSignature < kMinimumValue_Ring_Signature) ringSignature = kMinimumValue_Ring_Signature;
        if (ringSignature > kMaximumValue_Ring_Signature) ringSignature = kMaximumValue_Ring_Signature;
        if (ringSignature != lastRingSignature) lastRingSignature = ringSignature;
        
        ringSpeed = (int) GetParameter(kParameter_Ring_Speed);
        if (ringSpeed < kMinimumValue_Ring_Speed) ringSpeed = kMinimumValue_Ring_Speed;
        if (ringSpeed > kMaximumValue_Ring_Speed) ringSpeed = kMaximumValue_Ring_Speed;
        if (ringSpeed != lastRingSpeed) lastRingSpeed = ringSpeed;
        
        ringDepth = GetParameter (kParameter_Ring_Depth);
        if (ringDepth < kMinimumValue_Ring_Depth) ringDepth = kMinimumValue_Ring_Depth;
        if (ringDepth > kMaximumValue_Depth) ringDepth = kMaximumValue_Ring_Depth;
        if (ringDepth != lastRingDepth) lastRingDepth = ringDepth;
        
        signalPowerDirection =  (int) GetParameter (kParameter_Signal_Power_Direction);
        if (signalPowerDirection == kForward_Signal_Power_Direction) signalPowerDirection = 1;
        else signalPowerDirection = -1;
        
        signalPower = GetParameter (kParameter_Signal_Power);
        if (signalPower	< kMinimumValue_Signal_Power) signalPower = kMinimumValue_Signal_Power;
        if (signalPower	> kMaximumValue_Signal_Power) signalPower = kMaximumValue_Signal_Power;
        if (signalPower != lastSignalPower) lastSignalPower = signalPower;
        
        signalPowerScale = GetParameter (kParameter_Signal_Power_Scale);
        if (signalPowerScale < kMinimumValue_Signal_Power_Scale) signalPowerScale = kMinimumValue_Signal_Power_Scale;
        if (signalPowerScale > kMaximumValue_Signal_Power_Scale) signalPowerScale = kMaximumValue_Signal_Power_Scale;
        if (signalPowerScale != lastSignalPowerScale) lastSignalPowerScale = signalPowerScale;
        
        signalPowerSignature = (int) GetParameter(kParameter_Signal_Power_Signature);
        if (signalPowerSignature < kMinimumValue_Signal_Power_Signature) ringSignature = kMinimumValue_Signal_Power_Signature;
        if (signalPowerSignature > kMaximumValue_Signal_Power_Signature) ringSignature = kMaximumValue_Signal_Power_Signature;
        if (signalPowerSignature != lastSignalPowerSignature) lastSignalPowerSignature = signalPowerSignature;
        
        signalPowerSpeed = (int) GetParameter(kParameter_Signal_Power_Speed);
        if (signalPowerSpeed < kMinimumValue_Signal_Power_Speed) signalPowerSpeed = kMinimumValue_Signal_Power_Speed;
        if (signalPowerSpeed > kMaximumValue_Signal_Power_Speed) signalPowerSpeed = kMaximumValue_Signal_Power_Speed;
        if (signalPowerSpeed != lastSignalPowerSpeed) lastSignalPowerSpeed = signalPowerSpeed;
        
        signalPowerDepth = GetParameter (kParameter_Signal_Power_Depth);
        if (signalPowerDepth < kMinimumValue_Signal_Power_Depth) signalPowerDepth = kMinimumValue_Signal_Power_Depth;
        if (signalPowerDepth > kMaximumValue_Signal_Power_Depth) signalPowerDepth = kMaximumValue_Signal_Power_Depth;
        if (signalPowerDepth != lastSignalPowerDepth) lastSignalPowerDepth = signalPowerDepth;
        
        fade = GetParameter (kParameter_Fade);
        if (fade < kMinimumValue_Fade) fade = kMinimumValue_Fade;
        if (fade > kMaximumValue_Fade) fade = kMaximumValue_Fade;
        if (fade != lastFade) lastFade = fade;
        
        duck = GetParameter (kParameter_Duck);
        if (duck < kMinimumValue_Duck) duck = kMinimumValue_Duck;
        if (duck > kMaximumValue_Fade) duck = kMaximumValue_Duck;
        if (duck != lastDuck) lastDuck = duck;
        // ---- END UPDATE PARAMETERS ---- //
        
        Float64		bpm;
        OSStatus	err	= mAudioUnit->CallHostBeatAndTempo(NULL, &bpm);
        if (err == noErr) { bps = bpm/60; }
        
        samplesPerDelay = (int) (mSampleFrequency*length/(bps*signature*speed));
        samplesPerRingDelay = (int) (mSampleFrequency*length/(bps*ringSignature*ringSpeed));
        samplesPerSignalPowerDelay = (int) (mSampleFrequency*length/(bps*signalPowerSignature*signalPowerSpeed));
        
        int n = inSamplesToProcess;
        last = *sourceP;
        while(n--) {
            if (head == 0) {
                if (samplesPerDelay != lastRate) {
                    lastRate = samplesPerDelay;
                    memset(lastDelay,0,sizeof(float)*lastRate);
                    memset(delay,0,sizeof(float)*lastRate);
                }
                if (samplesPerRingDelay != lastRingRate) {
                    lastRingRate = samplesPerRingDelay;
                    memset(lastRingDelay,0,sizeof(float)*lastRingRate);
                    memset(ringDelay,0,sizeof(float)*lastRingRate);
                }
                if (samplesPerSignalPowerDelay != lastSignalPowerRate) {
                    lastSignalPowerRate = samplesPerSignalPowerDelay;
                    memset(lastSignalPowerDelay,0,sizeof(float)*maxDelaySamples);
                    memset(signalPowerDelay,0,sizeof(float)*maxDelaySamples);
                }
                
                if (direction != lastDirection) lastDirection = direction;
                if (ringDirection != lastRingDirection) lastRingDirection = ringDirection;
                if (signalPowerDirection != lastSignalPowerDirection) lastSignalPowerDirection = signalPowerDirection;
            }
            
            last = *sourceP++;
            
            float mod;
            if (lastFade == 0) mod = 1;
            else mod = lastFade*sinf(pi*head/(lastRate));
            if (mod < 0) mod = 0;
            else if (mod > 1) mod = 1;
            mod = (lastDuck*mod + 1)/(lastDuck+1);
            
            float rmod;
            if (lastFade == 0) rmod = 1;
            else rmod = lastFade*sinf(pi*rhead/(lastRingRate));
            if (rmod < 0) rmod = 0;
            else if (rmod > 1) rmod = 1;
            rmod = (lastDuck*rmod + 1)/(lastDuck+1);
            
            float pmod;
            if (lastFade == 0) pmod = 1;
            else pmod = lastFade*sinf(pi*phead/(lastSignalPowerRate));
            if (pmod < 0) pmod = 0;
            else if (pmod > 1) pmod = 1;
            pmod = (lastDuck*pmod + 1)/(lastDuck+1);

            
            delay[head] = (delay[(lastRate+head-1)%(lastRate)] + delay[head] + delay[(head+1)%(lastRate)])/3;
            ringDelay[rhead] = (ringDelay[(lastRingRate+rhead-1)%(lastRingRate)] + ringDelay[rhead]
                                + ringDelay[(rhead+1)%(lastRingRate)])/3;
            signalPowerDelay[phead] = (signalPowerDelay[(lastSignalPowerRate+phead-1)%(lastSignalPowerRate)]
                                       + signalPowerDelay[phead]
                                       + signalPowerDelay[(phead+1)%(lastSignalPowerRate)])/3;
            
//            delay[head] = (delay[(lastRate+head-2)%(lastRate)] + 2*delay[(lastRate+head-1)%(lastRate)]
//                           + 3*delay[head] + 2*delay[(head+1)%(lastRate)] + delay[(head+2)%(lastRate)])/9;
//            ringDelay[rhead] = (ringDelay[(lastRingRate+rhead-2)%(lastRingRate)]
//                                + 2*ringDelay[(lastRingRate+rhead-1)%(lastRingRate)] + 3*ringDelay[rhead]
//                                + 2*ringDelay[(rhead+1)%(lastRingRate)] + ringDelay[(rhead+2)%(lastRingRate)])/9;
//            signalPowerDelay[phead] = (signalPowerDelay[(lastSignalPowerRate+phead-2)%(lastSignalPowerRate)]
//                                       + 2*signalPowerDelay[(lastSignalPowerRate+phead-1)%(lastSignalPowerRate)]
//                                       + 3*signalPowerDelay[phead]
//                                       + 2*signalPowerDelay[(phead+1)%(lastSignalPowerRate)]
//                                       + signalPowerDelay[(phead+2)%(lastSignalPowerRate)])/9;
            
            *destP++ = (1-lastRingMix)*((1-lastMix)*last + mod*lastMix*delay[head])
                        + lastRingMix*(
                                       lastRing*(ringDelay[head]*ringDelay[(lastRingRate-rhead)%(lastRingRate)])
                                        + mod*pow(lastDelayPowerScale*delay[head], lastDelayPower)
                                        + pmod*pow(lastSignalPowerScale*signalPowerDelay[phead], lastSignalPower)
                                       );

            head = (head+1)%(lastRate);
            rhead = (rhead+1)%(lastRingRate);
            phead = (phead+1)%(lastSignalPowerRate);
            
            if (lastDirection > 0) dhead = head;
            else dhead = (lastRate-head)%(lastRate);
            if (lastRingDirection > 0) rdhead = rhead;
            else rdhead = (lastRingRate-rhead)%(lastRingRate);
            if (signalPowerDirection > 0) pdhead = phead;
            else pdhead = (lastSignalPowerRate-phead)%(lastSignalPowerRate);
            
            // write delay line
            lastDelay[dhead] = (lastDelay[(lastRate+dhead-1)%(lastRate)] + last + lastDelay[(dhead+1)%(lastRate)])/3;
            delay[dhead] = lastDelay[dhead] + lastDepth*delay[dhead];
            // write ring delay line
            lastRingDelay[rdhead] = (lastRingDelay[(lastRingRate+rdhead-1)%(lastRingRate)] + last + lastRingDelay[(rdhead+1)%(lastRingRate)])/3;
            ringDelay[rdhead] = lastRingDelay[rdhead] + lastRingDepth*ringDelay[rdhead];
            // write exponent delay line
            lastSignalPowerDelay[pdhead] = (lastSignalPowerDelay[(lastSignalPowerRate+pdhead-1)%(lastSignalPowerRate)] + last
                                            + lastSignalPowerDelay[(pdhead+1)%(lastSignalPowerRate)])/3;
            signalPowerDelay[pdhead] = lastSignalPowerDelay[pdhead] + lastSignalPowerDepth*signalPowerDelay[pdhead];
        }
	}
}
