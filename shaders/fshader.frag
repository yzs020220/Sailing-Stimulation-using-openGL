#version 330 core

// 给光源数据一个结构体
struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	vec3 position;
    // 光源衰减系数的三个参数
    float constant; // 常数项
    float linear;	// 一次项
    float quadratic;// 二次项

};

// 给物体材质数据一个结构体
struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;

	float shininess;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;
in vec3 color;

// 相机坐标
uniform vec3 eye_position;
// 光源
uniform Light light;
// 物体材质
uniform Material material;

uniform int type;
// 纹理数据
uniform sampler2D texture1;


out vec4 fColor;

void main()
{
	// 只使用贴图
	if (type == 1) {
		fColor = texture(texture1, texCoord);
		fColor.a = 1;
	}
	// 水
	else if(type == 2){
		fColor = texture(texture1, texCoord);
		fColor.a = 0.7;
	}
	// 贴图+Phong光照
	else if(type == 3){
		// 将顶点坐标、光源坐标和法向量转换到相机坐标系
		vec3 pos = position - eye_position;
		vec3 norm = (vec4(normal, 0.0)).xyz;
		vec3 l_pos = light.position - eye_position;

		vec3 N = normalize(norm);
		vec3 V = normalize(-pos);
		vec3 L = normalize(l_pos - pos);
		vec3 R = reflect(-L, N);

		// 环境光分量I_a
		vec4 I_a = light.ambient * material.ambient;

		// @TODO: Task2 计算系数和漫反射分量I_d
		float diffuse_dot = max(dot(L, N), 0);
		vec4 I_d = diffuse_dot *  light.diffuse * material.diffuse;

		// @TODO: Task2 计算系数和镜面反射分量I_s
		float specular_dot_pow = pow(max(dot(R, V), 0), material.shininess);
		vec4 I_s = specular_dot_pow * light.specular * material.specular;

		vec4 I = I_a + I_d + I_s;

		if( dot(L, N) < 0.0 ) {
		    I_s = vec4(0.0, 0.0, 0.0, 1.0);
		}

		fColor = texture(texture1, texCoord) * 0.7 + I * 0.3;
		fColor.a = 1;
	}
	// 地面
	else if (type == 4) {
		fColor = vec4(color, 1.0);
	}
	// 阴影
	else if (type == 5) {
		fColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else if (type == 0) {
		// 将顶点坐标、光源坐标和法向量转换到相机坐标系
		vec3 pos = position - eye_position;
		vec3 norm = (vec4(normal, 0.0)).xyz;
		vec3 l_pos = light.position - eye_position;

		vec3 N = normalize(norm);
		vec3 V = normalize(-pos);
		vec3 L = normalize(l_pos - pos);
		vec3 R = reflect(-L, N);

		// 环境光分量I_a
		vec4 I_a = light.ambient * material.ambient;

		// @TODO: Task2 计算系数和漫反射分量I_d
		float diffuse_dot = max(dot(L, N), 0);
		vec4 I_d = diffuse_dot *  light.diffuse * material.diffuse;

		// @TODO: Task2 计算系数和镜面反射分量I_s
		float specular_dot_pow = pow(max(dot(R, V), 0), material.shininess);
		vec4 I_s = specular_dot_pow * light.specular * material.specular;

		vec4 I = I_a + I_d + I_s;

		if( dot(L, N) < 0.0 ) {
		    I_s = vec4(0.0, 0.0, 0.0, 1.0);
		} 

		fColor =  I;
		// fColor.a = 1;
	}
}
