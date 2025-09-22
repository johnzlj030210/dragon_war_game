#pragma once
#include "typedefines.h"

class CGBitmapTemplet
{
public:
	CGBitmapTemplet(void);
	virtual ~CGBitmapTemplet(void);
protected:
	char *FileNameBuffer;
	int m_blockNumX;
	int m_blockNumY;
	int m_blockSizeX;
	int m_blockSizeY;
	// the real location m_size of the building
	RECT m_size;
public:
	char * GetFileName(void);
protected:
	// contains the block file bitmap
	GHANDLE surface;
public:
	// Get the block width
	int GetBlockWidth(void);
	// Get the block height
	int GetBlockHeight(void);
	// the real location m_size of the car
	RECT GetSize(void);
protected:
	// transfer the srcRect into surface with boundary processing
	void SourceBlockMaptoSurface(GHANDLE dstSurf, RECT & rect, GPOINT & pos, RECT & srcRect);
	// distance to the block top, used in focus bar
public:
	int m_tall;
};

class CGBitmapTempletMap :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletMap(char *fileName);
	CGBitmapTempletMap(DWORD rcName);
	~CGBitmapTempletMap(void);

public:
	// put a block on rect block(x,y) with origin offset
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & origin, int x, int y);
	// Get the rect from the map block bmp file.
	void SourceBlockFetch(int blockX, int blockY, RECT & rect);
private:
	char *m_art_tree;
};

class CGBitmapTempletCar :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletCar(char *fileName);
	CGBitmapTempletCar(DWORD rcName);
	~CGBitmapTempletCar(void);

	// put a block on rect with origin offset
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & origin, int angle);
	// Get the sub-block according to angle
	void SourceBlockFetch(int angle, RECT & rect);
};

class CGBitmapTempletBuild :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletBuild(void){};
	CGBitmapTempletBuild(char *fileName);
	CGBitmapTempletBuild(DWORD rcName);
	~CGBitmapTempletBuild(void);

	// Set many buildings in the map
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, BUILD_ID build_id, int blood);
	// Fetch the building block in the template file
	virtual void SourceBlockFetch(BUILD_ID build_id, int persent, RECT & rect);
};

class CGBitmapTempletSmoke :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletSmoke(char *fileName);
	CGBitmapTempletSmoke(DWORD rcName);
	virtual ~CGBitmapTempletSmoke(void);
	// draw bitmap block-wise
	void BitmapBlockLayout(GHANDLE dstSurf , RECT & rect, GPOINT & pos, int index_x, int index_y);
protected:
	// access the proper block in the template bitmap
	bool SourceBlockFetch(int index_x, int index_y, RECT & rect);
};

class CGBitmapTempletBomb :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletBomb(char *fileName);
	CGBitmapTempletBomb(DWORD rcName);
	~CGBitmapTempletBomb(void);
	// draw the bomb block-wise
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int index);
protected:
	// access the proper block in the template bitmap
	bool SourceBlockFetch(int index, RECT & rect);
};

class CGBitmapTempletBullet :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletBullet(char *fileName);
	CGBitmapTempletBullet(DWORD rcName);
	~CGBitmapTempletBullet(void);
	// draw the bomb block-wise
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int type, int index, int dir = 0);
protected:
	// access the proper block in the template bitmap
	bool SourceBlockFetch(int type, int index, RECT & rect);
	bool SourceBlockFetch(int type, int index, RECT & rect, int dir);
};

class CGBitmapTempletPhoto :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletPhoto(char *fileName, int width, int height);
	CGBitmapTempletPhoto(DWORD rcName, int width, int height);
	~CGBitmapTempletPhoto(void);
// draw the photo
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect);
};

class CGBitmapTempletConstructor :
	public CGBitmapTempletBuild
{
public:
	CGBitmapTempletConstructor(char *fileName);
	CGBitmapTempletConstructor(DWORD rcName);
	~CGBitmapTempletConstructor(void);

	// Fetch the building block in the template file
	void SourceBlockFetch(BUILD_ID build_id, int persent, RECT & rect);
};

class CGBitmapTempletFlag :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletFlag(char *fileName);
	CGBitmapTempletFlag(DWORD rcName);
	~CGBitmapTempletFlag(void);
	// draw the flag block-wise
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int type, int index);
protected:
	// access the proper block in the template bitmap
	bool SourceBlockFetch(int type, int index, RECT & rect);
};

class CGBitmapTempletLibs :
	public CGBitmapTemplet
{
public:
	CGBitmapTempletLibs(char *fileName);
	CGBitmapTempletLibs(DWORD rcName);
	~CGBitmapTempletLibs(void);
public:
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, int index);
	void SourceBlockFetch(int index, RECT & rect);
};

class CGBitmapTempletWarrior :
	public CGBitmapTemplet
{
	friend class CGWarrior;
public:
	CGBitmapTempletWarrior(void){};
	CGBitmapTempletWarrior(char *fileName);
	CGBitmapTempletWarrior(DWORD rcName);
	~CGBitmapTempletWarrior(void);
public:
	// put a block on rect with origin offset
	void BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, 
		GPOINT & origin, int angle,
		G_WARRIOR_POSTURE posture, int step);
	// Get the sub-block according to angle
	void SourceBlockFetch(int angle, G_WARRIOR_POSTURE posture, int step, RECT & rect);
protected:
	int m_block_stride_standby;
	int m_block_stride_walk;
	int m_block_stride_stick;
	int m_block_stride_shoot;
	int m_block_stride_fire;
};

class CGBitmapTempletCavalry :
	public CGBitmapTempletWarrior
{
public:
	CGBitmapTempletCavalry(char *fileName);
	CGBitmapTempletCavalry(DWORD rcName);
	~CGBitmapTempletCavalry(void);
};

class CGBitmapTempletBowman :
	public CGBitmapTempletWarrior
{
public:
	CGBitmapTempletBowman(char *fileName);
	CGBitmapTempletBowman(DWORD rcName);
	~CGBitmapTempletBowman(void);
};

class CGBitmapTempletChariot :
	public CGBitmapTempletWarrior
{
public:
	CGBitmapTempletChariot(char *fileName);
	CGBitmapTempletChariot(DWORD rcName);
	~CGBitmapTempletChariot(void);
};
