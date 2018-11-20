
% conversor jpg a dat

function im=imagenAd(path)
  sz=100;
  pkg load image;
  imrgb=imread(path);
  imggray=rgb2gray(imrgb);
  [h,w]=size(imggray);
  if((h/w)>=1)
    im=imresize(imggray,(sz/h));
    [h1,w1]=size(im);
    if(w!=100)
      im=[im zeros(100,uint16((100-w1)/2))];
      im=[zeros(100,((100-w1)-uint16((100-w1)/2))) im];
    endif
  endif
  
  if((h/w)<1)
    im=imresize(imggray,(sz/w));
    [h1,w1]=size(im);
    if(w!=100)
      im=[im;zeros(uint16((100-h1)/2),100)];
      im=[(100-h1)-zeros(uint16((100-h1)/2),100);im];
    endif
  endif
  im=imrotate(im,90);
  imshow(im)
  im=double(im)/255;
  im=im(:);
  im=[im; graythresh(imggray)];
  save('ImagenBin.dat','im','-ascii');
endfunction