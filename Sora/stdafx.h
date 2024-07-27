#pragma once

#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <filesystem>
#include <memory>
#include <cstdint>
#include <cassert>

#define NOMINMAX
#include <Windows.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


#include <d3d11.h>
#include <D3DCompiler.h>
#include <DirectXColors.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SimpleMath.h>


#include "Config.hpp"
#include "Logger.hpp"
