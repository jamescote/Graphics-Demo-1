OBJS = main.cpp PSpiral.cpp SDiamonds.cpp STriangle.cpp CmdHandler.cpp InputHandler.cpp Graphic.cpp GraphicsManager.cpp Matrix2D.cpp Matrix3D.cpp Vector2D.cpp Vector3D.cpp Image.cpp Mouse_Handler.cpp ImageReader.cpp Tess_Graphic.cpp Curve.cpp Text.cpp GlyphExtractor.cpp ShaderManager.cpp GeometryManager.cpp Shader.cpp ScrollingText.cpp 
GLFLAGS = -lglfw -lGL -lGraphicsMagick++ -lfreetype -o Assignment3
INC = -I/usr/include/GraphicsMagick -I/usr/include/freetype2
PREFLAGS = -std=c++11 -finput-charset=iso-8859-1

#GraphicsManager.o: GraphicsManager.cpp
#	g++ $(PREFLAGS) GraphicsManager.cpp $(GLFLAGS)

#Graphic.o: Graphic.cpp 
#	g++ $(PREFLAGS) Graphic.cpp $(GLFLAGS)

#CmdHandler.o: CmdHandler.cpp 
#	g++ $(PREFLAGS) CmdHandler.cpp $(GLFLAGS)

#InputHandler.o: InputHandler.cpp 
#	g++ $(PREFLAGS) InputHandler.cpp $(GLFLAGS)

#SDiamonds.o: SDiamonds.cpp 
#	g++ $(PREFLAGS) SDiamonds.cpp $(GLFLAGS)

assign: $(OBJS)
	g++ $(PREFLAGS) $(OBJS) -g $(GLFLAGS) $(INC)

clean: 
	\rm *.o *~ Assignment1


