#include <webgpu/webgpu_cpp.h>
#include <webgpu/webgpu_cpp_print.h>

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  static constexpr auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
  wgpu::InstanceDescriptor instanceDescriptor{
    .requiredFeatureCount = 1,
    .requiredFeatures = &kTimedWaitAny
  };

  wgpu::Instance instance = wgpu::CreateInstance(&instanceDescriptor);
  if (instance == nullptr) {
    std::cerr << "Instance creation failed!\n";
    return EXIT_FAILURE;
  }
  // Synchronously request the adapter.
  wgpu::RequestAdapterOptions options = {};
  wgpu::Adapter adapter;

  auto callback = [](wgpu::RequestAdapterStatus status, wgpu::Adapter adapter, const char *message, void *userdata) {
    if (status != wgpu::RequestAdapterStatus::Success) {
      std::cerr << "Failed to get an adapter:" << message;
      return;
    }
    *static_cast<wgpu::Adapter *>(userdata) = adapter;
  };


  auto callbackMode = wgpu::CallbackMode::WaitAnyOnly;
  void *userdata = &adapter;
  instance.WaitAny(instance.RequestAdapter(&options, callbackMode, callback, userdata), UINT64_MAX);
  if (adapter == nullptr) {
    std::cerr << "RequestAdapter failed!\n";
    return EXIT_FAILURE;
  }

  wgpu::DawnAdapterPropertiesPowerPreference power_props{};

  wgpu::AdapterInfo info{};
  info.nextInChain = &power_props;

  adapter.GetInfo(&info);
  std::cout << "VendorID: " << std::hex << info.vendorID << std::dec << "\n";
  std::cout << "Vendor: " << info.vendor << "\n";
  std::cout << "Architecture: " << info.architecture << "\n";
  std::cout << "DeviceID: " << std::hex << info.deviceID << std::dec << "\n";
  std::cout << "Name: " << info.device << "\n";
  std::cout << "Driver description: " << info.description << "\n";
  std::cout << "BackendType: " << info.backendType << "\n";
  std::cout << "AdapterType: " << info.adapterType << "\n";

  // Synchronously request a Device from the adapter.
  wgpu::Device device;

  wgpu::DeviceDescriptor deviceDesc = {};
  // Install a device-lost callback that logs the reason and optional message.
  // Use the no-userdata overload (allows capturing lambdas if needed).
  deviceDesc.SetDeviceLostCallback(callbackMode,
    [](const wgpu::Device& /*dev*/, wgpu::DeviceLostReason reason, wgpu::StringView message) {
      const char* msg = message.data ? message.data : "(no message)";
      std::cerr << "Device lost (reason " << static_cast<int>(reason) << "): " << msg << "\n";
    });


  auto deviceCallback = [](wgpu::RequestDeviceStatus status, wgpu::Device device, const char *message, void *userdata) {
    if (status != wgpu::RequestDeviceStatus::Success) {
      std::cerr << "Failed to get a device: " << (message ? message : "(no message)") << "\n";
      return;
    }
    *static_cast<wgpu::Device *>(userdata) = device;
  };

  void *deviceUserdata = &device;
  // Reuse the same callback mode used for the adapter request.
  instance.WaitAny(adapter.RequestDevice(&deviceDesc, callbackMode, deviceCallback, deviceUserdata), UINT64_MAX);

  if (device == nullptr) {
    std::cerr << "RequestDevice failed!\n";
    return EXIT_FAILURE;
  }

  std::cout << "Device successfully created. \n";

  // 在确认 device != nullptr 之后
  adapter = nullptr; // 释放 adapter 的引用，让它回收
  std::cout << "Adapter released; device still valid  ↓ ↓ ↓ ↓ ↓\n";


  wgpu::SupportedFeatures features;
  device.GetFeatures(&features);
  std::cout << "Supported features count: " << features.featureCount << "\n";
  for (size_t i = 0; i < features.featureCount; ++i) {
    std::cout << " Feature[" << i << "]: " << features.features[i] << "\n";
  }


  wgpu::Limits limits;
  device.GetLimits(&limits);
  std::cout << "Device limits:\n";
  std::cout << " maxTextureDimension1D: " << limits.maxTextureDimension1D << "\n";
  std::cout << " maxTextureDimension2D: " << limits.maxTextureDimension2D << "\n";
  std::cout << " maxTextureDimension3D: " << limits.maxTextureDimension3D << "\n";
  std::cout << " maxTextureArrayLayers: " << limits.maxTextureArrayLayers << "\n";
  std::cout << " maxBindGroups: " << limits.maxBindGroups << "\n";
  std::cout << " maxBufferSize: " << limits.maxBufferSize << "\n";
  std::cout << " maxColorAttachments: " << limits.maxColorAttachments << "\n";
  std::cout << " maxComputeWorkgroupStorageSize: " << limits.maxComputeWorkgroupStorageSize << "\n";
  std::cout << " maxComputeInvocationsPerWorkgroup: " << limits.maxComputeInvocationsPerWorkgroup << "\n";
  std::cout << " maxComputeWorkgroupSizeX: " << limits.maxComputeWorkgroupSizeX << "\n";
  std::cout << " maxComputeWorkgroupSizeY: " << limits.maxComputeWorkgroupSizeY << "\n";
  std::cout << " maxComputeWorkgroupSizeZ: " << limits.maxComputeWorkgroupSizeZ << "\n";
  std::cout << " maxComputeWorkgroupsPerDimension: " << limits.maxComputeWorkgroupsPerDimension << "\n";
  std::cout << " maxImmediateSize: " << limits.maxImmediateSize << "\n";

  return EXIT_SUCCESS;
}