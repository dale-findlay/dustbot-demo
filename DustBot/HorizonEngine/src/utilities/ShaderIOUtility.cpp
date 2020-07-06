#include "ShaderIOUtility.h"

#include "utilities\IOUtility.h"
#include "utilities\StringTools.h"

std::map<std::string, std::string> Horizon::ShaderIOUtility::GetShaderParts(const std::string& fileName)
{
	const std::string shaderSource = IOUtility::ReadFileToEnd(fileName);

	std::map<std::string, std::string> shaderParts;

	std::vector<std::string> shaderPartsString = StringTools::Split(shaderSource, "--");
	//format is --shader type--, remove the empty string at the start.
	shaderPartsString.erase(shaderPartsString.begin());

	std::string currentShaderPart = "";

	while (shaderPartsString.size() > 0)
	{
		if (currentShaderPart.size() > 0)
		{
			shaderParts[currentShaderPart] = shaderPartsString.front();
			currentShaderPart = "";
		}
		else
		{
			currentShaderPart = shaderPartsString.front();
		}

		shaderPartsString.erase(shaderPartsString.begin());
	}
	
	return shaderParts;
}
