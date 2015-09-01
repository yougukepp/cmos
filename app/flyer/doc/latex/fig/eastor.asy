import graph3;
import geometry;
import x11colors;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");

/***************************************** 定义 *****************************************/
/* 尺寸 */ 
real picSize=150;
size(picSize);
/* 半径 */
real r=picSize/1.5;
real perpendicularSize = picSize/20;
/* 原点 */ 
triple O=(0, 0, 0);
triple X=(r, 0, 0);
triple Y=(0, r, 0);
triple Z=(0, 0, r);
/* 观察点 */
triple lookPoint=(15, 30, 15);
currentprojection = orthographic(lookPoint, up=-Z, target=O);
/* 坐标轴 */
pen  XYZColor=black;

/***************************************** 绘图 *****************************************/
/* 原坐标轴 */
draw(Label("$x$", EndPoint), O--X, XYZColor, Arrow3(DefaultHead2));
draw(Label("$y$", EndPoint), O--Y, XYZColor, Arrow3(DefaultHead2));
draw(Label("$z$", EndPoint), O--Z, XYZColor, Arrow3(DefaultHead2));

/* 不变向量 */

