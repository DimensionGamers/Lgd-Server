#ifndef TRANSACTION_CALL_BACK_H
#define TRANSACTION_CALL_BACK_H

class TransactionCallBack
{
	public:
		explicit TransactionCallBack(): msec(MyGetTickCount())
		{
			
		}

		virtual ~TransactionCallBack()
		{

		}

		TCType GetMsec() const { return MyGetTickCount() - msec; }

		virtual void Finished() = 0;
		virtual void Cancelled() = 0;

		TCType msec;
};

typedef std::shared_ptr<TransactionCallBack> SQLTransactionCallBack;

#endif