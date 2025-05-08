#include <gtest/gtest.h>

#include "IoCContainer/IoCConnectContainerCommand.h"
#include "IoCContainer/ScopeTree.h"


class MyEnvironment : public ::testing::Environment {
public:
	~MyEnvironment() override {}

	// Override this to define how to set up the environment.
	void SetUp() override {
		IoCConnectContainerCommand((IIoCContainer*)&ScopeTree::g_scopeTree).execute();
	}

	// Override this to define how to tear down the environment.
	void TearDown() override { }
};

// define and init a global variable to set-up environment for my tests
testing::Environment* const foo_env = testing::AddGlobalTestEnvironment(new MyEnvironment);