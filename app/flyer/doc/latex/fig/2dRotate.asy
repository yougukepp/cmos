import graph;
import geometry;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");

/***************************************** 定义 *****************************************/
/* 尺寸 */ 
real picSize=150;
size(picSize);
/* 原点 */ 
pair O=(0,0);
/* 半径 */
real r=picSize/10;
/* 直角符号尺寸 */
real perpendicularSize = picSize/40;
/* 旋转角 */
real theta=10;
real thetaCos=cos(theta/180*pi);
real thetaSin=sin(theta/180*pi);
transform thetaRotate=rotate(theta, O);
pen  arcColor=red;
/* 不变矢量 */
real pLength=r;
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
pair X2=thetaRotate * X1;
pair Y2=thetaRotate * Y1;
pair PInX2=(P.x*thetaCos+P.y*thetaSin) * dir(theta);
pair PInY2=(-P.x*thetaSin+P.y*thetaCos) * dir(90+theta);

/***************************************** 绘图 *****************************************/
/* 原坐标轴 */
draw(Label("$x$", EndPoint), O--X1, X1OY1Color, Arrow);
draw(Label("$y$", EndPoint), O--Y1, X1OY1Color, Arrow);
/* 投影*/
draw(P--PInX1, X1OY1Color+dashed);
draw(P--PInY1, X1OY1Color+dashed);
//perpendicular(PInX1, SE, PInX1--P, perpendicularSize, X1OY1Color);
draw(Label("$x\mathbf{i}$", position=EndPoint, align=SE), O--PInX1, X1OY1Color, Arrow);
//perpendicular(PInY1, NE, PInY1--P, perpendicularSize, X1OY1Color);
draw(Label("$y\mathbf{j}$", position=EndPoint, align=NW), O--PInY1, X1OY1Color, Arrow);

/* 旋转后的坐标轴 */
draw(Label("$x'$", EndPoint), O--X2, X2OY2Color, Arrow);
draw(Label("$y'$", EndPoint), O--Y2, X2OY2Color, Arrow);
/* 投影*/
draw(P--PInX2, X2OY2Color+dashed);
draw(P--PInY2, X2OY2Color+dashed);
//perpendicular(PInX2, SE, PInX2--P, perpendicularSize, X2OY2Color);
draw(Label("$x'\mathbf{i'}$", position=EndPoint, align=SE), O--PInX2, X2OY2Color, Arrow);
//perpendicular(PInY2, NE, PInY2--P, perpendicularSize, X2OY2Color);
draw(Label("$y'\mathbf{j'}$", position=EndPoint, align=NW), O--PInY2, X2OY2Color, Arrow);

/* 不变向量 */
draw(Label("$\mathbf{p}$", EndPoint), O--P, pColor, Arrow);
/* 角度弧线 */
draw(Label("$\theta$"), arc(O, r/2, 0, theta), arcColor, Arrow);

