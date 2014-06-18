#include <string>
#include <stdio.h>
#include "glsl_optimizer.h"

static void replace_string (std::string& target, const std::string& search, const std::string& replace, size_t startPos)
{
	if (search.empty())
		return;
	
	std::string::size_type p = startPos;
	while ((p = target.find (search, p)) != std::string::npos)
	{
		target.replace (p, search.size (), replace);
		p += replace.size ();
	}
}

static bool ReadStringFromFile (const char* pathName, std::string& output)
{
	FILE* file = fopen( pathName, "rb" );
	if (file == NULL)
		return false;
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (length < 0)
	{
		fclose( file );
		return false;
	}
	output.resize(length);
	int readLength = fread(&*output.begin(), 1, length, file);
	fclose(file);
	if (readLength != length)
	{
		output.clear();
		return false;
	}

	replace_string(output, "\r\n", "\n", 0);
	return true;
}

int main(int argc, const char** argv)
{
   glslopt_target target = kGlslTargetOpenGLES20;
   glslopt_ctx* ctx = glslopt_initialize(target);

   const char* shaderPath = "shaders/test.frag";
   glslopt_shader_type shaderType = kGlslOptShaderFragment;
   glslopt_options options;

   std::string shaderSource;
   bool success = ReadStringFromFile(shaderPath, shaderSource);
   if (!success)
   {
     printf("Could not read file: %s\n", shaderPath);
     return 1;
   }
 
   glslopt_shader* shader = glslopt_optimize (ctx, shaderType, shaderSource.c_str(), options);
   if (glslopt_get_status (shader)) {
     const char* newSource = glslopt_get_output (shader);
     printf("Compiled without a hitch\n");
     //printf("%s", newSource);
   } else {
     const char* errorLog = glslopt_get_log (shader);
     printf("%s", errorLog);
   }
   glslopt_shader_delete (shader);

   glslopt_cleanup (ctx);
}
