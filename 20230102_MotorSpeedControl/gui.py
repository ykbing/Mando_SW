import sys
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QLabel, QLineEdit, QTextEdit, QPushButton


class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        grid = QGridLayout()
        self.setLayout(grid)

        label1 = QLabel('PWM :')
        label2 = QLabel('P :')
        label3 = QLabel('PI :')
        label4 = QLabel('PD :')

        grid.addWidget(label1, 0, 0)
        grid.addWidget(label2, 1, 0)
        grid.addWidget(label3, 2, 0)
        grid.addWidget(label4, 3, 0)

        grid.addWidget(QLineEdit(), 0, 1)
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


        self.setWindowTitle('모터제어 GUI')
        self.setGeometry(300, 300, 300, 200)
        self.show()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())