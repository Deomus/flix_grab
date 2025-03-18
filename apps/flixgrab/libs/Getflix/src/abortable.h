#pragma once

#include <QtCore>

// TODO: replace this with QPromise when we migrate to Qt 6.
class Abortable : public QObject
{
	Q_OBJECT
public:
	class Operation
	{
	public:
		Operation(Abortable* abortable = nullptr) : abortable_(abortable) { 
			if (abortable_)	
				abortable_->outstanding_operations_.ref();
		}

		~Operation() {
			if (abortable_ && !abortable_->outstanding_operations_.deref())	
				emit abortable_->finished();
		}

	private:
		Abortable* abortable_;
	};

	Abortable(QObject* parent = nullptr) : QObject(parent) {}

	virtual ~Abortable() {
		abort();
		waitForFinished();
	}

	void abort() {
		if (aborted_) {
			return;
		}
		aborted_ = true;
		emit aborted();
	}

	bool isAborted() const {
		return aborted_;
	}

	Operation startOperation() {
		return Operation(this);
	}

	void waitForFinished() {
		QEventLoop loop;
		auto connection = QObject::connect(this, &Abortable::finished, &loop, &QEventLoop::quit);

		if (outstanding_operations_.load() > 0)
			loop.exec();

		QObject::disconnect(connection);
	}

signals:
	void aborted();
	void finished();

private:
	volatile bool aborted_ = false;
	QAtomicInteger<int> outstanding_operations_;
};
