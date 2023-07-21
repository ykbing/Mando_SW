clear all;
clear all;
clc;

t0=0; tf=2*pi % 적분할 구간
y0=0;         % 초기값 설정

[t,y]=ode23('DerivCos',[t0 tf],y0);