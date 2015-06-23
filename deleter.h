#include <thread>
#include <functional>
#include <vector>

template <class T>
class Deleter
{
	public:
		Deleter(std::vector<T *> *pVec){ deleteVectorByAnotherThread(pVec); }
//		Deleter(std::list<std::vector<T *> *> *pListOfDeleteVec){ deleteVectorByAnotherThread(pVec); }

	private:
		std::function<void(std::vector<T *> *)> deleteNodeVector =
			[](std::vector<T *> *pVec)
			{
				for(Node *pElem : *pVec){ delete pElem; }
				delete pVec;
			};

		void deleteVectorByAnotherThread(std::vector<T *> *pVec)
		{
			std::thread delElems(deleteNodeVector, pVec);
			delElems.detach();
		}

		// 解放用vectorポインタのリスト　→　deleterは常に動作させる,　mutex　で高速化？
};

