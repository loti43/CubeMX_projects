# coding=utf-8
import serial
import time
import sys
import win32ui
import linecache


def select_file():
    dlg=win32ui.CreateFileDialog(1)
    dlg.SetOFNInitialDir('./')# 设置打开文件对话框中的初始显示目录
    dlg.DoModal()

    filename= dlg.GetPathName()# 获取选择的文件名称
    if filename =='':
        print("文件选择错误，程序退出")
        sys.exit()
    print(filename)
    return filename

def start():
    time_count = 0
    while time_count < 10 :
        revice = ser.read()
        # print(revice)
        # print(type(revice))
        if revice ==b's':
            print("握手成功，开始烧录")
            break
        else:
            time_count =time_count +1
            time.sleep(0.1)
    else:
        print("握手失败，请重新烧录，程序已退出")
        sys.exit()

# data_type: Hex,string
def send_data(filename,data_type = 'hex',):
    file = open(filename,'r',encoding='UTF-8')
    hex_line = len (file.readlines())
    print('数据大小:'+str(hex_line))
    send_count = 2
    error_count = 0
    while send_count <int(hex_line):

    # for line in file.readlines():
        print(send_count)
        line = linecache.getline(filename,send_count)
        # print (type(line))
        # print (len(line))
        if data_type == 'string':
            line = str.encode(line)

        elif data_type == 'hex':
            line = bytes.fromhex(line[1:-1])
        else  :
            line = bytes.fromhex(line[1:-1])
        ser.write (line)
        i = 0
        while i < 5 :
            revice = ser.read()
            print(revice)
            # print(type(revice))
            if revice ==b't':
                print("发送成功:"+ str(send_count)+'/'+str(hex_line))
                send_count = send_count +1
                error_count = 0
                time.sleep(0)
                break
            elif revice ==b'f':

                print("传输失败，重新发送")
                error_count = error_count+1
                break
            else:
                i =i +1
                time.sleep(0.1)
        else:
            print("等待超时")
            error_count = error_count+1
        if error_count >5:
            print('多次尝试失败，退出程序')
            file.close()
            sys.exit()

    file.close()

def endup():
    ser.write (b'E')
    time_count =0
    while time_count < 5:
        if ser.read()== b'e':
            print("烧录成功: 100%")
            break
        else:
            time_count =time_count +1
            time.sleep(0.1)
            pass
    else:
        print("烧录完成确认失败，请重新烧录，程序退出...")
        sys.exit()
    if ser.isOpen():
        ser.close()
    pass

if __name__ == "__main__":

    #修改串口号
    ser = serial.Serial('COM8',115200,timeout=0.5)

    # 选择烧录文件，两种方式
    file=select_file()
    # file = 'test.hex'

    # 请求更新程序
    start()

    # 发送Hex文件，通过字符串或16进制数发送
    send_data(file,'hex')
    # send_data(file,'string')

    # 发送传输结束标志
    endup()

# ser.flush()
# data = ser.readline(0,'\r\n')
