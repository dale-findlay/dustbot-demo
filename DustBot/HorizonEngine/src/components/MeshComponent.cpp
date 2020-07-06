#include "MeshComponent.h"

#include "core\GameObject.h"
#include "subsystems\EngineAllocator.h"
#include "subsystems\ResourceManager.h"
#include "subsystems\LightManager.h"
#include "utilities\Logger.h"
#include "platform\GLHelper.h"

#include <string>

using namespace Horizon::Graphics;

Horizon::MeshComponent::MeshComponent()
	: DrawableComponent(), m_meshResourceId(""), m_wireFrame(false)
{}

void Horizon::MeshComponent::CreateResources()
{
	std::vector<Resource*> resources;

	m_meshResource = (MeshResource*)ResourceManager::GetInstance()->GetResource(m_meshResourceId);

	if (m_meshResource == nullptr)
	{
		m_meshResource = EngineAllocateC(MeshResource, MeshResource(m_meshResourceId));
		resources.push_back(m_meshResource);
	}

	if (resources.size() > 0)
	{
		ResourceManager::GetInstance()->AddResources(resources, [this] {this->InitResourcesCallback(); }, resourceSetId);
	}
}


bool Horizon::MeshComponent::InitComponent()
{
	Logger::GetInstance()->LogWarning("Initializing a MeshComponent");

	auto resourceAttributes = ResourceManager::GetInstance()->GetResourceAttributes(m_meshResourceId);
	resourceAttributes["materialIds"] = m_materialIds;

	m_meshResource->ResourceInit(resourceAttributes);

	return true;
}

void Horizon::MeshComponent::UpdateComponent()
{

}

void Horizon::MeshComponent::DrawComponent()
{
	auto lightData = LightManager::GetInstance()->GetUniformLightData();

	for (auto it = m_meshResource->glIndexedDrawables.begin(); it != m_meshResource->glIndexedDrawables.end(); ++it)
	{
		if ((*it)->material != nullptr)
		{
			if ((*it)->vertexBuffer->GetVertexCount() > 0)
			{
				(*it)->material->BindMaterial();

				ShaderProgram* shaderProgram = (*it)->material->GetShaderProgram();

				glm::mat4 model = gameObject->transform.Model();

				glm::mat4 normalMat = transpose(inverse(model));

				shaderProgram->BindUniform("model", model);
				shaderProgram->BindUniform("normalMat", normalMat);

				for (int i = 0; i < lightData.size(); ++i)
				{
					if (lightData[i].lightType != -1)
					{
						std::string lightStr = "lights[" + std::to_string(i) + "]";
						shaderProgram->BindUniform(lightStr + ".lightType", lightData[i].lightType);
						shaderProgram->BindUniform(lightStr + ".lightPos", lightData[i].lightPos);
						shaderProgram->BindUniform(lightStr + ".lightDirection", lightData[i].lightDirection);
					}
				}

				(*it)->vertexArray->Bind();
				(*it)->indexBuffer->Bind();

				if (m_wireFrame)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				
				GL(glDrawElements(GL_TRIANGLES, (*it)->indexBuffer->GetIndexCount(), GL_UNSIGNED_INT, NULL));
				
				//This is just to reset the state.
				if (m_wireFrame)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				(*it)->material->UnbindMaterial();
			}
		}
	}
}

void Horizon::MeshComponent::ShutdownComponent()
{}

bool Horizon::MeshComponent::CreateComponent(const std::unordered_map<std::string, nlohmann::json::value_type>& componentAttributes)
{
	if (componentAttributes.count("meshResourceId") == 0)
	{
		Logger::GetInstance()->LogError("MeshComponent could not be created as a meshResourceId was not present in the scene definition.");
		return false;
	}

	std::string meshResourceId = std::string(componentAttributes.at("meshResourceId").get<std::string>());

	if (meshResourceId.empty())
	{
		Logger::GetInstance()->LogError("MeshComponent could not be created as a meshResourceId was not present in the scene definition.");
		return false;
	}
	
	m_meshResourceId = meshResourceId;


	if (componentAttributes.count("materialIds") > 0)
	{
		std::vector<std::string> materialIds = componentAttributes.at("materialIds").get<std::vector<std::string>>();
		m_materialIds = materialIds;
	}

	if (componentAttributes.count("wireframe") > 0)
	{
		m_wireFrame = componentAttributes.at("wireframe").get<bool>();
	}

	m_resourceCount++;

	return true;
}

