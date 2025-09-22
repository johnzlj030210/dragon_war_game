1. CGBitmapTemplet->CGBitmapTempletWarrior, 建造人物模板，包括配剑武士、弓箭手、骑兵等。
2. IGArmed->IGArmedSword, 为武士击剑接口
	IGArmed->IGArmedBow, 为射箭接口
3. CGCar->CGWarrior, 为人物基类，能移动，能站立，会死亡，不冒烟。
	武士、弓箭手、骑兵的共同特性：站立时左右变换姿势，死亡后尸体逐渐消失。
4. 定义配剑武士类 CGWarriorSword<-------CGWarrior
									|---IGArmedSword
	定义弓箭手类  CGWarriorBowman<------CGWarrior
									|---IGArmedArrow
5. VectorDirection()用整型数左移代替高精度浮点运算。
6. 创建IGWalkAutoAx类，用A*算法寻找路径。
	IGWalkAutoAx<---IGWalkAuto<---IGWalk
	IGWalk提供抽象接口，IGWalkAuto提供WalkAutoFindPath()接口，分别对车辆自动行驶和手动驾驶
	提供建立算法框架，具体路径寻找算法在IGWalkAutoAx中由FindNextStop()和FindNextStep()实现。
	FindNextStop()实现A*算法。
7. 如果目的地有物体存在且无法设为设计目标，则车辆应在其旁边停靠。
8. 定义pTemplate::m_tall变量，表示focus bar距离模板顶部的位置。当显示角色bar时，通过此变量
	得到bar的位置。
9. 增加弓箭手CGWarriorBowman对象。CGAreaMenu::LoadAllButtons()在CGAreaMenu画面上增加CGWarriorBowman
	按纽。当按此按纽时，调用IGGrowProduce::Produce()设置m_product_id. 然后在IGGrowProduce::
	TimerCallback_10MS()根据其m_product_id创建相应对象。
	更好的方案是在LoadAllButtons()时对每个buttun设置其详细属性。
10. CGBitmapTempletCavalry/CGBitmapTempletBowman基于CGBitmapTempletWarrior, 在
	CGWarrior::UpdateView()中调用CGBitmapTempletWarrior::BitmapBlockLayout()时得以简化。
11. CGWarriorBowman<--------CGWarrior
						|<--IGArmedArrow<-----IGArmedCannon<----IGArmed
	其中IGArmedArrow重载IGArmedCannon::Attack(), 以便在射击时改变姿势。
	IGArmedArrow和IGArmedCannon拥有一个CGBullet对象。
	该对象轨迹与IGArmedCannon不同。
12. 另外还可造出CGMissile导弹类轨迹。
13. IGArmedArrow中有CGArrow对象, 派生于CGBullet。
	CGArrow<---CGBullet
	CGArrow重载FindNextStep()，以产生射箭的轨迹，而不是炮弹轨迹。
14. 对于射箭，增加枚举类型 G_BULLET_TYPE::g_arrow_1，增加bool CGBitmapTempletBullet::SourceBlockFetch
	(int type, int index, RECT & rect, int dir)，其中dir为CBullet::m_angle确定，代表箭头
	的方向。
15. PocketPC移植: 
	1. bmp文件import到rc文件, 定义ID, 用bitmap_load装载该ID, 代替文件名装载.
	2. ppc的surface是16bit, CreateSurface生成任意多个surface, 但primary surface
		的大小不能改变, 其他surface可以任意指定.
	3. ppc的surface是16bit, 因此现实字符或显示画面时, 在LockSurface之后, 要填充
		16bit(RGB-565)的颜色. 该颜色值可以从24bit(RGB-888)截短获得.
				a1a2a3a4a5a6a7a8 | b1b2b3b4b5b6b7b8 | c1c2c3c4c5c6c7c8
			->	a1a2a3a4a5		 | b1b2b3b4b5b6		| c1c2c3c4c5
			->	a1a2a3a4 | a5b1b2b3 | b4b5b6c1 | c2c3c4c5
16. VS.2003转化为VS.2005工程，后者缺少ddraw.h, ddraw.lib, dxguid.lib, quartz.lib, strmiids.lib等文件，可从
	VS.2003或DXSDK中copy。

