clear all;
close all;
clc;


% figId=1;
% 
% %% PRR
% PackageLossDistance=load('loss_package_distance.txt');
% PackageTransimitDistance=load('distance.txt');
% 
% %IntersectDistance=intersect(unique(PackageLossDistance),unique(PackageTransimitDistance));
% IntersectDistance=0:20:max(PackageLossDistance);
% 
% [numPackageLossDistance,centerPackageLossDistance]=hist(PackageLossDistance',IntersectDistance);
% [numPackageTransimitDistance,centerPackageTransimitDistance]=hist(PackageTransimitDistance',IntersectDistance);
% 
% numPackageLossDistance=numPackageLossDistance./numPackageTransimitDistance;
% 
% figure(figId)
% figId=figId+1;
% plot(centerPackageLossDistance,1-numPackageLossDistance,'bo-','LineWidth',2);
% title('PDR','LineWidth',2);
% xlabel('Distance(m)','LineWidth',2);
% ylabel('Drop Rate','LineWidth',2);
% axis([0 3000 0 1]);
% grid on;

[data1,data2] = textread('route_udp_link_pdr_distance.txt','%n%n','delimiter', ',');
IntersectDistance=0:20:max(data2);
[numPackageDistance,centerPackageDistance]=hist(data2',IntersectDistance);
success=find(data1==1);
[numPackageLossDistance,centerPackageLossDistance]=hist(data2(success)',IntersectDistance);
pdr_distance=numPackageLossDistance./numPackageDistance;

plot(IntersectDistance,pdr_distance,'bo-','LineWidth',2);
title('PRR','LineWidth',2);
xlabel('Distance(m)','LineWidth',2);
ylabel('Receive Rate','LineWidth',2);
grid on;