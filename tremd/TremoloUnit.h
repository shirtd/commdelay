/*
Copyright (C) 2016 Apple Inc. All Rights Reserved.
See LICENSE.txt for this sample’s licensing information

Abstract:
Tremolo Effect AU
*/

#include "AUEffectBase.h"
#include "TremoloUnitVersion.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif

#ifndef __TremoloUnit_h__
#define __TremoloUnit_h__

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Constants for parameters and  factory presets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____TremoloUnit Parameter Constants
static CFStringRef kParamName_Mix = CFSTR ("mix");
static const float kDefaultValue_Mix = 0.5;
static const float kMinimumValue_Mix = 0.0;
static const float kMaximumValue_Mix = 1.0;

static CFStringRef kParamName_Length = CFSTR ("length");
static const int kDefaultValue_Length = 8;
static const int kMinimumValue_Length = 1;
static const int kMaximumValue_Length = 32;
static const long maxDelaySamples = 512*512;

static CFStringRef kParamName_Signature	= CFSTR ("signature");
static const int kDefaultValue_Signature = 1;
static const int kMinimumValue_Signature = 1;
static const int kMaximumValue_Signature = 5;

static CFStringRef kParamName_Speed	= CFSTR ("speed");
static const int kDefaultValue_Speed = 1;
static const int kMinimumValue_Speed = 1;
static const int kMaximumValue_Speed = 4;

static CFStringRef kParamName_Depth	= CFSTR ("depth");
static const float kDefaultValue_Depth = 0.6;
static const float kMinimumValue_Depth = 0.0;
static const float kMaximumValue_Depth = 1.0;

static CFStringRef kParamName_Direction	= CFSTR ("direction");
static const int kForward_Direction	= 1;
static const int kBackward_Direction = 2;
static const int kDefaultValue_Direction = kForward_Direction;

static CFStringRef kMenuItem_Forward = CFSTR ("forward");
static CFStringRef kMenuItem_Backward = CFSTR ("backward");

static CFStringRef kParamName_Ring	= CFSTR ("ring");
static const float kDefaultValue_Ring = 0.0;
static const float kMinimumValue_Ring = 0.0;
static const float kMaximumValue_Ring = 4.0;

static CFStringRef kParamName_Ring_Signature = CFSTR ("ring signature");
static const int kDefaultValue_Ring_Signature = 1;
static const int kMinimumValue_Ring_Signature = 1;
static const int kMaximumValue_Ring_Signature = 5;

static CFStringRef kParamName_Ring_Speed = CFSTR ("ring speed");
static const int kDefaultValue_Ring_Speed = 1;
static const int kMinimumValue_Ring_Speed = 1;
static const int kMaximumValue_Ring_Speed = 4;

static CFStringRef kParamName_Ring_Depth = CFSTR ("ring depth");
static const float kDefaultValue_Ring_Depth	= 0.4;
static const float kMinimumValue_Ring_Depth	= 0.0;
static const float kMaximumValue_Ring_Depth	= 1.0;

static CFStringRef kParamName_Ring_Direction = CFSTR ("ring direction");
static const int kForward_Ring_Direction = 1;
static const int kBackward_Ring_Direction = 2;
static const int kDefaultValue_Ring_Direction= kForward_Ring_Direction;

static CFStringRef kMenuItem_Ring_Forward = CFSTR ("forward");
static CFStringRef kMenuItem_Ring_Backward = CFSTR ("backward");

static CFStringRef kParamName_Signal_Power = CFSTR ("signal power");
static const float kDefaultValue_Signal_Power = 0.0;
static const float kMinimumValue_Signal_Power = 0.0;
static const float kMaximumValue_Signal_Power = 4.0;

static CFStringRef kParamName_Delay_Power = CFSTR ("delay power");
static const float kDefaultValue_Delay_Power = 0.0;
static const float kMinimumValue_Delay_Power = 0.0;
static const float kMaximumValue_Delay_Power = 4.0;

static CFStringRef kParamName_Fade = CFSTR ("fade");
static const int kDefaultValue_Fade	= 5;
static const int kMinimumValue_Fade	= 0;
static const int kMaximumValue_Fade	= 10;

enum {
    kParameter_Mix              = 0,
    kParameter_Length           = 1,
    kParameter_Signature        = 2,
    kParameter_Speed            = 3,
    kParameter_Depth            = 4,
    kParameter_Direction		= 5,
    kParameter_Ring             = 6,
    kParameter_Ring_Signature   = 7,
    kParameter_Ring_Speed       = 8,
    kParameter_Ring_Depth       = 9,
    kParameter_Ring_Direction   = 10,
    kParameter_Signal_Power     = 11,
    kParameter_Delay_Power      = 12,
    kParameter_Fade             = 13,
	kNumberOfParameters = 14
};

#pragma mark ____TremoloUnit Factory Preset Constants

static const long kParameter_Preset_Frequency_Slow	= 8;
static const long kParameter_Preset_Frequency_Fast	= 1;
static const float kParameter_Preset_Depth_Slow		= 0.2;
static const float kParameter_Preset_Depth_Fast		= 0.7;
static const float kParameter_Preset_Forward	= kForward_Direction;
static const float kParameter_Preset_Backward	= kBackward_Direction;

enum {
	// Defines a constant for the "Slow & Gentle" factory preset.
	kPreset_Slow	= 0,

	// Defines a constant for the "Fast & Hard" factory preset.
	kPreset_Fast	= 1,

	// Defines a constant representing the total number of factory presets.
	kNumberPresets	= 2
};

// Defines an array containing two Core Foundation string objects. The objects contain 
//  values for the menu items in the user interface corresponding to the factory presets.
static AUPreset kPresets [kNumberPresets] = {
	{kPreset_Slow, CFSTR ("Slow & Gentle")},
	{kPreset_Fast, CFSTR ("Fast & Hard")}
};

// Defines a constant representing the default factory preset, in this case the 
//  "Slow & Gentle" preset.
static const int kPreset_Default = kPreset_Slow;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TremoloUnit class
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#pragma mark ____TremoloUnit
class TremoloUnit : public AUEffectBase {

public:
	TremoloUnit (AudioUnit component);
	
#if AU_DEBUG_DISPATCHER
	virtual ~TremoloUnit () {delete mDebugDispatcher;}
#endif
	
	virtual AUKernelBase *NewKernel () {return new TremoloUnitKernel(this);}
	
	virtual	ComponentResult GetParameterValueStrings (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		CFArrayRef				*outStrings
	);
    
	virtual	ComponentResult GetParameterInfo (
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		AudioUnitParameterInfo	&outParameterInfo
	);
    
	virtual ComponentResult GetPropertyInfo (
		AudioUnitPropertyID		inID,
		AudioUnitScope			inScope,
		AudioUnitElement		inElement,
		UInt32					&outDataSize,
		Boolean					&outWritable
	);
	
	virtual ComponentResult GetProperty (
		AudioUnitPropertyID		inID,
		AudioUnitScope			inScope,
		AudioUnitElement		inElement,
		void					*outData
	);
	
 	// report that the audio unit supports the 
	//	kAudioUnitProperty_TailTime property
	virtual	bool SupportsTail () {return true;}
	
	// provide the audio unit version information
	virtual ComponentResult	Version () {return kTremoloUnitVersion;}

    // Declaration for the GetPresets method (for setting up the factory presets), 
	//  overriding the method from the AUBase superclass.
    virtual ComponentResult	GetPresets (
		CFArrayRef	*outData
	) const;
	
	// Declaration for the NewFactoryPresetSet method (for setting a factory preset 
	//  when requested by the host application), overriding the method from the 
	//  AUBase superclass.
    virtual OSStatus NewFactoryPresetSet (
		const AUPreset	&inNewFactoryPreset
	);

protected:
	class TremoloUnitKernel : public AUKernelBase {
		public:
			TremoloUnitKernel (AUEffectBase *inAudioUnit);
			
			// *Required* overides for the process method for this effect
			// processes one channel of interleaved samples
			virtual void Process (
				const Float32 	*inSourceP,
				Float32		 	*inDestP,
				UInt32 			inFramesToProcess,
				UInt32			inNumChannels, // equal to 1
				bool			&ioSilence
		);
		
        virtual void Reset ();
		
		private:
			Float32 mSampleFrequency;			// The "sample rate" of the audio signal being processed
			long	mSamplesProcessed;

            float bps = 1;
        
            int head = 0;
            int dhead = 0;
            int rhead = 0;
        
            float last;
            float prev = 0;
        
            float lastDelay[maxDelaySamples];
            float delay[maxDelaySamples];
        
            float lastRingDelay[maxDelaySamples];
            float ringDelay[maxDelaySamples];
        
            int lastRate = 4096*4;
            int lastRingRate = 2048*3;
        
            float lastMix = 0.5;
        
            int lastLength = 1;
            int lastSignature = 1;
            int lastSpeed = 1;
            float lastDepth = 0;
            int lastDirection = 1;
        
            float lastRing = 0;
            int lastRingSignature = 1;
            int lastRingSpeed = 1;
            float lastRingDepth = 0;
            int lastRingDirection = 1;
        
            float lastSignalPower= 0;
            float lastDelayPower = 0;
        
            int lastFade = 1;
    };
};

#endif
