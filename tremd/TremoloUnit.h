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

// Provides the user interface name for the Frequency parameter.
static CFStringRef kParamName_Tremolo_Freq		= CFSTR ("frequency");
// Defines a constant for the default value for the Frequency parameter, anticipating a 
//  unit of Hertz to be defined in the implementation file.
static const int kDefaultValue_Tremolo_Freq	= 4;
// Defines a constant for the minimum value for the Frequency parameter.
static const int kMinimumValue_Tremolo_Freq	= 1;
// Defines a constant for the maximum value for the Frequency parameter.
static const int kMaximumValue_Tremolo_Freq	= kMinimumValue_Tremolo_Freq*32;
static const long maxDelaySamples = 512*512;

static CFStringRef kParamName_Signature		= CFSTR ("signature");
static const int kDefaultValue_Signature	= 4;
static const int kMinimumValue_Signature	= 1;
static const int kMaximumValue_Signature	= 7;

static CFStringRef kParamName_Speed		= CFSTR ("speed");
static const int kDefaultValue_Speed	= 1;
static const int kMinimumValue_Speed	= 1;
static const int kMaximumValue_Speed	= 7;

static CFStringRef kParamName_Tremolo_Depth		= CFSTR ("depth");
static const float kDefaultValue_Tremolo_Depth	= 0.4;
static const float kMinimumValue_Tremolo_Depth	= 0.0;
static const float kMaximumValue_Tremolo_Depth	= 1.0;

static CFStringRef kParamName_Tremolo_Waveform	= CFSTR ("direction");
static const int kSineWave_Tremolo_Waveform		= 1;
static const int kSquareWave_Tremolo_Waveform	= 2;
static const int kDefaultValue_Tremolo_Waveform	= kSineWave_Tremolo_Waveform;

// Defines menu item names for the waveform parameter
static CFStringRef kMenuItem_Tremolo_Sine		= CFSTR ("forward");
static CFStringRef kMenuItem_Tremolo_Square		= CFSTR ("reverse");

//static CFStringRef kParamName_Unit	= CFSTR ("unit");
//static const int kSamples_Unit = 1;
//static const int kMSeconds_Unit	= 2;
//static const int kDefaultValue_Unit	= kSamples_Unit;
//
//// Defines menu item names for the waveform parameter
//static CFStringRef kMenuItem_Samples		= CFSTR ("samples/delay");
//static CFStringRef kMenuItem_MSeconds	= CFSTR ("ms/delay");

// Defines constants for identifying the parameters; defines the total number 
//  of parameters.
enum {
    kParameter_Frequency	= 0,
    kParameter_Signature    = 1,
    kParameter_Speed        = 2,
	kParameter_Depth		= 3,
	kParameter_Waveform		= 4,
//    kParameter_Unit         = 3,
	kNumberOfParameters		= 5
};

#pragma mark ____TremoloUnit Factory Preset Constants

// Defines a constant for the frequency value for the "Slow & Gentle" factory preset.
static const long kParameter_Preset_Frequency_Slow	= 1600;

// Defines a constant for the frequency value for the "Fast & Hard" factory preset.
static const long kParameter_Preset_Frequency_Fast	= 200;

// Defines a constant for the depth value for the "Slow & Gentle" factory preset.
static const float kParameter_Preset_Depth_Slow		= 0.2;

// Defines a constant for the depth value for the "Fast & Hard" factory preset.
static const float kParameter_Preset_Depth_Fast		= 0.7;

// Defines a constant for the waveform value for the "Slow & Gentle" factory preset.
static const float kParameter_Preset_Waveform_Slow	= kSineWave_Tremolo_Waveform;

// Defines a constant for the waveform value for the "Fast & Hard" factory preset.
static const float kParameter_Preset_Waveform_Fast	= kSquareWave_Tremolo_Waveform;

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
//			enum	{kWaveArraySize = 2000};	// The number of points in the wave table.
//			float	mSine [kWaveArraySize];		// The wave table for the tremolo sine wave.
//			float	mSquare [kWaveArraySize];	// The wave table for the tremolo square wave.
//			float	*waveArrayPointer;			// Points to the wave table to use for the current audio input buffer.
			Float32 mSampleFrequency;			// The "sample rate" of the audio signal being processed
			long	mSamplesProcessed;			// The number of samples processed since the audio unit
												//   started rendering or since this variable was last
												//   reset to 0. We have to keep track of this because
												//   we vary the tremolo continuously and independently
												//   of the input buffer size.
			enum	{sampleLimit = (int) 10E6};	// To keep the value of mSamplesProcessed within a 
												//   reasonable limit. 10E6 is equivalent to the number   
												//   of samples in 100 seconds of 96 kHz audio.
//			float	mCurrentScale;				// There are two scaling factors to allow the audio unit
												//   to switch to a new scaling factor at the beginning
												//   of the tremolo waveform, no matter when the user
												//   changes the tremolo frequency. mCurrentScale is 
												//   the scaling factor in use.
//			float	mNextScale;					// The scaling factor that the user most recently requested
												//   by moving the tremolo frequency slider
//            long     rate;
        
//            float    fbk;
            float bps;
        
            int head;
        
//            int shift = 0;
        
            bool direction = true;
        
//            int rate = 4096*4;
        
            // 2D delay array.
            Float32 lastDelay[maxDelaySamples];
	};
};

#endif
