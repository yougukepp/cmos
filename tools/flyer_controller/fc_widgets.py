#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import QtOpenGL
from PyQt4.uic import loadUiType, loadUi

try:
    from OpenGL import GL
except ImportError: 
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "飞控板控制",
            "需要安装PyOpenGL(使用pip installed PyOpenGL).\n")
    sys.exit(1) 
 
from fc_flyerWidget import FlyerWidget 

gGLSize = 600
FCWindowUIClass = loadUiType("fc_window.ui")

class FCWindow(QtGui.QWidget):
    def __init__(self):
        super(FCWindow, self).__init__() 
        
        # 初始化UI
        self.mUi = FCWindowUIClass[0]()
        self.mUi.setupUi(self)

        # 绑定控制界面
        # 通信
        self.mComWidget = FCComWidget(self.mUi.typeComboBox, self.mUi.simIntervalLineEdit, self.mUi.comPortComboBox, self.mUi.buardrateLineEdit, self.mUi.ipLineEdit, self.mUi.ssidLineEdit, self.mUi.passwordLineEdit, self.mUi.usbNameComboBox, self.mUi.ctrlpushButton)
        self.mPostWidget = self.mUi.postTab
        self.mCtrlWidget = self.mUi.ctrlTab
        # TODO:姿态
        # TODO:配置

        # 飞行器 加载OpenGL控件
        self.mFlyerWidget = FlyerWidget()
        self.mFlyerGroupBox = self.mUi.flyerGroupBox 
        
        self.mFlyerGroupBox.setFixedSize(gGLSize, gGLSize)

        # 保持方形
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.mFlyerWidget)
        self.mFlyerGroupBox.setLayout(vbox)

        # 标题
        self.setWindowTitle("飞控板控制")

class FCComWidget(QtGui.QWidget):
    def __init__(self, typeComboBox, simIntervalLineEdit, comPortComboBox, buardrateLineEdit, ipLineEdit, ssidLineEdit, passwordLineEdit, usbNameComboBox, ctrlpushButton):
        super(FCComWidget, self).__init__()

        self.mTypeComboBox = typeComboBox
        self.mSimIntervalLineEdit = simIntervalLineEdit
        self.mComPortComboBox = comPortComboBox
        self.mBuardrateLineEdit = buardrateLineEdit
        self.mIpLineEdit = ipLineEdit
        self.mSsidLineEdit = ssidLineEdit
        self.mPasswordLineEdit = passwordLineEdit
        self.mUsbNameComboBox = usbNameComboBox
        self.mCtrlpushButton = ctrlpushButton

        self.mTypeComboBox.addItem('模拟')
        self.mTypeComboBox.addItem('串口')
        self.mTypeComboBox.addItem('网络')
        self.mTypeComboBox.addItem('USB')

        self.mTypeComboBox.currentIndexChanged.connect(self.ChangeType) 
        
    def ChangeType(self, typeIndex): 
        if 0 != typeIndex: 
            msgBox = QtGui.QMessageBox();
            msgBox.setText("暂时只实现模拟数据.\n")
            msgBox.exec_();

            self.mTypeComboBox.setCurrentIndex(0)

class FCPostWidget(QtGui.QWidget):
    def __init__(self):
        super(FCPostWidget, self).__init__()

class FCCtrlWidget(QtGui.QWidget):
    def __init__(self):
        super(FCCtrlWidget, self).__init__()

if __name__ == '__main__': 
    app = QtGui.QApplication(sys.argv)
    win = FCWindow()
    win.show()
    sys.exit(app.exec_())

