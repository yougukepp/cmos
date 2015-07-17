#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
os:winxp sp3 32
pyqt:PyQt4-4.11.4-gpl-Py3.4-Qt4.8.7-x32
opengl:pip install PyOpenGL(PyOpenGL3.1.0)
"""

import sys
import math
import array
import datetime
from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import QtOpenGL

from PyQt4.uic import loadUiType, loadUi
UIClass = loadUiType("flyer_controller.ui")

try:
    from OpenGL import GL
except ImportError: 
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "飞控板控制",
            "需要安装PyOpenGL(使用pip installed PyOpenGL).\n")
    sys.exit(1) 

class GLWindow(QtGui.QWidget):
    def __init__(self):
        super(GLWindow, self).__init__() 
        
        self.mUi = UIClass[0]()
        self.mUi.setupUi(self)

        # 角度
        self.mXLabel = self.mUi.xLabel
        self.mXSlider = self.mUi.xSlider 
        self.mXSlider.setRange(0, 360)

        self.mYLabel = self.mUi.yLabel
        self.mYSlider = self.mUi.ySlider
        self.mYSlider.setRange(0, 360)

        self.mZLabel = self.mUi.zLabel
        self.mZSlider = self.mUi.zSlider
        self.mZSlider.setRange(0, 360)

        # 加载OpenGL控件
        self.mGlWidget = GLWidget()
        self.mGLGroupBox = self.mUi.glGroupBox
        # 保持方形
        self.mGLGroupBox.setFixedSize(500,500)
        vbox = QtGui.QVBoxLayout()
        vbox.addWidget(self.mGlWidget)
        self.mGLGroupBox.setLayout(vbox)

        # 标题
        self.setWindowTitle("飞控板控制")

        # 连接事件
        self.mXSlider.valueChanged.connect(self.mGlWidget.SetXNext)
        self.mYSlider.valueChanged.connect(self.mGlWidget.SetYNext)
        self.mZSlider.valueChanged.connect(self.mGlWidget.SetZNext)

class GLWidget(QtOpenGL.QGLWidget):
    def __init__(self, parent=None):
        super(GLWidget, self).__init__(parent)

        # 三维物体 及其初始角度
        self.mObj = None

        # 当前姿态
        self.mX = 0
        self.mY = 0
        self.mZ = 0

        # 需要调整的参数
        self.mXNext = 0
        self.mYNext = 0
        self.mZNext = 0

        # 定时器
        self.mPaintTimer = None
        self.mLasTPaintTimes = 0
        self.mNowPaintTimes = 0

        # 帧率
        self.mFps = 0

    def initializeGL(self):
        self.PrintGLInfo()
        self.qglClearColor(QtCore.Qt.black)
        self.mObj = GLObject()

        self.mPaintTimer = QtCore.QTimer();
        self.mPaintTimer.timeout.connect(self.Update);
        self.mPaintTimer.start(1) # 初始时以不可达的1000fps刷新

        self.mFpsTimer = QtCore.QTimer();
        self.mFpsTimer.timeout.connect(self.UpdateFps);
        self.mFpsTimer.start(1000) # 每秒输出帧率

    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT ) 
        GL.glViewport(0, 0, self.width(), self.height())

        # 计算旋转
        xRot = self.mXNext - self.mX
        yRot = self.mYNext - self.mY
        zRot = self.mZNext - self.mZ 

        # 更新姿态
        self.mX = self.mXNext
        self.mY = self.mYNext
        self.mZ = self.mZNext

        # 远离眼睛 保证可以看见
        GL.glRotated(xRot, 1.0, 0.0, 0.0)
        GL.glRotated(yRot, 0.0, 1.0, 0.0)
        GL.glRotated(zRot, 0.0, 0.0, 1.0)

        GL.glEnableClientState( GL.GL_COLOR_ARRAY )
        GL.glEnableClientState( GL.GL_VERTEX_ARRAY )
        GL.glColorPointer(4, GL.GL_FLOAT, 0, self.mObj.GetColors())
        GL.glVertexPointer(4, GL.GL_FLOAT, 0, self.mObj.GetVertices())
        for i in range(0, 3):
            GL.glDrawElements(GL.GL_LINES, 2, GL.GL_UNSIGNED_BYTE, self.mObj.GetIndices(i))

        GL.glDisableClientState( GL.GL_COLOR_ARRAY )
        GL.glDisableClientState( GL.GL_VERTEX_ARRAY ) 

        self.mNowPaintTimes += 1

    def Update(self):
        self.updateGL()

    def UpdateFps(self):
        self.mFps = self.mNowPaintTimes - self.mLasTPaintTimes
        self.mLasTPaintTimes = self.mNowPaintTimes
        print("姿态:x:%d, y:%d, z:%d" % (self.mX, self.mY, self.mZ))
        print("帧率:% 3dfps" % self.mFps)

    def SetXNext(self, angle):
        self.mXNext = angle

    def SetYNext(self, angle):
        self.mYNext = angle

    def SetZNext(self, angle):
        self.mZNext = angle

    def PrintGLInfo(self):
        print("生产商       ", end=":")
        print(GL.glGetString(GL.GL_VENDOR))

        print("渲染器       ", end=":")
        print(GL.glGetString(GL.GL_RENDERER))

        print("OpenGL版本   ", end=":")
        print(GL.glGetString(GL.GL_VERSION))

        print("着色语言版本 ", end=":")
        print(GL.glGetString(GL.GL_SHADING_LANGUAGE_VERSION))

class GLObject():
    def __init__(self):
        # 顶点 位置
        self.mVertices = array.array('f',
                [ 0.0,  0.0,  0.0,  1.0,
                  0.9,  0.0,  0.0,  1.0,
                  0.0,  0.9,  0.0,  1.0,
                  0.0,  0.0,  0.9,  1.0]) 

        # 顶点 颜色
        self.mColors = array.array('f',
                [ 1.0,  1.0,  1.0,  1.0,
                  1.0,  0.0,  0.0,  1.0,
                  0.0,  1.0,  0.0,  1.0,
                  0.0,  0.0,  1.0,  1.0] ) 

        # 顶点索引
        self.mIndices = []
        index = array.array('B', [0, 1])
        self.mIndices.append(index)
        index = array.array('B', [0, 2])
        self.mIndices.append(index)
        index = array.array('B', [0, 3])
        self.mIndices.append(index)

    def GetVertices(self):
        return self.mVertices.tostring()

    def GetColors(self):
        return self.mColors.tostring()

    def GetIndices(self, index):
        return self.mIndices[index].tostring()

if __name__ == '__main__': 
    app = QtGui.QApplication(sys.argv)
    win= GLWindow()
    win.showMaximized()
    sys.exit(app.exec_())

