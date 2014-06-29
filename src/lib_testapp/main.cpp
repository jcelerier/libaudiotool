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
		void loop(){ TestLoop(); }

//		void evaluation() { TestEvaluation(); }
//		void transforms() { TestTransform(); }
//		void window() { testWindow(); }
//		void copy() { testCopy(); }
//		void benchmark() { TestBenchmark(); }
//		void lsb() { TestLSB(); }
//		void ssw() { TestSSW(); }
//		void rlsb() { TestRLSB(); } // passer voir le RLSBDecode.h pour utilisation
};

int main(int argc, char** argv)
{
	TestLoop();
	return 0;
	QCoreApplication app(argc, argv);
	auto tests = new WatermarkingTests;
	QTest::qExec(tests);
	delete tests;
    return 0;
}

#include "main.moc"
