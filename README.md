用Qt5实现的轴承计算程序的一部分，用于绘制并管理图形曲线。

<div align=center>
	<image src="README/drawingsManagerTest.png" width = "40%"/>
</div>

其中二维曲线是对QtCharts的包装，三维曲线是用Opengl实现的，支持拖动旋转、自动调整刻度和坐标轴范围。点击载入数据会发送一个信号，设置处理函数可以实现选择文件载入数据。下面是绘图展示。

<div align=center>
	<image src="README/rotatePresentation.gif" width = "40%"/><image src="README/resizePresentation.gif" width = "40%"/>
</div>

<div align=center>
	<image src="README/2dDrawingTest.png" width = "40%"/><image src="README/3dDrawingTest.png" width = "40%"/>
</div>

支持各种绘图设置。

<div align=center>
	<image src="README/createDrawing.png" width = "30%"/><image src="README/addCurve.png" width = "33%"/><image src="README/3dDrawing.png" width = "30%"/>
</div>

