import serial as ser
import numpy
import PySimpleGUI as sg
import GUI_func
import state_function
import global_

#in this example we send a character, and receive a string in response only if the start character was 'u'

def main():
    state = "state0"
    num_of_scripts_state4 = 0

    while True:
        if state == "state0":
            state = GUI_func.main_menu()

        elif state == "state1":
            state_function.state1_func()
            state = "state0"
        elif state == "state2":
            state_function.state2_func()
            state = "state0"
        elif state == "state3":
            state_function.state3_func()
            state = "state0"
        elif state == "state4":
            num_of_scripts_state4 = state_function.state4_func(num_of_scripts_state4)
            state = "state0"





if __name__ == '__main__':
    main()






