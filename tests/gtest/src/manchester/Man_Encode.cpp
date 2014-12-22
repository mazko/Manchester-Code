#include <gtest/gtest.h>
#include <gmock/gmock.h>

extern "C"
{
	#include "../../../src/tx/Man_Encode.c"
}

class IManEncode {
	public:
		virtual void Man_Encode_One() = 0;
		virtual void Man_Encode_Zero() = 0;
};

/* Mock implementation */

ACTION_P2(Act_Inc_1, T, V) {
	ON_CALL(*T, Man_Encode_One()).WillByDefault(Act_Inc_1(T, testing::ByRef(++V)));
}

ACTION_P2(Act_Inc_0, T, V) {
	ON_CALL(*T, Man_Encode_Zero()).WillByDefault(Act_Inc_0(T, testing::ByRef(++V)));
}

class ManEncodeMock : public IManEncode {
	public:
		MOCK_METHOD0(Man_Encode_One,void());
		MOCK_METHOD0(Man_Encode_Zero,void());

		ManEncodeMock() {
			_total_1 = _total_0 = 0;
			ON_CALL(*this, Man_Encode_One())
				.WillByDefault(Act_Inc_1(this, testing::ByRef(_total_1)));
			ON_CALL(*this, Man_Encode_Zero())
				.WillByDefault(Act_Inc_0(this, testing::ByRef(_total_0)));
		}

		void Expect_Total_1_And_0_Eq(int total) {
			EXPECT_EQ(_total_1, total);
			EXPECT_EQ(_total_0, total);
		}

	private:
		int _total_1, _total_0;
};

/* Fixture class for each test */

class ManEncodeTest_F : public testing::Test {
	public:
		static ManEncodeMock* getMock() {
			return _ManEncodePtr;
		}

	protected:
		virtual void SetUp() {
			_ManEncodePtr = &_ManEncode;
		}

	private:
		ManEncodeMock _ManEncode;
		static ManEncodeMock* _ManEncodePtr;	
};

ManEncodeMock* ManEncodeTest_F::_ManEncodePtr;

/* Man_Encode externs (events) */

void On_Man_Encode_One() {
	ManEncodeTest_F::getMock()->Man_Encode_One();
}

void On_Man_Encode_Zero() {
	ManEncodeTest_F::getMock()->Man_Encode_Zero();
}

/* 0 => 1010101010101010(manchester) */

TEST_F(ManEncodeTest_F, Send_0) {
	testing::InSequence s;
	for (int i = 0; i < 8; i++) {
		EXPECT_CALL(*getMock(), Man_Encode_One()); // MSB
		EXPECT_CALL(*getMock(), Man_Encode_Zero());
	}

	Man_Encode(0);

	getMock()->Expect_Total_1_And_0_Eq(8);
}

/* 255(dec) => 11111111(bin) => 0101010101010101(manchester) */

TEST_F(ManEncodeTest_F, Send_255) {
	testing::InSequence s;
	for (int i = 0; i < 8; i++) {
		EXPECT_CALL(*getMock(), Man_Encode_Zero()); // MSB
		EXPECT_CALL(*getMock(), Man_Encode_One());
	}

	Man_Encode(255);

	getMock()->Expect_Total_1_And_0_Eq(8);
}

/* 170(dec) => 10101010(bin) => 0110011001100110(manchester) */

TEST_F(ManEncodeTest_F, Send_170) {
	testing::InSequence s;
	EXPECT_CALL(*getMock(), Man_Encode_Zero()); // MSB
	for (int i = 0; i < 3; i++) {
		EXPECT_CALL(*getMock(), Man_Encode_One()).Times(2);
		EXPECT_CALL(*getMock(), Man_Encode_Zero()).Times(2);
	}
	EXPECT_CALL(*getMock(), Man_Encode_One()).Times(2);
	EXPECT_CALL(*getMock(), Man_Encode_Zero());

	Man_Encode(170);

	getMock()->Expect_Total_1_And_0_Eq(8);
}

/* 85(dec) => 01010101(bin) => 1001100110011001(manchester) */

TEST_F(ManEncodeTest_F, Send_85) {
	testing::InSequence s;
	EXPECT_CALL(*getMock(), Man_Encode_One()); // MSB
	for (int i = 0; i < 3; i++) {
		EXPECT_CALL(*getMock(), Man_Encode_Zero()).Times(2);
		EXPECT_CALL(*getMock(), Man_Encode_One()).Times(2);
	}
	EXPECT_CALL(*getMock(), Man_Encode_Zero()).Times(2);
	EXPECT_CALL(*getMock(), Man_Encode_One());

	Man_Encode(85);

	getMock()->Expect_Total_1_And_0_Eq(8);
}

/* 84(dec) => 01010100(bin) => 1001100110011010(manchester) */

TEST_F(ManEncodeTest_F, Send_84) {
	testing::InSequence s;
	EXPECT_CALL(*getMock(), Man_Encode_One()); // MSB
	for (int i = 0; i < 3; i++) {
		EXPECT_CALL(*getMock(), Man_Encode_Zero()).Times(2);
		EXPECT_CALL(*getMock(), Man_Encode_One()).Times(2);
	}
	EXPECT_CALL(*getMock(), Man_Encode_Zero());
	EXPECT_CALL(*getMock(), Man_Encode_One());
	EXPECT_CALL(*getMock(), Man_Encode_Zero());

	Man_Encode(84);

	getMock()->Expect_Total_1_And_0_Eq(8);
}
