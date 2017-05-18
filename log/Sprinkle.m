[data1,data2] = textread('vue_coordinate.txt','%n%n');
figure(1);
plot(data1,data2,'x');
[data3,data4]=textread('last_location.txt','%n%n');
figure(2);
plot(data3,data4,'x');
