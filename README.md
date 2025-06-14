用Qt5实现的轴承计算程序的一部分，用于绘制并管理图形曲线。其中二维曲线是对QtCharts的包装，三维曲线是用Opengl实现的，能够拖动旋转。

<center>
	<image src="README/drawingsManager.png" width = "20%">
	<image src="README/drawingsManagerTest.png" width = "20%">
</center>

<center>
	<image src="README/createDrawing.png" width = "20%">
	<image src="README/addCurve.png" width = "20%">
	<image src="README/3dDrawing.png" width = "20%">
</center>

点击载入数据会发送一个信号，设置处理函数可以实现选择文件载入数据，下面是绘图测试的结果。

<center>
	<image src="README/2dDrawingTest.png" width = "20%">
	<image src="README/3dDrawingTest.png" width = "20%">
</center>
