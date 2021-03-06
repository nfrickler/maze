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
	bool isExpanded();
	void setExpanded(bool);
	bool isRoot();
	bool isGoal();
	bool isSolid();
	double getX(int i_width);
	double getY(int i_height);
	int getRow();
	int getColumn();

    protected:
	int m_id;
	int m_row;
	int m_column;
	int m_type;
	bool m_isexpanded;
};

#endif // _BLOCK_H
