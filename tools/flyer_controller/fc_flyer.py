#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import array
import json

from PyQt4 import QtGui
from fc_json_paser import FCJsonPaser
try:
    from OpenGL import GL
except ImportError: 
    app = QtGui.QApplication(sys.argv)
    QtGui.QMessageBox.critical(None, "飞控板控制",
            "需要安装PyOpenGL(使用pip installed PyOpenGL).\n")
    sys.exit(1) 

class FCFlyer():
    def __init__(self, jsonName = 'flyer.json'):
        self.mJsonPaser = FCJsonPaser(jsonName)

        self.mPropellerVertices = self.mJsonPaser.GetValue(("Propeller", "Vertices"))
        self.mPropellerColors = self.mJsonPaser.GetValue(("Propeller", "Colors"))
        self.mPropellerIndices = self.mJsonPaser.GetValue(("Propeller", "Indices"))
        self.mPropellerDrawType = self.mJsonPaser.GetValue(("Propeller", "Indices"))


    def PaintGL(self):
        pass
        """
        GL.glEnableClientState( GL.GL_COLOR_ARRAY )
        GL.glEnableClientState( GL.GL_VERTEX_ARRAY )
        GL.glColorPointer(4, GL.GL_FLOAT, 0, self.GetColors(axis))
        GL.glVertexPointer(4, GL.GL_FLOAT, 0, self.GetVertices(axis))
        for i in range(0, 5):
            GL.glDrawElements(GL.GL_LINES, 2, GL.GL_UNSIGNED_BYTE, self.GetIndices(axis, i))
        GL.glDisableClientState( GL.GL_COLOR_ARRAY )
        GL.glDisableClientState( GL.GL_VERTEX_ARRAY ) 
        """

    def GetVertices(self, axisName):
        pass

    def GetColors(self, axisName):
        pass

    def GetIndices(self, axisName, index):
        pass

if __name__ == '__main__': 
    flyer = FCFlyer()
    print('测试通过')

