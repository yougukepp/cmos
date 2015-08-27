import graph3;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");

triple O=(0, 0, 0);
triple X=(1, 0, 0);
triple Y=(0, 1, 0);
triple Z=(0, 0, 1);

/* x轴 */
real rho = 1;
real phi = 30;
real theta = 20;
real psi = 30;

real g0_x = 0;
real g0_y = 0;
real g0_z = rho;

size(200, 200);
defaultpen(linewidth(0.5));

currentprojection=orthographic((-100, 20, -10), up=-Z, target=O);

xlimits(-0.5, 1.5);
ylimits(-1, 1);
zlimits(-0.5, 1.5);
xaxis3(Label("$x$"), Arrow3(DefaultHead2));
yaxis3(Label("$y$"), Arrow3(DefaultHead2));
zaxis3(Label("$z$"), Arrow3(DefaultHead2));

path3 g0=(0,0,0)--(g0_x, g0_y, g0_z);
path3 g1 = rotate(phi, O, X) * g0;

draw(g0, red+1, Arrow3(DefaultHead2));
draw(g1, red+1, Arrow3(DefaultHead2));

real theta1 = 0;
real phi1 = 270;
real theta2 = phi;
real phi2 = 270;
draw("$\phi$", p=blue, arc(O, rho/2, theta1, phi1, theta2, phi2), Arrow3(DefaultHead2));


label("$\vec{g}$", Z, NE);

