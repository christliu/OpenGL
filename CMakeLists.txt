cmake_minimum_required (VERSION 3.8)
cmake_policy(VERSION 3.8)

Project(OpenGL)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)
add_library(Shader src/Shader.cpp)
set(LIBS glfw3 opengl32 assimp glew32s soil Shader)

set(Projects
	1.EmptyWindow
	2.MultiLighting
	3.ModelLoading
	4.DepthTest
	5.StencilTest
	6.FrameBuffer
	7.CubeMap
	8.UniformBuffer
	9.1.GeometryShader
	9.2.ExplodeShader
	9.3.NormalShow
	10.1.Instancing
	10.2.Instancing-planet
	11.1.ShadowMap
	11.2.ShadowMapScene
)

foreach(Proj ${Projects})
	file(GLOB SOURCE
        "src/${Proj}/*.h"
        "src/${Proj}/*.cpp"
    )
    message($SOURCE)
	add_executable(${Proj} ${SOURCE})
	target_link_libraries(${Proj} ${LIBS})
	set_target_properties(${Proj} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${Proj}")
	set_target_properties(${Proj} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endforeach(Proj)