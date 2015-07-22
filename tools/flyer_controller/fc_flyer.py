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
        self.mData = None
        self.ParserJson(jsonName)
        self.ApplyConfigs()

    def PaintGL(self):
        """
        colorsData = self.GetData(("Propeller", "Colors")).tostring()
        verticesData = self.GetData(("Propeller", "Vertices")).tostring()
        indicesData = self.GetData(("Propeller", "Indices")).tostring()
        drawType = self.GetData(("Propeller", "DrawType"))
        lineWidth = self.GetData(("Propeller", "LineWidth"))

        GL.glEnableClientState( GL.GL_COLOR_ARRAY )
        GL.glEnableClientState( GL.GL_VERTEX_ARRAY )
        GL.glColorPointer(4, GL.GL_FLOAT, 0, colorsData)
        GL.glVertexPointer(4, GL.GL_FLOAT, 0, verticesData)
        GL.glLineWidth(lineWidth)
        if "TringleFan" == drawType:
            GL.glDrawElements(GL.GL_TRIANGLE_FAN, len(indicesData), GL.GL_UNSIGNED_BYTE, indicesData)
        else:
            print('绘制类型错误.')

        GL.glDisableClientState( GL.GL_COLOR_ARRAY )
        GL.glDisableClientState( GL.GL_VERTEX_ARRAY ) 
        """
        pass

    def ParserJson(self, jsonName):
        self.mData = {}
        jsonPaser = FCJsonPaser(jsonName)
        self.mData = jsonPaser.ToDict()

    def ApplyConfigs(self):
        scale = self.GetData("Configs", "Scale")
        alpha = self.GetData("Configs", "Alpha")

        self.UpdateScale()
        self.UpdateAlpha()

        """
        # 顶点
        i = 0
        tempVetices = self.mData["Propeller"]["Vertices"]
        self.mData["Propeller"]["Vertices"] = []
        for d in tempVetices:
            if 0 == i%3 and 0 != i:
                self.mData["Propeller"]["Vertices"].append(scale)

            self.mData["Propeller"]["Vertices"].append(d) 
            i+=1 
        # 最后一个
        self.mData["Propeller"]["Vertices"].append(scale)

        # 颜色
        i = 0
        tempColors  = self.mData["Propeller"]["Colors"]
        self.mData["Propeller"]["Colors"] = []
        for d in tempColors:
            if 0 == i%3 and 0 != i:
                self.mData["Propeller"]["Colors"].append(alpha)

            self.mData["Propeller"]["Colors"].append(d)
            i+=1 
        # 最后一个
        self.mData["Propeller"]["Colors"].append(alpha)

        # 调试
        i = 0
        for d in self.mData["Propeller"]["Vertices"]:
            print("%7.4f" % d, end=',')

            #if 0 == (i+1)%3:
            if 0 == (i+1)%4:
                print()
            i += 1
        print()
        i = 0
        for d in self.mData["Propeller"]["Colors"]:
            print("%7.4f" % d, end=',')

            #if 0 == (i+1)%3:
            if 0 == (i+1)%4:
                print()
            i += 1
        print()
        # 转换为OpenGL可以接受的格式
        tempData = self.mData["Propeller"]["Vertices"]
        self.mData["Propeller"]["Vertices"] = None
        self.mData["Propeller"]["Vertices"] = array.array('f', tempData)

        tempData = self.mData["Propeller"]["Colors"]
        self.mData["Propeller"]["Colors"] = None
        self.mData["Propeller"]["Colors"] = array.array('f', tempData)

        tempData = self.mData["Propeller"]["Indices"]
        self.mData["Propeller"]["Indices"]  = None
        self.mData["Propeller"]["Indices"]  = array.array('B', tempData)
        # 调试
        pp = self.GetData(("Propeller", "Vertices"))
        print(pp)

        pp = self.GetData(("Propeller", "Colors"))
        print(pp)

        pp = self.GetData(("Propeller", "Indices"))
        print(pp)
        """

    def UpdateScale(self):
        """
        # 顶点
        i = 0
        tempVetices = self.mData["Propeller"]["Vertices"]
        self.mData["Propeller"]["Vertices"] = []
        for d in tempVetices:
            if 0 == i%3 and 0 != i:
                self.mData["Propeller"]["Vertices"].append(scale)

            self.mData["Propeller"]["Vertices"].append(d) 
            i+=1 
        # 最后一个
        self.mData["Propeller"]["Vertices"].append(scale)
        """

        pass
    
    def UpdateAlpha(self):
        pass

    def GetData(self, *keyTuple):
        mainKey = keyTuple[0]
        #print(mainKey)
        v = self.mData[mainKey]
        #print(v)

        maxIndex = len(keyTuple)
        for i in range(1, maxIndex):
            k = keyTuple[i]
            #print(k)
            v = v[k]

        return v

if __name__ == '__main__': 
    flyer = FCFlyer()
    print('测试通过')

