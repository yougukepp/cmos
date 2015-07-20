#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
from PyQt4 import QtGui

from PyQt4.uic import loadUiType, loadUi
FCWindowUIClass = loadUiType("fc_window.ui")
FCComParaGroupBoxUiClass = loadUiType("com_para_group.ui")

class FCWindow(QtGui.QWidget):
    def __init__(self):
        super(FCWindow, self).__init__() 
        
        # 初始化UI
        self.mUi = FCWindowUIClass[0]()
        self.mUi.setupUi(self)

        # 绑定控制界面
        self.mComWidget = FCComWidget(self.mUi.typeComboBox, self.mUi.paraGroupBox, self.mUi.ctrlpushButton)
        self.mPostWidget = self.mUi.postTab
        self.mCtrlWidget = self.mUi.ctrlTab

        """
        # 飞行器 加载OpenGL控件
        self.mFlyerWidget = FlyerWidget()
        self.mFlyerGroupBox = self.mUi.flyerWidget

        # 保持方形
        self.mFlyerGroupBox.setFixedSize(500,500)
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.mFlyerWidget)
        self.mFlyerGroupBox.setLayout(vbox)
        """

        # 标题
        self.setWindowTitle("飞控板控制")

class FCComWidget(QtGui.QWidget):
    def __init__(self, typeComboBox, paraGroupBox, ctrlpushButton):
        super(FCComWidget, self).__init__()

        self.mTypeComboBox = typeComboBox
        self.mParaGroupBox = paraGroupBox
        self.mctrlpushButton = ctrlpushButton

        self.mTypeComboBox.addItem('串口')
        self.mTypeComboBox.addItem('网络(WiFi&网口)')
        self.mTypeComboBox.addItem('nRFXX')

        # 设置UI 使用串口
        self.mParaGroupBoxUi = FCComParaGroupBoxUiClass[0]()
        self.mParaGroupBoxUi.setupUi(self.mParaGroupBox)

class FCPostWidget(QtGui.QWidget):
    def __init__(self):
        super(FCPostWidget, self).__init__()

class FCCtrlWidget(QtGui.QWidget):
    def __init__(self):
        super(FCCtrlWidget, self).__init__()

if __name__ == '__main__': 
    app = QtGui.QApplication(sys.argv)
    win = FCWindow()
    win.showMaximized()
    sys.exit(app.exec_())

