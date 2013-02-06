#ifndef _BLOCK_H
#define _BLOCK_H

#include <cairomm/context.h>

class Block {

    public:
	Block(int i_id, int i_row, int i_column, int i_type);

	bool drawme(Cairo::RefPtr<Cairo::Context> cr, double, double);
	void setType(int i_type);
	int getType();
	int getId();
	bool isRoot();
	bool isGoal();
	bool isSolid();
	double getX(int i_width);
	double getY(int i_height);

    protected:
	int m_id;
	int m_row;
	int m_column;
	int m_type;
};

#endif // _BLOCK_H
