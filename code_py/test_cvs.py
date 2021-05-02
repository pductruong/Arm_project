import csv
from serial import Serial

def get_state(csv_filename):
    step1 = []
    step2 = []
    with open(csv_filename, mode="r") as csv_file:
        csv_reader = csv.DictReader(csv_file,delimiter=',')
        for row in csv_reader:
            step1.append(row['step1'])
            step2.append(row['step2'])
    return step1, step2

def write_state(csv_filename,obj):
    with open(csv_filename, mode='a+',newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        csv_writer.writerow(obj)

def serial_send(port,data):
    ser = Serial(port,baudrate=9600)
    ser.write(data.encode())

def serial_recive(port):
    ser = Serial(port,baudrate=9600)
    x = str(ser.readline(),encoding='utf-8')
    if x.startswith('F'):
        ser.reset_input_buffer()
        motor_state1 = x.strip().replace('F','')
        goc = motor_state1
        print(goc)
    if x.startswith('S'):
        ser.reset_input_buffer()
        motor_state2 = x.strip().replace('S','')
        goc = motor_state2
        print(goc)

if __name__ == '__main__':
    state_file = '/home/tucuman/arm_project/save_state.txt'
    port = '/dev/ttyACM0'
    state = get_state(state_file)
    # print(state[0][-1])
    # print(state[1][-1])
    pub_data =str((state[0][-1]) + '$' + (state[1][-1])+'#')
    print(pub_data)
    obj = [123,54]
    # write_state(state_file,obj)
    serial_send(port,pub_data)
    
