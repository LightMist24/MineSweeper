# MineSweeper
一个基于C++和Easyx的扫雷。

# DownSide
但是有一些功能没有完善，如，点击可能这就会闪一下。
还有些功能甚至都没有做，如，左右键同时点击雷块数字会将周围的雷块打开（雷已经被旗帜标记的情况下）。

还有游戏的ico没有特别美观，如果不介意，可以使用images里的MineSweeper（1）.ico来更替图标。

打开MineSweeper.sln可以编辑代码，里面的#define MINE_NUM 99 常量可以更改游戏雷的数量，
但是地图就没有做其他的了（所以地图不会更改）。

胜利与结束做成了弹窗式。可能不太习惯或不喜欢。可以在MineSweeper.sln里把bool类型与showJudge函数关系优化掉。

可能第一开始就点到雷。:(

# Game Screenshots
![MineSweeper](https://github.com/LightMist24/MineSweeper/assets/142841012/4ceab8cd-c3ae-40f7-b1d9-732ed94bfc07)
![Playing](https://github.com/LightMist24/MineSweeper/assets/142841012/6f22c9aa-0f97-4026-8937-b54e7e3b023a)
![Failing](https://github.com/LightMist24/MineSweeper/assets/142841012/7f7874d5-ea8e-4bd5-b764-bb67d89c436f)

# How it workds
效果是对雷块进行加密，密码是偏移数值20。
再贴上对应的图片，之后刷新图片，解码就是加上-20。

一些图片编号：
Mine（雷）：-1。
Number1~8（数字1~8）：1~8。
Flag（旗帜）：29~38。
......

# Other
游戏运行会显示cmd，上面的数字表示加密后的雷数值，可以直接对照找到雷在哪里。

Easyx的下载地址：https://easyx.cn/

游戏内的代码编辑非常简单，很容易看懂。
