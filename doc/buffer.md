# Buffer

## 顶点缓冲对象(Vertex Buffer Objects, VBO)

* 存在于显存中。

* 一次性发送大批数据，从CPU发送数据到GPU是一个耗时的过程。

  ~~~ cpp
  GLuint VBO;
  glGenBuffers(1, &VBO);  // 在GPU中创建一块缓存，VBO是cpu中用来唯一标识其的tag.
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);  // 把新创建的VBO绑定到GL_ARRAY_BUFFER目标上。
  // 从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  ~~~

* OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。

## Vertex Shader

~~~ cpp
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 coord;
~~~

## 顶点链接属性

~~~ cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);
// 对应以上的VS中location=0的position在VBO中的位置。
~~~

## 顶点数组对象（Vertex Array Object, VAO）

* 绑定VAO之后，随后的顶点属性调用会存储在VAO中。

* 带来的好处，切换VBO之后，不用重新设置顶点属性。

* **核心模式**要求必须使用VAO。

  ~~~ cpp
  GLuint VAO;
  glGenVertexArrays(1, &VAO); 
  
  // 1. 绑定VAO
  glBindVertexArray(VAO);
      // 2. 把顶点数组复制到缓冲中供OpenGL使用
      glBindBuffer(GL_ARRAY_BUFFER, VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
      // 3. 设置顶点属性指针
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
      glEnableVertexAttribArray(0);
  //4. 解绑VAO
  glBindVertexArray(0);
  
  [...]
  glBindVertexArray(VAO);
  drawcall();
  glBindVertexArray(0); // 通常情况下当我们配置好OpenGL对象以后要解绑它们，这样我们才不会在其它地方错误地配置它们。
  ~~~

## 索引缓冲对象（EBO、IBO）

~~~
GLfloat vertices[] = {
    0.5f, 0.5f, 0.0f,   // 右上角
    0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f, // 左下角
    -0.5f, 0.5f, 0.0f   // 左上角
};

GLuint indices[] = { // 注意索引从0开始! 
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

// 1. 绑定顶点数组对象
glBindVertexArray(VAO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 复制我们的索引数组到一个索引缓冲中，供OpenGL使用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 3. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
// 4. 解绑VAO（不是EBO！）
glBindVertexArray(0);

[...]

// ..:: 绘制代码（游戏循环中） :: ..

glUseProgram(shaderProgram);
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0)
glBindVertexArray(0);
~~~

