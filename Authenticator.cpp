// Brandon Cotesta | 10/27/2025 | 10:20 PM
// MOOD: Exhausted - legit feels like 4am.

#include "Authenticator.h"

// Singleton instance getter
Authenticator& Authenticator::getInstance() {
	static Authenticator instance;
	return instance;
}