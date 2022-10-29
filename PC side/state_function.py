import serial as ser
import numpy
import GUI_func
import pc_side
import PySimpleGUI as sg
import os
from tkinter import *
from tkinter import ttk
import time
from time import strftime



def init():
    global num_of_scripts_state4
    num_of_scripts_state4 = 0



def state1_func():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)  # timeout of 1 sec so that the read and write operations are blocking,
                    # after the timeout the program continues

    enableTX = True
    state1_flag = 1
    s.reset_input_buffer()
    s.reset_output_buffer()
    delta_phi = 0.707
    big_step = int(90/delta_phi)
    max_deg = 90
    pointer_deg = 0
    motor_direc = 0
    motor_command = 0
    ignor_amp = 1
    while(state1_flag):
        while (s.in_waiting > 0 and state1_flag):  # while the input buffer isn't empty


            line = s.read_until(expected='\n')  # read 1 char from the input buffer  # read  from the buffer until the terminator is received,
            # readline() can also be used if the terminator is '\n'
            print(line)
            x_axis = int.from_bytes(line[0:2], byteorder='big')
            y_axis = int.from_bytes(line[2:4], byteorder='big')
            pointer_deg = int.from_bytes(line[4:6], byteorder='big')
            print('motor:', pointer_deg)
            norm = [y_axis - 475, x_axis - 475]
            if(ignor_amp):
                amp = 20
                ignor_amp = 0
            else:
                amp = numpy.sqrt(pow(norm[0], 2) + pow(norm[1], 2))
            print('amp:  ',amp)
            if (amp >= 350):
                stick_deg = (numpy.arctan2(norm[0], norm[1])) * 180 / 3.14
                print('stick:', stick_deg)
                if (stick_deg < 0):
                    stick_deg = stick_deg + 360
                deg_diff = stick_deg - pointer_deg
                if ((deg_diff > 3) or (deg_diff < -3)):
                    steps = int(deg_diff / delta_phi)
                    if (steps > 120):
                        steps = 120
                    if (steps < -120):
                        steps = -120
                    if (pointer_deg < 180):
                        if ((deg_diff > 0) and (deg_diff < 180)):
                            motor_direc = steps
                        else:
                            if (pointer_deg < 90):
                                if ((deg_diff > 180) and (deg_diff < (360 - max_deg))):
                                    motor_direc = 120 + steps
                                else:
                                    if (deg_diff < 0):
                                        motor_direc = 120 - steps
                                    else:
                                        motor_direc = (((360 - deg_diff) / delta_phi) % 120) + 120
                            else:
                                if ((deg_diff < 0) and (deg_diff > - max_deg)):
                                    motor_direc = 120 - steps
                                else:
                                    motor_direc = 240

                    else:
                        if ((deg_diff > -180) and (deg_diff < 0)):
                            motor_direc = 120 - steps
                        else:
                            if (pointer_deg < 270):
                                if ((deg_diff > 0) and (deg_diff < 90)):
                                    motor_direc = steps
                                else:
                                    motor_direc = 120
                            else:
                                if ((deg_diff < -180) and (deg_diff > -270)):
                                    motor_direc = 120
                                else:
                                    if (deg_diff > 0):
                                        motor_direc = steps
                                    else:
                                        motor_direc = (((360 + deg_diff) / delta_phi) % 120)

                else:
                    motor_direc = 0
            else:
                motor_direc = 0

            motor_command = 1

            if (s.in_waiting == 0):
                enableTX = True
        while ((s.out_waiting > 0 or enableTX) and state1_flag):  # while the output buffer isn't empty
            if (motor_command == 0):
                inChar = '1'
                bytesChar = bytes(inChar, 'ascii')
                s.write(bytesChar)
                GUI_func.start_func()
                if s.out_waiting == 0 and '1' in inChar:
                    print('s')
                    inChar_start = 's'
                    bytesChar_start = bytes(inChar_start, 'ascii')
                    s.write(bytesChar_start)
                    GUI_func.stop_func()
                    if s.out_waiting == 0 :
                        print('t')
                        inChar_stop = 't'
                        bytesChar_stop = bytes(inChar_stop, 'ascii')
                        s.write(bytesChar_stop)
                        if (s.out_waiting == 0):
                            enableTX = False
                            layout = [
                                [sg.Text("Push to Stop state1")],
                                [sg.Button("Change State")]]

                            window = sg.Window("Demo", layout)

            else:
                event, value = window.Read(timeout=10)
                if event == "Change State" or event == sg.WIN_CLOSED:
                    state1_flag = 0
                    window.close()
                    temp = (255).to_bytes(1, byteorder='little')
                    # print('send:',motor_direc)
                    s.write(temp)
                    if (s.out_waiting == 0):
                        break

                motor_send = int(motor_direc).to_bytes(1, byteorder='little')
                # print('send:',motor_direc)
                s.write(motor_send)
                motor_command = 0
                if (s.out_waiting == 0):
                    enableTX = False


def state2_func():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)
    stick_deg = 0
    board_amp = 0
    x_axis = 450
    y_axis = 450
    enableTX = True
    state2_flag = 1
    s.reset_input_buffer()
    s.reset_output_buffer()
    ignor_amp = 1
    print_color = 0
    while (state2_flag):

        while ((s.out_waiting > 0 or enableTX) and state2_flag):  # while the output buffer isn't empty
            inChar = '2'
            bytesChar = bytes(inChar, 'ascii')
            s.write(bytesChar)
            if s.out_waiting == 0:
                enableTX = False


        x_curr = 250
        y_curr = 250
        x_next = 250
        y_next = 250


        root = Tk()


        exit_button = Button(root, text="Exit", command=root.destroy)
        exit_button.pack(pady=20)
        canvas = Canvas(root)
        canvas.pack()
        canvas.config(width=500, height=500 , background = 'white')
        canvas.create_line(x_curr, y_curr, x_next, y_next, fill='blue', width=5)
        root.update()
        new_xy = 1
        while(root._tclCommands != None):
            while (s.in_waiting > 0 and state2_flag):  # while the input buffer isn't empty

                line = s.read(size = 4)
                print(line)
                x_axis = int.from_bytes(line[0:2], byteorder='big')
                y_axis = int.from_bytes(line[2:4], byteorder='big')
                if((x_axis == 0xFFFF) or (y_axis == 0xFFFF)):
                    print_color = 1 - print_color
                #print('x_axis:', x_axis)
                #print('y_axis:', y_axis)

                norm = [y_axis - 475, x_axis - 475]
                if (ignor_amp):
                    amp = 20
                    ignor_amp = 0
                else:
                    amp = numpy.sqrt(pow(norm[0], 2) + pow(norm[1], 2))
                #print('amp:  ', amp)
                if (amp >= 350):
                    stick_deg = (numpy.arctan2(norm[0], norm[1])) * 180 / 3.14
                    if(stick_deg < 0):
                        stick_deg = 360 + stick_deg
                    board_amp = 10
                else:
                    stick_deg = 0
                    board_amp = 0

                new_xy = 0




            time.sleep(0.25)
            board_deg = (stick_deg + 90)%360
            print('x_axis:', x_axis)
            print('y_axis:', y_axis)
            print("stick_deg",stick_deg)
            print("board_deg",board_deg)
            x_next = int(x_next + board_amp * (numpy.cos(board_deg* (3.14 / 180))))
            y_next = int(y_next + board_amp * (numpy.sin(board_deg* (3.14 / 180))))
            print('x_next----------------------------------',x_next)
            print('y_next----------------------------------',y_next)
            if(print_color == 0):
                canvas.create_line(x_curr, y_curr, x_next, y_next, fill='blue', width=4)
            else:
                canvas.create_line(x_curr, y_curr, x_next, y_next, fill='white', width=10)
            root.update()
            x_curr = x_next
            y_curr = y_next
            if(root._tclCommands == None):
                enableTX = True
                while ((s.out_waiting > 0 or enableTX) and state2_flag):  # while the output buffer isn't empty
                    inChar = 'x'
                    bytesChar = bytes(inChar, 'ascii')
                    s.write(bytesChar)
                    if s.out_waiting == 0:
                        enableTX = False
                state2_flag = 0










def state3_func():
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)  # timeout of 1 sec so that the read and write operations are blocking,
    # after the timeout the program continues

    enableTX = True
    state3_flag = 1
    s.reset_input_buffer()
    s.reset_output_buffer()
    while (state3_flag):


        while ((s.out_waiting > 0 or enableTX) and state3_flag):  # while the output buffer isn't empty
            inChar = '3'
            bytesChar = bytes(inChar, 'ascii')
            s.write(bytesChar)
            GUI_func.start_func()
            if s.out_waiting == 0:
                inChar_start = 's'
                bytesChar_start = bytes(inChar_start, 'ascii')
                s.write(bytesChar_start)
                GUI_func.stop_func()
                if s.out_waiting == 0:
                    inChar_stop = 't'
                    bytesChar_stop = bytes(inChar_stop, 'ascii')
                    s.write(bytesChar_stop)
                    GUI_func.start_func()
                    if s.out_waiting == 0:
                        inChar_start = 's'
                        bytesChar_start = bytes(inChar_start, 'ascii')
                        s.write(bytesChar_start)
                        GUI_func.stop_func()
                        if s.out_waiting == 0:
                            inChar_stop = 't'
                            bytesChar_stop = bytes(inChar_stop, 'ascii')
                            s.write(bytesChar_stop)
                            if s.out_waiting == 0:
                                enableTX = False

        while (s.in_waiting > 0 and state3_flag):  # while the input buffer isn't empty

            num_of_steps = s.read(size=2)
                                # read 1 char from the input buffer  # read  from the buffer until the terminator is received,
                                # readline() can also be used if the terminator is '\n'
            print("num_of_steps" ,num_of_steps)
            num_of_steps_int = int.from_bytes(num_of_steps[0:2], byteorder='big')
            print("num_of_steps_int", num_of_steps_int)
            delta_phi = 360/num_of_steps_int
            print("delta_phi", delta_phi)
            GUI_func.display4scan(num_of_steps_int, delta_phi)
            state3_flag = 0






































def state4_func(num_of_scripts_state4):
    s = ser.Serial('COM3', baudrate=9600, bytesize=ser.EIGHTBITS,
                   parity=ser.PARITY_NONE, stopbits=ser.STOPBITS_ONE,
                   timeout=1)
    enableTX = True
    state4_flag = 1
    s.reset_input_buffer()
    s.reset_output_buffer()




    while ((s.out_waiting > 0 or enableTX) and state4_flag):  # while the output buffer isn't empty
        inChar = '4'
        bytesChar = bytes(inChar, 'ascii')
        s.write(bytesChar)
        if (s.out_waiting == 0):
            enableTX = False

    ack = 1
    while (state4_flag):
        if(ack):
            if(num_of_scripts_state4 == 0):
                command = GUI_func.upload_file1()
            elif (num_of_scripts_state4 == 1):
                command = GUI_func.upload_file2()
            elif (num_of_scripts_state4 == 2):
                command = GUI_func.upload_file3()
            elif (num_of_scripts_state4 == 3):
                command = GUI_func.upload_file4()
            enableTX = True
            s.reset_input_buffer()
            s.reset_output_buffer()

            while ((s.out_waiting > 0 or enableTX) and state4_flag):  # while the output buffer isn't empty
                if (command == 'x'):
                    state4_flag = 0
                inChar = command
                bytesChar = bytes(inChar, 'ascii')
                s.write(bytesChar)
                if (s.out_waiting == 0):
                    enableTX = False
            ack = 0
            if(command == 'a'):
                num_of_scripts_state4 = num_of_scripts_state4 + 1
                if(num_of_scripts_state4 > 3):
                    num_of_scripts_state4 = 1

                txt_address = GUI_func.get_path()
                with open(txt_address) as f:
                    lines = f.readlines()

                num_of_opcode = len(lines)

                enableTX = True
                s.reset_input_buffer()
                s.reset_output_buffer()

                while ((s.out_waiting > 0 or enableTX) and state4_flag):  # while the output buffer isn't empty
                    num_of_opcode_byte = int(num_of_opcode).to_bytes(1, byteorder='little')
                    s.write(num_of_opcode_byte)
                    if (s.out_waiting == 0):
                        for x in range(num_of_opcode):
                            line1 = lines[x][0:2]
                            if(line1 != "05" and line1 != "08"):
                                line2 = lines[x][2:4]
                            if(line1 == "07"):
                                line3 = lines[x][4:6]
                            bytesChar = bytes(line1, 'ascii')
                            s.write(bytesChar)
                            if s.out_waiting == 0 and (line1 != "05" and line1 != "08"):
                                bytesChar = bytes(line2, 'ascii')
                                s.write(bytesChar)
                                if (s.out_waiting == 0) and (line1 == "07"):
                                    bytesChar = bytes(line3, 'ascii')
                                    s.write(bytesChar)
                            if (s.out_waiting == 0):
                                pass
                        if (s.out_waiting == 0):
                            bytesChar = bytes('s', 'ascii')
                            s.write(bytesChar)
                            if (s.out_waiting == 0):
                                enableTX = False
            elif(state4_flag):
                GUI_func.start_func()
                inChar_start = 's'
                bytesChar_start = bytes(inChar_start, 'ascii')
                s.write(bytesChar_start)
                GUI_func.stop_func()
                if s.out_waiting == 0:
                    inChar_stop = 't'
                    bytesChar_stop = bytes(inChar_stop, 'ascii')
                    s.write(bytesChar_stop)
                    if (s.out_waiting == 0):
                        enableTX = False

        while (s.in_waiting > 0 and state4_flag):  # while the input buffer isn't empty

            ack_read_deg = s.read(size=1)
                                # read 1 char from the input buffer  # read  from the buffer until the terminator is received,
                                # readline() can also be used if the terminator is '\n'
            ack_read = int.from_bytes(ack_read_deg, byteorder='big')

            if(ack_read == 255):
                ack = 1
            else:
                layout = [
                    [sg.Text("motor degree is: " + str(ack_read))]]
                window = sg.Window("Demo", layout)
                temp = 0
                while True:
                    temp = temp + 1
                    event, value = window.read(timeout=10)
                    if event == sg.WIN_CLOSED:
                        break
                    if (temp == 100) or s.in_waiting > 0:
                        break
                window.close()

                print('Script angle: ',ack_read)
    return num_of_scripts_state4
















