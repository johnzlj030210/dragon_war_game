1. CGBitmapTemplet->CGBitmapTempletWarrior, ��������ģ�壬�����佣��ʿ�������֡�����ȡ�
2. IGArmed->IGArmedSword, Ϊ��ʿ�����ӿ�
	IGArmed->IGArmedBow, Ϊ����ӿ�
3. CGCar->CGWarrior, Ϊ������࣬���ƶ�����վ��������������ð�̡�
	��ʿ�������֡�����Ĺ�ͬ���ԣ�վ��ʱ���ұ任���ƣ�������ʬ������ʧ��
4. �����佣��ʿ�� CGWarriorSword<-------CGWarrior
									|---IGArmedSword
	���幭������  CGWarriorBowman<------CGWarrior
									|---IGArmedArrow
5. VectorDirection()�����������ƴ���߾��ȸ������㡣
6. ����IGWalkAutoAx�࣬��A*�㷨Ѱ��·����
	IGWalkAutoAx<---IGWalkAuto<---IGWalk
	IGWalk�ṩ����ӿڣ�IGWalkAuto�ṩWalkAutoFindPath()�ӿڣ��ֱ�Գ����Զ���ʻ���ֶ���ʻ
	�ṩ�����㷨��ܣ�����·��Ѱ���㷨��IGWalkAutoAx����FindNextStop()��FindNextStep()ʵ�֡�
	FindNextStop()ʵ��A*�㷨��
7. ���Ŀ�ĵ�������������޷���Ϊ���Ŀ�꣬����Ӧ�����Ա�ͣ����
8. ����pTemplate::m_tall��������ʾfocus bar����ģ�嶥����λ�á�����ʾ��ɫbarʱ��ͨ���˱���
	�õ�bar��λ�á�
9. ���ӹ�����CGWarriorBowman����CGAreaMenu::LoadAllButtons()��CGAreaMenu����������CGWarriorBowman
	��Ŧ�������˰�Ŧʱ������IGGrowProduce::Produce()����m_product_id. Ȼ����IGGrowProduce::
	TimerCallback_10MS()������m_product_id������Ӧ����
	���õķ�������LoadAllButtons()ʱ��ÿ��buttun��������ϸ���ԡ�
10. CGBitmapTempletCavalry/CGBitmapTempletBowman����CGBitmapTempletWarrior, ��
	CGWarrior::UpdateView()�е���CGBitmapTempletWarrior::BitmapBlockLayout()ʱ���Լ򻯡�
11. CGWarriorBowman<--------CGWarrior
						|<--IGArmedArrow<-----IGArmedCannon<----IGArmed
	����IGArmedArrow����IGArmedCannon::Attack(), �Ա������ʱ�ı����ơ�
	IGArmedArrow��IGArmedCannonӵ��һ��CGBullet����
	�ö���켣��IGArmedCannon��ͬ��
12. ���⻹�����CGMissile������켣��
13. IGArmedArrow����CGArrow����, ������CGBullet��
	CGArrow<---CGBullet
	CGArrow����FindNextStep()���Բ�������Ĺ켣���������ڵ��켣��
14. �������������ö������ G_BULLET_TYPE::g_arrow_1������bool CGBitmapTempletBullet::SourceBlockFetch
	(int type, int index, RECT & rect, int dir)������dirΪCBullet::m_angleȷ���������ͷ
	�ķ���
15. PocketPC��ֲ: 
	1. bmp�ļ�import��rc�ļ�, ����ID, ��bitmap_loadװ�ظ�ID, �����ļ���װ��.
	2. ppc��surface��16bit, CreateSurface����������surface, ��primary surface
		�Ĵ�С���ܸı�, ����surface��������ָ��.
	3. ppc��surface��16bit, �����ʵ�ַ�����ʾ����ʱ, ��LockSurface֮��, Ҫ���
		16bit(RGB-565)����ɫ. ����ɫֵ���Դ�24bit(RGB-888)�ض̻��.
				a1a2a3a4a5a6a7a8 | b1b2b3b4b5b6b7b8 | c1c2c3c4c5c6c7c8
			->	a1a2a3a4a5		 | b1b2b3b4b5b6		| c1c2c3c4c5
			->	a1a2a3a4 | a5b1b2b3 | b4b5b6c1 | c2c3c4c5
16. VS.2003ת��ΪVS.2005���̣�����ȱ��ddraw.h, ddraw.lib, dxguid.lib, quartz.lib, strmiids.lib���ļ����ɴ�
	VS.2003��DXSDK��copy��

