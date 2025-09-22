#include "StdAfx.h"
#include ".\gbitmapTemplet.h"
#include "..\wrapper\\wrapper.h"
#include ".\gameload.h"
#include "common.h"

CGBitmapTemplet::CGBitmapTemplet(void)
: m_blockNumX(0)
, m_blockNumY(0)
, m_blockSizeX(0)
, m_blockSizeY(0)
, m_tall(0)
{
	FileNameBuffer = new char[50];
	memset(&m_size, 0, sizeof(m_size));
}

CGBitmapTemplet::~CGBitmapTemplet(void)
{
	delete FileNameBuffer;
}

CGBitmapTempletMap::CGBitmapTempletMap(char *fileName)
: m_art_tree(NULL)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 32;
	m_blockSizeY = 32;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);

	m_art_tree = new char[50*50];
	memset(m_art_tree, 0, 50*50);
	print_string_byte(m_art_tree+50*10+5, 50, "¼Ñ Áú\0");
//	print_string_byte(m_art_tree+50*26+5, 50, "»¨ Ô°\0");
}

CGBitmapTempletMap::CGBitmapTempletMap(DWORD rcName)
: m_art_tree(NULL)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 32;
	m_blockSizeY = 32;
	//STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(rcName);

	m_art_tree = new char[50*50];
	memset(m_art_tree, 0, 50*50);
	print_string_byte(m_art_tree+50*10+5, 50, "¼Ñ Áú\0");
//	print_string_byte(m_art_tree+50*26+5, 50, "»¨ Ô°\0");
}

CGBitmapTempletMap::~CGBitmapTempletMap(void)
{
	GUnloadBitmapSurface(surface);
	delete m_art_tree;
}

// put a block on rect block(x,y) with origin offset
void CGBitmapTempletMap::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & origin, int x, int y)
{
	RECT srcRect;
	GPOINT pos;

	pos.x = m_blockSizeX * x - origin.x;
	pos.y = m_blockSizeY * y - origin.y;

	SourceBlockFetch(x, y, srcRect);
	SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
}

// Get the rect from the map block bmp file.
void CGBitmapTempletMap::SourceBlockFetch(int blockX, int blockY, RECT & rect)
{
	if(blockX==0 && blockY==0){
		rect.left = 128;
		rect.top = 640;
	}else if(blockX==0 && blockY==49){
		rect.left = 128;
		rect.top = 640;
	}else if(blockX==49 && blockY==0){
		rect.left = 128;
		rect.top = 640;
	}else if(blockX==49 && blockY==49){
		rect.left = 128;
		rect.top = 640;
	}else if(blockX==0){
		rect.left = 64;
		rect.top = 640;
	}else if(blockY==0){
		rect.left = 96;
		rect.top = 640;
	}else if(blockX==49){
		rect.left = 64;
		rect.top = 640;
	}else if(blockY==49){
		rect.left = 96;
		rect.top = 640;
	}else if(blockY==10 || blockY == 30){
		rect.left = 0;
		rect.top = 1984;
	}else if(blockX==10 || blockX == 30){
		rect.left = 0;
		rect.top = 1984;
	}else{
		rect.left = 0;
		rect.top = 0;
	}
	if(m_art_tree[blockY*50+blockX]==1){
		rect.left = 96;
		rect.top = 1984;
	}
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
}

CGBitmapTempletCar::CGBitmapTempletCar(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;
	m_size.left = 15;
	m_size.right = 49;
	m_size.top = 5;
	m_size.bottom = 59;

	m_tall = 5;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletCar::CGBitmapTempletCar(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;
	m_size.left = 15;
	m_size.right = 49;
	m_size.top = 5;
	m_size.bottom = 59;

	m_tall = 5;

	//STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(rcName);
}


CGBitmapTempletCar::~CGBitmapTempletCar(void)
{
	GUnloadBitmapSurface(surface);
}
char * CGBitmapTemplet::GetFileName(void)
{
	return FileNameBuffer;
}

// the real location m_size of the car
RECT CGBitmapTemplet::GetSize(void)
{
	return m_size;
}

// put a block on rect block(x,y) with origin offset
void CGBitmapTempletCar::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & origin, int angle)
{
	RECT srcRect;

	SourceBlockFetch(angle, srcRect);
	SourceBlockMaptoSurface(dstSurf, rect, origin, srcRect);
}

// Get the sub-block according to angle
void CGBitmapTempletCar::SourceBlockFetch(int angle, RECT & rect)
{
	switch(angle)
	{
	case 0:
		rect.left = 0;
		rect.top  = 0;
		break;
	case 1:
		rect.left = m_blockSizeX;
		rect.top  = 0;
		break;
	case 2:
		rect.left = m_blockSizeX*2;
		rect.top  = 0;
		break;
	case 3:
		rect.left = 0;
		rect.top  = m_blockSizeY;
		break;
	case 4:
		rect.left = m_blockSizeX;
		rect.top  = m_blockSizeY;
		break;
	case 5:
		rect.left = m_blockSizeX*2;
		rect.top  = m_blockSizeY;
		break;
	case 6:
		rect.left = 0;
		rect.top  = m_blockSizeY*2;
		break;
	case 7:
		rect.left = m_blockSizeX;
		rect.top  = m_blockSizeY*2;
		break;
	default:
		break;
	}
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
}

// Get the block width
int CGBitmapTemplet::GetBlockWidth(void)
{
	return m_blockSizeX;
}

// Get the block height
int CGBitmapTemplet::GetBlockHeight(void)
{
	return m_blockSizeY;
}

// transfer the srcRect into surface with boundary processing
void CGBitmapTemplet::SourceBlockMaptoSurface(GHANDLE dstSurf, RECT & rect, GPOINT & pos, RECT & srcRect)
{
	RECT dstRect;

	dstRect.left = rect.left + pos.x;
	dstRect.top = rect.top + pos.y;
	dstRect.right = dstRect.left + (srcRect.right - srcRect.left);
	dstRect.bottom = dstRect.top + (srcRect.bottom - srcRect.top);

	if(dstRect.right < rect.left ||
		dstRect.bottom < rect.top ||
		dstRect.left > rect.right ||
		dstRect.top > rect.bottom)
		return;
	if(dstRect.left<rect.left){
		srcRect.left += (rect.left - dstRect.left);
		dstRect.left = rect.left;
	}
	if(dstRect.top<rect.top){
		srcRect.top += (rect.top - dstRect.top);
		dstRect.top = rect.top;
	}
	if(dstRect.right>rect.right){
		srcRect.right -= (dstRect.right - rect.right);
		dstRect.right = rect.right;
	}
	if(dstRect.bottom>rect.bottom){
		srcRect.bottom -= (dstRect.bottom - rect.bottom);
		dstRect.bottom = rect.bottom;
	}

	GSurfaceTransfer(dstSurf, surface, dstRect, srcRect, true);
}

// Set many buildings in the map
void CGBitmapTempletBuild::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, BUILD_ID build_id, int persent)
{
	RECT srcRect;

	SourceBlockFetch(build_id, persent, srcRect);
	SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
}

// Fetch the building block in the template file
void CGBitmapTempletBuild::SourceBlockFetch(BUILD_ID build_id, int persent, RECT & rect)
{
	if(build_id == 1){
		rect.left = 128;
		rect.top = 704;
		rect.right = rect.left + 64;
		rect.bottom = rect.top + 64;
	}else if(build_id == 2){
		rect.left = 64;
		rect.top = 450;
		rect.right = rect.left + 64;
		rect.bottom = rect.top + 64;
	}else if(build_id == 3){
		rect.left = 0;
		rect.top = 1248;
		rect.right = rect.left + 96;
		rect.bottom = rect.top + 96;
	}else if(build_id == 4){
		rect.left = 256;
		rect.top = 0;
		rect.right = rect.left + 96;
		rect.bottom = rect.top + 96;
	}else if(build_id == 5){
		rect.left = 350;
		rect.top = 69;
		rect.right = rect.left + 64;
		rect.bottom = rect.top + 64;
	}else if(build_id == 6){
		rect.left = 380;
		rect.top = 1472;
		rect.right = rect.left + 64;
		rect.bottom = rect.top + 64;
	}else if(build_id == 7){
		rect.left = 0;
		rect.top = 160;
		rect.right = 110;
		rect.bottom = 268;
	}else if(build_id == 8){
		rect.left = 110;
		rect.top = 0;
		rect.right = 308;
		rect.bottom = 182;
	}else if(build_id == 9){
		rect.left = 115;
		rect.top = 180;
		rect.right = 305;
		rect.bottom = 305;
	}else{
		rect.left = 0;
		rect.top = 1248;
		rect.right = rect.left + 96;
		rect.bottom = rect.top + 96;
	}
}

CGBitmapTempletBuild::CGBitmapTempletBuild(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
	m_size.left = 5;
	m_size.right = 60;
	m_size.top = 5;
	m_size.bottom = 60;
}

CGBitmapTempletBuild::CGBitmapTempletBuild(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;

	surface = GLoadBitmapSurface(rcName);
	m_size.left = 5;
	m_size.right = 60;
	m_size.top = 5;
	m_size.bottom = 60;
}

CGBitmapTempletBuild::~CGBitmapTempletBuild(void)
{
	GUnloadBitmapSurface(surface);
}

CGBitmapTempletSmoke::~CGBitmapTempletSmoke(void)
{
	GUnloadBitmapSurface(surface);
}

CGBitmapTempletSmoke::CGBitmapTempletSmoke(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletSmoke::CGBitmapTempletSmoke(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 64;
	m_blockSizeY = 64;

	surface = GLoadBitmapSurface(rcName);
}

// draw bitmap block-wise
void CGBitmapTempletSmoke::BitmapBlockLayout(GHANDLE dstSurf , RECT & rect, GPOINT & origin, 
											   int index_x, int index_y)
{
	RECT srcRect;

	if(SourceBlockFetch(index_x, index_y, srcRect))
		SourceBlockMaptoSurface(dstSurf, rect, origin, srcRect);
}

// access the proper block in the template bitmap
bool CGBitmapTempletSmoke::SourceBlockFetch(int index_x, int index_y, RECT & rect)
{
	if(index_x<0 || index_x>7 || index_y<0 || index_y>4)
		return false;
	rect.left = index_x * m_blockSizeX;
	rect.top = index_y * m_blockSizeY;
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
	return true;
}

CGBitmapTempletBomb::CGBitmapTempletBomb(char *fileName)
{
	m_blockNumX = 2;
	m_blockNumY = 2;
	m_blockSizeX = 64;
	m_blockSizeY = 64;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletBomb::CGBitmapTempletBomb(DWORD rcName)
{
	m_blockNumX = 2;
	m_blockNumY = 2;
	m_blockSizeX = 64;
	m_blockSizeY = 64;

	//STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(rcName);
}

CGBitmapTempletBomb::~CGBitmapTempletBomb(void)
{
	GUnloadBitmapSurface(surface);
}

// draw the bomb block-wise
void CGBitmapTempletBomb::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int index)
{
	RECT srcRect;

	if(SourceBlockFetch(index, srcRect))
		SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
}

// access the proper block in the template bitmap
bool CGBitmapTempletBomb::SourceBlockFetch(int index, RECT & rect)
{
	switch(index)
	{
	case 0:
		rect.left = 0;
		rect.top = 0;
		break;
	case 1:
		rect.left = 0;
		rect.top = 0;
		break;
	case 2:
		rect.left = m_blockSizeX;
		rect.top = 0;
		break;
	case 3:
		rect.left = 0;
		rect.top = m_blockSizeY;
		break;
	case 4:
		rect.left = m_blockSizeX;
		rect.top = m_blockSizeY;
		break;
	default:
		return false;
		
	}
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
	return true;
}

CGBitmapTempletBullet::CGBitmapTempletBullet(char *fileName)
{
	m_blockNumX = 4;
	m_blockNumY = 4;
	m_blockSizeX = 16;
	m_blockSizeY = 16;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletBullet::CGBitmapTempletBullet(DWORD rcName)
{
	m_blockNumX = 4;
	m_blockNumY = 4;
	m_blockSizeX = 16;
	m_blockSizeY = 16;

	surface = GLoadBitmapSurface(rcName);
}

CGBitmapTempletBullet::~CGBitmapTempletBullet(void)
{
	GUnloadBitmapSurface(surface);
}

// draw the bomb block-wise, int type---the bullet type
void CGBitmapTempletBullet::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int type, int index, int dir/*=0*/)
{
	RECT srcRect;

	switch(type)
	{
	case g_bullet_1:
	case g_bullet_2:
		if(SourceBlockFetch(type-1, index, srcRect))
			SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
		break;
	case g_arrow_1:
		if(SourceBlockFetch(type-1, index, srcRect, dir))
			SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
		break;
	default:
		;
	}
}

// access the proper block in the template bitmap
bool CGBitmapTempletBullet::SourceBlockFetch(int type, int index, RECT & rect)
{
	switch(index)
	{
	case 0:
		rect.left = 0;
		rect.top = 0;
		break;
	case 1:
		rect.left = m_blockSizeX;
		rect.top = 0;
		break;
	case 2:
		rect.left = m_blockSizeY*2;
		rect.top = 0;
		break;
	case 3:
		rect.left = m_blockSizeX*3;
		rect.top = 0;
		break;
	default:
		return false;
		
	}
	rect.top += type*m_blockSizeY;
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
	return true;
}

// access the proper block in the template bitmap
bool CGBitmapTempletBullet::SourceBlockFetch(int type, int index, RECT & rect, int dir)
{
	switch(dir)
	{
	case 0:
	case 4:
		rect.left = 0;
		rect.top = 0;
		break;
	case 1:
	case 5:
		rect.left = m_blockSizeX;
		rect.top = 0;
		break;
	case 2:
	case 6:
		rect.left = m_blockSizeY*2;
		rect.top = 0;
		break;
	case 3:
	case 7:
		rect.left = m_blockSizeX*3;
		rect.top = 0;
		break;
	default:
		return false;
		
	}
	rect.top += type*m_blockSizeY;
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
	return true;
}

CGBitmapTempletPhoto::CGBitmapTempletPhoto(char *fileName, int width, int height)
{
	m_blockNumX = 1;
	m_blockNumY = 1;
	m_blockSizeX = width;
	m_blockSizeY = height;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletPhoto::CGBitmapTempletPhoto(DWORD rcName, int width, int height)
{
	m_blockNumX = 1;
	m_blockNumY = 1;
	m_blockSizeX = width;
	m_blockSizeY = height;

	surface = GLoadBitmapSurface(rcName);
}

CGBitmapTempletPhoto::~CGBitmapTempletPhoto(void)
{
	GUnloadBitmapSurface(surface);
}

// draw the photo
void CGBitmapTempletPhoto::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect)
{
	RECT dstRect = rect;//destination rect
	RECT srcRect = {0,0,m_blockSizeX, m_blockSizeY};

	GSurfaceTransfer(dstSurf, surface, dstRect, srcRect, true);
}

CGBitmapTempletConstructor::CGBitmapTempletConstructor(char *fileName)
{
	m_blockNumX = 5;
	m_blockNumY = 2;
	m_blockSizeX = 160;
	m_blockSizeY = 256;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
	m_size.left = 5;
	m_size.right = 156;
	m_size.top = 5;
	m_size.bottom = 250;
}

CGBitmapTempletConstructor::CGBitmapTempletConstructor(DWORD rcName)
{
	m_blockNumX = 5;
	m_blockNumY = 2;
	m_blockSizeX = 160;
	m_blockSizeY = 256;

	surface = GLoadBitmapSurface(rcName);
	m_size.left = 5;
	m_size.right = 156;
	m_size.top = 5;
	m_size.bottom = 250;
}

CGBitmapTempletConstructor::~CGBitmapTempletConstructor(void)
{
	GUnloadBitmapSurface(surface);
}

// Fetch the building block in the template file
void CGBitmapTempletConstructor::SourceBlockFetch(BUILD_ID build_id, int persent, RECT & rect)
{
	int subId = persent/25;

	rect.left = 0;
	rect.top = (build_id-1)*256;

	rect.left += (160*subId);
	rect.right = rect.left + 160;
	rect.bottom = rect.top + 256;
}


CGBitmapTempletFlag::CGBitmapTempletFlag(char *fileName)
{
	m_blockNumX = 7;
	m_blockNumY = 2;
	m_blockSizeX = 32;
	m_blockSizeY = 32;

	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletFlag::CGBitmapTempletFlag(DWORD rcName)
{
	m_blockNumX = 7;
	m_blockNumY = 2;
	m_blockSizeX = 32;
	m_blockSizeY = 32;

	surface = GLoadBitmapSurface(rcName);
}

CGBitmapTempletFlag::~CGBitmapTempletFlag(void)
{
	GUnloadBitmapSurface(surface);
}

// access the proper block in the template bitmap
bool CGBitmapTempletFlag::SourceBlockFetch(int type, int index, RECT & rect)
{
	rect.left = index*32;
	rect.top = type*32;
	rect.right = rect.left + 32;
	rect.bottom = rect.top + 32;
	return true;
}

void CGBitmapTempletFlag::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, GPOINT & pos, int type, int index)
{
	RECT srcRect;

	if(SourceBlockFetch(type, index, srcRect))
		SourceBlockMaptoSurface(dstSurf, rect, pos, srcRect);
}

CGBitmapTempletLibs::CGBitmapTempletLibs(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 50;
	m_blockSizeY = 60;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);
}

CGBitmapTempletLibs::CGBitmapTempletLibs(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 50;
	m_blockSizeY = 60;

	surface = GLoadBitmapSurface(rcName);
}

CGBitmapTempletLibs::~CGBitmapTempletLibs(void)
{
	GUnloadBitmapSurface(surface);
}

// draw the sample in the lib
void CGBitmapTempletLibs::BitmapBlockLayout(GHANDLE dstSurf, RECT & rect, int index)
{
	RECT dstRect;//destination rect
	RECT srcRect;

	SourceBlockFetch(index, srcRect);

	dstRect = rect;

	GSurfaceTransfer(dstSurf, surface, dstRect, srcRect, true);
}

// Fetch the building block in the template file
void CGBitmapTempletLibs::SourceBlockFetch(int index, RECT & rect)
{
	int x = index%4;
	int y = index/4;
	rect.left = x*m_blockSizeX;
	rect.top = y*m_blockSizeY;
	rect.right = rect.left + m_blockSizeX;
	rect.bottom = rect.top + m_blockSizeY;
}

CGBitmapTempletWarrior::CGBitmapTempletWarrior(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 200;
	m_blockSizeY = 54;
	m_size.left = 82;
	m_size.right = 112;
	m_size.top = 24;
	m_size.bottom = 54;

	surface = GLoadBitmapSurface(rcName);

	m_block_stride_standby	= 5;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 15;
	m_block_stride_shoot	= 0;
	m_block_stride_fire		= 0;
}


CGBitmapTempletWarrior::CGBitmapTempletWarrior(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 200;
	m_blockSizeY = 54;
	m_size.left = 82;
	m_size.right = 112;
	m_size.top = 24;
	m_size.bottom = 54;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);

	m_block_stride_standby	= 5;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 15;
	m_block_stride_shoot	= 0;
	m_block_stride_fire		= 0;
}

CGBitmapTempletWarrior::~CGBitmapTempletWarrior(void)
{
	GUnloadBitmapSurface(surface);
}

// put a block on rect with origin offset
void CGBitmapTempletWarrior::BitmapBlockLayout(
		GHANDLE dstSurf, RECT & rect, 
		GPOINT & origin, int angle,
		G_WARRIOR_POSTURE posture, int step)
{
	RECT srcRect;

	SourceBlockFetch(angle, posture, step, srcRect);
	SourceBlockMaptoSurface(dstSurf, rect, origin, srcRect);
}
// Get the sub-block according to angle
void CGBitmapTempletWarrior::SourceBlockFetch(
		int angle, 
		G_WARRIOR_POSTURE posture, 
		int step, 
		RECT & rect)
{
	int delta = 5;
	switch(posture)
	{
	case g_warrior_standby:
		delta = m_block_stride_standby;
		break;
	case g_warrior_walk:
		delta = m_block_stride_walk;
		break;
	case g_warrior_stick:
		delta = m_block_stride_stick;
		break;
	case g_warrior_shoot:
		delta = m_block_stride_shoot;
		break;
	case g_warrior_fire:
		delta = m_block_stride_fire;
		break;
	default:
		;
	}
	if(posture == g_warrior_standby ||
		posture == g_warrior_walk)
	{
		rect.left = (int)posture*m_blockSizeX;
	}
	else
	{
		rect.left = 2*m_blockSizeX;
	}
	rect.top = (angle*delta+step)*m_blockSizeY;
	rect.right  = rect.left + m_blockSizeX;
	rect.bottom = rect.top  + m_blockSizeY;
}

CGBitmapTempletCavalry::CGBitmapTempletCavalry(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 120;
	m_blockSizeY = 80;
	m_size.left = 40;
	m_size.right = 80;
	m_size.top = 40;
	m_size.bottom = 70;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);

	m_block_stride_standby	= 13;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 13;
	m_block_stride_shoot	= 0;
	m_block_stride_fire		= 0;

	m_tall = 5;
}


CGBitmapTempletCavalry::CGBitmapTempletCavalry(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 120;
	m_blockSizeY = 80;
	m_size.left = 40;
	m_size.right = 80;
	m_size.top = 40;
	m_size.bottom = 70;

	surface = GLoadBitmapSurface(rcName);

	m_block_stride_standby	= 13;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 13;
	m_block_stride_shoot	= 0;
	m_block_stride_fire		= 0;

	m_tall = 5;
}

CGBitmapTempletCavalry::~CGBitmapTempletCavalry(void)
{
	GUnloadBitmapSurface(surface);
}


CGBitmapTempletBowman::CGBitmapTempletBowman(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 80;
	m_blockSizeY = 80;
	m_size.left = 30;
	m_size.right = 50;
	m_size.top = 40;
	m_size.bottom = 60;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);

	m_block_stride_standby	= 5;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 0;
	m_block_stride_shoot	= 10;
	m_block_stride_fire		= 0;

	m_tall = 10;
}


CGBitmapTempletBowman::CGBitmapTempletBowman(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 80;
	m_blockSizeY = 80;
	m_size.left = 30;
	m_size.right = 50;
	m_size.top = 40;
	m_size.bottom = 60;

	surface = GLoadBitmapSurface(rcName);

	m_block_stride_standby	= 5;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 0;
	m_block_stride_shoot	= 10;
	m_block_stride_fire		= 0;

	m_tall = 10;
}

CGBitmapTempletBowman::~CGBitmapTempletBowman(void)
{
	GUnloadBitmapSurface(surface);
}


CGBitmapTempletChariot::CGBitmapTempletChariot(char *fileName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 120;
	m_blockSizeY = 120;
	m_size.left = 40;
	m_size.right = 80;
	m_size.top = 40;
	m_size.bottom = 100;
	STR_COPY(FileNameBuffer, fileName);
	surface = GLoadBitmapSurface(FileNameBuffer);

	m_block_stride_standby	= 13;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 0;
	m_block_stride_shoot	= 13;
	m_block_stride_fire		= 0;

	m_tall = 10;
}


CGBitmapTempletChariot::CGBitmapTempletChariot(DWORD rcName)
{
	m_blockNumX = 20;
	m_blockNumY = 100;
	m_blockSizeX = 120;
	m_blockSizeY = 120;
	m_size.left = 40;
	m_size.right = 80;
	m_size.top = 40;
	m_size.bottom = 100;

	surface = GLoadBitmapSurface(rcName);

	m_block_stride_standby	= 13;
	m_block_stride_walk		= 10;
	m_block_stride_stick	= 0;
	m_block_stride_shoot	= 13;
	m_block_stride_fire		= 0;

	m_tall = 10;
}

CGBitmapTempletChariot::~CGBitmapTempletChariot(void)
{
	GUnloadBitmapSurface(surface);
}

