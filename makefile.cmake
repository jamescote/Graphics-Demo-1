OBJS = Graphic.o GraphicsManager.o CmdHandler.o InputHandler.o SDiamonds.o stdafx.o

p1: $(OBJS)
	g++ -Wall -c -g $(OBJS) -o p1

GraphicsManager.o: GraphicsManager.cpp GraphicsManager.h stdafx.h GraphicsHeader.h SDiamonds.h
	g++ -Wall -c GraphicsManager.cpp
	
Graphic.o: Graphic.cpp Graphic.h GraphicsHeader.h stdafx.h
	g++ -Wall -c Graphic.cpp

CmdHandler.o: CmdHandler.cpp CmdHandler.h stdafx.h InputHandler.h GraphicsHeader.h
	g++ -Wall -c CmdHandler.cpp

InputHandler.o: InputHandler.cpp InputHandler.h 
	g++ -Wall -c InputHandler.cpp
	
SDiamonds.o: SDiamonds.cpp SDiamonds.h Graphic.h
	g++ -Wall -c SDiamonds.cpp
	
stdafx.o: stdafx.cpp stdafx.h
	g++ -Wall -c stdafx.cpp
	
clean: 
	\rm *.o *~ p1