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
triple lookPoint=(-r, r/5, -r/5);
currentprojection = orthographic(lookPoint, up=-Z, target=O);
/* 旋转角 */
real theta=10;
transform3 thetaRotate=rotate(theta, O, Z);
real thetaCos=cos(theta/180*pi);
real thetaSin=sin(theta/180*pi);
real theta1 = -theta;
real phi1 = 90;
real theta2 = 0;
real phi2 = 90;
/* 正东矢量 */

/* 原坐标 */
triple X1=(r,0,0);
triple Y1=(0,r,0);
pen  X1Y1Color=black;
/* 旋转后的坐标 */

/* 旋转轴 */
pen  ZColor=DarkCyan;
triple Z12=(0,0,r);

/***************************************** 绘图 *****************************************/
/* 原坐标轴 */
draw(Label("$x$", EndPoint), O--X1,  X1Y1Color, Arrow3(DefaultHead2));
draw(Label("$y$", EndPoint), O--Y1,  X1Y1Color, Arrow3(DefaultHead2));
//draw(Label("$z,z'$", EndPoint), O--Z12, ZColor, Arrow3(DefaultHead2));

/* 不变向量 */

/* 旋转后的坐标轴 */

/* 角度弧线 */

