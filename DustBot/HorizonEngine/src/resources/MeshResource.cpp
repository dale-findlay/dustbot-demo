#include "MeshResource.h"

#include "subsystems\EngineAllocator.h"
#include "subsystems\MaterialFactory.h"
#include "subsystems\VirtualFileSystem.h"
#include "structures\ObjVertex.h"
#include "structures\GLIndexedDrawable.h"
#include "platform\VertexBufferLayout.h"
#include "utilities\IOUtility.h"
#include "utilities\Logger.h"

#include <thread>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

using namespace Horizon::Graphics;

Horizon::MeshResource::MeshResource(const std::string resourceId)
	: Resource(resourceId)
{}

bool Horizon::MeshResource::ResourceLoad(std::string fileName)
{
	Logger::GetInstance()->LogWarning("Loading a mesh resource right now...");

	meshData = LoadMesh(fileName);

	return true;
}


std::vector<Horizon::MeshData> Horizon::MeshResource::LoadMesh(const std::string & fileName)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string resolvedFileName = VirtualFileSystem::GetInstance()->ResolveFilePath(fileName);

	std::string filePath = IOUtility::GetFilePath(resolvedFileName) + "\\";

	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, resolvedFileName.c_str(), filePath.c_str())) {
		Logger::GetInstance()->LogError("Failed to load mesh at location" + fileName + " error: " + err);
	}

	std::vector<ObjVertex> vertices;

	std::unordered_map<int, std::vector<ObjVertex>> materialMeshes;
	std::vector<MeshData> meshes;

	for (int s = 0; s < shapes.size(); ++s) {

		for (int f = 0; f < shapes[s].mesh.indices.size() / 3; ++f) {

			tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
			tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
			tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

			ObjVertex vertex1;
			ObjVertex vertex2;
			ObjVertex vertex3;

			int materialId = shapes[s].mesh.material_ids[f];

			if ((materialId < 0) || (materialId >= static_cast<int>(materials.size()))) {

				materialId = 0;
			}

			int fp0 = idx0.vertex_index;
			int fp1 = idx1.vertex_index;
			int fp2 = idx2.vertex_index;
			for (int k = 0; k < 4; k++) {

				if (k == 3)
				{
					vertex1.position[3] = 1.0f;
					vertex2.position[3] = 1.0f;
					vertex3.position[3] = 1.0f;
				}
				else
				{
					vertex1.position[k] = attrib.vertices[3 * fp0 + k];
					vertex2.position[k] = attrib.vertices[3 * fp1 + k];
					vertex3.position[k] = attrib.vertices[3 * fp2 + k];
				}
			}

			vertex1.texCoord[0] = attrib.texcoords[2 * idx0.texcoord_index];
			vertex1.texCoord[1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
			vertex2.texCoord[0] = attrib.texcoords[2 * idx1.texcoord_index];
			vertex2.texCoord[1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
			vertex3.texCoord[0] = attrib.texcoords[2 * idx2.texcoord_index];
			vertex3.texCoord[1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];

			int fn0 = idx0.normal_index;
			int fn1 = idx1.normal_index;
			int fn2 = idx2.normal_index;
			for (int k = 0; k < 4; k++) {

				if (k == 3)
				{
					vertex1.normal[3] = 1.0f;
					vertex2.normal[3] = 1.0f;
					vertex3.normal[3] = 1.0f;
				}
				else
				{
					vertex1.normal[k] = attrib.normals[3 * fn0 + k];
					vertex2.normal[k] = attrib.normals[3 * fn1 + k];
					vertex3.normal[k] = attrib.normals[3 * fn2 + k];
				}
			}

			materialMeshes[materialId].push_back(vertex1);
			materialMeshes[materialId].push_back(vertex2);
			materialMeshes[materialId].push_back(vertex3);
		}
	}

	for (auto it = materialMeshes.begin(); it != materialMeshes.end(); ++it)
	{
		const auto& mesh = (*it).second;

		MeshData meshData;
		std::unordered_map<Vertex1P1U1N, uint32> distinctVertices;

		for (auto vertexIt = mesh.begin(); vertexIt != mesh.end(); ++vertexIt)
		{
			const ObjVertex& objVertex = (*vertexIt);

			Vertex1P1U1N vertex;
			vertex.position = objVertex.position;
			vertex.texCoord = objVertex.texCoord;
			vertex.normal = objVertex.normal;
			
			if (distinctVertices.count(vertex) == 0)
			{
				distinctVertices[vertex] = (uint32)meshData.vertices.size();
				meshData.vertices.push_back(vertex);
			}			
			
			meshData.indices.push_back(distinctVertices[vertex]);
		}

		meshes.push_back(meshData);
	}

	return meshes;
}


bool Horizon::MeshResource::ResourceInit(const std::unordered_map<std::string, nlohmann::json::value_type>& resourceAttributes)
{
	glIndexedDrawables.reserve(meshData.size());

	int i = 0;
	for (auto it = meshData.begin(); it != meshData.end(); ++it)
	{
		const MeshData& meshData = *it;

		GLIndexedDrawable* indexedDrawable = EngineAllocate(GLIndexedDrawable);
		indexedDrawable->vertexArray = EngineAllocateC(VertexArray, VertexArray());
		indexedDrawable->vertexArray->Bind();


		indexedDrawable->vertexBuffer = EngineAllocateC(VertexBuffer, VertexBuffer(
			(void*)meshData.vertices.data(),
			meshData.vertices.size() * sizeof(Vertex1P1U1N),
			(uint32)meshData.vertices.size()));

		VertexBufferLayout* vertexBufferLayout = EngineAllocateC(VertexBufferLayout, VertexBufferLayout());
		vertexBufferLayout->AddElement<float>(4, offsetof(Vertex1P1U1N, position)); //position
		vertexBufferLayout->AddElement<float>(2, offsetof(Vertex1P1U1N, texCoord)); //texCoord
		vertexBufferLayout->AddElement<float>(4, offsetof(Vertex1P1U1N, normal)); //normal

		indexedDrawable->vertexArray->AddBufferLayout(indexedDrawable->vertexBuffer, vertexBufferLayout);
		indexedDrawable->indexBuffer = EngineAllocateC(IndexBuffer, IndexBuffer((unsigned int*)meshData.indices.data(), meshData.indices.size() * sizeof(unsigned int), (uint32)meshData.indices.size()));

		if (resourceAttributes.at("materialIds").size() > i)
		{
			std::string materialId = resourceAttributes.at("materialIds")[i];
			indexedDrawable->material = MaterialFactory::GetInstance()->GetMaterialFromId(materialId);
		}
		else
		{
			Logger::GetInstance()->LogWarning("Material Id was missing for mesh component " + GetResourceId()->GetId());
			indexedDrawable->material = MaterialFactory::GetInstance()->GetDefaultMaterial();
		}

		glIndexedDrawables.push_back(indexedDrawable);

		i++;
	}
	return true;
}

void Horizon::MeshResource::ResourceShutdown()
{

}

std::string Horizon::MeshResource::GetMaterialIdFromMaterialIndex(const uint32 & index, const std::vector<std::string>& materialIds)
{
	if (materialIds.size() <= index)
	{
		return MaterialFactory::GetInstance()->GetDefaultMaterial()->GetMaterialId();
	}

	return "";
}
