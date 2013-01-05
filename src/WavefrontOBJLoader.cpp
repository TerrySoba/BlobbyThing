/*
 * WavefrontOBJLoader.cpp
 *
 *  Created on: 17.02.2012
 *      Author: yoshi252
 */


#include "WavefrontOBJLoader.h"
#include "ErrorLogging.h"
#include "Exception.h"
#include "VectorTriangleObject.h"
#include <boost/foreach.hpp>
#ifndef _MSC_VER
#include "libgen.h"
#endif
#include "common.h"

using blobby::string::format;

#ifdef _MSC_VER
#define strtok_r(tok, delim, state) strtok(tok, delim)

std::string dirname(std::string path) {
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( path.c_str(), drive, dir, fname, ext );

	return std::string(dir);
}

#endif

/**
 * Returns the directory portion of a path.
 *
 * \param [in] path To be split.
 * \return the directory portion of the given path.
 */
std::string dirName(const std::string& path)
{
	std::vector<char> data(path.size()+1);
	memcpy(data.data(), path.c_str(), path.size()+1);
	return std::string(dirname(data.data()));
}

struct FaceIndex {
	uint32_t vertexIndex;
	uint32_t textureIndex;
	uint32_t normalIndex;
};

typedef std::vector<FaceIndex> Face;
typedef std::vector<MyGLVertex> Polygon;

std::vector<Polygon> convertPolygonToTriangles(Polygon& polygon) {
	std::vector<Polygon> triVector;

	for (size_t i = 0; polygon.size() > 2 && i < polygon.size() - 2; i++) {
		Polygon tri;

		tri.push_back(polygon[0]);
		tri.push_back(polygon[i + 1]);
		tri.push_back(polygon[i + 2]);

		triVector.push_back(tri);
	}

	return triVector;
}

WavefrontOBJLoader::WavefrontOBJLoader() {
	// TODO Auto-generated constructor stub

}

WavefrontOBJLoader::~WavefrontOBJLoader() {
	// TODO Auto-generated destructor stub
}

std::map<std::string, ObjMaterial> WavefrontOBJLoader::loadMaterial(const std::string& path) {
	std::map<std::string, ObjMaterial> mtl;

	const size_t lineBufferSize = 1000;
	char lineBuffer[lineBufferSize];
	// FILE* fp = fopen(path.c_str(), "r");
	std::fstream fp(path, std::ios_base::in);

	if (!fp.is_open()) {
		THROW_BLOBBY_EXCEPTION(format("Could not open file: %1%", path));
	}

	// this loop is some kind of state machine, these are the variables...
	std::string mtlname = "";

	uint32_t lineNumber = 0;
	while (!fp.eof()) {
		lineNumber++;
		char cur;
		size_t linePos = 0;
		/* read a single line */
		do {
			checked_fread(&cur, 1, 1, fp); // read one character
			lineBuffer[linePos++] = cur;
			if (linePos >= lineBufferSize) {
				THROW_BLOBBY_EXCEPTION(format("%2%:%1%) Line too long.", lineNumber, path));
			}
		} while (!fp.eof() && cur != '\n' && cur != '\r');
		lineBuffer[linePos] = '\0'; // terminate string with null

		/* now parse line */
		// get first token to determine what to expect
		char *saveptr = NULL;
		char *token;
		token = strtok_r(lineBuffer, " \n\r", &saveptr);

		// now determine action according to token
		if (token == NULL)
			continue; // empty line, read next line
		if (token[0] == '#')
			continue; // skip comments

		if (strcmp("newmtl", token) == 0) { // new material
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) { // initialize new material with default values
				mtlname = token;
				mtl[mtlname].name = mtlname;
			}
			continue;
		}

		if (strcmp("map_Kd", token) == 0) { // path to diffuse map texture
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				if (token[0] == '/' || token[0] == '\\') { // absolute path
					mtl[mtlname].diffuseMapPath = token;
				} else { // relative path
					mtl[mtlname].diffuseMapPath = (boost::format("%1%/%2%")%dirName(path)%token).str();
				}
			}
			continue;
		}

		if (strcmp("fragment_shader", token) == 0) { // path to fragment_shader
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				if (token[0] == '/' || token[0] == '\\') { // absolute path
					mtl[mtlname].fragmentShaderPath = token;
				} else { // relative path
					mtl[mtlname].fragmentShaderPath = (boost::format("%1%/%2%")
							% dirName(path) % token).str();
				}
			}
			continue;
		}

		if (strcmp("vertex_shader", token) == 0) { // path to vertex_shader
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				if (token[0] == '/' || token[0] == '\\') { // absolute path
					mtl[mtlname].vertexShaderPath = token;
				} else { // relative path
					mtl[mtlname].vertexShaderPath = (boost::format("%1%/%2%")
							% dirName(path) % token).str();
				}
			}
			continue;
		}

		if (strcmp("Ns", token) == 0) { // specular factor
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float specular;
				if (sscanf(token, "%f", &specular) == 1) {
					mtl[mtlname].specular = specular;
				} else {
					THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid format of specular value.", path, lineNumber));
				}
			}
			continue;
		}

		if (strcmp("d", token) == 0 || strcmp("Tr", token) == 0) { // transparency
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float transparency;
				if (sscanf(token, "%f", &transparency) == 1) {
					mtl[mtlname].transparency = transparency;
				} else {
					THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid format of transparency value.", path, lineNumber));
				}
			}
			continue;
		}

		if (strcmp("Ka", token) == 0) { // ambient color
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float color[3];
				if (sscanf(token, "%f %f %f", &(color[0]), &(color[1]), &(color[2])) == 3) {
					for (int i = 0; i < 3; i++) {
						mtl[mtlname].ambientColor[i] = color[i];
					}
				} else {
					THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid format of ambient color.", path, lineNumber));
				}
			}
			continue;
		}

		if (strcmp("Kd", token) == 0) { // diffuse color
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float color[3];
				if (sscanf(token, "%f %f %f", &(color[0]), &(color[1]), &(color[2])) == 3) {
					for (int i = 0; i < 3; i++) {
						mtl[mtlname].diffuseColor[i] = color[i];
					}
				} else {
					THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid format of diffuse color.", path, lineNumber));
				}
			}
			continue;
		}

		if (strcmp("Ks", token) == 0) { // specular color
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float color[3];
				if (sscanf(token, "%f %f %f", &(color[0]), &(color[1]), &(color[2])) == 3) {
					for (int i = 0; i < 3; i++) {
						mtl[mtlname].specularColor[i] = color[i];
					}
				} else {
					THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid format of specular color.", path, lineNumber));
				}
			}
			continue;
		}
	}

	return mtl;
}


std::vector<shared_ptr<ObjModel>> WavefrontOBJLoader::load(const std::string& path) {
	std::vector<shared_ptr<ObjModel>> modelsRet;
	const size_t lineBufferSize = 1000;
	char lineBuffer[lineBufferSize];
	// FILE* fp = fopen(path.c_str(), "r");
	std::fstream fp(path, std::ios_base::in);

	if (!fp.is_open()) {
		THROW_BLOBBY_EXCEPTION(format("Could not open file: %1%", path));
	}

	std::vector<Vector3f> vertices;
	std::vector<Vector2f> texCoords;
	std::vector<Vector3f> normals;

	typedef std::pair<std::string, // model name
			           std::string  // material name
			          > ModelMaterialIndex;

	std::map<
	    ModelMaterialIndex,
		std::vector<Face>
	        > faces;



	std::string currentMaterial = "";
	std::string currentObjectName = "";

	std::map<std::string, ObjMaterial> mtlLib;

	uint32_t lineNumber = 0;
	while (!fp.eof()) {
		lineNumber++;
		char cur;
		size_t linePos = 0;
		/* read a single line */
		do {
			checked_fread(&cur, 1, 1, fp); // read one character
			lineBuffer[linePos++] = cur;
			if (linePos >= lineBufferSize) {
				THROW_BLOBBY_EXCEPTION(format("%1%:%2% Line too long!.", path, lineNumber));
			}
		} while (!fp.eof() && cur != '\n' && cur != '\r');
		lineBuffer[linePos] = '\0'; // nullterminate string

		// now we have a single line in lineBuffer

		/* now parse line */
		// get first token to determine what to expect
		char *saveptr = NULL;
		char *token;
		token = strtok_r(lineBuffer, " \n\r", &saveptr);

		// now determine action according to token
		if (token == NULL)
			continue; // empty line, read next line
		if (token[0] == '#')
			continue; // skip comments

		if (strlen(token) == 1) {
			if (token[0] == 'o') { // found object name
				token = strtok_r(NULL, " \n\r", &saveptr);
				if (token) {
					currentObjectName = token;
					LOG(format("%1%:%2% Found object named: %3% ", path, lineNumber, token));
				}
				continue;
			} // end object name

			if (token[0] == 'v') { // found vertex
				Vector3f vertex;
				for (int i = 0; i < 3; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						int foundValues = sscanf(token, "%f", &vertex[i]);
						if (foundValues != 1) { // check if conversion worked
							THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid float format.", path, lineNumber));
						}
					} else { // token was invalid
						THROW_BLOBBY_EXCEPTION(format("%1%:%2% Too few tokens in vertex.", path, lineNumber));
					}
				}
				vertices.push_back(vertex);
				continue;
			} // end vertex

			if (token[0] == 'f') { // found face
				Face face;
				while ((token = strtok_r(NULL, " \n\r", &saveptr))) {
					FaceIndex faceIndex;
					int foundValues = sscanf(token, "%d/%d/%d",
							&faceIndex.vertexIndex, &faceIndex.textureIndex,
							&faceIndex.normalIndex);
					if (foundValues != 3) {
						THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid face description. Probably missing normals and/or texture coordinates.", path, lineNumber));
					}
					face.push_back(faceIndex);
				}

				auto index = std::make_pair(currentObjectName, currentMaterial);
				faces[index].push_back(face);

				continue;
			} // end face
		}



		if (strlen(token) == 2) {
			if (token[0] == 'v' && token[1] == 't') { // found texture coordinate
				Vector2f texCoord;
				for (int i = 0; i < 2; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						int foundValues = sscanf(token, "%f", &texCoord[i]);
						if (foundValues != 1) { // check if conversion worked
							THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid float format.", path, lineNumber));
						}
					} else { // token was invalid
						THROW_BLOBBY_EXCEPTION(format("%1%:%2% Too few tokens in texture coordinate.", path, lineNumber));
					}
				}
				texCoords.push_back(texCoord);
				continue;
			} // end texture coord

			if (token[0] == 'v' && token[1] == 'n') { // found normal
				Vector3f normal;
				for (int i = 0; i < 3; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						int foundValues = sscanf(token, "%f", &normal[i]);
						if (foundValues != 1) { // check if conversion worked
							THROW_BLOBBY_EXCEPTION(format("%1%:%2% Invalid float format.", path, lineNumber));
						}
					} else { // token was invalid
						THROW_BLOBBY_EXCEPTION(format("%1%:%2% Too few tokens in normal.", path, lineNumber));
					}

				}
				normals.push_back(normal);
				continue;

			} // end vertex

		} // end strlen(token) == 2

		if (strcmp("mtllib", token) == 0) {
			// load material library
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				std::string mtllibPath = format("%1%/%2%", dirName(path), token);
				mtlLib = loadMaterial(mtllibPath.c_str());
			}

			continue;
		}

		if (strcmp("usemtl", token) == 0) {
			// read name of material to use
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				currentMaterial = token;
				LOG("Using material named: ", currentMaterial);
			}

			continue;
		}

	} // while !feof()

	if (fp.is_open()) {
		fp.close();
	}

	/* now convert data into triangles */
	std::map<ModelMaterialIndex, std::vector<Polygon>> triangles;

	BOOST_FOREACH(auto &fa, faces) {
		BOOST_FOREACH(Face& f, fa.second) {
			// create a Polygon from the faces
			Polygon p;
			MyGLVertex vertex;

			BOOST_FOREACH(FaceIndex& fi, f) {
				for (int n = 0; n < 3; n++) {
					vertex.v[n] = vertices.at(fi.vertexIndex-1)[n];
					vertex.n[n] = normals.at(fi.normalIndex-1)[n];
				}
				for (int n = 0; n < 2; n++) {
					vertex.vt[n] = texCoords.at(fi.textureIndex-1)[n];
				}

				p.push_back(vertex);
			}

			std::vector<Polygon> tmpTri = convertPolygonToTriangles(p);

			// now add triangles to real triangle object
			const ModelMaterialIndex& index = fa.first;
			triangles[index].insert(triangles[index].end(), tmpTri.begin(), tmpTri.end());
		}
	}

	uint32_t triangle_count = 0;

	std::map<std::string /* model name */, std::vector<ModelPart>> models;

	typedef const std::pair<ModelMaterialIndex, std::vector<Polygon>> IndexPolygon;

	BOOST_FOREACH(IndexPolygon &modelPart, triangles) {
		// first insert triangles
		shared_ptr<VectorTriangleObject> vertexes = make_shared<VectorTriangleObject>();
		// VectorTriangleObject* vertexes = ;
		BOOST_FOREACH(const Polygon& p, modelPart.second) {
			if (p.size() >= 3) {
				for (int i = 0; i < 3; i++) {
					vertexes->addVertex(p.at(i));
				}
			}
			triangle_count++;
		}

		ModelPart part;
		part.triangles = vertexes;
		const std::string& materialName = modelPart.first.second;
		const std::string& modelName = modelPart.first.first;
		part.material = mtlLib[materialName];

		models[modelName].push_back(part);
	}

	BOOST_FOREACH(auto& modelTmp, models) {
		shared_ptr<ObjModel> model = make_shared<ObjModel>();
		model->name = modelTmp.first;
		model->modelParts = modelTmp.second;
		modelsRet.push_back(model);
	}

	LOG("We have ", modelsRet.size(), " model(s).");
	LOG("We have ", triangle_count, " triangles.");

	return modelsRet;
}
