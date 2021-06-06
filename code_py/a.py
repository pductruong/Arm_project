from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
from PyQt5.QtWidgets import QDialog, QFileDialog

def openSer():
    serial.setPortName(ui.comlist.currentText())
    serial.open(QIODevice.ReadWrite)
    print("Da ket noi voi Arduino")

def closeSer():
    serial.close()
    print("Da ngat ket noi")

def onRead():
    global dc1
    global dc2
    rx = serial.readLine()
    cvt_rx = str(rx, 'utf-8').strip()
    data = cvt_rx.split(',')
    dc1 = data[0]
    dc2 = data[1]
    print(dc1,dc2)
    ui.showDC1.setText(str(dc1))
    ui.showDC2.setText(str(dc2))

def browsefile():
    frame = QFileDialog.getOpenFileName('Open File', '/home/tucuman', 'Text (*.txt)')
    ui.filename.setText(frame[0])

#GUI
app = QtWidgets.QApplication([])
ui = uic.loadUi("/home/tucuman/Arduino/Project/code_py/test.ui")
ui.setWindowTitle("Control Panel")

#Serial
serial = QSerialPort()
serial.setBaudRate(115200)
port_list = []
ports = QSerialPortInfo().availablePorts()
for port in ports:
    port_list.append(port.portName())
ui.comlist.addItems(port_list)
serial.readyRead.connect(onRead)

#Nut bam
ui.openSer.clicked.connect(openSer)
ui.closeSer.clicked.connect(closeSer)
ui.browse.clicked.connect(browsefile)


ui.show()
app.exec_()