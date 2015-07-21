#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys

from PyQt4 import QtGui
from PyQt4 import QtCore
from PyQt4 import QtOpenGL

from fc_axis import FCAxis

try:
    from OpenGL import GL
except ImportError: 
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "飞控板控制",
            "需要安装PyOpenGL(使用pip installed PyOpenGL).\n")
    sys.exit(1) 
 
class FlyerWidget(QtOpenGL.QGLWidget):
    def __init__(self, parent=None):
        super(FlyerWidget, self).__init__(parent)

        # 三维物体 及其初始角度
        self.mAxis = FCAxis() 
        
        # 绘制定时器
        self.mPaintTimer = None

    def initializeGL(self):
        #self.PrintGLInfo()
        self.qglClearColor(QtCore.Qt.black)

        self.mPaintTimer = QtCore.QTimer();
        self.mPaintTimer.timeout.connect(self.Update);
        self.mPaintTimer.start(500) # 初始时以不可达的1000fps刷新

    def Update(self):
        self.updateGL()

    def paintGL(self):
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT ) 
        GL.glViewport(0, 0, self.width(), self.height()) 
        self.mAxis.PaintGL()

    def PrintGLInfo(self):
        print("生产商       ", end=":")
        print(GL.glGetString(GL.GL_VENDOR))

        print("渲染器       ", end=":")
        print(GL.glGetString(GL.GL_RENDERER))

        print("OpenGL版本   ", end=":")
        print(GL.glGetString(GL.GL_VERSION))

        print("着色语言版本 ", end=":")
        print(GL.glGetString(GL.GL_SHADING_LANGUAGE_VERSION))

        """
        print("API:")
        for func in dir(GL):
            print(func) 
        print(GL.__doc__)
        """

if __name__ == '__main__': 
    app = QtGui.QApplication(sys.argv)
    win = FlyerWidget()
    win.show()
    win.setFixedSize(500,500)
    sys.exit(app.exec_())

