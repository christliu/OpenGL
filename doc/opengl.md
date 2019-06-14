# OpenGL简介

## 规范

OpenGL多被认为是一套图形渲染API，其实它仅仅是一个由[Khronos组织](http://www.khronos.org/)制定并维护的规范(Specification)，至于内部具体每个函数是如何实现(Implement)的，将由OpenGL库的开发者自行决定。

## 状态机

OpenGL的状态通常被称为OpenGL上下文(Context)

当使用OpenGL的时候，我们会遇到一些状态设置函数(State-changing Function)，这类函数将会改变上下文。以及状态应用函数(State-using Function)，这类函数会根据当前OpenGL的状态执行一些操作。

## 对象

在OpenGL中一个对象是指一些选项的集合，它代表OpenGL状态的一个子集。

~~~ cpp
// 创建对象
GLuint objectId = 0;
glGenObject(1, &objectId);
// 绑定对象至上下文
glBindObject(GL_WINDOW_TARGET, objectId);
// 设置当前绑定到 GL_WINDOW_TARGET 的对象的一些选项
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH, 800);
glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
// 将上下文对象设回默认
glBindObject(GL_WINDOW_TARGET, 0);
~~~

* 我们首先创建一个对象，然后用一个id保存它的引用（实际数据被储存在后台）。

* 我们将对象绑定至上下文的目标位置（例子中窗口对象目标的位置被定义成GL_WINDOW_TARGET）。

* 接下来我们设置窗口的选项。

* 最后我们将目标位置的对象id设回0，解绑这个对象。

设置的选项将被保存在objectId所引用的对象中，一旦我们重新绑定这个对象到GL_WINDOW_TARGET位置，这些选项就会重新生效。

> ** **基元类型(Primitive Type)**
>
> 使用OpenGL时，建议使用OpenGL定义的基元类型。比如使用`float`时我们加上前缀`GL`（因此写作`GLfloat`）。`int`、`uint`、`char`、`bool`等等也类似。OpenGL定义的这些GL基元类型的内存布局是与平台无关的，而int等基元类型在不同操作系统上可能有不同的内存布局。使用GL基元类型可以保证你的程序在不同的平台上工作一致。