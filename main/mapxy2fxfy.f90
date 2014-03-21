   subroutine mapxy2fxfy(fx,fy,x,y,px,py,n,ni,nj)
   integer n,ni,nj
   real fx(n),fy(n),x(n),y(n)
   real px(ni,nj),py(ni,nj)
   integer i,i1,j1
   real x1,y1,x2,y2
   real frac_x1, frac_y1, frac_x2, frac_y2
   do i=1,n
      i1 = min(int(x(n)),ni-1)
      j1 = min(int(y(n)),nj-1)
      frac_x1 = x(n)-i1
      frac_y1 = y(n)-j1
      x1 = px(i1,j1)+frac_x1*(px(i1+1,j1)-px(i1,j1))
      y1 = py(i1,j1)+frac_x1*(py(i1+1,j1)-py(i1,j1))
      x2 = px(i1,j1+1)+frac_x1*(px(i1+1,j1+1)-px(i1,j1+1))
      y2 = py(i1,j1+1)+frac_x1*(py(i1+1,j1+1)-py(i1,j1+1))
      fx(n) = x1 + frac_y1 * (x2-x1)
      fy(n) = y1 + frac_y1 * (y2-y1)
   enddo
   return
   end subroutine mapxy2fxfy
