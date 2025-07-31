#include<bits/stdc++.h>
#include<raylib.h>
using namespace std;
const int screenWidth=1600,screenHeight=1200;//屏幕分辨率
float dis(float x,float y)//距离
{
	x=x-screenWidth/2,y=y-screenHeight/2;
	float len=sqrt(x*x+y*y);
	return len;
}
int main()
{
	InitWindow(screenWidth,screenHeight,"Mlocc.io Games");//初始化窗口
	SetTargetFPS(60);//设置帧率
	float PlayerRadius=5.0f;//玩家半径
	float MapRadius=1000.0f;//地图半径
	Vector2 playerPosition={screenWidth/2.0f,screenHeight/2.0f};//玩家坐标
	Camera2D camera={0};//相机
	camera.target=playerPosition;//相机跟随玩家
	camera.offset={screenWidth/2.0f,screenHeight/2.0f};//固定玩家为相机中心
	camera.rotation=0.0f;//初始旋转角度为 0
	camera.zoom=10.0f;//初始缩放比例
	float Speed=5.0f;//玩家移动速度
	//游戏主循环
	while(!WindowShouldClose())
	{
		Vector2 mousePosition=GetMousePosition();//鼠标坐标
		//绝对鼠标坐标
		Vector2 transformedMousePosition=
		{
			(mousePosition.x-camera.offset.x)/camera.zoom+camera.target.x,
			(mousePosition.y-camera.offset.y)/camera.zoom+camera.target.y
		};
		//绝对鼠标方向
		Vector2 direction={transformedMousePosition.x-playerPosition.x,transformedMousePosition.y-playerPosition.y};
		//绝对鼠标距离
		float length=sqrtf(direction.x*direction.x+direction.y*direction.y);
		//玩家移动
		if(length>0)
		{
			float len=dis(playerPosition.x,playerPosition.y);
			if(len>MapRadius-10)
			{
				float ppo=(MapRadius-10)/len;
				playerPosition.x=playerPosition.x*ppo,playerPosition.y=playerPosition.y*ppo;
			}else
			{
				direction.x/=length;
				direction.y/=length;
				playerPosition.x+=direction.x*Speed;
				playerPosition.y+=direction.y*Speed;
			}
		}
		//缩放（后将根据触角花瓣改变视距）
		if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			float wheelMove=GetMouseWheelMove();
			if(wheelMove!=0)
			{
				camera.zoom+=wheelMove*0.01f;
				if(camera.zoom<0.1f)camera.zoom=0.1f;
				if(camera.zoom>2.0f)camera.zoom=2.0f;
			}
		}
		camera.target=playerPosition;
		BeginDrawing();
		//——————————————————————————————开始绘画——————————————————————————————
		ClearBackground(BLACK);//背景
		BeginMode2D(camera);//开启相机
		DrawCircle(screenWidth/2,screenHeight/2,MapRadius,ColorAlpha(DARKGRAY,0.7f));//地图
		Color pointerColor=GRAY;//指针颜色
		float lineThickness=PlayerRadius*0.75f;//指针粗细
		DrawLineEx(playerPosition,transformedMousePosition,lineThickness,pointerColor);//绘制指针
		DrawCircleV(playerPosition,PlayerRadius,YELLOW);//玩家绘制
		EndMode2D();//结束相机
		DrawCircle(screenWidth-200,200,160,ColorAlpha(BLACK,0.75f));//地图边框
		DrawCircle(screenWidth-200,200,150,ColorAlpha(GRAY,0.75f));//地图
		DrawText("Maps",screenWidth-240,365,30,WHITE);//地图文字
		string Position_View=to_string((int)playerPosition.x/10)+","+to_string((int)playerPosition.y/10);
		int Position_View_Len=Position_View.size();
		DrawText(Position_View.c_str(),screenWidth-235-(Position_View_Len-5)*5,400,25,BLUE);//地图文字
		Vector2 miniPlayerPosition={(playerPosition.x-screenWidth/2)/MapRadius*150,(playerPosition.y-screenHeight/2)/MapRadius*150};
		DrawCircle(screenWidth-200+miniPlayerPosition.x,200+miniPlayerPosition.y,10,ColorAlpha(YELLOW,0.75f));//玩家小地图绘制
		//当前帧率
		int fps=GetFPS();
		DrawText(TextFormat("FPS: %d",fps),10,10,20,WHITE);
		//当前视野
		DrawText(TextFormat("Zoom: %.2f",camera.zoom),10,40,20,WHITE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

