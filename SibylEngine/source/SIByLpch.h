#pragma once

#include <iostream>
#include <memory>
#include <cstdint>
#include <mutex>
#include <utility>
#include <algorithm>
#include <functional>
#include <new>
#include <cmath>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <codecvt>
#include <comdef.h>
#include <mutex>
#include <filesystem>

#include <map>
#include <set>
#include <vector>
#include <deque>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

#include "Sibyl/Core/Log.h"
#include "Sibyl/Core/Instrumental.h"
#include "entt.hpp"

#include "Sibyl/Basic/ThreadSafeQueue.h"

#ifdef SIBYL_PLATFORM_WINDOWS

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

#include <Windows.h>
#include <wincodec.h>
#include <windowsx.h>
#include <wrl.h>
#include <dxgi1_4.h>

#include <d3d12.h>

#include <d3dcompiler.h>
#include <dxgidebug.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <Platform/DirectX12/include/d3dx12.h>

using namespace Microsoft::WRL;
using namespace DirectX;

#endif // SIBYL_PLATFORM_WINDOWS