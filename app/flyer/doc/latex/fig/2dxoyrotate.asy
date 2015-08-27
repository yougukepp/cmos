import graph;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");



size(200);

real theta=10;
pair O=(0,0);
pair X1=(10,0);
pair Y1=(0,10);

pair P=(8, 4);

/* 旋转后的坐标轴 */
pair X2=rotate(theta,O) * X1;
pair Y2=rotate(theta,O) * Y1;

/* 绘制坐标轴 */
draw(Label("$x$", EndPoint), O--X1, Arrow);
draw(Label("$y$", EndPoint), O--Y1, Arrow);

draw(Label("$x'$", EndPoint), O--X2, Arrow);
draw(Label("$y'$", EndPoint), O--Y2, Arrow);

/* 绘制向量 */
draw(Label("$\mathbf{p}$", EndPoint), O--P, red, Arrow);

