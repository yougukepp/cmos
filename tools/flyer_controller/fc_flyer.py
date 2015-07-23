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
        self.mConfigs = None
        self.mObjs = None

        self.ParserJson(jsonName)
        self.MakeFlyer()
        self.ApplyConfigs() 
        #print(self.mObjs["Propeller"])
        self.DataToArray()

    def PaintGL(self):
        objsDict = self.mObjs

        GL.glEnableClientState( GL.GL_COLOR_ARRAY )
        GL.glEnableClientState( GL.GL_VERTEX_ARRAY )

        # 逐个obj
        for objName in objsDict:
            self.AObjPaintGL(objName)

        GL.glDisableClientState( GL.GL_COLOR_ARRAY )
        GL.glDisableClientState( GL.GL_VERTEX_ARRAY ) 

    def AObjPaintGL(self, objName):
        # 图形数据
        objDict = self.mObjs[objName]

        colorsData = objDict['Colors'].tostring()
        verticesData = objDict['Vertices'].tostring()
        indicesData = objDict['Indices'].tostring()
        drawType = objDict['DrawType']
        lineWidth = objDict['LineWidth']

        #print(objName)
        #print(verticesData)
        #print(colorsData)
        #print(indicesData)

        if 0 == len(indicesData) or 0 == len(colorsData) or 0 == len(verticesData):
            #print('%s无数据可绘制' % objName)
            return

        GL.glVertexPointer(4, GL.GL_FLOAT, 0, verticesData)
        GL.glColorPointer(4,  GL.GL_FLOAT, 0, colorsData)
        GL.glLineWidth(lineWidth)
        if 'TringleFan' == drawType:
            GL.glDrawElements(GL.GL_TRIANGLE_FAN, len(indicesData), GL.GL_UNSIGNED_BYTE, indicesData)
        elif 'Lines' == drawType:
            GL.glDrawElements(GL.GL_LINES,        len(indicesData), GL.GL_UNSIGNED_BYTE, indicesData)
        else:
            print('绘制类型%s错误.' % DrawType)

    def ParserJson(self, jsonName):
        self.mConfigs = {}
        self.mObjs = {}

        jsonPaser = FCJsonPaser(jsonName)

        data = jsonPaser.ToDict()

        self.mConfigs = data['Configs']
        self.mObjs = data['Objs']

    def ApplyConfigs(self):
        scale = self.mConfigs["Scale"]
        alpha = self.mConfigs["Alpha"]

        self.UpdateScale(scale)
        self.UpdateAlpha(alpha)

    def UpdateScale(self, scale):
        self.UpdateAAttribute(scale, 'Vertices')

    def UpdateAlpha(self, alpha):
        self.UpdateAAttribute(alpha, 'Colors')

    def UpdateAAttribute(self, para, attributeName):
        """
        更新某一个参数 的配置

        para 参数值 :
        scale 缩放
        alpha 透明度

        attributeName 文件属性名字:
        Vertices 顶点
        Colors 颜色
        """
        objsDict = self.mObjs

        # 逐个obj
        for objName in objsDict:
            #print(objName)
            objData = objsDict[objName]
            oldData = objData[attributeName]
            newData = self.UpdateAConfig(oldData, para)
            self.mObjs[objName][attributeName] = newData

    def UpdateAConfig(self, oldData, config): 
        i = 0
        newData = []
        for aData in oldData:
            if 0 == i%3 and 0 != i:
                newData.append(config)
            newData.append(aData)
            i += 1
        newData.append(config) 
        #print('old:')
        #print(oldData) 
        #print('new:')
        #print(newData)

        return newData

    def DataToArray(self):
        objsDict = self.mObjs

        # 逐个obj
        for objName in objsDict:
            #print(objName)
            objData = objsDict[objName]
            for attributeName in objData: 
                listData = objData[attributeName]
                if 'Vertices' == attributeName or 'Colors' == attributeName:
                    arrayData = array.array('f', listData)
                elif 'Indices' == attributeName:
                    arrayData = array.array('B', listData)
                else: # 其他属性不转换
                    continue
                self.mObjs[objName][attributeName] = arrayData
                #print(self.mObjs[objName][attributeName])

    def MakeFlyer(self):
        """
        平移出一个部分
        旋转出三个对称的部分
        """
        pass

if __name__ == '__main__': 
    flyer = FCFlyer()
    print('测试通过')

