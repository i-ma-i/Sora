#pragma once

#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <filesystem>
#include <memory>
#include <cassert>

#define NOMINMAX
#include <Windows.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXColors.h>
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SimpleMath.h>


#include "Logger.hpp"
