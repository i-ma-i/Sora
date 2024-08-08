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

// DirectX11
#include <d3d11.h>
#include <D3DCompiler.h>
#include <DirectXColors.h>

// DirectX Tool Kit
#include <DirectXTK/VertexTypes.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/SimpleMath.h>
#include <DirectXTK/GeometricPrimitive.h>

// spdlog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

// magic_enum
#include <magic_enum.hpp>


#include "Config.hpp"
#include "Logger.hpp"
#include "Input.hpp"
