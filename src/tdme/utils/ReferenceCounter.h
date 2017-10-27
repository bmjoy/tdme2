#pragma once

#include <tdme/utils/fwd-tdme.h>

/**
 * Reference counter implementation which should be used with inheritance
 * @author Andreas Drewke
 */
class tdme::utils::ReferenceCounter {
public:
	/**
	 * @brief Public constructor
	 */
	ReferenceCounter();

	/**
	 * @brief destructor
	 */
	virtual ~ReferenceCounter();

	/**
	 * @brief acquires a reference, incrementing the counter
	 */
	void acquireReference();

	/**
	 * @brief releases a reference, thus decrementing the counter and delete it if reference counter is zero
	 */
	void releaseReference();
private:
	int referenceCounter;
};