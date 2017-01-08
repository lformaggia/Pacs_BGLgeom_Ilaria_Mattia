% plot Fractures_intersection example

f1 = [0,2 ; 2,2];
f2 = [0,1 ; 3,4];
f3 = [2,0 ; 2,4];

f1_new = [0,2 ; 1,2 ; 2,2];
f2_new = [0,1 ; 1,2 ; 2,3 ; 3,4];
f3_new = [2,0 ; 2,2 ; 2,3 ; 2,4];
%%
figure
subplot(1,2,1)
axis equal
plot(f1(:,1),f1(:,2),'r-', f2(:,1),f2(:,2), 'b-', f3(:,1),f3(:,2),'g-')
title('Input')
subplot(1,2,2)
axis equal
plot(f1(:,1),f1(:,2),'r', f2(:,1),f2(:,2), 'b', f3(:,1),f3(:,2),'g')
hold on
plot(f1_new(:,1),f1_new(:,2),'.k','MarkerSize',20)
plot(f2_new(:,1),f2_new(:,2), '.k','MarkerSize',20)
plot(f3_new(:,1),f3_new(:,2),'.k','MarkerSize',20)
title('Output')
%%
print -depsc frac_inters