#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
	#include "../../../src/rx/Man_Decode.c"
}

class IManDecode {
	public:
		virtual void Man_Dec_Add_1() = 0;
		virtual void Man_Dec_Add_0() = 0;
};

/* Mock implementation */

class ManDecodeMock : public IManDecode {
	public:
		MOCK_METHOD0(Man_Dec_Add_1,void());
		MOCK_METHOD0(Man_Dec_Add_0,void());
};

/* Fixture class for each test */

class ManDecodeTest_F : public testing::Test {
	public:
		static ManDecodeMock* getMock() {
			return _ManDecodePtr;
		}

	protected:
		virtual void SetUp() {
			_total_1 = _total_0 = 0;
			_ManDecodePtr = &_ManDecode;
		}

		void Expect_Total_1_And_0_Eq(int total) {
			EXPECT_EQ(_total_1, total);
			EXPECT_EQ(_total_0, total);
		}

		void Perform_Stable_Zero(unsigned char periods) {
			_total_0 += periods;
			Man_Decode_Stable_Zero(periods);
		}

		void Perform_Stable_One(unsigned char periods) {
			_total_1 += periods;
			Man_Decode_Stable_One(periods);
		}

	private:
		ManDecodeMock _ManDecode;
		static ManDecodeMock* _ManDecodePtr;
		int _total_1, _total_0;
};

ManDecodeMock* ManDecodeTest_F::_ManDecodePtr;

/* Man_Decode externs (events) */

void On_Man_Decode_Add_1() {
	ManDecodeTest_F::getMock()->Man_Dec_Add_1();
}

void On_Man_Decode_Add_0() {
	ManDecodeTest_F::getMock()->Man_Dec_Add_0();
}

/* Helpers for TEST_F */

#define EXPECT_MAN_SYNCH(x,y)                                                             \
	EXPECT_CALL(*getMock(), Man_Dec_Add_##x()).Times(testing::AtMost(1)); /* Clay? */ \
	EXPECT_CALL(*getMock(), Man_Dec_Add_##y()).Times(testing::AtMost(1)); /* Clay? */ \
	EXPECT_CALL(*getMock(), Man_Dec_Add_##x());                           /* Sync  */ \
	EXPECT_CALL(*getMock(), Man_Dec_Add_##y())                            /* Sync  */

#define PERFORM_MAN_SYNCH(x,y)                                   \
	Perform_Stable_##x(1);  /* Clay balance (1 - 1 = 0) */   \
	Perform_Stable_##y(2);  /* Sync balance (2 - 2 = 0) */   \
	Perform_Stable_##x(2);  /* Sync */                       \
	Perform_Stable_##y(1)   /* Clay */

/* 1010101010101010(manchester) => 0 */

TEST_F(ManDecodeTest_F, Decode_0) {
	testing::InSequence s;
	EXPECT_MAN_SYNCH(1,0);
	EXPECT_CALL(*getMock(), Man_Dec_Add_0()).Times(8);

	PERFORM_MAN_SYNCH(One, Zero);
	for (int i = 0; i < 8; i++) {
		Perform_Stable_One(1);
		Perform_Stable_Zero(1);
	}

	Expect_Total_1_And_0_Eq(11); // Sync(3) + Byte(8)
}

/* 0101010101010101(manchester) => 11111111(bin) => 255(dec) */

TEST_F(ManDecodeTest_F, Decode_255) {
	testing::InSequence s;
	EXPECT_MAN_SYNCH(0,1);
	EXPECT_CALL(*getMock(), Man_Dec_Add_1()).Times(8);

	PERFORM_MAN_SYNCH(Zero, One);
	for (int i = 0; i < 8; i++) {
		Perform_Stable_Zero(1);
		Perform_Stable_One(1);
	}

	Expect_Total_1_And_0_Eq(11); // Sync(3) + Byte(8)
}

/* 0110011001100110(manchester) => 10101010(bin) => 170(dec) */

TEST_F(ManDecodeTest_F, Decode_170) {
	testing::InSequence s;
	EXPECT_MAN_SYNCH(0,1);
	for (int i = 0; i < 4; i++) {
		EXPECT_CALL(*getMock(), Man_Dec_Add_1());
		EXPECT_CALL(*getMock(), Man_Dec_Add_0());
	}

	PERFORM_MAN_SYNCH(Zero, One);
	Perform_Stable_Zero(1);
	for (int i = 0; i < 3; i++) {
		Perform_Stable_One(2);
		Perform_Stable_Zero(2);
	}
	Perform_Stable_One(2);
	Perform_Stable_Zero(1);

	Expect_Total_1_And_0_Eq(11); // Sync(3) + Byte(8)
}

/* 1001100110011001(manchester) => 01010101(bin) => 85(dec) */

TEST_F(ManDecodeTest_F, Decode_85) {
	testing::InSequence s;
	EXPECT_MAN_SYNCH(1,0);
	for (int i = 0; i < 4; i++) {
		EXPECT_CALL(*getMock(), Man_Dec_Add_0());
		EXPECT_CALL(*getMock(), Man_Dec_Add_1());
	}

	PERFORM_MAN_SYNCH(One, Zero);
	Perform_Stable_One(1);
	for (int i = 0; i < 3; i++) {
		Perform_Stable_Zero(2);
		Perform_Stable_One(2);
	}
	Perform_Stable_Zero(2);
	Perform_Stable_One(1);

	Expect_Total_1_And_0_Eq(11); // Sync(3) + Byte(8)
}

/* 1001100110011010(manchester) => 84(dec) => 01010100(bin) */

TEST_F(ManDecodeTest_F, Decode_84) {
	testing::InSequence s;
	EXPECT_MAN_SYNCH(1,0);
	for (int i = 0; i < 3; i++) {
		EXPECT_CALL(*getMock(), Man_Dec_Add_0());
		EXPECT_CALL(*getMock(), Man_Dec_Add_1());
	}
	EXPECT_CALL(*getMock(), Man_Dec_Add_0()).Times(2);

	PERFORM_MAN_SYNCH(One, Zero);
	Perform_Stable_One(1);
	for (int i = 0; i < 3; i++) {
		Perform_Stable_Zero(2);
		Perform_Stable_One(2);
	}
	Perform_Stable_Zero(1);
	Perform_Stable_One(1);
	Perform_Stable_Zero(1);

	Expect_Total_1_And_0_Eq(11); // Sync(3) + Byte(8)
}
