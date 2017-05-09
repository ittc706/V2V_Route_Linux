clear all;
close all;
clc;

[data1] = textread('route_udp_delay.txt','%n');
data1=data1+1;
y=0:1:20;
[number,center]=hist(data1,y);
number=number./(sum(number));
bar(center,number);


