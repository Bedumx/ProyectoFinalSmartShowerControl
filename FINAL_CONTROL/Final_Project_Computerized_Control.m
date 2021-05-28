clear all
clc

N=300;
r=15.7*ones(N); %15.67345=35°C, 13.4344=30°C, 20.68=46.18°C

%IAE:
kp=47.29753112;
ki=0.04056413725;

%ITAE:
%kp=43.05452335
%ki=0.03759132612

T=120;
                
c(1)=0;
e(1)=r(1)-c(1);
m(1)=0.5*((2*kp+T*ki)*e(1));
if m(1)>100
    m(1)=100
else m(1)<100
    m(1)=m(1)
end

c(2)=0.9025*c(1)+0.01034*m(1);
e(2)=r(2)-c(2);
m(2)=0.5*(2*m(1)+(2*kp+T*ki)*e(2)+(T*ki-2*kp)*e(1));
if m(2)>100
    m(2)=100
else m(2)<100
    m(2)=m(2)
end

for k=3:1:N
    c(k)=0.9025*c(k-1)+0.01034*m(k-1)+0.009823*m(k-2);
    e(k)=r(k)-c(k);
    m(k)=0.5*(2*m(k-1)+(2*kp+T*ki)*e(k)+(T*ki-2*kp)*e(k-1));
    if m(k)>100
        m(k)=100
    else m(k)<100
        m(k)=m(k)
    end
end

T=120*(1:N);
subplot(3,1,1),plot(T,c,'r*');
title('PI Controller');
xlabel('k');ylabel('Process Output');grid;legend('c(k)');
subplot(3,1,2),plot(T,e,'b.');
xlabel('k');ylabel('Error');grid;legend('e(k)');
subplot(3,1,3),plot(T,m,'k-');
xlabel('k');ylabel('Controller Output');grid;legend('m(k)')