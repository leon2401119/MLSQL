import mysql.connector
import numpy as np
import cv2
from matplotlib import pyplot as plt
import time


def resize(img,s_w,s_h):
    h,w,c=img.shape
    return cv2.resize(img,(int(w*s_w),int(h*s_h)))

'''
def random_crop(img,s_w,s_h,r,g,b):
    h, w, ch = img.shape
    w_c=int(w*s_w)
    h_c=int(h*s_h)
    x=np.random.randint(w-w_c)
    y=np.random.randint(h-h_c)
    for i in range(h_c):
        for j in range(w_c):
            img[y+i,x+j,:]=np.array([r,g,b])
    return img
'''
def random_crop(img,s_w,s_h,r,g,b):
    h, w, ch = img.shape
    w_c=int(w*s_w)
    h_c=int(h*s_h)
    x=np.random.randint(w-w_c)
    y=np.random.randint(h-h_c)
    points=np.array([[x,y],[x+w_c,y],[x+w_c,y+h_c],[x,y+h_c]])
    cv2.fillPoly(img, [points], (r,g,b))
    return img

def color_to_gray(img):
    return cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

def gaussian_blur(img,k1,k2,sx,sy):
    #sigma = 3.0
    #radius = int(3*sigma)
    #ksize = 2*radius + 1
    kk1 = int(k1*img.shape[0])
    kk2 = int(k2*img.shape[1])
    kk1 = kk1+1 if not kk1%2 else kk1
    kk2 = kk2+1 if not kk2%2 else kk2
    out = cv2.GaussianBlur(src=img,ksize=(int(kk1), int(kk2)),sigmaX=sx,sigmaY=sy)
    return out

def median_blur(img):
    out = cv2.medianBlur(img, 7)
    return out

def rotation(img,angle):
    R=cv2.getRotationMatrix2D((img.shape[1]*0.5,img.shape[0]*0.5),angle,1)
    out=cv2.warpAffine(img, R, (img.shape[1],img.shape[0]))
    #out=cv2.rotate(img, cv2.ROTATE_90_CLOCKWISE)
    return out

def vertical_flip(img):
    return cv2.flip(img, 0)

def horizontal_flip(img):
    return cv2.flip(img, 1)

def translation(img):
    height, width = img.shape[:2] 
    quarter_height, quarter_width = height / 4, width / 4
    T=np.float32([[1, 0, quarter_width], [0, 1, quarter_height]])
    out=cv2.warpAffine(img, T, (width, height))
    return out

def gaussian_noise(img):
    gaussian_noise = np.zeros((img.shape[0], img.shape[1]),dtype=np.uint8)
    #cv2.randn(gaussian_noise, 0, 50)
    out=np.zeros(img.shape,dtype=np.uint8)
    for i in range(img.shape[2]):
        cv2.randn(gaussian_noise, 0, 50)
        out[:,:,i]=img[:,:,i]+gaussian_noise
    return out

def channel_shuffle(img):
    n=np.random.randint(float(10e8))
    m=n%6
    out=np.zeros(img.shape,dtype=np.uint8)
    if m==0:
        out[:,:,0]=img[:,:,0]
        out[:,:,1]=img[:,:,1]
        out[:,:,2]=img[:,:,2]
    elif m==1:
        out[:,:,0]=img[:,:,0]
        out[:,:,1]=img[:,:,2]
        out[:,:,2]=img[:,:,1]
    elif m==2:
        out[:,:,0]=img[:,:,1]
        out[:,:,1]=img[:,:,0]
        out[:,:,2]=img[:,:,2]
    elif m==3:
        out[:,:,0]=img[:,:,1]
        out[:,:,1]=img[:,:,2]
        out[:,:,2]=img[:,:,0]
    elif m==4:
        out[:,:,0]=img[:,:,2]
        out[:,:,1]=img[:,:,0]
        out[:,:,2]=img[:,:,1]
    else:
        out[:,:,0]=img[:,:,2]
        out[:,:,1]=img[:,:,1]
        out[:,:,2]=img[:,:,0]
    return out



if __name__ == '__main__':
    mydb = mysql.connector.connect(host="127.0.0.1", user="yangck", passwd='PASSWORDHERE', raw=True)
    mycursor = mydb.cursor()
    mycursor.execute("use aug")

    start = time.time()
    mycursor.execute("select * from img")
    buf = None
    for i in mycursor:
        buf = i[1]
        #print(type(buf))
        nparr = np.asarray(buf, np.uint8)
        img = cv2.imdecode(nparr,cv2.IMREAD_COLOR)
        img = color_to_gray(img)
        #img = gaussian_noise(img)
        #img = rotation(img,-30)
        #img = vertical_flip(img)
        #img = rotation(img,-20)
        #img = channel_shuffle(img)
        #img = horizontal_flip(img)
        #img = resize(img,0.5,1.2)
        #img = random_crop(img,0.2,0.2,255,255,255)
        #img = gaussian_blur(img,0.1,0.1,1,1)
        cv2.imencode('.jpg',img)
        #cv2.imshow('decoded',img)
        #cv2.waitKey(0)

    end = time.time()

    print(f'{end - start}')


    start = time.time()
    mycursor.execute("select id,aug(image) from img")
    buf = None
    for i in mycursor:
        buf = i[1]
        #print(type(buf))
        nparr = np.asarray(buf, np.uint8)
        img = cv2.imdecode(nparr,cv2.IMREAD_COLOR)
        #cv2.imshow('decoded',i0)
        #cv2.waitKey(0)

    end = time.time()

    print(f'{end - start}')
