#include <cxxtest/TestSuite.h>
#include "../pipes/pipes.h"

#include <iostream>

using namespace pipes;
using IntPair = std::pair<int,int>;
template <size_t N>
using IntArray = std::array<int,N>;
template <size_t N>
using IntPairArray = std::array<IntPair,N>;
using IntVector = std::vector<int>;

class ArrayTestSuite : public CxxTest::TestSuite {
public:

  void testArray(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    Pipe<int> doublesPipe = pipe.map<int>([](int i){return i*2;});
    IntArray<5> doubles = doublesPipe.toArray<5>();
    TS_ASSERT_EQUALS(doubles.size(), a.size());
    for (int i {}; i!=a.size(); i++) {
      TS_ASSERT_EQUALS(doubles[i], a[i]*2);
    }
  }

  void testArrayMapPipeline(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> doubles = pipe.map<int>([](int i){return i*2;}).toArray<5>();
    TS_ASSERT_EQUALS(doubles.size(), a.size());
    for (int i {}; i!=a.size(); i++) {
      TS_ASSERT_EQUALS(doubles[i], a[i]*2);
    }
  }

  void testArrayFilterPipeline(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<2> evens = pipe.filter([](int i){return i%2==0;}).toArray<2>();
    TS_ASSERT_EQUALS(evens.size(), 2);
    TS_ASSERT_EQUALS(evens[0], 2);
    TS_ASSERT_EQUALS(evens[1], 4);
  }

  void testArrayFilterMapPipeline(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<2> doubleEvens = pipe.filter([](int i){return i%2==0;}).map<int>([](int i){return i*2;}).toArray<2>();
    TS_ASSERT_EQUALS(doubleEvens.size(), 2);
    TS_ASSERT_EQUALS(doubleEvens[0], 4);
    TS_ASSERT_EQUALS(doubleEvens[1], 8);
  }

  void testArrayCollect(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    int sum {pipe.collect(0, [](int z, int i){return z+i;})};
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testArrayFindSome(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    std::optional<int> firstEven = pipe.find([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(*firstEven, 2);
  }

  void testArrayFindNone(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    std::optional<int> none = pipe.find([](int i){return i>10;});
    TS_ASSERT_EQUALS(none, std::nullopt);
  }

  void testGroupBy(void) {
    IntPairArray<6> pairs {IntPair{1, 2}, {5, 7}, {2, 8}, {1, 5}, {3, 1}, {2, 4}};
    Pipe<IntPair> pipe {pairs};
    auto groups = pipe.groupBy<int>([](IntPair p){ return p.first; });
    TS_ASSERT_EQUALS(groups.size(), 4);
    TS_ASSERT_EQUALS(groups[1].size(), 2);
    TS_ASSERT_EQUALS(groups[2].size(), 2);
    TS_ASSERT_EQUALS(groups[3].size(), 1);
    TS_ASSERT_EQUALS(groups[5].size(), 1);
  }

  void testArraySize(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.size(), 5);
    Pipe<int> evens = pipe.filter([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(evens.size(), 2);
  }

  void testArrayEmpty(void) {
    IntArray<0> a {};
    Pipe<int> pipe {a};
    TS_ASSERT(pipe.isEmpty());
  }

  void testArrayEmptyNot(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT(!pipe.isEmpty());
  }

  void testArrayForEach(void) {
    int sum {0};
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    pipe.forEach([&sum](int i){sum += i;});
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testArrayFlatMap(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    Pipe<int> multiples = pipe.flatMap<int>([](int i){return IntVector(i, i);});
    TS_ASSERT_EQUALS(multiples.size(), 15);
  }

  void testArrayTake(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<3> firstThree = pipe.take(3).toArray<3>();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
    TS_ASSERT_EQUALS(firstThree[0], 1);
    TS_ASSERT_EQUALS(firstThree[1], 2);
    TS_ASSERT_EQUALS(firstThree[2], 3);
  }

  void testArrayTakeAll(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> all = pipe.take(5).toArray<5>();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testArrayTakeMany(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> all = pipe.take(10).toArray<5>();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testArrayDrop(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<3> lastThree = pipe.drop(2).toArray<3>();
    TS_ASSERT_EQUALS(lastThree.size(), 3);
    TS_ASSERT_EQUALS(lastThree[0], 3);
    TS_ASSERT_EQUALS(lastThree[1], 4);
    TS_ASSERT_EQUALS(lastThree[2], 5);
  }

  void testArrayDropNone(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> all = pipe.drop(0).toArray<5>();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testArrayDropNeg(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> all = pipe.drop(-2).toArray<5>();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testArrayTakeWhile(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<3> firstThree = pipe.takeWhile([](int i){return i<4;}).toArray<3>();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
    TS_ASSERT_EQUALS(firstThree[0], 1);
    TS_ASSERT_EQUALS(firstThree[1], 2);
    TS_ASSERT_EQUALS(firstThree[2], 3);
  }

  void testArrayTakeWhileMultiMatch(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<1> initial = pipe.takeWhile([](int i){return i%2!=0;}).toArray<1>();
    TS_ASSERT_EQUALS(initial.size(), 1);
    TS_ASSERT_EQUALS(initial[0], 1);
  }

  void testArrayDropWhile(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<2> lastTwo = pipe.dropWhile([](int i){return i<4;}).toArray<2>();
    TS_ASSERT_EQUALS(lastTwo.size(), 2);
    TS_ASSERT_EQUALS(lastTwo[0], 4);
    TS_ASSERT_EQUALS(lastTwo[1], 5);
  }

  void testArrayDropWhileMultiMatch(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<4> allButFirst = pipe.dropWhile([](int i){return i%2!=0;}).toArray<4>();
    TS_ASSERT_EQUALS(allButFirst.size(), 4);
    TS_ASSERT_EQUALS(allButFirst[0], 2);
    TS_ASSERT_EQUALS(allButFirst[1], 3);
    TS_ASSERT_EQUALS(allButFirst[2], 4);
    TS_ASSERT_EQUALS(allButFirst[3], 5);
  }

  void testArrayExists(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT(pipe.exists([](int i){return i%2==0;}));
  }

  void testArrayExistsNot(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT(!pipe.exists([](int i){return i==0;}));
  }

  void testArrayForAll(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT(pipe.forAll([](int i){return i<10;}));
  }

  void testArrayForAllNot(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT(!pipe.forAll([](int i){return i%2==0;}));
  }

  void testArrayMax(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.max(), 5);
  }

  void testArrayMaxNone(void) {
    IntArray<0> a {};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.max(), std::nullopt);
  }

  void testArrayMin(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.min(), 1);
  }

  void testArrayMinNone(void) {
    IntArray<0> a {};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.min(), std::nullopt);
  }

  void testArrayJoin(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.join(), "12345");
  }

  void testArrayJoinSep(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.join(", "), "1, 2, 3, 4, 5");
  }

  void testArrayJoinEmpty(void) {
    IntArray<0> a {};
    Pipe<int> pipe {a};
    TS_ASSERT_EQUALS(pipe.join(), "");
    TS_ASSERT_EQUALS(pipe.join(", "), "");
  }

  void testArrayPartition(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    auto [even, odd] = pipe.partition([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(even.size(), 2);
    TS_ASSERT_EQUALS(even[0], 2);
    TS_ASSERT_EQUALS(even[1], 4);
    TS_ASSERT_EQUALS(odd.size(), 3);
    TS_ASSERT_EQUALS(odd[0], 1);
    TS_ASSERT_EQUALS(odd[1], 3);
    TS_ASSERT_EQUALS(odd[2], 5);
  }

  void testArrayReverse(void) {
    IntArray<5> a {1, 2, 3, 4, 5};
    Pipe<int> pipe {a};
    IntArray<5> reverse = pipe.reverse().toArray<5>();
    TS_ASSERT_EQUALS(reverse.size(), 5);
    TS_ASSERT_EQUALS(reverse[0], 5);
    TS_ASSERT_EQUALS(reverse[1], 4);
    TS_ASSERT_EQUALS(reverse[2], 3);
    TS_ASSERT_EQUALS(reverse[3], 2);
    TS_ASSERT_EQUALS(reverse[4], 1);
  }
};
