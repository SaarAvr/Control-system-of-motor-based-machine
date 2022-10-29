import PySimpleGUI as sg
import os

def main_menu():
    layout = [
        [sg.Text("Choose Function")],
        [sg.Button("state1")],
        [sg.Button("state2")],
        [sg.Button("state3")],
        [sg.Button("state4")]]
    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "state1" or event == "state2" or event == "state3" or event == "state4" or event == sg.WIN_CLOSED:
            if (event == "state1"):
                state = "state1"
            elif (event == "state2"):
                state = "state2"
            elif (event == "state3"):
                state = "state3"
            elif (event == "state4"):
                state = "state4"
            break

    window.close()
    return state


def start_func():
    layout = [
        [sg.Text("Push to Start")],
        [sg.Button("START")]]

    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "START" or event == sg.WIN_CLOSED:
            break

    window.close()


def stop_func():
    layout = [
        [sg.Text("Push to Stop")],
        [sg.Button("STOP")]]

    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "STOP" or event == sg.WIN_CLOSED:
            break

    window.close()

def get_path():
    working_directory = os.getcwd()

    layout = [
        [sg.Text("Choose a Text file:")],
        [sg.InputText(key="-FILE_PATH-"),
         sg.FileBrowse(initial_folder=working_directory, file_types=[("txt Files", "*.txt")])],
        [sg.Button('Submit'), sg.Exit()]
    ]

    window = sg.Window("Display Text", layout)

    while True:
        event, values = window.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            break
        elif event == "Submit":
            txt_address = values["-FILE_PATH-"]
            break
    window.close()
    return txt_address

def upload_file1():
    layout = [
        [sg.Text("Choose Function")],
        [sg.Button("upload file 1")],
        [sg.Button("Change State")]]
    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "upload file 1" or event == "Change State" or event == sg.WIN_CLOSED:
            if (event == "upload file 1"):
                temp = 'a'
            elif (event == "Change State"):
                temp = 'x'
            break

    window.close()
    return temp

def upload_file2():
    layout = [
        [sg.Text("Choose Function")],
        [sg.Button("upload file 2")],
        [sg.Button("run script 1")],
        [sg.Button("Change State")]]
    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "upload file 2" or event == "run script 1" or event == "Change State" or event == sg.WIN_CLOSED:
            if (event == "upload file 2"):
                temp = 'a'
            elif (event == "run script 1"):
                temp = 'b'
            elif (event == "Change State"):
                temp = 'x'
            break

    window.close()
    return temp
def upload_file3():
    layout = [
        [sg.Text("Choose Function")],
        [sg.Button("upload file 3")],
        [sg.Button("run script 1")],
        [sg.Button("run script 2")],
        [sg.Button("Change State")]]
    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "upload file 3" or event == "run script 1" or event == "run script 2" or event == "Change State" or event == sg.WIN_CLOSED:
            if (event == "upload file 3"):
                temp = 'a'
            elif (event == "run script 1"):
                temp = 'b'
            elif (event == "run script 2"):
                temp = 'c'
            elif (event == "Change State"):
                temp = 'x'
            break

    window.close()
    return temp

def upload_file4():
    layout = [
        [sg.Text("Choose Function")],
        [sg.Button("RESET and upload file 1")],
        [sg.Button("run script 1")],
        [sg.Button("run script 2")],
        [sg.Button("run script 3")],
        [sg.Button("Change State")]]
    window = sg.Window("Demo", layout)

    while True:
        event, value = window.read()
        if event == "RESET and upload file 1" or event == "run script 1" or event == "run script 2" or event == "run script 3" or event == "Change State" or event == sg.WIN_CLOSED:
            if (event == "RESET and upload file 1"):
                temp = 'a'
            elif (event == "run script 1"):
                temp = 'b'
            elif (event == "run script 2"):
                temp = 'c'
            elif (event == "run script 3"):
                temp = 'd'
            elif (event == "Change State"):
                temp = 'x'
            break

    window.close()
    return temp

def display4deg(deg):
    layout = [
        [sg.Text("motor degree is: " + str(deg))]]
    window = sg.Window("Demo", layout)
    temp = 0
    while True:
        temp = temp + 1
        event, value = window.read(timeout=10)
        if event == sg.WIN_CLOSED:
            break
        if(temp == 100):
            break

    window.close()

def display4scan(num, delta):
    layout = [
        [sg.Text("num of steps: " + str(num))],
        [sg.Text("delta phi is: " + str(delta))],
        [sg.Button("Close")]]
    window = sg.Window("Demo", layout)
    while True:
        event, value = window.read(timeout=10)
        if event == "Close" or event == sg.WIN_CLOSED:
            break

    window.close()




















