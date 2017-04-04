clear all;
close all;
clc;

baseDir='E:\\输出';

load('E:\\输出\\highspeed_11\\log\\loss_package_distance.txt');

% 文件名
dirNames={'\\highspeed_11','\\highspeed_12','\\highspeed_31','\\highspeed_41'};

% 作图用的pattern
figPattern={'b-','r-','k-','g-'};

dirNum=length(dirNames);

% 作图横坐标间隔数量
interval=20;

% 作图打点数量
pointNum=50;

% 保存作图信息
figData=zeros(2,pointNum,dirNum);

for iter=1:1:dirNum
    [figData(1,:,iter),figData(2,:,iter)]=pdr_calculate(strcat(baseDir,dirNames(iter)),interval,pointNum);
end



figure(1);
hold on;
for iter=1:1:dirNum
    plot(figData(1,:,iter),figData(2,:,iter),figPattern{iter},'LineWidth',2);
end
hold off;
legend()
grid on;


