// Copyright (c) libASPL authors
// Licensed under MIT

#include <aspl/Driver.hpp>

#include <CoreAudio/AudioServerPlugIn.h>

#include <cmath>
#include <limits>

namespace {

std::shared_ptr<aspl::Driver> CreateExampleDriver()
{
    // Create context, shared between all other objects.
    // You can provide custom tracer here.
    auto context = std::make_shared<aspl::Context>();

    // Create device object with some custom parameters.
    aspl::DeviceParameters deviceParams;
    deviceParams.Name = "Hovt Device";
    deviceParams.SampleRate = 44100;
    deviceParams.ChannelCount = 2;

    auto device = std::make_shared<aspl::Device>(context, deviceParams);
    device->SetAvailableSampleRatesAsync({});

    aspl::StreamParameters params;

    params.Direction = aspl::Direction::Input;
    params.Format.mSampleRate = 48000;
    params.Format.mChannelsPerFrame = device->GetPreferredChannelCount();
    params.Format.mBytesPerFrame =
        params.Format.mChannelsPerFrame * (params.Format.mBitsPerChannel / 8);
    params.Format.mFramesPerPacket = 1;
    params.Format.mBytesPerPacket = params.Format.mBytesPerFrame;
    params.StartingChannel = 1;
    
    auto stream = device->AddStreamWithControlsAsync(params);
    stream->SetAvailablePhysicalFormatsAsync({});

    params.Direction = aspl::Direction::Output;

    stream = device->AddStreamWithControlsAsync(params);
    stream->SetAvailablePhysicalFormatsAsync({});

    // Create plugin object, the root of the object hierarchy, and add
    // our device to it.
    //
    // The main purpose of plugin is to provide the list of devices to HAL.
    //
    // For simplicity we use default parameters.
    auto plugin = std::make_shared<aspl::Plugin>(context);

    plugin->AddDevice(device);

    // Create driver, the top-level entry point.
    // Driver owns plugin object and thus the whole object hierarchy,
    // and provides C interface for HAL.
    auto driver = std::make_shared<aspl::Driver>(context, plugin);

    return driver;
}

} // namespace

extern "C" void* HovtEntryPoint(CFAllocatorRef allocator, CFUUIDRef typeUUID)
{
    // The UUID of the plug-in type (443ABAB8-E7B3-491A-B985-BEB9187030DB).
    if (!CFEqual(typeUUID, kAudioServerPlugInTypeUUID)) {
        return nullptr;
    }

    // Store shared pointer to the driver to keep it alive.
    static std::shared_ptr<aspl::Driver> driver = CreateExampleDriver();

    return driver->GetReference();
}

// sudo killall coreaudiod