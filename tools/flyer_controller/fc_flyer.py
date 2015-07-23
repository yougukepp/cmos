#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import array
import json
import math

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

        self.Parse(jsonName)
        self.ObjNESToGLXYZ()
        self.ApplyConfigs() 
        self.DataToArray()

    def Parse(self, jsonName):
        jsonPaser = FCJsonPaser(jsonName)
        data = jsonPaser.ToDict()

        # 配置
        self.mConfigs = {}
        self.mConfigs = data['Configs']

        # obj对象
        self.mObjs = {}
        objsDict = data['Objs']
        for objName in objsDict:
            self.mObjs[objName] = FCObj(objsDict, objName)





        # TODO:优化
        # 以下部分分离为独立函数 并且目前的逻辑可读性太差
        # 制作飞行器
        """
        平移出一个部分
        旋转出三个对称的部分
        """
        objsDict = self.mObjs
        config = self.mConfigs
        angleList = config['AngleList']
        armLength = config['ArmLength']

        # 平移 所有顶点E-armLength
        # print(armLength)
        for obj in self.mObjs:
            #print(objName)
            self.mObjs[obj].Translate(0, armLength, 0)

        #print(angleList)
        # 旋转 
        newObjs = {}
        for obj in self.mObjs:
            for angle in angleList:
                objName = obj + str(angle) 
                #print(objName)
                newObjs[objName] = FCObj(objsDict, obj) # 使用原始数据初始化
                newVertices = self.mObjs[obj].GetMirrorVertices(angle)
                newObjs[objName].UpdateVertices(newVertices)
        # TODO:合并所有obj 字典

    def PaintGL(self):
        objsDict = self.mObjs

        GL.glEnableClientState( GL.GL_COLOR_ARRAY )
        GL.glEnableClientState( GL.GL_VERTEX_ARRAY )

        # 逐个obj
        for objName in objsDict:
            objsDict[objName].PaintGL()

        GL.glDisableClientState( GL.GL_COLOR_ARRAY )
        GL.glDisableClientState( GL.GL_VERTEX_ARRAY ) 

    def ApplyConfigs(self):
        scale = self.mConfigs["Scale"]
        alpha = self.mConfigs["Alpha"]

        for obj in self.mObjs:
            #print(obj)
            self.mObjs[obj].UpdateScaleOrAlpha('Vertices', scale)
            self.mObjs[obj].UpdateScaleOrAlpha('Colors', alpha)

    def DataToArray(self):
        for obj in self.mObjs:
            self.mObjs[obj].DataToArray()

    # 转换到 模型:北N东E天S => GL:XYZ
    def ObjNESToGLXYZ(self):
        for obj in self.mObjs:
            #print(objName)
            self.mObjs[obj].NESToGLXYZ()


class FCObj():
    def __init__(self, jsonDict, objName):
        #print(objName)
        self.mName = objName
        self.mData = jsonDict[objName]

    def PaintGL(self):
        # 图形数据
        data = self.mData

        colorsData = data['Colors']
        verticesData = data['Vertices']
        indicesData = data['Indices']
        drawType = data['DrawType']
        lineWidth = data['LineWidth']

        #print(self.mName)
        #print(verticesData)
        #print(colorsData)
        #print(indicesData)
        if 0 == len(indicesData) or 0 == len(colorsData) or 0 == len(verticesData):
            #print('%s无数据可绘制' % objName)
            return

        GL.glVertexPointer(4, GL.GL_FLOAT, 0, verticesData.tostring())
        GL.glColorPointer(4,  GL.GL_FLOAT, 0, colorsData.tostring())
        GL.glLineWidth(lineWidth)

        if 'TringleFan' == drawType:
            for indiceItem in indicesData:
                if len(indiceItem) < 3: # 不是三角形
                    continue
                GL.glDrawElements(GL.GL_TRIANGLE_FAN, len(indiceItem), GL.GL_UNSIGNED_BYTE, indiceItem.tostring())

        elif 'Tringles' == drawType:
            for indiceItem in indicesData:
                if len(indiceItem) < 3: # 不是三角形
                    continue
                GL.glDrawElements(GL.GL_TRIANGLES, len(indiceItem), GL.GL_UNSIGNED_BYTE, indiceItem.tostring())

        elif 'Lines' == drawType:
            for indiceItem in indicesData:
                if len(indiceItem) < 2: # 不是线
                    continue
                GL.glDrawElements(GL.GL_LINES,        len(indiceItem), GL.GL_UNSIGNED_BYTE, indiceItem.tostring())

        else:
            print('绘制类型%s错误.' % DrawType)

    # 齐次坐标 加入到最后一维
    def UpdateScaleOrAlpha(self, keyStr, scaleOrAlpha):
        i = 0
        oldData = self.mData[keyStr]
        newData = []
        #print("%s.%s:" % (self.mName, keyStr))
        #print(oldData)
        for aData in self.mData[keyStr]:
            newData.append(aData)
            if 0 == (i+1)%3:
                newData.append(scaleOrAlpha)
            i += 1
        #print(newData)
        self.mData[keyStr] = newData

    def Translate(self, n, e, s):
        """
        平移
        n 北
        e 东
        s 天
        """
        #print("%s translate:" % self.mName) 
        data = self.mData['Vertices']
        i = 0
        iMax = len(data) 
        #print(data)
        for i in range(0, iMax):
            #北
            if 0 == i % 3:
                self.mData['Vertices'][i] = data[i] + n
            #东
            if 0 == (i+2) % 3:
                self.mData['Vertices'][i] = data[i] + e
            #天
            if 0 == (i+1) % 3:
                self.mData['Vertices'][i] = data[i] + s
        #print(self.mData['Vertices'])

    def DataToArray(self): 
        for attributeName in self.mData: 
            #print("%s.%s:" % (self.mName, attributeName)) 
            if 'Vertices' == attributeName or 'Colors' == attributeName:
                """
                顶点和色彩转换为浮点数组
                """ 
                oldData = self.mData[attributeName]
                newData = array.array('f', oldData)
            elif 'Indices' == attributeName:
                """
                索引转换为Byte数组
                """ 
                oldData = self.mData[attributeName]
                newData = []
                for aIndiceItem in oldData:
                    aIndiceArrayItem = array.array('B', aIndiceItem)
                    newData.append(aIndiceArrayItem)
                newData.append([]) # 保证一维也对 
            else: 
                """其他属性不转换"""
                continue 
            #print(oldData)
            #print(newData)
            self.mData[attributeName] = newData

    def NESToGLXYZ(self):
        """
        N => Z    0 => 2
        E => X    1 => 0
        S => Y    2 => 1
        """
        #print("%s NESToGLXYZ:" % self.mName) 
        data = self.mData['Vertices']
        i = 0
        step = 3
        iMax = len(data) 
        #print(data)
        for i in range(0, iMax, step):
            newPoint = []
            newPoint.append(data[i+1])
            newPoint.append(data[i+2])
            newPoint.append(data[i])
            self.mData['Vertices'][i] = newPoint[0]
            self.mData['Vertices'][i+1] = newPoint[1]
            self.mData['Vertices'][i+2] = newPoint[2]

        #print(self.mData['Vertices'])

    def UpdateVertices(self, vertices):
        pass
        #print(self.mData)

    def GetMirrorVertices(self, angleS):
        """
        N * cosA
        E * sinA
        S * 1
        """
        oldData = self.mData['Vertices']
        newData = []
        i = 0
        step = 3
        iMax = len(oldData) 
        radianS = math.radians(angleS)
        #print("%s MakeMirrorObj:" % self.mName) 
        #print(oldData)
        for i in range(0, iMax, step):
            newData.append(math.cos(radianS) * oldData[i])
            newData.append(math.sin(radianS) * oldData[i+1])
            newData.append(oldData[i+2])
        return newData

if __name__ == '__main__': 
    flyer = FCFlyer()
    print('测试通过')


