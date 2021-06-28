# coding=UTF-8
import mysql.connector
import pandas as pd
import cv2
import numpy as np
import time

def prep(mycursor):
    mycursor.execute("create table img(id int, image longblob)")
    for _ in range(10):
        mycursor.execute("insert into img values(%s,%s)",(1,"load_file('/var/lib/mysql-files/S.png')"))


if __name__ == '__main__':
    mydb = mysql.connector.connect(host="127.0.0.1", user="yangck", passwd='PASSWORDHERE', raw=True, connect_timeout=10000)
    mycursor = mydb.cursor()
    mycursor.execute("set global max_allowed_packet=1073741824")
    mycursor.execute("use aug")
    
    # prep(mycursor)

    start = time.time()
    mycursor.execute("select * from img")
    buf = None
    for i in mycursor:
        buf = i[1]
        #print(type(buf))
        nparr = np.asarray(buf, np.uint8)
        i0 = cv2.imdecode(nparr,cv2.IMREAD_COLOR)
        flipped = cv2.flip(i0,0)
        fliiped = cv2.flip(flipped,0)
        #cv2.imshow('decoded',i0)
        #cv2.waitKey(0)

    end = time.time()

    print(f'{end - start}')
    # mycursor.execute("drop table img")
    
    
    # prep(mycursor)
    
    start = time.time()
    mycursor.execute("select id,hflip(image) from img")
    #mycursor.execute("select id,hflip(%s) from img",("image",))
    #mycursor.execute("select hflip(%s)",("load_file('/var/lib/mysql-files/S.png')",))
    buf = None
    for i in mycursor:
        buf = i[1]
        nparr = np.asarray(buf, np.uint8)
        i0 = cv2.imdecode(nparr,cv2.IMREAD_COLOR)
        #flipped = cv2.flip(buf,0)
        #cv2.imshow('decoded',i0)
        #cv2.waitKey(0)

    end = time.time()

    print(f'{end - start}')
    

