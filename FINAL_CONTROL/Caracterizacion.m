clc;

load('C:\Users\samve\Downloads\datos.txt')

[r,c] = size(datos);

t = datos(:,1);
y = datos(:,2);

% miller 

%           Ke^(-theta s)
%   G(s) = ------------------
%           tau s + 1

K = 20.68;
tao = 1170;%1170;
theta = 60;

% model
Gzn = tf([K],[tao 1],'InputDelay',theta)

%step function ---> response
[Yzn, t] = step(Gzn, t) 


% model validation
%
%                           R^2 < 0.3           VERY BAD
%               VNE         0.3 < R^2 < 0.4     BAD
%   R^2 := 1 - -------      0.4 < R^2 < 0.5     TOLERABLE
%               VT          0.5 < R^2 < 0.85    GOOD
%                          0.85 < R^2           SUSPECT
%

Yavg = sum(y)/r;            %valor promedio
VT = sum((y - Yavg).^2);    %variance total

%quitar + 100 para normal
VNE = sum((y - (Yzn+25.5)).^2);    %varianza no explicada
R2 = 1 - VNE/VT;
fprintf('\n Coeficiente de determinacion R^2 = %.4f \n', R2);


%%%%%%% window config 

fig1 = figure('Name','Identificacion de procesos de primer orden');
hold on
title(sprintf('Approximation of the response of a superior order system through a first order model with the Miller method'));
%title(sprintf('Open loop process - Graph obtained with experimental data'));
xlabel('Time (seconds)');ylabel('Temperature (°C)');
%title(sprintf('Ziegler y Nichols'));
%xlabel(' Tiempo s ', 'fontsize', 16,'k');
%ylabel(' Amplitud ', 'fontsize', 16,'b');

ejex = 0 : 16; dmx = ejex.*0;
ejey = 0 : 6;  dmy = ejey.*0;

%plot(ejex, dmx, 'k', 'linewidth', 2, 'DisplayName', 'x axis');

%plot(dmy, ejey, 'k', 'linewidth', 2, 'DisplayName', 'y axis');


plot(t, y,'-','color','b')
grid minor
hold on

%quitar + 100 para normal
plot(t, Yzn+25.5,'-','color','r')

legend('Experimental graph','Mathematical model');

%legend('show', 'Location', 'northeast');

%text(10, 4, 'El modelo tiene un coeficiende de determinacion', 'Color', 'b', 'Fontsize', 16);

text(110 , 10, sprintf('R^2 = %.4f', R2), 'Color', 'b', 'Fontsize', 16)





