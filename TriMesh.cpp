#include "TriMesh.h"

// 一些基础颜色
const glm::vec3 basic_colors[8] = {
	glm::vec3(1.0, 1.0, 1.0), // White
	glm::vec3(1.0, 1.0, 0.0), // Yellow
	glm::vec3(0.0, 1.0, 0.0), // Green
	glm::vec3(0.0, 1.0, 1.0), // Cyan
	glm::vec3(1.0, 0.0, 1.0), // Magenta
	glm::vec3(1.0, 0.0, 0.0), // Red
	glm::vec3(0.0, 0.0, 0.0), // Black
	glm::vec3(0.0, 0.0, 1.0)	 // Blue
};


// 立方体的各个点
const glm::vec3 cube_vertices[8] = {
	glm::vec3(-0.5, -0.5, -0.5),
	glm::vec3(0.5, -0.5, -0.5),
	glm::vec3(-0.5, 0.5, -0.5),
	glm::vec3(0.5, 0.5, -0.5),
	glm::vec3(-0.5, -0.5, 0.5),
	glm::vec3(0.5, -0.5, 0.5),
	glm::vec3(-0.5, 0.5, 0.5),
	glm::vec3(0.5, 0.5, 0.5) };

// 三角形的点
const glm::vec3 triangle_vertices[3] = {
	glm::vec3(-0.5, -0.5, 0.0),
	glm::vec3(0.5, -0.5, 0.0),
	glm::vec3(0.0, 0.5, 0.0) };

// 正方形平面
const glm::vec3 square_vertices[4] = {
	glm::vec3(-0.5, -0.5, 0.0),
	glm::vec3(0.5, -0.5, 0.0),
	glm::vec3(0.5, 0.5, 0.0),
	glm::vec3(-0.5, 0.5, 0.0),
};

TriMesh::TriMesh()
{
	scale = glm::vec3(1.0);
	rotation = glm::vec3(0.0);
	translation = glm::vec3(0.0);
}


TriMesh::~TriMesh()
{
}

std::vector<glm::vec3> TriMesh::getVertexPositions()
{
	return vertex_positions;
}

std::vector<glm::vec3> TriMesh::getVertexColors()
{
	return vertex_colors;
}

std::vector<glm::vec3> TriMesh::getVertexNormals()
{
	return vertex_normals;
}

std::vector<glm::vec2> TriMesh::getVertexTextures()
{
	return vertex_textures;
}

std::vector<vec3i> TriMesh::getFaces()
{
	return faces;
}

std::vector<glm::vec3> TriMesh::getPoints()
{
	return points;
}

std::vector<glm::vec3> TriMesh::getColors()
{
	return colors;
}

std::vector<glm::vec3> TriMesh::getNormals()
{
	return normals;
}

std::vector<glm::vec2> TriMesh::getTextures()
{
	return textures;
}

void TriMesh::computeTriangleNormals()
{
	face_normals.resize(faces.size());
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto& face = faces[i];
		glm::vec3 v01 = vertex_positions[face.y] - vertex_positions[face.x];
		glm::vec3 v02 = vertex_positions[face.z] - vertex_positions[face.x];
		face_normals[i] = normalize(cross(v01, v02));
	}
}

void TriMesh::computeVertexNormals()
{
	// 计算面片的法向量
	if (face_normals.size() == 0 && faces.size() > 0)
	{
		computeTriangleNormals();
	}
	// 初始化法向量为0
	vertex_normals.resize(vertex_positions.size(), glm::vec3(0, 0, 0));
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto& face = faces[i];
		vertex_normals[face.x] += face_normals[i];
		vertex_normals[face.y] += face_normals[i];
		vertex_normals[face.z] += face_normals[i];
	}
	for (size_t i = 0; i < vertex_normals.size(); i++)
	{
		vertex_normals[i] = normalize(vertex_normals[i]);
	}
	// 球心在原点的球法向量为坐标
	// for (int i = 0; i < vertex_positions.size(); i++)
	// 	vertex_normals.push_back(vertex_positions[i] - vec3(0.0, 0.0, 0.0));
}

void TriMesh::setNormalize(bool do_norm) { do_normalize_size = do_norm; }
bool TriMesh::getNormalize() { return do_normalize_size; }
float TriMesh::getDiagonalLength() { return diagonal_length; }

glm::vec3 TriMesh::getTranslation()
{
	return translation;
}

glm::vec3 TriMesh::getRotation()
{
	return rotation;
}

glm::vec3 TriMesh::getScale()
{
	return scale;
}

glm::mat4 TriMesh::getModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 trans = getTranslation();
	model = glm::translate(model, getTranslation());
	model = glm::rotate(model, glm::radians(getRotation()[2]), glm::vec3(0.0, 0.0, 1.0));
	model = glm::rotate(model, glm::radians(getRotation()[1]), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(getRotation()[0]), glm::vec3(1.0, 0.0, 0.0));
	model = glm::scale(model, getScale());
	return model;
}

void TriMesh::setTranslation(glm::vec3 translation)
{
	this->translation = translation;
}

void TriMesh::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void TriMesh::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

glm::vec4 TriMesh::getAmbient() { return ambient; };
glm::vec4 TriMesh::getDiffuse() { return diffuse; };
glm::vec4 TriMesh::getSpecular() { return specular; };
float TriMesh::getShininess() { return shininess; };

void TriMesh::setAmbient(glm::vec4 _ambient) { ambient = _ambient; };
void TriMesh::setDiffuse(glm::vec4 _diffuse) { diffuse = _diffuse; };
void TriMesh::setSpecular(glm::vec4 _specular) { specular = _specular; };
void TriMesh::setShininess(float _shininess) { shininess = _shininess; };

void TriMesh::cleanData()
{
	vertex_positions.clear();
	vertex_colors.clear();
	vertex_normals.clear();
	vertex_textures.clear();

	faces.clear();
	normal_index.clear();
	color_index.clear();
	texture_index.clear();

	face_normals.clear();


	points.clear();
	colors.clear();
	normals.clear();
	textures.clear();
}

void TriMesh::storeFacesPoints()
{
	// 看是否归一化物体大小，是的话，这里将物体顶点缩放到对角线长度为1的包围盒内
	if (do_normalize_size) {
		// 记录物体包围盒大小，可以用于大小的归一化
		// 先获得包围盒的对角顶点
		float max_x = -FLT_MAX;
		float max_y = -FLT_MAX;
		float max_z = -FLT_MAX;
		float min_x = FLT_MAX;
		float min_y = FLT_MAX;
		float min_z = FLT_MAX;
		for (int i = 0; i < vertex_positions.size(); i++) {
			auto& position = vertex_positions[i];
			if (position.x > max_x) max_x = position.x;
			if (position.y > max_y) max_y = position.y;
			if (position.z > max_z) max_z = position.z;
			if (position.x < min_x) min_x = position.x;
			if (position.y < min_y) min_y = position.y;
			if (position.z < min_z) min_z = position.z;
		}
		up_corner = glm::vec3(max_x, max_y, max_z);
		down_corner = glm::vec3(min_x, min_y, min_z);
		center = glm::vec3((min_x + max_x) / 2.0, (min_y + max_y) / 2.0, (min_z + max_z) / 2.0);

		diagonal_length = length(up_corner - down_corner);

		for (int i = 0; i < vertex_positions.size(); i++) {
			vertex_positions[i] = (vertex_positions[i] - center) / diagonal_length;
		}
	}

	// 计算法向量
	if (vertex_normals.size() == 0)
		computeVertexNormals();

	// 根据每个三角面片的顶点下标存储要传入GPU的数据
	for (int i = 0; i < faces.size(); i++)
	{
		// 坐标
		points.push_back(vertex_positions[faces[i].x]);
		points.push_back(vertex_positions[faces[i].y]);
		points.push_back(vertex_positions[faces[i].z]);
		// 颜色
		colors.push_back(vertex_colors[color_index[i].x]);
		colors.push_back(vertex_colors[color_index[i].y]);
		colors.push_back(vertex_colors[color_index[i].z]);
		// 法向量
		if (vertex_normals.size() != 0)
		{
			normals.push_back(vertex_normals[normal_index[i].x]);
			normals.push_back(vertex_normals[normal_index[i].y]);
			normals.push_back(vertex_normals[normal_index[i].z]);
		}
		// 纹理
		if (vertex_textures.size() != 0)
		{
			textures.push_back(vertex_textures[texture_index[i].x]);
			textures.push_back(vertex_textures[texture_index[i].y]);
			textures.push_back(vertex_textures[texture_index[i].z]);
		}
	}
}


void binomial_Coefficient(int* C, int n)
{
	int k, j;

	for (k = 0; k <= n; k++)
	{
		C[k] = 1;
		for (j = n; j >= k + 1; j--)
		{
			C[k] *= j;
		}
		for (j = n - k; j >= 2; j--)
		{
			C[k] /= j;
		}
	}
}


// 获得贝塞尔曲线上的点
void generateBezierPoints(glm::vec2 vertices[], glm::vec3 colors[], int startVertexIndex, int numPoints,
	glm::vec2 controlPoints[], int numCtrlPts, glm::vec3 color)
{
	int vertexIndex = startVertexIndex;
	int* C = new int[numCtrlPts];
	binomial_Coefficient(C, numCtrlPts - 1);
	for (int vertexIndex = 0; vertexIndex < numPoints; vertexIndex++)
	{
		vertices[vertexIndex] = glm::vec2(0, 0);
		// 得出t
		double t = double(vertexIndex) / ((double)numPoints - 1);

		// 求和公式实现
		for (int j = 0; j < numCtrlPts; j++)
		{
			vertices[vertexIndex].x += C[j] * pow((1 - t), numCtrlPts - j - 1) * pow(t, j) * controlPoints[j].x;
			vertices[vertexIndex].y += C[j] * pow((1 - t), numCtrlPts - j - 1) * pow(t, j) * controlPoints[j].y;
		}
		colors[vertexIndex] = color;
	}
}


void TriMesh::generateSquare(glm::vec3 color)
{
	// 创建顶点前要先把那些vector清空
	cleanData();

	for (int i = 0; i < 4; i++)
	{
		vertex_positions.push_back(square_vertices[i]);
		vertex_colors.push_back(color);
	}

	// 每个三角面片的顶点下标
	faces.push_back(vec3i(0, 1, 2));
	faces.push_back(vec3i(0, 2, 3));

	// 顶点纹理坐标
	vertex_textures.push_back(glm::vec2(0, 0));
	vertex_textures.push_back(glm::vec2(1, 0));
	vertex_textures.push_back(glm::vec2(1, 1));
	vertex_textures.push_back(glm::vec2(0, 1));

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
}

void TriMesh::readOff(const std::string& filename)
{
	// fin打开文件读取文件信息
	if (filename.empty())
	{
		return;
	}
	std::ifstream fin;
	fin.open(filename);
	if (!fin)
	{
		printf("File on error\n");
		return;
	}
	else
	{
		printf("File open success\n");

		cleanData();

		int nVertices, nFaces, nEdges;

		// 读取OFF字符串
		std::string str;
		fin >> str;
		// 读取文件中顶点数、面片数、边数
		fin >> nVertices >> nFaces >> nEdges;
		// 根据顶点数，循环读取每个顶点坐标
		for (int i = 0; i < nVertices; i++)
		{
			glm::vec3 tmp_node;
			fin >> tmp_node.x >> tmp_node.y >> tmp_node.z;
			vertex_positions.push_back(tmp_node);
			vertex_colors.push_back(tmp_node);
		}
		// 根据面片数，循环读取每个面片信息，并用构建的vec3i结构体保存
		for (int i = 0; i < nFaces; i++)
		{
			int num, a, b, c;
			// num记录此面片由几个顶点构成，a、b、c为构成该面片顶点序号
			fin >> num >> a >> b >> c;
			faces.push_back(vec3i(a, b, c));
		}
	}
	fin.close();

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
};

// Light
glm::mat4 Light::getShadowProjectionMatrix()
{
	// 这里只实现了Y=0平面上的阴影投影矩阵，其他情况自己补充
	float lx, ly, lz;

	glm::mat4 modelMatrix = this->getModelMatrix();
	glm::vec4 light_position = modelMatrix * glm::vec4(this->translation, 1.0);

	lx = light_position[0];
	ly = light_position[1];
	lz = light_position[2];

	return glm::mat4(
		-ly, 0.0, 0.0, 0.0,
		lx, 0.0, lz, 1.0,
		0.0, 0.0, -ly, 0.0,
		0.0, 0.0, 0.0, -ly);
}

// 设置衰减系数的参数
void Light::setAttenuation(float _constant, float _linear, float _quadratic)
{
	constant = _constant;
	linear = _linear;
	quadratic = _quadratic;
}

float Light::getConstant() { return constant; };
float Light::getLinear() { return linear; };
float Light::getQuadratic() { return quadratic; };

void TriMesh::readObj(const std::string& filename)
{
	std::ifstream fin(filename);
	std::string line;

	if (!fin)
	{
		std::cout << "ERROR: cannot open the file: " << filename << std::endl;
		exit(0);	// 退出程序
	}

	cleanData();

	while (std::getline(fin, line))
	{
		std::istringstream sin(line);
		std::string type;
		GLfloat _x, _y, _z;
		int a0, b0, c0;
		int a1, b1, c1;
		int a2, b2, c2;
		char slash;

		sin >> type;

		// @TODO: Task2 读取obj文件，记录里面的这些数据，可以参考readOff的写法
		// vertex_positions
		if (type == "v")
		{
			sin >> _x >> _y >> _z;
			vertex_positions.push_back(glm::vec3(_x, _y, _z));
		}
		// vertex_normals
		if (type == "vn")
		{
			sin >> _x >> _y >> _z;
			vertex_normals.push_back(glm::vec3(_x, _y, _z));
			vertex_colors.push_back(glm::vec3(_x, _y, _z));
		}
		// vertex_textures		
		if (type == "vt")
		{
			sin >> _x >> _y;
			vertex_textures.push_back(glm::vec2(_x, _y));
		}
		if (type == "f")
		{
			sin >> a0 >> slash >> b0 >> slash >> c0;
			sin >> a1 >> slash >> b1 >> slash >> c1;
			sin >> a2 >> slash >> b2 >> slash >> c2;
			// faces
			faces.push_back(vec3i(a0 - 1, a1 - 1, a2 - 1));
			// normal_index
			normal_index.push_back(vec3i(c0 - 1, c1 - 1, c2 - 1));
			color_index.push_back(vec3i(c0 - 1, c1 - 1, c2 - 1));
			// texture_index
			texture_index.push_back(vec3i(b0 - 1, b1 - 1, b2 - 1));
		}

		// 其中vertex_color和color_index可以用法向量的数值赋值
	}
	// std::cout << "read finished" << std::endl;

	storeFacesPoints();
}

void TriMesh::readObjMtl(const std::string& objFile, const std::string& mtlFile)
{
	std::ifstream fin(objFile);
	std::string line;

	if (!fin)
	{
		std::cout << "ERROR: cannot open the file: " << objFile << std::endl;
		exit(0);	// 退出程序
	}

	cleanData();

	while (std::getline(fin, line))
	{
		std::istringstream sin(line);
		std::string type;
		GLfloat _x, _y, _z;
		int a0, b0, c0;
		int a1, b1, c1;
		int a2, b2, c2;
		char slash;

		sin >> type;

		// @TODO: Task2 读取obj文件，记录里面的这些数据，可以参考readOff的写法
		// vertex_positions
		if (type == "v")
		{
			sin >> _x >> _y >> _z;
			vertex_positions.push_back(glm::vec3(_x, _y, _z));
		}
		// vertex_normals
		if (type == "vn")
		{
			sin >> _x >> _y >> _z;
			vertex_normals.push_back(glm::vec3(_x, _y, _z));
			vertex_colors.push_back(glm::vec3(_x, _y, _z));
		}
		// vertex_textures		
		if (type == "vt")
		{
			sin >> _x >> _y >> _z;
			vertex_textures.push_back(glm::vec3(_x, _y, _z));
		}
		if (type == "f")
		{
			sin >> a0 >> slash >> b0 >> slash >> c0;
			sin >> a1 >> slash >> b1 >> slash >> c1;
			sin >> a2 >> slash >> b2 >> slash >> c2;
			// faces
			faces.push_back(vec3i(a0, a1, a2));
			// normal_index
			normal_index.push_back(vec3i(c0, c1, c2));
			color_index.push_back(vec3i(c0, c1, c2));
			// texture_index
			texture_index.push_back(vec3i(b0, b1, b2));
		}
	}

	fin = std::ifstream(mtlFile);

	if (!fin)
	{
		std::cout << "ERROR: cannot open the file: " << mtlFile << std::endl;
		exit(0);	// 退出程序
	}


}

void TriMesh::movement(float speed)
{
	float theta = 0.005;
	setTranslation(glm::vec3(
		translation.x + speed * theta * -sin(glm::radians(rotation.y + 65)),
		translation.y,
		translation.z + speed * theta * -cos(glm::radians(rotation.y + 65))));
}

void TriMesh::shark_movement(float speed)
{
	float theta = 0.005;
	setTranslation(glm::vec3(
		translation.x + speed * theta * -sin(glm::radians(rotation.y - 180)),
		translation.y,
		translation.z + speed * theta * -cos(glm::radians(rotation.y - 180))));
}


void TriMesh::changeDir(float dir)
{
	float theta = 0.2;
	rotation.y += dir * theta;
}

void TriMesh::shake(float offset)
{
	rotation.x = offset * 60;
	rotation.z = offset * 30;
}

void Fluid::generateSurface(int n, glm::vec3 color, float _c, float _u, float _d)
{
	cleanData();
	numOfWidth = n;
	c = _c;
	u = _u;
	d = _d;

	float step = 1.0f / n;

	// 计算顶点位置
	for (float i = 0; i < n + 1; i++)
	{
		for (float j = 0; j < n + 1; j++)
		{
			vertex_positions.push_back(glm::vec3(-0.5f + step * j, 0, -0.5f + step * i));
			vertex_colors.push_back(color);
			vertex_textures.push_back(glm::vec2(step * j, step * i));
		}
	}

	d = step;

	vpb = vertex_positions;
	vpa = vertex_positions;

	int tmp = 0;

	// 随机化之前的坐标
	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < n; j++)
		{
			tmp = rand() % 2;
			vpb[i * (n + 1) + j].y = d * (tmp - 0.5f) * 0.5f;
		}
	}

	// 每个三角面片的顶点下标
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int a, b, c, d;
			a = i * (n + 1) + j;
			b = a + 1;
			c = b + n;
			d = c + 1;
			faces.push_back(vec3i(a, b, c));
			faces.push_back(vec3i(b, c, d));
		}
	}

	normal_index = faces;
	color_index = faces;
	texture_index = faces;

	storeFacesPoints();
}


void Fluid::updateSurfacePosition()
{
	// 顶点位置为最新的顶点
	vertex_positions = vpa;

	for (int i = 0; i < numOfWidth + 1; i++)
	{
		for (int j = 0; j < numOfWidth + 1; j++)
		{
			float curY = vertex_positions[i * (numOfWidth + 1) + j].y;
			float beforeY = vpb[i * (numOfWidth + 1) + j].y;
			float updateY = k1 * curY + k2 * beforeY;
			float deltaY = 0.0f;
			if (i > 0) deltaY += vertex_positions[(i - 1) * (numOfWidth + 1) + j].y;
			if (i < numOfWidth) deltaY += vertex_positions[(i + 1) * (numOfWidth + 1) + j].y;
			if (j > 0) deltaY += vertex_positions[i * (numOfWidth + 1) + j - 1].y;
			if (j < numOfWidth) deltaY += vertex_positions[i * (numOfWidth + 1) + j + 1].y;
			updateY += deltaY * k3;
			vpa[i * (numOfWidth + 1) + j].y = updateY;
		}
	}
	// 之前的顶点位置变成当前顶点位置，当前更新为计算出后的顶点位置
	vpb = vertex_positions;
	vertex_positions = vpa;

	int j = 0;

	for (int i = 0; i < faces.size(); i++)
	{
		// 坐标
		points[j++] = vertex_positions[faces[i].x];
		points[j++] = vertex_positions[faces[i].y];
		points[j++] = vertex_positions[faces[i].z];
	}
}

void Fluid::coefficientCount(float t)
{
	float f1 = c * c * t * t / d / d;
	float f2 = 1.0f / (u * t + 2);
	k1 = (4 - 8 * f1) * f2;
	k2 = (u * t - 2) * f2;
	k3 = 2 * f1 * f2;
}

float Fluid::getOffset(glm::vec3 pos)
{
	float x = pos.x;
	float z = pos.z;
	int i = (x + 0.5) * (numOfWidth + 1.0);
	int j = (z + 0.5) * (numOfWidth + 1.0);
	if (i < numOfWidth + 1 && j < numOfWidth + 1 && i >= 0 && j >= 0)
	{
		float ay = vertex_positions[i * (numOfWidth + 1) + j].y;
		float by = vertex_positions[i * (numOfWidth + 1) + j + 1].y;
		float cy = vertex_positions[(i + 1) * (numOfWidth + 1) + j].y;
		float dy = vertex_positions[(i + 1) * (numOfWidth + 1) + j + 1].y;
		return (ay + by + cy + dy) / 4;
	}
	return 0.0f;
}

void Fluid::cleanData()
{
	vpa.clear();
	vpb.clear();
	vertex_positions.clear();
	vertex_colors.clear();
	vertex_normals.clear();
	vertex_textures.clear();

	faces.clear();
	normal_index.clear();
	color_index.clear();
	texture_index.clear();

	face_normals.clear();


	points.clear();
	colors.clear();
	normals.clear();
	textures.clear();
}
