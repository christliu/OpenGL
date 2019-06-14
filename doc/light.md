# Lighting

![图片](pic\basic_lighting_phong.png)

## 环境光（ambient）

~~~ cpp
void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    vec3 result = ambient * objectColor;
    color = vec4(result, 1.0f);
}
~~~

## 漫反射光

![漫反射](/pic/diffuse_light.png)

### 法向量

* 更新顶点数据，加上法向量信息。

* VS里面把normal传给PS

* PS里面normal与direction点乘。角度越大，值越小。

* VS里面传入片段的位置（顶点的世界坐标），model * position

  ~~~ cpp
  out vec3 FragPos;
  out vec3 Normal;
  
  void main()
  {
      gl_Position = projection * view * model * vec4(position, 1.0f);
      FragPos = vec3(model * vec4(position, 1.0f));
      Normal = normal;
  }
  ~~~

* 片段着色器计算反射的强度。

  ~~~ cpp
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  ~~~

* Normal计算错误。

  ~~~
  Normal = vec3(model *　vec4(normal, 0.0f)); // 解决不了不等比缩放的问题。
  
  // 正确的做法
  Normal = mat3(transpose(inverse(model))) * normal;
  ~~~

## 镜面光照

![镜面](pic/basic_lighting_specular_theory.png)

~~~ cpp
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor;
~~~



## 冯氏光照

~~~cpp
vec3 result = (ambient + diffuse + specular) * objectColor;
color = vec4(result, 1.0f);
~~~

