#include <mutex>

template<class T>
class MutexVal
{
	public:
		MutexVal()
		{
			pVal = new T;
			pMutex = new std::mutex;
		}
		
		virtual ~MutexVal()
		{
			delete pVal;
			delete pMutex;
		}

		// データ競合回避のためにconst関数にする
		const T& operator()() const
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			return *pVal;
		}

		void operator()(const T& a_val) const
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			*pVal = a_val;
		}

		void operator+=(const T& a_val) const
		{
			std::lock_guard<std::mutex> lock(*pMutex);
			*pVal += a_val;
		}

	private:
		// const関数内で値を変更するため、ポインタで保持
		T *pVal;
		std::mutex *pMutex;
};