local function setup_openal(directory)
	project "OpenAL-Soft"
		kind "SharedLib"
		cdialect "C11"
		cppdialect "C++14"

		warnings "High"

		files {
			"common/albit.h",
			"common/albyte.h",
			"common/alcomplex.cpp",
			"common/alcomplex.h",
			"common/aldeque.h",
			"common/alfstream.cpp",
			"common/alfstream.h",
			"common/almalloc.cpp",
			"common/almalloc.h",
			"common/alnumbers.h",
			"common/alnumeric.h",
			"common/aloptional.h",
			"common/alspan.h",
			"common/alstring.cpp",
			"common/alstring.h",
			"common/altraits.h",
			"common/atomic.h",
			"common/comptr.h",
			"common/dynload.cpp",
			"common/dynload.h",
			"common/intrusive_ptr.h",
			"common/opthelpers.h",
			"common/phase_shifter.h",
			"common/polyphase_resampler.cpp",
			"common/polyphase_resampler.h",
			"common/pragmadefs.h",
			"common/ringbuffer.cpp",
			"common/ringbuffer.h",
			"common/strutils.cpp",
			"common/strutils.h",
			"common/threads.cpp",
			"common/threads.h",
			"common/vecmat.h",
			"common/vector.h"
		}

		filter "system:windows"
			defines {
				"NOMINMAX",
				"_WIN32",
				"_CRT_SECURE_NO_WARNINGS",
				"ALC_API=__declspec(dllexport)",
				"AL_API=__declspec(dllexport)"
			}

			flags {
				"/w14640", "/wd4065", "/wd4127", "/wd4268", "/wd4324", "/wd5030", "/wd5051"
			}
end

return setup_openal