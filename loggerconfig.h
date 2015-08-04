#ifndef LOGGERCONFIG_H
#define LOGGERCONFIG_H


class LoggerConfig
{
	bool mTimestamp;
	bool mStart;
	bool mEnd;
	bool mName;
	bool mDirection;
	bool mFingerDimensions;
	bool mArm;

public:
    LoggerConfig();
    ~LoggerConfig();

	inline void logTimestamp(bool v){ this->mTimestamp = v; }
	inline bool isLoggingTimestamp() const { return this->mTimestamp; }

	inline void logStart(bool v){ this->mStart = v; }
	inline bool isLoggingStart() const { return this->mStart; }

	inline void logEnd(bool v){ this->mEnd = v; }
	inline bool isLoggingEnd() const { return this->mEnd; }

	inline void logName(bool v){ this->mName = v; }
	inline bool isLoggingName() const { return this->mName; }

	inline void logDirection(bool v){ this->mDirection = v; }
	inline bool isLoggingDirection() const { return this->mDirection; }

	inline void logFingerDimensions(bool v){ this->mFingerDimensions = v; }
	inline bool isLoggingFingerDimensions() const { return this->mFingerDimensions; }

	inline void logArm(bool v){ this->mArm = v; }
	inline bool isLoggingArm() const { return this->mArm; }
};

#endif // LOGGERCONFIG_H
