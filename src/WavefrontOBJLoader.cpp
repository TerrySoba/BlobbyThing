/*
 * WavefrontOBJLoader.cpp
 *
 *  Created on: 17.02.2012
 *      Author: yoshi252
 */


#include "WavefrontOBJLoader.h"
#include "ErrorLogging.h"
#include "VectorTriangleObject.h"
#include <boost/foreach.hpp>
#include "libgen.h"
#include "common.h"

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

std::map<std::string, ObjMaterial> WavefrontOBJLoader::loadMaterial(const char* path) {
	std::map<std::string, ObjMaterial> mtl;


	const size_t lineBufferSize = 1000;
	char lineBuffer[lineBufferSize];
	FILE* fp = fopen(path, "r");

	if (!fp) {
		ERR("Could not open file: ", path);
		return mtl;
	}

	// this loop is some kind of state machine, these are the variables...
	std::string mtlname = "";

	while (!feof(fp)) {
		char cur;
		size_t linePos = 0;
		/* read a single line */
		do {
			checked_fread(&cur, 1, 1, fp); // read one character
			lineBuffer[linePos++] = cur;
			if (linePos >= lineBufferSize) {
				ERR("Line too long!");
				return mtl;
			}
		} while (!feof(fp) && cur != '\n' && cur != '\r');
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
					char* pathTmp = new char[strlen(path)+1];
					strcpy(pathTmp, path);
					// LOG("pathTmp: ", pathTmp, "  token: ", token);
					mtl[mtlname].diffuseMapPath = (boost::format("%1%/%2%")%dirname(pathTmp)%token).str();
					delete[] pathTmp;
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
					char* pathTmp = new char[strlen(path) + 1];
					strcpy(pathTmp, path);
					// LOG("pathTmp: ", pathTmp, "  token: ", token);
					mtl[mtlname].fragmentShaderPath = (boost::format("%1%/%2%")
							% dirname(pathTmp) % token).str();
					delete[] pathTmp;
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
					char* pathTmp = new char[strlen(path) + 1];
					strcpy(pathTmp, path);
					// LOG("pathTmp: ", pathTmp, "  token: ", token);
					mtl[mtlname].vertexShaderPath = (boost::format("%1%/%2%")
							% dirname(pathTmp) % token).str();
					delete[] pathTmp;
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
					ERR("Invalid format of specular value");
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
					ERR("Invalid format of transparency value");
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
					ERR("Invalid format of ambient color");
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
					ERR("Invalid format of diffuse color");
				}
			}
			continue;
		}

		if (strcmp("Ks", token) == 0) { // specular color
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				float color[3];
				if (sscanf(token, "%f %f %f", &(color[0]), &(color[1]),	&(color[2])) == 3) {
					for (int i = 0; i < 3; i++) {
						mtl[mtlname].specularColor[i] = color[i];
					}
				} else {
					ERR("Invalid format of specular color");
				}
			}
			continue;
		}
	}

	if (fp) {
		fclose(fp);
	}

	return mtl;
}


std::vector<shared_ptr<ObjModel>> WavefrontOBJLoader::load(const char* path) {
	std::vector<shared_ptr<ObjModel>> modelsRet;
	const size_t lineBufferSize = 1000;
	char lineBuffer[lineBufferSize];
	FILE* fp = fopen(path, "r");

	if (!fp) {
		ERR("Could not open file: ", path);
		return modelsRet;
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

	while (!feof(fp)) {
		char cur;
		size_t linePos = 0;
		/* read a single line */
		do {
			checked_fread(&cur, 1, 1, fp); // read one character
			lineBuffer[linePos++] = cur;
			if (linePos >= lineBufferSize) {
				ERR("Line too long!");
				return modelsRet;
			}
		} while (!feof(fp) && cur != '\n' && cur != '\r');
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
					LOG("Found object named: ", token);
				}
				continue;
			} // end object name

			if (token[0] == 'v') { // found vertex
				bool vertexOK = true;
				Vector3f vertex;
				for (int i = 0; i < 3; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						vertex[i] = strtof(token, &endptr);
						if (token == endptr) { // check if conversion worked
							ERR("Invalid float format.");
							vertexOK = false;
						}
					} else { // token was invalid
						ERR("Too few tokens in vertex.");
						vertexOK = false;
					}

				}
				if (vertexOK) {
					vertices.push_back(vertex);
					// LOG("Found vertex %f, %f, %f", vertex[0], vertex[1], vertex[2]);
				}
				continue;
			} // end vertex


			if (token[0] == 'f') { // found face
				// std::vector<Face> faces;
				Face face;
				bool faceOK = true;
				while ((token = strtok_r(NULL, " \n\r", &saveptr))) {
					FaceIndex faceIndex;
					int foundValues = sscanf(token, "%d/%d/%d",
							&faceIndex.vertexIndex, &faceIndex.textureIndex,
							&faceIndex.normalIndex);
					if (foundValues != 3) {
						faceOK = false;
						ERR("Invalid face description. Probably missing normals and/or texture coordinates!");
						break;
					}
					face.push_back(faceIndex);
				}

				if (faceOK) {
					auto index = std::make_pair(currentObjectName, currentMaterial);
					faces[index].push_back(face);
				}

				continue;
			} // end face

		}

		if (strlen(token) == 2) {
			if (token[0] == 'v' && token[1] == 't') { // found texture coordinate
				bool vertexOK = true;
				Vector2f texCoord;
				for (int i = 0; i < 2; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						texCoord[i] = strtof(token, &endptr);
						if (token == endptr) { // check if conversion worked
							ERR("Invalid float format.");
							vertexOK = false;
						}
					} else { // token was invalid
						ERR("Too few tokens in texture coordinate.");
						vertexOK = false;
					}

				}
				if (vertexOK) {
					texCoords.push_back(texCoord);
					// LOG("Found texture coord %f, %f", texCoord[0], texCoord[1]);
				}
				continue;
			} // end texture coord

			if (token[0] == 'v' && token[1] == 'n') { // found normal
				bool vertexOK = true;
				Vector3f normal;
				for (int i = 0; i < 3; i++) {
					token = strtok_r(NULL, " \n\r", &saveptr);
					if (token) {
						char* endptr;
						normal[i] = strtof(token, &endptr);
						if (token == endptr) { // check if conversion worked
							ERR("Invalid float format.");
							vertexOK = false;
						}
					} else { // token was invalid
						ERR("Too few tokens in normal.");
						vertexOK = false;
					}

				}
				if (vertexOK) {
					normals.push_back(normal);
					// LOG("Found normal %f, %f, %f", normal[0], normal[1], normal[2]);
				}
				continue;

			} // end vertex

		} // end strlen(token) == 2

		if (strcmp("mtllib", token) == 0) {
			// load material library
			token = strtok_r(NULL, "\n\r", &saveptr);
			if (token) {
				char* tmpPath = new char[strlen(path)+1];
				strcpy(tmpPath, path);
				std::string mtllibPath = (boost::format("%1%/%2%") % dirname(tmpPath) % token).str();
				delete[] tmpPath;
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

	if (fp) {
		fclose(fp);
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
