import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt

class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):

        grid = QGridLayout()
        self.setLayout(grid)

        pwm_spinbox = QSpinBox()
        pwm_spinbox.setRange(-255, 255)
        pwm_spinbox.setSingleStep(1)

        label1 = QLabel('PWM :')
        label2 = QLabel('P :')
        label3 = QLabel('PI :')
        label4 = QLabel('PD :')

        grid.addWidget(label1, 0, 0)
        grid.addWidget(label2, 1, 0)
        grid.addWidget(label3, 2, 0)
        grid.addWidget(label4, 3, 0)

        grid.addWidget(pwm_spinbox, 0, 1)
        grid.addWidget(QLineEdit(), 1, 1)
        grid.addWidget(QLineEdit(), 2, 1)
        grid.addWidget(QLineEdit(), 3, 1)

        pwm_btn = QPushButton('input', self)
        p_btn = QPushButton('input', self)
        pi_btn = QPushButton('input', self)
        pd_btn = QPushButton('input', self)

        grid.addWidget(pwm_btn, 0, 2)
        grid.addWidget(p_btn, 1, 2)
        grid.addWidget(pi_btn, 2, 2)
        grid.addWidget(pd_btn, 3, 2)

        c_btn = QCheckBox('Control Choice', self)
        c_btn.move(20, 20)
        grid.addWidget(c_btn, 4, 0)
        c_btn.setChecked(True)

        sp_btn = QRadioButton('Speed Control', self)
        sp_btn.setChecked(True)

        vel_btn = QRadioButton(self)
        vel_btn.setText('Position Control')

        grid.addWidget(sp_btn, 4, 1)
        grid.addWidget(vel_btn, 4, 2)

        resultTable = QTableWidget()
        resultTable.setRowCount(4)
        resultTable.setColumnCount(4)

        resultTable.setEditTriggers(QAbstractItemView.AllEditTriggers)
        resultTable.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeToContents)

        grid.addWidget(resultTable, 5, 1)

        self.setWindowTitle('모터제어 GUI')
        self.setGeometry(300, 300, 430, 300)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())