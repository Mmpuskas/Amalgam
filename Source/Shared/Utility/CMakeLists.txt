target_sources(Shared
    PRIVATE
        Private/Log.cpp
        Private/PeriodicCaller.cpp
	    Private/ResourceManager.cpp
        Private/Timer.cpp
        Private/TransformationHelpers.cpp
    PUBLIC
        Public/EventHandler.h
        Public/Ignore.h
        Public/Log.h
        Public/PeriodicCaller.h
        Public/Profiler.h
        Public/ResourceManager.h
        Public/SharedConfig.h
        Public/TextureHandle.h
        Public/Timer.h
        Public/TransformationHelpers.h
)

target_include_directories(Shared
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

# Build our precompiled header.
target_precompile_headers(Shared PRIVATE Private/pch.h)
