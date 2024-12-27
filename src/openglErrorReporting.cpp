#include "openglErrorReporting.h"
#include <iostream>

//https://learnopengl.com/In-Practice/Debugging
void GLAPIENTRY glDebugOutput(GLenum source,
	GLenum type,
	unsigned int id,
	GLenum severity,
	GLsizei length,
	const char* message,
	const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204
		|| id == 131222
		) return;
	if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API_ARB:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION_ARB:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER_ARB:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR_ARB:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY_ARB:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE_ARB:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_OTHER_ARB:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH_ARB:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM_ARB:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW_ARB:          std::cout << "Severity: low"; break;
	} std::cout << std::endl;
	std::cout << std::endl;

}

void enableReportGlErrors()
{

    std::cout << "Debug Output Support: " << (GLAD_GL_ARB_debug_output?"Supported":"Not supported") << "\n";

    if (!GLAD_GL_ARB_debug_output)
    {
        std::cerr << "Debug output not supported\n";
        return;
    }
	glEnable(GL_ARB_debug_output);



    glDebugMessageCallbackARB(glDebugOutput, nullptr);

    // Enable synchronous callback. This ensures that your callback function is called
    // right after an error has occurred. This capability is not defined in the AMD
    // version.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

    // Enable all messages including deprecated messages
    glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    // Disable some messages
    // glDebugMessageControlARB(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);

}
