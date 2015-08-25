import graph3;

settings.tex = "xelatex";
usepackage("xeCJK");
texpreamble("\setCJKmainfont{AR PL UKai CN}");

size(8cm, 0);
currentprojection=orthographic((0.05, 0.05, 1), up=Y, target=O);

xlimits(0, 100);
ylimits(0, 100);
zlimits(0, 100);

xaxis3("$x$");
yaxis3("$y$");
zaxis3("$z$");

