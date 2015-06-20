#pragma once

#include "output_window.h"
#include "output_data.h"


class Monitor : public OutputWindow
{
public:
	virtual ~Monitor() = default;

	//OPERATORS
	template<class T>
	Monitor& operator<<(T);

	//OPERATIONS
	static void AllWindowFlip();

	//ACCESS
	static Monitor& mout(int idx);

private:
	//生成禁止 - インスタンス管理はこのクラス内で行う
	Monitor(int);

	static std::vector<std::shared_ptr<Monitor> > outputs;
	int windowNum;

};


template<class T>
inline Monitor& Monitor::operator<<(T val)
{
	std::stringstream buf;
	buf << val;
	Print( buf.str() );

	return *this;
}


inline Monitor& Monitor::mout(int idx)
{
	if( !outputs[idx] )
	{
		outputs[idx] = std::shared_ptr<Monitor>(new Monitor(idx));
		outputs[idx]->Init();
	}

	return *outputs[idx];
}


inline void Monitor::AllWindowFlip()
{
	for each (std::shared_ptr<Monitor> ow in outputs)
	{
		if( !ow ) continue;
		*ow << Command::flip;
	}
}