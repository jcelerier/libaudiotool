#include "TestHeader.h"
void TestLSB();
void TestBenchmark();
void testCopy();
void testWindow();
void TestRLSB();
void TestSSW();
void TestTransform();
void TestEvaluation();
void TestLoop();

class WatermarkingTests: public QObject
{
		Q_OBJECT
	private slots:
		void test()
		{
			/*
			TestLSB();
			TestBenchmark();
			testCopy();
			testWindow();
			TestRLSB();
			TestSSW();
			TestTransform();
			TestEvaluation();
			*/
			TestLoop();
		}

};

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	auto tests = new WatermarkingTests;
	QTest::qExec(tests);
	delete tests;

	return 0;
}

#include "main.moc"
