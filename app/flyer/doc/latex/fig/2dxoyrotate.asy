import graph;
import geometry;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");


size(200);

/***************************************** 定义 *****************************************/
/* 原点 */ 
pair O=(0,0);
/* 半径 */
real r=10;
/* 旋转角 */
real theta=10;
real thetaCos=cos(theta/180*pi);
real thetaSin=sin(theta/180*pi);
/* 不变矢量 */
real pLength=10;
real pAngle=45;
real pCos=cos(pAngle/180*pi);
real pSin=sin(pAngle/180*pi);
pair P=(pCos*pLength, pSin*pLength);
pen  pColor=blue;
/* 原坐标 */
pen  X1OY1Color=black;
pair X1=(r,0);
pair Y1=(0,r);
pair PInX1=(P.x,0);
pair PInY1=(0,P.y);
/* 旋转后的坐标 */
pen  X2OY2Color=red;
pair X2=rotate(theta, O) * X1;
pair Y2=rotate(theta, O) * Y1;
pair PInX2=(P.x*thetaCos+P.y*thetaSin) * dir(theta);
pair PInY2=(-P.x*thetaSin+P.y*thetaCos) * dir(90+theta);

/***************************************** 绘图 *****************************************/
/* 原坐标轴 */
draw(Label("$x$", EndPoint), O--X1, X1OY1Color, Arrow);
draw(Label("$y$", EndPoint), O--Y1, X1OY1Color, Arrow);
/* 投影*/
draw(P--PInX1, X1OY1Color+dashed);
draw(P--PInY1, X1OY1Color+dashed);
perpendicular(PInX1, NE, PInX1--P, X1OY1Color);
perpendicular(PInY1, NE, PInY1--P, X1OY1Color);

/* 旋转后的坐标轴 */
draw(Label("$x'$", EndPoint), O--X2, X2OY2Color, Arrow);
draw(Label("$y'$", EndPoint), O--Y2, X2OY2Color, Arrow);
/* 投影*/
draw(P--PInX2, X2OY2Color+dashed);
draw(P--PInY2, X2OY2Color+dashed);
perpendicular(PInX2, NE, PInX2--P, X2OY2Color);
perpendicular(PInY2, NE, PInY2--P, X2OY2Color);

/* 向量 */
draw(Label("$\mathbf{p}$", EndPoint), O--P, pColor, Arrow);

