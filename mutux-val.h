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

		// �f�[�^��������̂��߂�const�֐��ɂ���
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
		// const�֐����Œl��ύX���邽�߁A�|�C���^�ŕێ�
		T *pVal;
		std::mutex *pMutex;
};