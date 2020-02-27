#include <cxxtest/TestSuite.h>
#include "../pipes/pipes.h"

#include <iostream>

using namespace pipes;
using IntPair = std::pair<int,int>;
using IntVector = std::vector<int>;
using IntPairVector = std::vector<IntPair>;

class VectorTestSuite : public CxxTest::TestSuite {
public:
  void testVector(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    Pipe<int> doublesPipe = pipe.map<int>([](int i){return i*2;});
    IntVector doubles = doublesPipe.toVector();
    TS_ASSERT_EQUALS(doubles.size(), v.size());
    for (int i {}; i!=v.size(); i++) {
      TS_ASSERT_EQUALS(doubles[i], v[i]*2);
    }
  }

  void testVectorMapPipeline(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector doubles = pipe.map<int>([](int i){return i*2;}).toVector();
    TS_ASSERT_EQUALS(doubles.size(), v.size());
    for (int i {}; i!=v.size(); i++) {
      TS_ASSERT_EQUALS(doubles[i], v[i]*2);
    }
  }

  void testVectorFilterPipeline(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector evens = pipe.filter([](int i){return i%2==0;}).toVector();
    TS_ASSERT_EQUALS(evens.size(), 2);
    TS_ASSERT_EQUALS(evens[0], 2);
    TS_ASSERT_EQUALS(evens[1], 4);
  }

  void testVectorFilterMapPipeline(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector doubleEvens = pipe.filter([](int i){return i%2==0;}).map<int>([](int i){return i*2;}).toVector();
    TS_ASSERT_EQUALS(doubleEvens.size(), 2);
    TS_ASSERT_EQUALS(doubleEvens[0], 4);
    TS_ASSERT_EQUALS(doubleEvens[1], 8);
  }

  void testVectorCollect(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    int sum {pipe.collect(0, [](int z, int i){return z+i;})};
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testVectorFindSome(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    std::optional<int> firstEven = pipe.find([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(*firstEven, 2);
  }

  void testVectorFindNone(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    std::optional<int> none = pipe.find([](int i){return i>10;});
    TS_ASSERT_EQUALS(none, std::nullopt);
  }

  void testGroupBy(void) {
    IntPairVector pairs {{1, 2}, {5, 7}, {2, 8}, {1, 5}, {3, 1}, {2, 4}};
    Pipe<IntPair> pipe {pairs};
    auto groups = pipe.groupBy<int>([](IntPair p){ return p.first; });
    TS_ASSERT_EQUALS(groups.size(), 4);
    TS_ASSERT_EQUALS(groups[1].size(), 2);
    TS_ASSERT_EQUALS(groups[2].size(), 2);
    TS_ASSERT_EQUALS(groups[3].size(), 1);
    TS_ASSERT_EQUALS(groups[5].size(), 1);
  }

  void testVectorSize(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.size(), 5);
    Pipe<int> evens = pipe.filter([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(evens.size(), 2);
  }

  void testVectorEmpty(void) {
    IntVector v {};
    Pipe<int> pipe {v};
    TS_ASSERT(pipe.isEmpty());
  }

  void testVectorEmptyNot(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT(!pipe.isEmpty());
  }

  void testVectorForEach(void) {
    int sum {0};
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    pipe.forEach([&sum](int i){sum += i;});
    TS_ASSERT_EQUALS(sum, 15);
  }

  void testVectorFlatMap(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    Pipe<int> multiples = pipe.flatMap<int>([](int i){return IntVector(i, i);});
    TS_ASSERT_EQUALS(multiples.size(), 15);
  }

  void testVectorTake(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector firstThree = pipe.take(3).toVector();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
    TS_ASSERT_EQUALS(firstThree[0], 1);
    TS_ASSERT_EQUALS(firstThree[1], 2);
    TS_ASSERT_EQUALS(firstThree[2], 3);
  }

  void testVectorTakeAll(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector all = pipe.take(5).toVector();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testVectorTakeMany(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector all = pipe.take(10).toVector();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testVectorTakeNone(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector none = pipe.take(0).toVector();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testVectorTakeNeg(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector none = pipe.take(-2).toVector();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testVectorDrop(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector lastThree = pipe.drop(2).toVector();
    TS_ASSERT_EQUALS(lastThree.size(), 3);
    TS_ASSERT_EQUALS(lastThree[0], 3);
    TS_ASSERT_EQUALS(lastThree[1], 4);
    TS_ASSERT_EQUALS(lastThree[2], 5);
  }

  void testVectorDropNone(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector all = pipe.drop(0).toVector();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testVectorDropNeg(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector all = pipe.drop(-2).toVector();
    TS_ASSERT_EQUALS(all.size(), 5);
    TS_ASSERT_EQUALS(all[0], 1);
    TS_ASSERT_EQUALS(all[1], 2);
    TS_ASSERT_EQUALS(all[2], 3);
    TS_ASSERT_EQUALS(all[3], 4);
    TS_ASSERT_EQUALS(all[4], 5);
  }

  void testVectorDropAll(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector none = pipe.drop(5).toVector();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testVectorDropMany(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector none = pipe.drop(10).toVector();
    TS_ASSERT_EQUALS(none.size(), 0);
  }

  void testVectorTakeWhile(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector firstThree = pipe.takeWhile([](int i){return i<4;}).toVector();
    TS_ASSERT_EQUALS(firstThree.size(), 3);
    TS_ASSERT_EQUALS(firstThree[0], 1);
    TS_ASSERT_EQUALS(firstThree[1], 2);
    TS_ASSERT_EQUALS(firstThree[2], 3);
  }

  void testVectorTakeWhileMultiMatch(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector initial = pipe.takeWhile([](int i){return i%2!=0;}).toVector();
    TS_ASSERT_EQUALS(initial.size(), 1);
    TS_ASSERT_EQUALS(initial[0], 1);
  }

  void testVectorDropWhile(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector lastTwo = pipe.dropWhile([](int i){return i<4;}).toVector();
    TS_ASSERT_EQUALS(lastTwo.size(), 2);
    TS_ASSERT_EQUALS(lastTwo[0], 4);
    TS_ASSERT_EQUALS(lastTwo[1], 5);
  }

  void testVectorDropWhileMultiMatch(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector allButFirst = pipe.dropWhile([](int i){return i%2!=0;}).toVector();
    TS_ASSERT_EQUALS(allButFirst.size(), 4);
    TS_ASSERT_EQUALS(allButFirst[0], 2);
    TS_ASSERT_EQUALS(allButFirst[1], 3);
    TS_ASSERT_EQUALS(allButFirst[2], 4);
    TS_ASSERT_EQUALS(allButFirst[3], 5);
  }

  void testVectorExists(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT(pipe.exists([](int i){return i%2==0;}));
  }

  void testVectorExistsNot(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT(!pipe.exists([](int i){return i==0;}));
  }

  void testVectorForAll(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT(pipe.forAll([](int i){return i<10;}));
  }

  void testVectorForAllNot(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT(!pipe.forAll([](int i){return i%2==0;}));
  }

  void testVectorMax(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.max(), 5);
  }

  void testVectorMaxNone(void) {
    IntVector v {};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.max(), std::nullopt);
  }

  void testVectorMin(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.min(), 1);
  }

  void testVectorMinNone(void) {
    IntVector v {};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.min(), std::nullopt);
  }

  void testVectorJoin(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.join(), "12345");
  }

  void testVectorJoinSep(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.join(", "), "1, 2, 3, 4, 5");
  }

  void testVectorJoinEmpty(void) {
    IntVector v {};
    Pipe<int> pipe {v};
    TS_ASSERT_EQUALS(pipe.join(), "");
    TS_ASSERT_EQUALS(pipe.join(", "), "");
  }

  void testVectorPartition(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    auto [even, odd] = pipe.partition([](int i){return i%2==0;});
    TS_ASSERT_EQUALS(even.size(), 2);
    TS_ASSERT_EQUALS(even[0], 2);
    TS_ASSERT_EQUALS(even[1], 4);
    TS_ASSERT_EQUALS(odd.size(), 3);
    TS_ASSERT_EQUALS(odd[0], 1);
    TS_ASSERT_EQUALS(odd[1], 3);
    TS_ASSERT_EQUALS(odd[2], 5);
  }

  void testVectorReverse(void) {
    IntVector v {1, 2, 3, 4, 5};
    Pipe<int> pipe {v};
    IntVector reverse = pipe.reverse().toVector();
    TS_ASSERT_EQUALS(reverse.size(), 5);
    TS_ASSERT_EQUALS(reverse[0], 5);
    TS_ASSERT_EQUALS(reverse[1], 4);
    TS_ASSERT_EQUALS(reverse[2], 3);
    TS_ASSERT_EQUALS(reverse[3], 2);
    TS_ASSERT_EQUALS(reverse[4], 1);
  }
};
