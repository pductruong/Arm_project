from serial import Serial
import csv

ser = Serial('/dev/ttyACM0',baudrate=9600)
step1 = ""
step2 = ""
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
    with open(csv_filename, mode='w',newline='') as csv_file:
        fieldnames = ['step1','step2']
        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerow({'step1': obj[0], 'step2': obj[1]})


def serial_send(data):
    ser.write(data.encode())

if __name__ == '__main__':
    state_file = '/home/tucuman/arm_project/save_state.txt'
    state = get_state(state_file)
    # print(state[0][-1])
    pub_data =str((state[0][-1]) + '$' + (state[1][-1])+'#')
    serial_send(pub_data)
    # print(pub_data)
    while True:
        x = str(ser.readline(),encoding='utf-8')
        if x.startswith('F'):
            ser.reset_input_buffer()
            step1 = x.strip().replace('F','')
            # print(step1)
        if x.startswith('S'):
            ser.reset_input_buffer()
            step2 = x.strip().replace('S','')
            # print(step2)
        new_state = (step1,step2)
        print(new_state)
        write_state(state_file,new_state)
