#include <cxxtest/TestSuite.h>
#include "../pipes/pipes.h"

#include <iostream>

using namespace pipes;
using IntPair = std::pair<int,int>;
using IntSet = std::set<int>;
using IntPairSet = std::set<IntPair>;
using IntVector = std::vector<int>;

class SetTestSuite : public CxxTest::TestSuite {
public:
  void testSet(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    Pipe<int> doublesPipe = pipe.map<int>([](int i){return i*2;});
    IntSet doubles = doublesPipe.toSet();
    TS_ASSERT_EQUALS(doubles.size(), s.size());
  }

  void testSetMapPipeline(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet doubles = pipe.map<int>([](int i){return i*2;}).toSet();
    TS_ASSERT_EQUALS(doubles.size(), s.size());
  }

  void testSetFilterPipeline(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet evens = pipe.filter([](int i){return i%2==0;}).toSet();
    TS_ASSERT_EQUALS(evens.size(), 2);
  }

  void testSetFilterMapPipeline(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet doubleEvens = pipe.filter([](int i){return i%2==0;}).map<int>([](int i){return i*2;}).toSet();
    TS_ASSERT_EQUALS(doubleEvens.size(), 2);
  }

  void testSetCollect(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    int sum {pipe.collect(0, [](int z, int i){return z+i;})};
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testSetFindSome(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    std::optional<int> firstEven = pipe.find([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(*firstEven, 2);
  }

  void testSetFindNone(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    std::optional<int> none = pipe.find([](int i){return i>10;});
    TS_ASSERT_EQUALS(none, std::nullopt);
  }

  void testGroupBy(void) {
    IntPairSet pairs {{1, 2}, {5, 7}, {2, 8}, {1, 5}, {3, 1}, {2, 4}};
    Pipe<IntPair> pipe {pairs};
    auto groups = pipe.groupBy<int>([](IntPair p){ return p.first; });
    TS_ASSERT_EQUALS(groups.size(), 4);
    TS_ASSERT_EQUALS(groups[1].size(), 2);
    TS_ASSERT_EQUALS(groups[2].size(), 2);
    TS_ASSERT_EQUALS(groups[3].size(), 1);
    TS_ASSERT_EQUALS(groups[5].size(), 1);
  }

  void testSetSize(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.size(), 5);
    Pipe<int> evens = pipe.filter([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(evens.size(), 2);
  }

  void testSetEmpty(void) {
    IntSet s {};
    Pipe<int> pipe {s};
    TS_ASSERT(pipe.isEmpty());
  }

  void testSetEmptyNot(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT(!pipe.isEmpty());
  }

  void testSetForEach(void) {
    int sum {0};
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    pipe.forEach([&sum](int i){sum += i;});
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testSetFlatMap(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    Pipe<int> multiples = pipe.flatMap<int>([](int i){return IntVector(i, i);});
    TS_ASSERT_EQUALS(multiples.size(), 15);
  }

  void testSetTake(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet firstThree = pipe.take(3).toSet();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
  }

  void testSetTakeAll(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet all = pipe.take(5).toSet();
    TS_ASSERT_EQUALS(all.size(), 5);
  }

  void testSetTakeMany(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet all = pipe.take(10).toSet();
    TS_ASSERT_EQUALS(all.size(), 5);
  }

  void testSetTakeNone(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet none = pipe.take(0).toSet();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testSetTakeNeg(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet none = pipe.take(-2).toSet();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testSetDrop(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet lastThree = pipe.drop(2).toSet();
    TS_ASSERT_EQUALS(lastThree.size(), 3);
  }

  void testSetDropNone(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet all = pipe.drop(0).toSet();
    TS_ASSERT_EQUALS(all.size(), 5);
  }

  void testSetDropNeg(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet all = pipe.drop(-2).toSet();
    TS_ASSERT_EQUALS(all.size(), 5);
  }

  void testSetDropAll(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet none = pipe.drop(5).toSet();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testSetDropMany(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet none = pipe.drop(10).toSet();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testSetTakeWhile(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet firstThree = pipe.takeWhile([](int i){return i<4;}).toSet();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
  }

  void testSetTakeWhileMultiMatch(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet initial = pipe.takeWhile([](int i){return i%2!=0;}).toSet();
    TS_ASSERT_EQUALS(initial.size(), 1);
  }

  void testSetDropWhile(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet lastTwo = pipe.dropWhile([](int i){return i<4;}).toSet();
    TS_ASSERT_EQUALS(lastTwo.size(), 2);
  }

  void testSetDropWhileMultiMatch(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet allButFirst = pipe.dropWhile([](int i){return i%2!=0;}).toSet();
    TS_ASSERT_EQUALS(allButFirst.size(), 4);
  }

  void testSetExists(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT(pipe.exists([](int i){return i%2==0;}));
  }

  void testSetExistsNot(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT(!pipe.exists([](int i){return i==0;}));
  }

  void testSetForAll(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT(pipe.forAll([](int i){return i<10;}));
  }

  void testSetForAllNot(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT(!pipe.forAll([](int i){return i%2==0;}));
  }

  void testSetMax(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.max(), 5);
  }

  void testSetMaxNone(void) {
    IntSet s {};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.max(), std::nullopt);
  }

  void testSetMin(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.min(), 1);
  }

  void testSetMinNone(void) {
    IntSet s {};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.min(), std::nullopt);
  }

  void testSetJoin(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.join(), "12345");
  }

  void testSetJoinSep(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.join(", "), "1, 2, 3, 4, 5");
  }

  void testSetJoinEmpty(void) {
    IntSet s {};
    Pipe<int> pipe {s};
    TS_ASSERT_EQUALS(pipe.join(), "");
    TS_ASSERT_EQUALS(pipe.join(", "), "");
  }

  void testSetPartition(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    auto [even, odd] = pipe.partition([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(even.size(), 2);
    TS_ASSERT_EQUALS(even[0], 2);
    TS_ASSERT_EQUALS(even[1], 4);
    TS_ASSERT_EQUALS(odd.size(), 3);
    TS_ASSERT_EQUALS(odd[0], 1);
    TS_ASSERT_EQUALS(odd[1], 3);
    TS_ASSERT_EQUALS(odd[2], 5);
  }

  void testSetReverse(void) {
    IntSet s {1, 2, 3, 4, 5};
    Pipe<int> pipe {s};
    IntSet reverse = pipe.reverse().toSet();
    TS_ASSERT_EQUALS(reverse.size(), 5);
  }
};
