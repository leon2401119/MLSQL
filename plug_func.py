# coding=UTF-8
import mysql.connector
import time
import getpass

def sql_load(name):
    while True:
        try:
            mycursor.execute("drop function if exists " + name)
            mycursor.execute("create function " + name + " returns string soname 'plugin.so'")
            print(f'{name} successfully loaded')
            break
        except Exception as e:
            connect()

def connect():
    t = 5
    while True:
        try:
            mydb = mysql.connector.connect(host="127.0.0.1", user="root", passwd=pw, connect_timeout = 1)
            mycursor = mydb.cursor()
            mycursor.execute("set global max_allowed_packet=1073741824")
            break
        except Exception as e:
            print(f'sleeping for {t} sec before retry')
            time.sleep(t)
            t*=2

if __name__ == '__main__':
    pw = getpass.getpass('enter password for mysql root : ')
    mydb = mysql.connector.connect(host="127.0.0.1", user="root", passwd=pw, connect_timeout = 1)
    mycursor = mydb.cursor()
    mycursor.execute("set global max_allowed_packet=1073741824")

    sql_load('hflip')
    sql_load('vflip')
    sql_load('chshuffle')
    sql_load('randcrop')
    sql_load('cvt2gray')
    sql_load('rotation')
    sql_load('resize')
    sql_load('noise')
    sql_load('medianblur')
    sql_load('gaussianblur')
    sql_load('aug')
